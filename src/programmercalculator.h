#ifndef PROGRAMMERCALCULATOR_H
#define PROGRAMMERCALCULATOR_H

#include "calculatorbase.h"
#include <QLineEdit>
#include <QButtonGroup>
#include <QRadioButton>

class ProgrammerCalculator : public CalculatorBase
{
    Q_OBJECT

public:
    explicit ProgrammerCalculator(QWidget *parent = nullptr);
    ~ProgrammerCalculator() override = default;

    // Переопределенные методы из CalculatorBase
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
    void onBaseGroupButtonClicked(QAbstractButton* button);

    // Слоты для скобок
    void leftParenClicked() override;
    void rightParenClicked() override;

protected:
    void setupProgrammerUI();
    void updateNumberSystemDisplays();
    void updateNumberSystemButtons();
    void updateExpressionWithCurrentNumber();

    int m_current_base;
    QLineEdit *m_display_bin;
    QLineEdit *m_display_oct;
    QLineEdit *m_display_dec;
    QLineEdit *m_display_hex;

    MyButton *m_buttonA;
    MyButton *m_buttonB;
    MyButton *m_buttonC;
    MyButton *m_buttonD;
    MyButton *m_buttonE;
    MyButton *m_buttonF;
};

#endif // PROGRAMMERCALCULATOR_H
