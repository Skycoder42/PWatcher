#-------------------------------------------------
#
# Project created by QtCreator 2016-02-19T18:59:50
#
#-------------------------------------------------

QT       += core gui
TEMPLATE = app

QT += gui widgets

TARGET = PWatcher
VERSION = 1.0.0

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

SOURCES += main.cpp\
		controldialog.cpp \
	displaywindow.cpp \
	imageloader.cpp

HEADERS  += controldialog.h \
	displaywindow.h \
	imageloader.h

FORMS    += controldialog.ui

RESOURCES += \
	pwatcher_res.qrc
