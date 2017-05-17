#include "cpphelper.h"

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include <quickextras.h>
#include <imagemodel.h>

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication app(argc, argv);
	QApplication::setApplicationName(TARGET);
	QApplication::setApplicationVersion(VERSION);
	QApplication::setOrganizationName(COMPANY);
	QApplication::setOrganizationDomain(QStringLiteral("de.skycoder42."));
	QApplication::setApplicationDisplayName(DISPLAY_NAME);
	QApplication::setWindowIcon(QIcon(QStringLiteral(":/icons/main.ico")));

	ImageModel model;

	qmlRegisterSingletonType<CppHelper>("de.skycoder42.pwatcher", 1, 1, "CppHelper", &CppHelper::createQmlSingleton);

	QQmlApplicationEngine engine;
	QuickExtras::setupEngine(&engine);

	engine.rootContext()->setContextProperty("imageModel", &model);
	engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));

	return app.exec();
}
