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
	this->setTerminationEnabled(true);
	this->start();
}

ImageLoader::~ImageLoader()
{
	this->requestInterruption();
	if(!this->wait(3000))
		this->terminate();
}

void ImageLoader::updateLoad()
{
	this->loaderSemaphore.release();
}

void ImageLoader::resetData(const DisplayWindow::ViewInfo &mainInfo, const QSize &screenSize)
{
	Q_ASSERT(!this->hasNextInfo);
	while(this->loaderSemaphore.tryAcquire());
	this->nextInfo = mainInfo;
	this->nextScreenSize = screenSize;
	this->hasNextInfo = true;
	while(this->hasNextInfo)
		QThread::msleep(50);
}

void ImageLoader::updateMovieBase(const QSharedPointer<QMovie> &movieBase)
{
	this->movieMutex.lock();
	movieBase->setCacheMode(QMovie::CacheAll);
	this->movieBase.enqueue(movieBase);
	this->movieMutex.unlock();
}

void ImageLoader::run()
{
	qsrand(QDateTime::currentMSecsSinceEpoch());

	do {
		if(this->hasNextInfo) {
			this->mainInfo = this->nextInfo;
			this->screenSize = this->nextScreenSize;
			this->currentIndex = -1;
			this->loaderSemaphore.release(ImageLoader::PreloadAmount);
			this->hasNextInfo = false;
		}

		if(this->loaderSemaphore.tryAcquire(1, 100)) {
			if(this->mainInfo.files.isEmpty())
				continue;
			DisplayWindow::ImageObject imageObject;

			do {
				if(this->isInterruptionRequested())
					break;

				if(this->mainInfo.random) {
					do {
						this->currentIndex = qFloor(qrand() * this->mainInfo.files.size()) / RAND_MAX;
					} while(this->currentIndex >= this->mainInfo.files.size());
				} else {
					if(++this->currentIndex >= this->mainInfo.files.size()) {
						if(this->mainInfo.repeat)
							this->currentIndex = 0;
						else
							break;
					}
				}

				Q_ASSERT(this->currentIndex >= 0);
				Q_ASSERT(this->currentIndex < this->mainInfo.files.size());
				const QFileInfo &info = this->mainInfo.files[this->currentIndex];

				if(this->mainInfo.movieFormats.contains(info.suffix())) {
					this->movieMutex.lock();
					Q_ASSERT(!this->movieBase.isEmpty());
					QSharedPointer<QMovie> movie(this->movieBase.first());
					this->movieMutex.unlock();

					movie->setFileName(info.absoluteFilePath());
					if(movie->isValid()) {
						movie->setBackgroundColor(Qt::transparent);
						movie->jumpToNextFrame();
						QSize movieSize = movie->frameRect().size();
						if(movieSize.width() > this->screenSize.width() ||
						   movieSize.height() > this->screenSize.height()) {
							movieSize = movieSize.scaled(this->screenSize, Qt::KeepAspectRatio);
							movie->setScaledSize(movieSize);
						}

						connect(movie.data(), &QMovie::finished, movie.data(), &QMovie::start);
						imageObject.isMovie = true;
						imageObject.movie = movie;
						imageObject.fileName = info.absoluteFilePath();

						this->movieMutex.lock();
						this->movieBase.dequeue();
						this->movieMutex.unlock();
					}
				} else {
					QPixmap image(info.absoluteFilePath());
					if(!image.isNull()) {
						if(image.width() > this->screenSize.width() ||
						   image.height() > this->screenSize.height()) {
							image = image.scaled(this->screenSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
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
	} while(!this->isInterruptionRequested());
}
