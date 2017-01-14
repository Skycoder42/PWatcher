#include "controldialog.h"
#include <QApplication>
#include <QIcon>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QApplication::setApplicationName(TARGET);
	QApplication::setApplicationVersion(VERSION);
	QApplication::setOrganizationName(COMPANY);
	QApplication::setOrganizationDomain(QStringLiteral("skynet.is-an-engineer.com"));
	QApplication::setApplicationDisplayName(DISPLAY_NAME);
	QApplication::setWindowIcon(QIcon(QStringLiteral(":/icons/main.ico")));
	QApplication::setQuitOnLastWindowClosed(false);

	qRegisterMetaType<DisplayWindow::ImageObject>();

	ControlDialog w;
	QObject::connect(&w, &ControlDialog::rejected, qApp, &QApplication::quit);

	if(QApplication::arguments().size() > 1)
		w.setPath(QApplication::arguments()[1]);

	w.show();
	return a.exec();
}
