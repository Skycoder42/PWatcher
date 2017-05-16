#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <quickextras.h>
#include <imagemodel.h>

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication app(argc, argv);

	ImageModel model;

	QQmlApplicationEngine engine;
	QuickExtras::setupEngine(&engine);

	engine.rootContext()->setContextProperty("imageModel", &model);
	engine.load(QUrl(QLatin1String("qrc:/qml/main.qml")));

	return app.exec();
}
