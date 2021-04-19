TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        src/main.c \
        src/sighandler.c \
        src/strutils.c \
        src/userdata.c

HEADERS += \
    src/command.h \
    src/sighandler.h \
    src/strutils.h \
    src/types.h \
    src/userdata.h
