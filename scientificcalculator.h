#ifndef SCIENTIFICCALCULATOR_H
#define SCIENTIFICCALCULATOR_H

#include "calculatorbase.h"

class ScientificCalculator : public CalculatorBase
{
    Q_OBJECT

public:
    explicit ScientificCalculator(QWidget *parent = nullptr);

public slots:
    void sinClicked();
    void cosClicked();
    void tanClicked();
    void ctanClicked();
    void logClicked();
    void lnClicked();
    void factorialClicked();
    void powerClicked();
    void piClicked();
    void eClicked();
    void TenInXClicked();
    void modulClicked();
    void modClicked();
    void leftParenClicked();
    void rightParenClicked();

private:
    void setupUI();
};

#endif // SCIENTIFICCALCULATOR_H
