#ifndef SCIENTIFICCALCULATOR_H
#define SCIENTIFICCALCULATOR_H

#include "calculatorbase.h"
#include <QLineEdit>
#include <QButtonGroup>
#include <QRadioButton>

// Forward declaration
class QGridLayout;
class QVBoxLayout;

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
    void TenInXClicked();
    void modulClicked();
    void modClicked();

    void piClicked();
    void eClicked();

    void leftParenClicked() override;
    void rightParenClicked() override;

private:
    void setupScientificUI();
};

#endif // SCIENTIFICCALCULATOR_H
