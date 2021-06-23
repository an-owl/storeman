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
    dialog.cpp \
    main.cpp \
    mainwindow.cpp \
    querysearch.cpp \
    return.cpp

HEADERS += \
    database.hpp \
    dialog.h \
    mainwindow.h \
    querysearch.h \
    return.h \
    stddef.hpp

FORMS += \
    dialog.ui \
    mainwindow.ui \
    return.ui

TRANSLATIONS += \
    storeman_en_NZ.ts
    ../build-storeman-Desktop-Debug/moc_mainwindow.cpp


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
