#ifndef CALCULATORBASE_H
#define CALCULATORBASE_H

#include <QWidget>
#include <QLineEdit>
#include <QList>
#include "mathoperations.h"
#include "mybutton.h"

class CalculatorBase : public QWidget
{
    Q_OBJECT

public:
    explicit CalculatorBase(QWidget *parent = nullptr);

public slots:
    void digitClicked();
    void pointClicked();
    void changeSignClicked();
    void backspaceClicked();
    void clear();
    void clearAll();
    void unaryOperatorClicked();
    void doubleOperandClicked();
    void equalClicked();

    // Память
    void clearMemory();
    void readMemory();
    void addToMemory();
    void minToMemory();

protected:
    QLineEdit* createDisplay();
    MyButton* createButton(const QString &text, const char *member);
    void showOperation(const QString &operationText, double result);
    bool calculate(double operand);

    QLineEdit *m_display;
    MathOperations m_mathOps;

    double m_sum_in_memory;
    QString m_pending_operation;
    double m_stored_value;
    bool m_waiting_for_operand;
    double m_result;
    bool m_newCalculation;

    QList<CalculationNode> m_expression;
    MyButton *m_digitButtons[10];

private:
    void setupUI();
};

#endif // CALCULATORBASE_H
