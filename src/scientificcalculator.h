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

    // Метод для завершения инициализации
    void initialize() override;

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

protected:
    void setupUI() override;
    void setupConnections() override;

private:
    Ui::ScientificCalculator *ui;
};

#endif // SCIENTIFICCALCULATOR_H
