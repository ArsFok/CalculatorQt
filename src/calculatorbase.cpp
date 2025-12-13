#include "calculatorbase.h"
#include "forms/ui_basiccalculator.h"
#include <QPushButton>
#include <QDebug>

CalculatorBase::CalculatorBase(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::CalculatorBase),
      m_sum_in_memory(0.0),
      m_pending_operation(""),
      m_stored_value(0.0),
      m_waiting_for_operand(true),
      m_result(0.0),
      m_newCalculation(true),
      m_shouldUpdateHistory(false)
{
}

CalculatorBase::~CalculatorBase()
{
    delete ui;
}

void CalculatorBase::initialize()
{
    setupConnections();
}

void CalculatorBase::setupUI()
{
    ui->setupUi(this);

    setStyleSheet(
        "CalculatorBase {"
        "   background: #f0f8ff;"
        "   border: 3px solid #4682b4;"
        "   border-radius: 12px;"
        "}"
    );
}

void CalculatorBase::setupConnections()
{
    // Память (проверяем наличие кнопок)
    if (ui->clearMemoryButton) {
        connect(ui->clearMemoryButton, &QPushButton::clicked, this, &CalculatorBase::clearMemory);
    }
    if (ui->readMemoryButton) {
        connect(ui->readMemoryButton, &QPushButton::clicked, this, &CalculatorBase::readMemory);
    }
    if (ui->addToMemoryButton) {
        connect(ui->addToMemoryButton, &QPushButton::clicked, this, &CalculatorBase::addToMemory);
    }
    if (ui->minToMemoryButton) {
        connect(ui->minToMemoryButton, &QPushButton::clicked, this, &CalculatorBase::minToMemory);
    }

    // Цифры (общие для всех калькуляторов)
    QList<QPushButton*> digitButtons = {
        ui->button0, ui->button1, ui->button2, ui->button3, ui->button4,
        ui->button5, ui->button6, ui->button7, ui->button8, ui->button9
    };

    for (QPushButton* btn : digitButtons) {
        if (btn) {
            connect(btn, &QPushButton::clicked, this, &CalculatorBase::digitClicked);
        }
    }

    // Основные функции (проверяем наличие)
    if (ui->pointButton) {
        connect(ui->pointButton, &QPushButton::clicked, this, &CalculatorBase::pointClicked);
    }
    if (ui->changeSignButton) {
        connect(ui->changeSignButton, &QPushButton::clicked, this, &CalculatorBase::changeSignClicked);
    }
    if (ui->backspaceButton) {
        connect(ui->backspaceButton, &QPushButton::clicked, this, &CalculatorBase::backspaceClicked);
    }
    if (ui->clearButton) {
        connect(ui->clearButton, &QPushButton::clicked, this, &CalculatorBase::clear);
    }
    if (ui->clearAllButton) {
        connect(ui->clearAllButton, &QPushButton::clicked, this, &CalculatorBase::clearAll);
    }

    // Операторы (проверяем наличие)
    if (ui->plusButton) {
        connect(ui->plusButton, &QPushButton::clicked, this, &CalculatorBase::doubleOperandClicked);
    }
    if (ui->minusButton) {
        connect(ui->minusButton, &QPushButton::clicked, this, &CalculatorBase::doubleOperandClicked);
    }
    if (ui->multiplyButton) {
        connect(ui->multiplyButton, &QPushButton::clicked, this, &CalculatorBase::doubleOperandClicked);
    }
    if (ui->divideButton) {
        connect(ui->divideButton, &QPushButton::clicked, this, &CalculatorBase::doubleOperandClicked);
    }
    if (ui->equalButton) {
        connect(ui->equalButton, &QPushButton::clicked, this, &CalculatorBase::equalClicked);
    }

    // Унарные операции (проверяем наличие)
    if (ui->sqrtButton) {
        connect(ui->sqrtButton, &QPushButton::clicked, this, &CalculatorBase::unaryOperatorClicked);
    }
    if (ui->squareButton) {
        connect(ui->squareButton, &QPushButton::clicked, this, &CalculatorBase::unaryOperatorClicked);
    }
    if (ui->reciprocalButton) {
        connect(ui->reciprocalButton, &QPushButton::clicked, this, &CalculatorBase::unaryOperatorClicked);
    }
    if (ui->percentButton) {
        connect(ui->percentButton, &QPushButton::clicked, this, &CalculatorBase::unaryOperatorClicked);
    }
}

QString CalculatorBase::formatNumberForDisplay(double value)
{
    if (qIsNaN(value) || qIsInf(value)) {
        return "Error";
    }

    double intPart;
    double fracPart = std::modf(value, &intPart);

    if (qFuzzyIsNull(fracPart)) {
        return QString::number(static_cast<long long>(value));
    }

    QString result = QString::number(value, 'f', 15);

    if (result.contains('.')) {
        while (result.endsWith('0')) {
            result.chop(1);
        }
        if (result.endsWith('.')) {
            result.chop(1);
        }
    }

    double absValue = qAbs(value);
    if (absValue >= 1.0e12 || (absValue > 0 && absValue < 1.0e-10)) {
        return QString::number(value, 'g', 12);
    }

    return result;
}

void CalculatorBase::digitClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton) return;

    QString digit = clickedButton->text();
    handleDigitClick(digit);
}

void CalculatorBase::handleDigitClick(QString digit)
{
    QString currentText = ui->display->text();

    if (currentText == "Error") {
        clearAll();
        currentText = "0";
    }

    if (m_waiting_for_operand) {
        ui->display->clear();
        ui->display->setText(digit);
        m_waiting_for_operand = false;

        if (m_newCalculation) {
            m_expression.clear();
            m_newCalculation = false;
        }

        updateExpressionWithCurrentNumber();
        updateHistoryDisplay();
        return;
    }

    if (m_newCalculation) {
        ui->display->clear();
        ui->display->setText(digit);
        m_expression.clear();
        m_newCalculation = false;
        m_waiting_for_operand = false;

        updateExpressionWithCurrentNumber();
        updateHistoryDisplay();
        return;
    }

    if (currentText == "0") {
        ui->display->setText(digit);
    } else {
        ui->display->setText(currentText + digit);
    }

    updateExpressionWithCurrentNumber();
    updateHistoryDisplay();
}

void CalculatorBase::pointClicked()
{
    QString currentText = ui->display->text();

    if (currentText == "Error") {
        clearAll();
        currentText = "0";
    }

    if (m_waiting_for_operand) {
        ui->display->setText("0.");
        m_waiting_for_operand = false;
        updateHistoryDisplay();
        return;
    }

    if (m_newCalculation) {
        ui->display->setText("0.");
        m_expression.clear();
        m_newCalculation = false;
        m_waiting_for_operand = false;
        updateHistoryDisplay();
        return;
    }

    if (!currentText.contains('.')) {
        if (currentText.isEmpty()) {
            ui->display->setText("0.");
        } else {
            ui->display->setText(currentText + ".");
        }
        updateHistoryDisplay();
    }
}

void CalculatorBase::changeSignClicked()
{
    QString text = ui->display->text();
    if (text.isEmpty()) {
        text = "0";
    }

    bool ok;
    double value = text.toDouble(&ok);
    if (!ok) {
        ui->display->setText("Error");
        return;
    }

    double result = changeSign(value);
    ui->display->setText(formatNumberForDisplay(result));
}

void CalculatorBase::backspaceClicked()
{
    QString text = ui->display->text();
    if (text.isEmpty() || text == "0" || m_waiting_for_operand) {
        ui->display->setText("0");
        m_waiting_for_operand = true;
        return;
    }

    text.chop(1);
    if (text.isEmpty()) {
        ui->display->setText("0");
        m_waiting_for_operand = true;
    } else {
        ui->display->setText(text);
    }
}

void CalculatorBase::clearAll()
{
    ui->display->setText("0");
    ui->historyDisplay->setText("0");
    m_expression.clear();
    m_stored_value = 0.0;
    m_result = 0.0;
    m_waiting_for_operand = true;
    m_newCalculation = true;
    m_pending_operation = "";
}

void CalculatorBase::clear()
{
    ui->display->clear();
    m_waiting_for_operand = true;
}

void CalculatorBase::unaryOperatorClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton) return;

    QString operation = clickedButton->text();

    updateExpressionWithCurrentNumber();

    QString displayText = ui->display->text();

    if (displayText.contains("=")) {
        QStringList parts = displayText.split("=");
        if (parts.size() > 1) {
            displayText = parts.last().trimmed();
        }
    }

    bool ok;
    double operand = displayText.toDouble(&ok);
    if (!ok) {
        ui->display->setText("Error");
        return;
    }

    double result = 0.0;

    if (operation == "1/x") {
        result = reciprocal(operand);
    }
    else if (operation == "x²") {
        result = square(operand);
    }
    else if (operation == "√") {
        result = squareRoot(operand);
    }
    else if (operation == "%") {
        result = percent(operand);
    }

    if (qIsNaN(result)) {
        ui->display->setText("Error");
        return;
    }

    QString formattedResult = formatNumberForDisplay(result);
    ui->display->setText(formattedResult);

    if (!m_expression.isEmpty()) {
        m_expression.last().value = result;
    } else {
        m_expression.append({result, "", false});
    }

    updateHistoryDisplay();
    m_waiting_for_operand = false;
    m_newCalculation = true;
}

void CalculatorBase::doubleOperandClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton) return;

    QString operation = clickedButton->text();
    handleOperatorClick(operation);
}

void CalculatorBase::handleOperatorClick(QString operation)
{
    updateExpressionWithCurrentNumber();

    if (m_expression.isEmpty()) {
        m_expression.append({0.0, "", false});
        m_expression.append({0.0, operation, true});
    }
    else if (m_expression.last().isOperator) {
        if (m_expression.last().operation != "(" &&
            m_expression.last().operation != ")") {
            m_expression.last().operation = operation;
        } else {
            m_expression.append({0.0, operation, true});
        }
    }
    else {
        m_expression.append({0.0, operation, true});
    }

    m_pending_operation = operation;
    updateHistoryDisplay();

    ui->display->clear();
    m_waiting_for_operand = true;
}

void CalculatorBase::equalClicked()
{
    updateExpressionWithCurrentNumber();

    if (m_expression.isEmpty()) {
        QString currentText = ui->display->text();
        if (currentText.isEmpty() || currentText == "Error") {
            ui->display->setText("0");
        }
        m_waiting_for_operand = true;
        m_newCalculation = true;
        return;
    }

    double result = evaluateExpression(m_expression);

    if (qIsNaN(result) || qIsInf(result)) {
        ui->display->setText("Error");
        ui->historyDisplay->setText("Error");
        m_expression.clear();
        m_waiting_for_operand = true;
        m_newCalculation = true;
        return;
    }

    QString formattedResult = formatNumberForDisplay(result);
    ui->display->setText(formattedResult);

    QString historyText;
    for (const auto& node : qAsConst(m_expression)) {
        if (node.isOperator) {
            if (node.operation == "(" || node.operation == ")") {
                historyText += node.operation;
            } else {
                historyText += " " + node.operation + " ";
            }
        } else {
            historyText += formatNumberForDisplay(node.value);
        }
    }
    historyText += " = " + formattedResult;
    ui->historyDisplay->setText(historyText.trimmed());

    m_result = result;
    m_expression.clear();
    m_expression.append({result, "", false});

    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void CalculatorBase::updateHistoryDisplay()
{
    QString historyText;
    for (const auto& node : qAsConst(m_expression)) {
        if (node.isOperator) {
            if (node.operation == "(" || node.operation == ")") {
                historyText += node.operation;
            } else {
                historyText += " " + node.operation + " ";
            }
        } else {
            historyText += formatNumberForDisplay(node.value);
        }
    }

    historyText = historyText.trimmed();

    if (!historyText.isEmpty()) {
        ui->historyDisplay->setText(historyText);
    } else {
        ui->historyDisplay->setText("0");
    }
}

void CalculatorBase::resetHistoryDisplay()
{
    ui->historyDisplay->setText("0");
}

void CalculatorBase::clearMemory()
{
    m_sum_in_memory = 0.0;
}

void CalculatorBase::readMemory()
{
    ui->display->setText(formatNumberForDisplay(m_sum_in_memory));
    m_waiting_for_operand = false;
}

void CalculatorBase::addToMemory()
{
    bool ok;
    double value = ui->display->text().toDouble(&ok);
    if (!ok) {
        ui->display->setText("Error");
        return;
    }
    m_sum_in_memory += value;
}

void CalculatorBase::minToMemory()
{
    bool ok;
    double value = ui->display->text().toDouble(&ok);
    if (!ok) {
        ui->display->setText("Error");
        return;
    }
    m_sum_in_memory -= value;
}

void CalculatorBase::updateExpressionWithCurrentNumber()
{
    QString currentText = ui->display->text();
    if (currentText.isEmpty() || currentText == "Error") {
        return;
    }

    bool ok;
    double value = currentText.toDouble(&ok);
    if (!ok) {
        return;
    }

    if (!m_expression.isEmpty() && !m_expression.last().isOperator) {
        m_expression.last().value = value;
    } else {
        m_expression.append({value, "", false});
    }
}
