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

    // Разделенные функции setupScientificUI
    void createScientificButtons();
    void setupScientificButtonStyles();
    void arrangeScientificButtons();

private:
    // Кнопки для научных функций
    MyButton *m_sinButton;
    MyButton *m_cosButton;
    MyButton *m_tanButton;
    MyButton *m_ctanButton;
    MyButton *m_piButton;
    MyButton *m_logButton;
    MyButton *m_lnButton;
    MyButton *m_powerButton;
    MyButton *m_tenPowerButton;
    MyButton *m_eButton;
    MyButton *m_factorialButton;
    MyButton *m_sqrtButton;
    MyButton *m_squareButton;
    MyButton *m_reciprocalButton;
    MyButton *m_absButton;
    MyButton *m_leftParenButton;
    MyButton *m_rightParenButton;
    MyButton *m_modButton;
    MyButton *m_percentButton;
    MyButton *m_divisionButton;
    MyButton *m_timesButton;
    MyButton *m_minusButton;
    MyButton *m_plusButton;
    MyButton *m_equalButton;
    MyButton *m_pointButton;
    MyButton *m_changeSignButton;
    MyButton *m_backspaceButton;
    MyButton *m_clearButton;
    MyButton *m_clearAllButton;
};

#endif // SCIENTIFICCALCULATOR_H
