#-------------------------------------------------
#
# Project created by QtCreator 2016-02-19T18:59:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PWatcher
TEMPLATE = app
VERSION = 1.0.0

DEFINES += "TARGET=\\\"$$TARGET\\\""
DEFINES += "VERSION=\\\"$$VERSION\\\""
DEFINES += QT_USE_QSTRINGBUILDER

win32 {
	RC_ICONS += ./icons/main.ico
	QMAKE_TARGET_COMPANY = "Skycoder Soft"
	QMAKE_TARGET_PRODUCT = $$TARGET
	QMAKE_TARGET_DESCRIPTION = $$QMAKE_TARGET_PRODUCT

	DEFINES += "COMPANY=\"\\\"$$QMAKE_TARGET_COMPANY\\\"\""
	DEFINES += "DISPLAY_NAME=\"\\\"$$QMAKE_TARGET_PRODUCT\\\"\""
}

include(C:\C++Libraries\Qt\QAtomicBool\qatomicbool.pri)
include(C:\C++Libraries\Qt\QPathEdit\qpathedit.pri)

SOURCES += main.cpp\
        controldialog.cpp \
    displaywindow.cpp \
    imageloader.cpp \
    formatsdialog.cpp

HEADERS  += controldialog.h \
    displaywindow.h \
    imageloader.h \
    formatsdialog.h

FORMS    += controldialog.ui \
    formatsdialog.ui

RESOURCES += \
    pwatcher_res.qrc
