TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    uart.c \
    util.c

HEADERS += \
    def.h \
    uart.h \
    util.h
