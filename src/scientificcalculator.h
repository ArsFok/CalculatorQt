#ifndef SCIENTIFICCALCULATOR_H
#define SCIENTIFICCALCULATOR_H

#include "calculatorbase.h"

namespace Ui {
class ScientificCalculator;
}

class ScientificCalculator : public CalculatorBase
{
    Q_OBJECT

public:
    explicit ScientificCalculator(QWidget *parent = nullptr);
    ~ScientificCalculator();

    void setupUI() override;
    void setupConnections() override;

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
    Ui::ScientificCalculator *scientificUi;
};

#endif // SCIENTIFICCALCULATOR_H
