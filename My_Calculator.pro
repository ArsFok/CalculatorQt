QT += widgets
TARGET = My_Calculator
TEMPLATE = app

SOURCES += \
    src/calculator.cpp \
    src/calculatorbase.cpp \
    src/main.cpp \
    src/mathoperations.cpp \
    src/mybutton.cpp \
    src/programmercalculator.cpp \
    src/scientificcalculator.cpp

HEADERS += \
    src/calculator.h \
    src/calculatorbase.h \
    src/mathoperations.h \
    src/mybutton.h \
    src/programmercalculator.h \
    src/scientificcalculator.h

# Для правильной работы moc
CONFIG += c++11
