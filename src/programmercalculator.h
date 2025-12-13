#ifndef PROGRAMMERCALCULATOR_H
#define PROGRAMMERCALCULATOR_H

#include "calculatorbase.h"
#include <QLineEdit>

namespace Ui {
class ProgrammerCalculator;
}

class ProgrammerCalculator : public CalculatorBase
{
    Q_OBJECT

public:
    explicit ProgrammerCalculator(QWidget *parent = nullptr);
    ~ProgrammerCalculator();

    void initialize() override;

    // Переопределенные методы
    void digitClicked() override;
    void pointClicked() override;
    void changeSignClicked() override;
    void backspaceClicked() override;
    void clear() override;
    void clearAll() override;
    void unaryOperatorClicked() override;
    void doubleOperandClicked() override;
    void equalClicked() override;

public slots:
    void numberSystemDigitClicked();
    void bitwiseOperationClicked();
    void onBaseGroupButtonClicked();
    void leftParenClicked() override;
    void rightParenClicked() override;

protected:
    void setupUI() override;
    void setupConnections() override;
    void updateNumberSystemDisplays();
    void updateNumberSystemButtons();
    void updateExpressionWithCurrentNumber() override;

private:
    Ui::ProgrammerCalculator *ui;
    int m_current_base;
};

#endif // PROGRAMMERCALCULATOR_H
