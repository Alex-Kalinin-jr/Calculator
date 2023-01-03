QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cc \
    mainwindow.cc \
    qcustomplot.cpp \
    replenishments.cc \
    s21_controller.cc \
    s21_credit_model.cc \
    s21_depo_model.cc \
    s21_model.cc

HEADERS += \
    mainwindow.h \
    qcustomplot.h \
    replenishments.h \
    s21_controller.h \
    s21_credit_model.h \
    s21_depo_model.h \
    s21_error_handler.h \
    s21_model.h

FORMS += \
    mainwindow.ui \
    replenishments.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
