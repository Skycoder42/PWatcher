#include "displaywindow.h"
#include <QApplication>
#include <QAction>
#include <QDesktopWidget>
#include <QMovie>
#include <QScreen>
#include <QWindow>
#ifdef Q_OS_WIN
#include <Windows.h>
#include <ShellAPI.h>
#endif
#include "imageloader.h"

static QAction *seperator(QObject *parent);

DisplayWindow::DisplayWindow(QWidget *parent) :
	QLabel(Q_NULLPTR, Qt::Window | Qt::FramelessWindowHint),
	controlParent(parent),
	displayTimer(new QTimer(this)),
	loader(new ImageLoader(this)),
	mainInfo(),
	backCache(42),
	showIndex(-1),
	loopCon(),
	currentLoops(),
	shouldNext(false),
	mouseHideTimer(new QTimer(this))
{
	this->setWindowTitle(tr("Diashow"));
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setAlignment(Qt::AlignCenter);
	this->setScaledContents(false);
	this->setContextMenuPolicy(Qt::ActionsContextMenu);
	this->setAutoFillBackground(true);
	this->setMouseTracking(true);
	QPalette pal = this->palette();
	pal.setColor(QPalette::Window, Qt::black);
	pal.setColor(QPalette::WindowText, Qt::white);
	this->setPalette(pal);

	connect(this, &DisplayWindow::destroyed,
			qApp, &QApplication::quit);
	connect(parent, &QWidget::destroyed,
			this, &DisplayWindow::deleteLater);

	connect(this->displayTimer, &QTimer::timeout,
			this, &DisplayWindow::timerNext);
	connect(this->loader, &ImageLoader::loadReady,
			this, &DisplayWindow::imageLoaded, Qt::QueuedConnection);

	//create actions
	QAction *playAction = new QAction(tr("Play/Pause"), this);
	playAction->setShortcuts({Qt::Key_MediaTogglePlayPause, Qt::Key_Space});
	connect(playAction, &QAction::triggered,
			this, &DisplayWindow::togglePlay);
	QAction *nextAction = new QAction(tr("Next"), this);
	nextAction->setShortcuts({Qt::Key_MediaNext, Qt::Key_Right});
	connect(nextAction, &QAction::triggered,
			this, &DisplayWindow::next);
	QAction *previousAction = new QAction(tr("Previous"), this);
	previousAction->setShortcuts({Qt::Key_MediaPrevious, Qt::Key_Left});
	connect(previousAction, &QAction::triggered,
			this, &DisplayWindow::previous);

	QAction *deleteAction = new QAction(tr("Delete (Trash)"), this);
	deleteAction->setShortcut(QKeySequence::Delete);
	connect(deleteAction, &QAction::triggered,
			this, &DisplayWindow::deleteCurrent);

	QAction *closeAction = new QAction(tr("Close Diashow"), this);
	closeAction->setShortcut(QKeySequence::Cancel);
	connect(closeAction, &QAction::triggered,
			this, &DisplayWindow::stopShow);
	QAction *quitAction = new QAction(tr("Quit"), this);
#ifdef Q_OS_WIN
	quitAction->setShortcut(Qt::ALT | Qt::Key_F4);
#else
	quitAction->setShortcut(QKeySequence::Close);
#endif
	connect(quitAction, &QAction::triggered,
			this, &DisplayWindow::close);

	this->addActions({
						 playAction,
						 nextAction,
						 previousAction,
						 seperator(this),
						 deleteAction,
						 seperator(this),
						 closeAction,
						 quitAction
					 });

	for(int i = 0; i < ImageLoader::PreloadAmount; ++i) {
		QSharedPointer<QMovie> movie(new QMovie());
		this->loader->updateMovieBase(movie);
	}

	//start cursor hiding timer
	this->mouseHideTimer->setSingleShot(true);
	this->mouseHideTimer->setInterval(1000);
	connect(this->mouseHideTimer, &QTimer::timeout,
			this, &DisplayWindow::hideMouse);
}

void DisplayWindow::setInfo(const DisplayWindow::ViewInfo &viewInfo)
{
	this->mainInfo = viewInfo;
	this->loader->resetData(viewInfo, QApplication::desktop()->screenGeometry(this).size());
	this->displayTimer->setInterval(viewInfo.waitDelay);
	this->imageQueue.clear();
	this->backCache.clear();
	this->showIndex = -1;
	if(viewInfo.files.isEmpty())
		this->setObject(tr("The selected directory has no loadable images!"));
	else
		this->setObject(tr("Loading images, please wait…"));
}

void DisplayWindow::startShow(bool blackOutScreens)
{
	this->showFullScreen();
	this->raise();
	this->activateWindow();
	this->displayTimer->start();
	this->mouseHideTimer->start();
	if(this->imageQueue.isEmpty())
		QTimer::singleShot(250, this, &DisplayWindow::showNext);
	else
		this->showNext();

	//"black out" all other screens
	if(blackOutScreens) {
		QList<QScreen*> screens = QApplication::screens();
		foreach(QScreen *screen, screens) {
			if(screen != this->windowHandle()->screen()) {
				QWidget *blackScreen = new QWidget(this, Qt::Window);
				blackScreen->setAttribute(Qt::WA_DeleteOnClose);
				connect(this, &DisplayWindow::hidden,
						blackScreen, &QWidget::close);
				blackScreen->setAutoFillBackground(true);
				QPalette pal = blackScreen->palette();
				pal.setColor(QPalette::Window, Qt::black);
				blackScreen->setPalette(pal);
				blackScreen->show();
				blackScreen->windowHandle()->setScreen(screen);
				blackScreen->showFullScreen();
			}
		}
		QApplication::setActiveWindow(this);
	}
}

void DisplayWindow::stopShow()
{
	this->displayTimer->stop();
	this->setObject(tr("No Image folder loaded!"));
	this->hide();
	this->controlParent->show();
	this->controlParent->raise();
	this->controlParent->activateWindow();
}

void DisplayWindow::mouseMoveEvent(QMouseEvent *ev)
{
	this->setCursor(Qt::ArrowCursor);
	this->mouseHideTimer->start();
	this->QLabel::mouseMoveEvent(ev);
}

void DisplayWindow::hideEvent(QHideEvent *ev)
{
	this->QLabel::hideEvent(ev);
	emit hidden();
}

void DisplayWindow::showNext()
{
	if(this->showIndex == this->backCache.lastIndex()) {
		if(!this->imageQueue.isEmpty()) {
			ImageObject obj = this->imageQueue.dequeue();
			this->loader->updateLoad();
			this->backCache.append(obj);
			this->showIndex++;
			this->setObject(obj);
		}
	} else {
		Q_ASSERT(this->backCache.containsIndex(this->showIndex + 1));
		this->setObject(this->backCache.at(++this->showIndex));
	}
}

void DisplayWindow::showPrevious()
{
	if(this->showIndex > this->backCache.firstIndex()) {
		Q_ASSERT(this->backCache.containsIndex(this->showIndex - 1));
		this->setObject(this->backCache.at(--this->showIndex));
	}
}

void DisplayWindow::imageLoaded(const ImageObject &object)
{
	if(object.isMovie) {
		QSharedPointer<QMovie> movie(new QMovie());
		this->loader->updateMovieBase(movie);
	}
	this->imageQueue.enqueue(object);
}

void DisplayWindow::updateMovieLoops(int currentFrame)
{
	Q_ASSERT(dynamic_cast<QMovie*>(QObject::sender()));
	if(currentFrame == (static_cast<QMovie*>(QObject::sender())->frameCount() - 1)) {
		if(++this->currentLoops >= this->mainInfo.minMovieLoop &&
		   this->shouldNext) {
			this->shouldNext = false;
			this->next();
		}
	}
}

void DisplayWindow::timerNext()
{
	if(this->loopCon) {
		if(this->currentLoops < this->mainInfo.minMovieLoop) {
			this->shouldNext = true;
			return;
		}
	}

	this->showNext();
}

void DisplayWindow::togglePlay()
{
	if(this->displayTimer->isActive())
		this->displayTimer->stop();
	else
		this->displayTimer->start();
}

void DisplayWindow::next()
{
	this->showNext();
	if(this->displayTimer->isActive())
		this->displayTimer->start();
}

void DisplayWindow::previous()
{
	this->showPrevious();
	if(this->displayTimer->isActive())
		this->displayTimer->start();
}

void DisplayWindow::deleteCurrent()
{
	Q_ASSERT(this->backCache.containsIndex(this->showIndex));
	QString path = this->backCache.at(this->showIndex).fileName;

#ifdef Q_OS_WIN
	path.append(QLatin1Char('\0'));
	SHFILEOPSTRUCT shfos = {};
	shfos.hwnd   = Q_NULLPTR;       // handle to window that will own generated windows, if applicable
	shfos.wFunc  = FO_DELETE;
	shfos.pFrom  = (wchar_t*)path.utf16();
	shfos.pTo    = Q_NULLPTR;       // not used for deletion operations
	shfos.fFlags = FOF_ALLOWUNDO | FOF_NO_UI; // use the recycle bin

	const int retVal = SHFileOperation(&shfos);
	if (retVal != 0)
		qWarning("Failed to move file to trash");
#else
//#error "NOT IMPLEMENTED"
#endif

	this->next();
}

void DisplayWindow::setObject(const DisplayWindow::ImageObject &object)
{
	QMovie *oldMovie = this->movie();
	if(oldMovie) {
		this->disconnect(this->loopCon);
		this->loopCon = QMetaObject::Connection();
		this->currentLoops = 0;
		oldMovie->stop();
	}

	if(object.isNull())
		this->setText(tr("Invalid image!"));
	else {
		if(object.isMovie) {
			this->loopCon = connect(object.movie.data(), &QMovie::frameChanged,
									this, &DisplayWindow::updateMovieLoops);
			object.movie->start();
			this->setMovie(object.movie.data());
		} else
			this->setPixmap(object.pixmap);
	}

	this->repaint();
}

void DisplayWindow::setObject(const QString &placeholder)
{
	QMovie *oldMovie = this->movie();
	if(oldMovie) {
		this->disconnect(this->loopCon);
		this->loopCon = QMetaObject::Connection();
		this->currentLoops = 0;
		oldMovie->stop();
	}
	this->setText(placeholder);
	this->repaint();
}

void DisplayWindow::hideMouse()
{
	this->setCursor(Qt::BlankCursor);
}



bool DisplayWindow::ImageObject::isNull() const
{
	if(this->isMovie)
		return this->movie.isNull();
	else
		return this->pixmap.isNull();
}



static QAction *seperator(QObject *parent)
{
	QAction *seperator = new QAction(parent);
	seperator->setSeparator(true);
	return seperator;
}
