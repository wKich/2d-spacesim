QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 2d-spacesim
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++0x

RESOURCES += \
    main.qrc

HEADERS += \
    mainwindow.h \
    spaceship.h

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    spaceship.cpp

OTHER_FILES += \
    main.frag \
    main.vert \
    bg.frag
