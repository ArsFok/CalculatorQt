#ifndef PROGRAMMERCALCULATOR_H
#define PROGRAMMERCALCULATOR_H

#include "calculatorbase.h"

class ProgrammerCalculator : public CalculatorBase
{
    Q_OBJECT

public:
    explicit ProgrammerCalculator(QWidget *parent = nullptr);

public slots:
    void numberSystemDigitClicked();
    void bitwiseOperationClicked();
    void updateNumberSystemDisplays();
    void updateNumberSystemButtons();
    void onBaseGroupButtonClicked(QAbstractButton* button);

private:
    void setupUI();

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
