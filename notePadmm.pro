#-------------------------------------------------
#
# Project created by QtCreator 2017-08-17T19:30:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = notePadmm
TEMPLATE = app


SOURCES += main.cpp\
        NotePad.cpp \
    Content.cpp \
    AboutNotePad.cpp \
    CountString.cpp \
    Finddlg.cpp \
    ReplaceDlg.cpp \
    Config.cpp \
    Dlg.cpp

HEADERS  += NotePad.h \
    Content.h \
    AboutNotePad.h \
    CountString.h \
    Finddlg.h \
    ReplaceDlg.h \
    Config.h \
    Dlg.h

RESOURCES += \
    src.qrc

RC_FILE = app.rc
DISTFILES += \
    app.rc
