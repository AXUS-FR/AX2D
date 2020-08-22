#-------------------------------------------------
#
# Project created by QtCreator 2020-08-22T09:52:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT       += core
QT       += gui

greaterThan(QT_MAJOR_VERSION,4):QT += widgets printsupport
												  

CONFIG += c++11
CONFIG += console
CONFIG -= app_bundle

INCLUDEPATH += D:\opencv\build\include
INCLUDEPATH += D:\SFML\SFML-2.5.1\include

			 
			  

																   
																		   
																   
																	 
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

			   

SOURCES += \
    AxArc.cpp \
    AxBorder.cpp \
    AxCircle.cpp \
    AxLine.cpp \
    AxShape.cpp \
    AxTriangle.cpp \
    Vertice.cpp \
    bisection_intersection.cpp \
    clipper.cpp \
    common_functions.cpp \
    main.cpp \
    mainwindow.cpp \
					  

HEADERS += \
    AxArc.h \
    AxBorder.h \
    AxCircle.h \
    AxLine.h \
    AxShape.h \
    AxTriangle.h \
    Vertice.h \
    bisection_intersection.h \
    clipper.hpp \
    common_functions.h \
    mainwindow.h \
				
					

FORMS += \
    mainwindow.ui

DISTFILES +=

STATECHARTS +=
									  