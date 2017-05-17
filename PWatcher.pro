TEMPLATE = app

QT += qml quick widgets
CONFIG += c++11

TARGET = pwatcher
VERSION = 1.1.0

include(vendor/vendor.pri)

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp \
	imagemodel.cpp \
	cpphelper.cpp

RESOURCES += qml.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
	imagemodel.h \
	cpphelper.h
