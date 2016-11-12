#-------------------------------------------------
#
# Project created by QtCreator 2014-07-01T17:43:47
#
#-------------------------------------------------

QT       += core gui sql
CONFIG   += c++11

#QMAKE_MAC_SDK = macosx10.9 # all libraries should be biult in the same sdk - curvefit
#QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QTLog
TEMPLATE = app

CONFIG -= app_bundle


INCLUDEPATH += 

DEPENDPATH +=

SOURCES += \  
    MainWindow.cpp \
    main.cpp \
    Database.cpp \
    LogEntry.cpp \
    MergeWindow.cpp
HEADERS  += \
    Database.hpp \
    MainWindow.hpp \
    LogEntry.hpp \
    MergeWindow.hpp


QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE *= -O2
QMAKE_CXXFLAGS_RELEASE -= -Ofast
