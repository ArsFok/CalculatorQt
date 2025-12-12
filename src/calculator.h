#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>
#include <QStackedWidget>
#include "calculatorbase.h"
#include "scientificcalculator.h"
#include "programmercalculator.h"

class Calculator : public QMainWindow
{
    Q_OBJECT

public:
    explicit Calculator(QWidget *parent = nullptr);
    ~Calculator();

private slots:
    void switchPage();
    void onStackedWidgetChanged(int index);

private:
    void createNavigation();

    QStackedWidget *m_stackedWidget;
    CalculatorBase *m_basicCalculator;
    ScientificCalculator *m_scientificCalculator;
    ProgrammerCalculator *m_programmerCalculator;

    QButtonGroup *m_navButtonGroup;
    QWidget *m_navContainer;
};

#endif // CALCULATOR_H
