#ifndef CALCULATORBASE_H
#define CALCULATORBASE_H

#include <QWidget>
#include "mathfunctions.h"

namespace Ui {
class CalculatorBase;
}

class CalculatorBase : public QWidget
{
    Q_OBJECT

public:
    explicit CalculatorBase(QWidget *parent = nullptr);
    virtual ~CalculatorBase();

    void setupCalculator();

public slots:
    virtual void digitClicked();
    virtual void pointClicked();
    virtual void changeSignClicked();
    virtual void backspaceClicked();
    virtual void clear();
    virtual void clearAll();
    virtual void unaryOperatorClicked();
    virtual void doubleOperandClicked();
    virtual void equalClicked();

    virtual void leftParenClicked() {}
    virtual void rightParenClicked() {}

    virtual void clearMemory();
    virtual void readMemory();
    virtual void addToMemory();
    virtual void minToMemory();

protected:
    virtual void setupUI();
    virtual void setupConnections();

    QString formatNumberForDisplay(double value);
    void updateHistoryDisplay();
    void resetHistoryDisplay();
    virtual void updateExpressionWithCurrentNumber();

    virtual void handleDigitClick(QString digit);
    virtual void handleOperatorClick(QString operation);

    Ui::CalculatorBase* getUI() { return ui; }

protected:
    Ui::CalculatorBase *ui;

    double m_sum_in_memory;
    QString m_pending_operation;
    double m_stored_value;
    bool m_waiting_for_operand;
    double m_result;
    bool m_newCalculation;
    bool m_shouldUpdateHistory;

    QList<CalculationNode> m_expression;
};

#endif // CALCULATORBASE_H
