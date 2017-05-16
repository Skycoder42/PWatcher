#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <QThread>
#include <QSemaphore>
#include <QMutex>
#include <QPixmap>
#include <QQueue>
#include "displaywindow.h"

class ImageLoader : public QThread
{
	Q_OBJECT

public:
	static const int PreloadAmount = 10;

	explicit ImageLoader(QObject *parent = Q_NULLPTR);
	~ImageLoader();

	void updateLoad();
	void resetData(const DisplayWindow::ViewInfo &mainInfo,
				   const QSize &screenSize);

	void updateMovieBase(const QSharedPointer<QMovie> &movieBase);

signals:
	void loadReady(const DisplayWindow::ImageObject &object);

protected:
	// QThread interface
	void run() Q_DECL_OVERRIDE;

private:
	QSemaphore loaderSemaphore;
	QQueue<QSharedPointer<QMovie>> movieBase;
	QMutex movieMutex;

	DisplayWindow::ViewInfo mainInfo;
	QSize screenSize;
	int currentIndex;

	DisplayWindow::ViewInfo nextInfo;
	QSize nextScreenSize;
	bool hasNextInfo;
};

#endif // IMAGELOADER_H
