#include "cpphelper.h"
#include <QCursor>

CppHelper::CppHelper(QObject *parent) :
	QObject(parent)
{}

QObject *CppHelper::createQmlSingleton(QQmlEngine *engine, QJSEngine *scriptEngine)
{
	Q_UNUSED(scriptEngine);
	return new CppHelper(engine);
}

void CppHelper::moveCursor()
{
	QCursor::setPos(QCursor::pos() + QPoint(0, 1));
}
