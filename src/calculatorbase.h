#ifndef CALCULATORBASE_H
#define CALCULATORBASE_H

#include <QWidget>
#include <QLineEdit>
#include <QList>
#include "mathoperations.h"

// Forward declarations
class MyButton;
class QGridLayout;
class QVBoxLayout;

class CalculatorBase : public QWidget
{
    Q_OBJECT

public:
    explicit CalculatorBase(QWidget *parent = nullptr, bool setupDefaultUI = true);
    virtual ~CalculatorBase() = default;

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
    QLineEdit* createDisplay();
    QLineEdit* createHistoryDisplay();
    MyButton* createButton(const QString &text, const char *member);
    bool calculate(double operand);
    void updateHistoryDisplay();
    void resetHistoryDisplay();
    virtual void updateExpressionWithCurrentNumber();

    QString formatNumberForDisplay(double value);

protected:
    QLineEdit *m_display;
    QLineEdit *m_historyDisplay;

    double m_sum_in_memory;
    QString m_pending_operation;
    double m_stored_value;
    bool m_waiting_for_operand;
    double m_result;
    bool m_newCalculation;
    bool m_shouldUpdateHistory;

    QList<CalculationNode> m_expression;
    MyButton *m_digitButtons[10];

    bool m_uiInitialized;
};

#endif // CALCULATORBASE_H
