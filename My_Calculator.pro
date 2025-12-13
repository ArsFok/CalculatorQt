QT += widgets
TARGET = My_Calculator
TEMPLATE = app

FORMS += \
    src/forms/basiccalculator.ui \
    src/forms/scientificcalculator.ui \
    src/forms/programmercalculator.ui

SOURCES += \
    src/MainWindow.cpp \
    src/calculatorbase.cpp \
    src/main.cpp \
    src/mathfunctions.cpp \
    src/mybutton.cpp \
    src/programmercalculator.cpp \
    src/scientificcalculator.cpp

HEADERS += \
    src/MainWindow.h \
    src/calculatorbase.h \
    src/mathfunctions.h \
    src/mybutton.h \
    src/programmercalculator.h \
    src/scientificcalculator.h

CONFIG += c++11

# Папка для сгенерированных ui_*.h файлов
UI_DIR = src/forms
MOC_DIR = src/moc
OBJECTS_DIR = src/obj