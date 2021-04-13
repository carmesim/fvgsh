TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        src/main.c \
        src/sighandler.c

HEADERS += \
    src/sighandler.h \
    src/types.h
