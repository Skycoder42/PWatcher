#ifndef CPPHELPER_H
#define CPPHELPER_H

#include <QObject>
#include <QQmlEngine>

class CppHelper : public QObject
{
	Q_OBJECT

public:
	explicit CppHelper(QObject *parent = nullptr);

	static QObject *createQmlSingleton(QQmlEngine *engine, QJSEngine *scriptEngine);

public slots:
	void moveCursor();
};

#endif // CPPHELPER_H
