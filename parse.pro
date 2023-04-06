#-------------------------------------------------
#
# Project created by QtCreator 2023-03-22T13:00:14
#
#-------------------------------------------------
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = parse
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp\
        parser.cpp\
        parsertable.cpp\
    ast.cpp \
    check.cpp



HEADERS  += mainwindow.h \
    token.h \
    unistd.h \
    parser.h  \
    parsertable_p.h \
    lex.yy.c \
    ast.h \
    check.h


DISTFILES += parse.l \ parse.g \
    sample.txt
FORMS    += mainwindow.ui
