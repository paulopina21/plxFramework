#-------------------------------------------------
#
# Project created by QtCreator 2013-11-16T11:20:00
#
#-------------------------------------------------

QT       += qml network sql
QT       += gui

#QMAKE_CXXFLAGS_DEBUG -= -O
#QMAKE_CXXFLAGS_DEBUG -= -O1
#QMAKE_CXXFLAGS_DEBUG -= -O2
#QMAKE_CXXFLAGS_DEBUG *= -O3

QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE *= -O3

TARGET = plxFW
TEMPLATE = lib
CONFIG += staticlib

SOURCES += plxfw.cpp \
    media/Song.cpp \
    media/mediaitem.cpp \
    media/Artist.cpp \
    media/Album.cpp \
    database/database.cpp \
    utils/xml.cpp \
    utils/image.cpp \
    utils/tinyxml/tinyxmlparser.cpp \
    utils/tinyxml/tinyxmlerror.cpp \
    utils/tinyxml/tinyxml.cpp \
    utils/tinyxml/tinystr.cpp \
    network/NetworkAccess.cpp \
    scraping/providers/htbackdrops.cpp \
    models/SubListedListModel.cpp \
    models/ListModel.cpp \
    application/Settings.cpp \
    media/Catalog.cpp \
    application/GuiApplication.cpp \
    application/QuickApplication.cpp \
    utils/FrequencyMonitor.cpp \
    utils/PerformanceMonitor.cpp \
    filesystem/Rcc.cpp \
    utils/log.cpp

HEADERS += plxfw.h \
    media/Song.h \
    media/mediaitem.h \
    media/Artist.h \
    media/Album.h \
    database/database.h \
    utils/xml.h \
    utils/image.h \
    utils/tinyxml/tinyxml.h \
    utils/tinyxml/tinystr.h \
    network/NetworkAccess.h \
    scraping/providers/htbackdrops.h \
    scraping/Provider.h \
    models/SubListedListItem.h \
    models/ListModel.h \
    models/ListItem.h \
    models/SubListedListModel.h \
    concurrent/Worker.h \
    utils/log.h \
    application/Settings.h \
    media/Catalog.h \
    application/GuiApplication.h \
    application/QuickApplication.h \
    utils/FrequencyMonitor.h \
    utils/PerformanceMonitor.h \
    filesystem/Rcc.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

android {
  message(ANDROID BUILD SELECTED)

 SOURCES += \
    filesystem/mountprovider.cpp
 HEADERS += \
    filesystem/mountprovider.h
}

macx {
  message(UNIX BUILD SELECTED)
  INCLUDEPATH += /usr/local/include
  INCLUDEPATH += /usr/include

  LIBS += -L/usr/local/lib
  LIBS += -L/usr/lib
  LIBS += -ltag
}

windows {
  message(WINDOWS BUILD SELECTED)
  INCLUDEPATH += c:/usr/include

  LIBS += -Lc:/usr/lib
  LIBS += -ltag
}

message(The project will be built in $$OUT_PWD)


