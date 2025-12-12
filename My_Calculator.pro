QT += widgets
TARGET = My_Calculator
TEMPLATE = app

SOURCES += \
    src/MainWindow.cpp \
    src/calculatorbase.cpp \
    src/main.cpp \
    src/mathoperations.cpp \
    src/mybutton.cpp \
    src/programmercalculator.cpp \
    src/scientificcalculator.cpp

HEADERS += \
    src/MainWindow.h \
    src/calculatorbase.h \
    src/mathoperations.h \
    src/mybutton.h \
    src/programmercalculator.h \
    src/scientificcalculator.h

# Для правильной работы moc
CONFIG += c++11

# Отключаем отладочный вывод в релизной версии
CONFIG(release, debug|release): DEFINES += QT_NO_DEBUG_OUTPUT
