#-------------------------------------------------
#
# Project created by QtCreator 2018-01-22T10:09:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Snake
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    snakebody.cpp \
    snake.cpp \
    snakethread.cpp

HEADERS  += widget.h \
    snakebody.h \
    snake.h \
    snakethread.h
