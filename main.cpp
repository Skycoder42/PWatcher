#include <QApplication>
#include <QQmlApplicationEngine>
#include <quickextras.h>

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication app(argc, argv);

	QQmlApplicationEngine engine;
	QuickExtras::setupEngine(&engine);
	engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));

	return app.exec();
}
