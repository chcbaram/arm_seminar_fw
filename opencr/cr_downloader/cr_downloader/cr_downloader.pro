TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    uart.c \
    util.c \
    cmd.c \
    boot.c \
    download.c

HEADERS += \
    def.h \
    uart.h \
    util.h \
    cmd.h \
    boot.h \
    download.h
