QT       += \
    core gui\
    sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    database.cpp \
    main.cpp \
    mainwindow.cpp \
    #mainwindow.ui

HEADERS += \
    database.hpp \
    mainwindow.h \
    stddef.hpp

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    storeman_en_NZ.ts
    ../build-storeman-Desktop-Debug/moc_mainwindow.cpp

OTHER_FILES += \
    ../build-storeman-Desktop-Debug/ui_mainwindow.h
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
