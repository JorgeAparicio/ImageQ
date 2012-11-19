QT       += core gui

win32 {
  LIBS    += -lqwt

  LIBS    += -lopencv_core242.dll
  LIBS    += -lopencv_highgui242.dll
  LIBS    += -lopencv_imgproc242.dll
}

unix {
  CONFIG  +=  qwt

  LIBS    += -lopencv_core
  LIBS    += -lopencv_highgui
  LIBS    += -lopencv_imgproc
}

TARGET = ImageQ
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mat2qimage.cpp \
    histogram.cpp \
    aboutwindow.cpp \
    histogramwindow.cpp \
    blurwindow.cpp \
    cannywindow.cpp \
    gradientwindow.cpp \
    morphologywindow.cpp \
    thresholdwindow.cpp \
    image.cpp \
    imagelabel.cpp \
    setscalewindow.cpp \
    textlistwindow.cpp

HEADERS  += mainwindow.h \
    mat2qimage.h \
    histogram.h \
    aboutwindow.h \
    histogramwindow.h \
    blurwindow.h \
    cannywindow.h \
    gradientwindow.h \
    morphologywindow.h \
    thresholdwindow.h \
    image.h \
    imagelabel.h \
    setscalewindow.h \
    textlistwindow.h

FORMS    += mainwindow.ui \
    aboutwindow.ui \
    histogramwindow.ui \
    blurwindow.ui \
    cannywindow.ui \
    gradientwindow.ui \
    morphologywindow.ui \
    thresholdwindow.ui \
    image.ui \
    setscalewindow.ui \
    textlistwindow.ui

OTHER_FILES += \
    README.txt \
    COPYING.GPL3.txt
