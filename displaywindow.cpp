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
	setWindowTitle(tr("Diashow"));
	setAttribute(Qt::WA_DeleteOnClose);
	setAlignment(Qt::AlignCenter);
	setScaledContents(false);
	setContextMenuPolicy(Qt::ActionsContextMenu);
	setAutoFillBackground(true);
	setMouseTracking(true);
	QPalette pal = palette();
	pal.setColor(QPalette::Window, Qt::black);
	pal.setColor(QPalette::WindowText, Qt::white);
	setPalette(pal);

	connect(this, &DisplayWindow::destroyed,
			qApp, &QApplication::quit);
	connect(parent, &QWidget::destroyed,
			this, &DisplayWindow::deleteLater);

	connect(displayTimer, &QTimer::timeout,
			this, &DisplayWindow::timerNext);
	connect(loader, &ImageLoader::loadReady,
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

	addActions({
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
		loader->updateMovieBase(movie);
	}

	//start cursor hiding timer
	mouseHideTimer->setSingleShot(true);
	mouseHideTimer->setInterval(1000);
	connect(mouseHideTimer, &QTimer::timeout,
			this, &DisplayWindow::hideMouse);
}

void DisplayWindow::setInfo(const DisplayWindow::ViewInfo &viewInfo)
{
	mainInfo = viewInfo;
	loader->resetData(viewInfo, QApplication::desktop()->screenGeometry(this).size());
	displayTimer->setInterval(viewInfo.waitDelay);
	imageQueue.clear();
	backCache.clear();
	showIndex = -1;
	if(viewInfo.files.isEmpty())
		setObject(tr("The selected directory has no loadable images!"));
	else
		setObject(tr("Loading images, please wait…"));
}

void DisplayWindow::startShow(bool blackOutScreens)
{
	showFullScreen();
	raise();
	activateWindow();
	displayTimer->start();
	mouseHideTimer->start();
	if(imageQueue.isEmpty())
		QTimer::singleShot(250, this, &DisplayWindow::showNext);
	else
		showNext();

	//"black out" all other screens
	if(blackOutScreens) {
		QList<QScreen*> screens = QApplication::screens();
		foreach(QScreen *screen, screens) {
			if(screen != windowHandle()->screen()) {
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
	displayTimer->stop();
	setObject(tr("No Image folder loaded!"));
	hide();
	controlParent->show();
	controlParent->raise();
	controlParent->activateWindow();
}

void DisplayWindow::mouseMoveEvent(QMouseEvent *ev)
{
	setCursor(Qt::ArrowCursor);
	mouseHideTimer->start();
	QLabel::mouseMoveEvent(ev);
}

void DisplayWindow::hideEvent(QHideEvent *ev)
{
	QLabel::hideEvent(ev);
	emit hidden();
}

void DisplayWindow::showNext()
{
	if(showIndex == backCache.lastIndex()) {
		if(!imageQueue.isEmpty()) {
			ImageObject obj = imageQueue.dequeue();
			loader->updateLoad();
			backCache.append(obj);
			showIndex++;
			setObject(obj);
		}
	} else {
		Q_ASSERT(backCache.containsIndex(showIndex + 1));
		setObject(backCache.at(++showIndex));
	}
}

void DisplayWindow::showPrevious()
{
	if(showIndex > backCache.firstIndex()) {
		Q_ASSERT(backCache.containsIndex(showIndex - 1));
		setObject(backCache.at(--showIndex));
	}
}

void DisplayWindow::imageLoaded(const ImageObject &object)
{
	if(object.isMovie) {
		QSharedPointer<QMovie> movie(new QMovie());
		loader->updateMovieBase(movie);
	}
	imageQueue.enqueue(object);
}

void DisplayWindow::updateMovieLoops(int currentFrame)
{
	Q_ASSERT(dynamic_cast<QMovie*>(QObject::sender()));
	if(currentFrame == (static_cast<QMovie*>(QObject::sender())->frameCount() - 1)) {
		if(++currentLoops >= mainInfo.minMovieLoop &&
		   shouldNext) {
			shouldNext = false;
			next();
		}
	}
}

void DisplayWindow::timerNext()
{
	if(loopCon) {
		if(currentLoops < mainInfo.minMovieLoop) {
			shouldNext = true;
			return;
		}
	}

	showNext();
}

void DisplayWindow::togglePlay()
{
	if(displayTimer->isActive())
		displayTimer->stop();
	else
		displayTimer->start();
}

void DisplayWindow::next()
{
	showNext();
	if(displayTimer->isActive())
		displayTimer->start();
}

void DisplayWindow::previous()
{
	showPrevious();
	if(displayTimer->isActive())
		displayTimer->start();
}

void DisplayWindow::deleteCurrent()
{
	Q_ASSERT(backCache.containsIndex(showIndex));
	QString path = backCache.at(showIndex).fileName;

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

	next();
}

void DisplayWindow::setObject(const DisplayWindow::ImageObject &object)
{
	QMovie *oldMovie = movie();
	if(oldMovie) {
		disconnect(loopCon);
		loopCon = QMetaObject::Connection();
		currentLoops = 0;
		oldMovie->stop();
	}

	if(object.isNull())
		setText(tr("Invalid image!"));
	else {
		if(object.isMovie) {
			loopCon = connect(object.movie.data(), &QMovie::frameChanged,
									this, &DisplayWindow::updateMovieLoops);
			object.movie->start();
			setMovie(object.movie.data());
		} else
			setPixmap(object.pixmap);
	}

	repaint();
}

void DisplayWindow::setObject(const QString &placeholder)
{
	QMovie *oldMovie = movie();
	if(oldMovie) {
		disconnect(loopCon);
		loopCon = QMetaObject::Connection();
		currentLoops = 0;
		oldMovie->stop();
	}
	setText(placeholder);
	repaint();
}

void DisplayWindow::hideMouse()
{
	setCursor(Qt::BlankCursor);
}



bool DisplayWindow::ImageObject::isNull() const
{
	if(isMovie)
		return movie.isNull();
	else
		return pixmap.isNull();
}



static QAction *seperator(QObject *parent)
{
	QAction *seperator = new QAction(parent);
	seperator->setSeparator(true);
	return seperator;
}
