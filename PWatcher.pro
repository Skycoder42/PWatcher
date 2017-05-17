TEMPLATE = app

QT += qml quick widgets
CONFIG += c++11

TARGET = pwatcher
VERSION = 1.1.0

DEFINES += "TARGET=\\\"$$TARGET\\\""
DEFINES += "VERSION=\\\"$$VERSION\\\""
DEFINES += QT_USE_QSTRINGBUILDER

win32 {
	RC_ICONS += ./icons/main.ico
	QMAKE_TARGET_COMPANY = "Skycoder42"
	QMAKE_TARGET_PRODUCT = $$TARGET
	QMAKE_TARGET_DESCRIPTION = $$QMAKE_TARGET_PRODUCT

	DEFINES += "COMPANY=\"\\\"$$QMAKE_TARGET_COMPANY\\\"\""
	DEFINES += "DISPLAY_NAME=\"\\\"$$QMAKE_TARGET_PRODUCT\\\"\""
} else {
	DEFINES += "COMPANY=\"\\\"Skycoder42\\\"\""
	DEFINES += "DISPLAY_NAME=\"\\\"$$TARGET\\\"\""
}

include(vendor/vendor.pri)

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp \
	imagemodel.cpp \
	cpphelper.cpp

RESOURCES += qml.qrc

# Default rules for deployment.
qnx:target.path = /tmp/$${TARGET}/bin
else:unix:!android: target.path = /usr/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
	imagemodel.h \
	cpphelper.h
