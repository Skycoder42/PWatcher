#include "imageloader.h"
#include <QDateTime>
#include <QtMath>
#include <QMovie>
#include <QDebug>

ImageLoader::ImageLoader(QObject *parent) :
	QThread(parent),
	loaderSemaphore(0),
	movieBase(),
	movieMutex(QMutex::NonRecursive),
	mainInfo(),
	screenSize(),
	currentIndex(-1),
	nextInfo(),
	nextScreenSize(),
	hasNextInfo(false)
{
	setTerminationEnabled(true);
	start();
}

ImageLoader::~ImageLoader()
{
	requestInterruption();
	if(!wait(3000))
		terminate();
}

void ImageLoader::updateLoad()
{
	loaderSemaphore.release();
}

void ImageLoader::resetData(const DisplayWindow::ViewInfo &mainInfo, const QSize &screenSize)
{
	Q_ASSERT(!hasNextInfo);
	while(loaderSemaphore.tryAcquire());
	nextInfo = mainInfo;
	nextScreenSize = screenSize;
	hasNextInfo = true;
	while(hasNextInfo)
		QThread::msleep(50);
}

void ImageLoader::updateMovieBase(const QSharedPointer<QMovie> &movieBase)
{
	movieMutex.lock();
	movieBase->setCacheMode(QMovie::CacheAll);
	this->movieBase.enqueue(movieBase);
	movieMutex.unlock();
}

void ImageLoader::run()
{
	qsrand(QDateTime::currentMSecsSinceEpoch());

	do {
		if(hasNextInfo) {
			mainInfo = nextInfo;
			screenSize = nextScreenSize;
			currentIndex = -1;
			loaderSemaphore.release(ImageLoader::PreloadAmount);
			hasNextInfo = false;
		}

		if(loaderSemaphore.tryAcquire(1, 100)) {
			if(mainInfo.files.isEmpty())
				continue;
			DisplayWindow::ImageObject imageObject;

			do {
				if(isInterruptionRequested())
					break;

				if(mainInfo.random) {
					do {
						currentIndex = qFloor(qrand() * mainInfo.files.size()) / RAND_MAX;
					} while(currentIndex >= mainInfo.files.size());
				} else {
					if(++currentIndex >= mainInfo.files.size()) {
						if(mainInfo.repeat)
							currentIndex = 0;
						else
							break;
					}
				}

				Q_ASSERT(currentIndex >= 0);
				Q_ASSERT(currentIndex < mainInfo.files.size());
				const QFileInfo &info = mainInfo.files[currentIndex];

				if(QMovie::supportedFormats().contains(info.suffix().toUtf8())) {
					movieMutex.lock();
					Q_ASSERT(!movieBase.isEmpty());
					QSharedPointer<QMovie> movie(movieBase.first());
					movieMutex.unlock();

					movie->setFileName(info.absoluteFilePath());
					if(movie->isValid()) {
						movie->setBackgroundColor(Qt::transparent);
						movie->jumpToNextFrame();
						QSize movieSize = movie->frameRect().size();
						if(movieSize.width() > screenSize.width() ||
						   movieSize.height() > screenSize.height()) {
							movieSize = movieSize.scaled(screenSize, Qt::KeepAspectRatio);
							movie->setScaledSize(movieSize);
						}

						connect(movie.data(), &QMovie::finished, movie.data(), &QMovie::start);
						imageObject.isMovie = true;
						imageObject.movie = movie;
						imageObject.fileName = info.absoluteFilePath();

						movieMutex.lock();
						movieBase.dequeue();
						movieMutex.unlock();
					}
				} else {
					QPixmap image(info.absoluteFilePath());
					if(!image.isNull()) {
						if(image.width() > screenSize.width() ||
						   image.height() > screenSize.height()) {
							image = image.scaled(screenSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
						}
						imageObject.isMovie = false;
						imageObject.pixmap = image;
						imageObject.fileName = info.absoluteFilePath();
					}
				}
			} while(imageObject.isNull());


			if(!imageObject.isNull())
				emit loadReady(imageObject);
		}
	} while(!isInterruptionRequested());
}
