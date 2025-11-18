QT += widgets
TARGET = My_Calculator
TEMPLATE = app

SOURCES += calculator.cpp \
           calculatorbase.cpp \
           main.cpp \
           mathoperations.cpp \
           mybutton.cpp \
           programmercalculator.cpp \
           scientificcalculator.cpp

HEADERS += calculator.h \
           calculatorbase.h \
           mathoperations.h \
           mybutton.h \
           programmercalculator.h \
           scientificcalculator.h

# Для правильной работы moc
CONFIG += c++11
