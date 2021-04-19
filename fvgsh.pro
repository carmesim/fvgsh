TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        src/command.c \
        src/main.c \
        src/sighandler.c \
        src/strutils.c \
        src/tokens.c \
        src/userdata.c

HEADERS += \
    src/basictypes.h \
    src/command.h \
    src/sighandler.h \
    src/strutils.h \
    src/tokens.h \
    src/userdata.h
