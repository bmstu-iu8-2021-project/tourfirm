QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    backgroundsetting.cpp \
    buttoncolor.cpp \
    client.cpp \
    colorerror1.cpp \
    colorsetting.cpp \
    main.cpp \
    mainwindow.cpp \
    menu.cpp \
    msgwarning.cpp \
    multiplier.cpp \
    reservation.cpp \
    scaledpixmap.cpp \
    sha512.cpp \
    tour.cpp

HEADERS += \
    backgroundsetting.h \
    buttoncolor.h \
    client.h \
    clientFunctions.h \
    colorerror1.h \
    colorsetting.h \
    mainwindow.h \
    menu.h \
    msgwarning.h \
    multiplier.h \
    multipliersFunctions.h \
    reservation.h \
    reservationsFunctions.h \
    scaledpixmap.h \
    sha512.h \
    tour.h \
    tourFunctions.h

FORMS += \
    backgroundsetting.ui \
    buttoncolor.ui \
    client.ui \
    colorerror1.ui \
    colorsetting.ui \
    mainwindow.ui \
    menu.ui \
    msgwarning.ui \
    multiplier.ui \
    reservation.ui \
    tour.ui

TRANSLATIONS += \
    kurs_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
