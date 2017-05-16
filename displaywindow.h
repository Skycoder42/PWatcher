#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <QLabel>
#include <QString>
#include <QTimer>
#include <QFileInfo>
#include <QQueue>
#include <QContiguousCache>
#include <QSharedPointer>
#include <QPixmap>
class ImageLoader;

class DisplayWindow : public QLabel
{
	Q_OBJECT
public:
	struct ViewInfo {
		QFileInfoList files;
		bool random;
		bool repeat;
		int waitDelay;
		int minMovieLoop;
	};
	struct ImageObject {
		bool isMovie;
		QPixmap pixmap;
		QSharedPointer<QMovie> movie;
		QString fileName;

		bool isNull() const;
	};

	explicit DisplayWindow(QWidget *parent = Q_NULLPTR);

	void setInfo(const ViewInfo &viewInfo);

public slots:
	void startShow(bool blackOutScreens);
	void stopShow();

signals:
	void hidden();

protected:
	void mouseMoveEvent(QMouseEvent *ev);
	void hideEvent(QHideEvent *ev);

private slots:
	void showNext();
	void showPrevious();
	void imageLoaded(const DisplayWindow::ImageObject &object);
	void updateMovieLoops(int currentFrame);

	void timerNext();
	void togglePlay();
	void next();
	void previous();
	void deleteCurrent();

	void setObject(const ImageObject &object);
	void setObject(const QString &placeholder);

	void hideMouse();

private:
	QWidget *controlParent;
	QTimer *displayTimer;
	ImageLoader *loader;

	ViewInfo mainInfo;

	QQueue<ImageObject> imageQueue;
	QContiguousCache<ImageObject> backCache;
	int showIndex;

	QMetaObject::Connection loopCon;
	int currentLoops;
	bool shouldNext;

	QTimer *mouseHideTimer;
};

Q_DECLARE_METATYPE(DisplayWindow::ImageObject)

#endif // DISPLAYWINDOW_H
