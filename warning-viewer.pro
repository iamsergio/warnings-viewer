######################################################################
# Automatically generated by qmake (3.0) Sat Dec 12 22:23:18 2015
######################################################################

TEMPLATE = app
TARGET = warning-viewer
INCLUDEPATH += .

# Input
SOURCES += main.cpp mainwindow.cpp warningmodel.cpp warningproxymodel.cpp tab.cpp \
           settingswindow.cpp \
           settings.cpp \
    clazychecks.cpp \
    warningtypefilterproxymodel.cpp

HEADERS += mainwindow.h warningmodel.h warningproxymodel.h tab.h \
           settingswindow.h \
           settings.h \
    clazychecks.h \
    warningtypefilterproxymodel.h

FORMS += mainwindow.ui \
         settings.ui

CONFIG += c++11
QT += widgets

RESOURCES += \
    resources.qrc
