#-------------------------------------------------
#
# Project created by QtCreator 2012-02-10T20:39:47
#
#-------------------------------------------------

QT       += core gui

TARGET = kabedoneexplosion
TEMPLATE = app
macx{
LIBS += -L/Users/oonomakoto/bass24-osx -lbass

INCLUDEPATH += /Users/oonomakoto/bass24-osx
}
win32{
LIBS += C:\bass24\c\bass.lib

INCLUDEPATH += C:\bass24\c
}


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h




