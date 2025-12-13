#include "programmercalculator.h"
#include "ui_programmercalculator.h"
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>
#include <QDebug>

ProgrammerCalculator::ProgrammerCalculator(QWidget *parent)
    : CalculatorBase(parent),
      programmerUi(new Ui::ProgrammerCalculator),
      m_current_base(10)
{
}

ProgrammerCalculator::~ProgrammerCalculator()
{
    delete programmerUi;
}


void ProgrammerCalculator::setupUI()
{
    programmerUi->setupUi(this);

    setStyleSheet(
        "ProgrammerCalculator {"
        "   background: #fff0f5;"
        "   border: 3px solid #db7093;"
        "   border-radius: 12px;"
        "}"
    );

    // Настраиваем дисплеи систем счисления
    programmerUi->display_bin->setReadOnly(true);
    programmerUi->display_oct->setReadOnly(true);
    programmerUi->display_dec->setReadOnly(true);
    programmerUi->display_hex->setReadOnly(true);

    setupConnections();
    updateNumberSystemButtons();
    updateNumberSystemDisplays();
}

void ProgrammerCalculator::setupConnections()
{
    // Наследуем базовые подключения
    CalculatorBase::setupConnections();

    // Подключаем дополнительные кнопки
    connect(programmerUi->buttonA, &QPushButton::clicked, this, &ProgrammerCalculator::numberSystemDigitClicked);
    connect(programmerUi->buttonB, &QPushButton::clicked, this, &ProgrammerCalculator::numberSystemDigitClicked);
    connect(programmerUi->buttonC, &QPushButton::clicked, this, &ProgrammerCalculator::numberSystemDigitClicked);
    connect(programmerUi->buttonD, &QPushButton::clicked, this, &ProgrammerCalculator::numberSystemDigitClicked);
    connect(programmerUi->buttonE, &QPushButton::clicked, this, &ProgrammerCalculator::numberSystemDigitClicked);
    connect(programmerUi->buttonF, &QPushButton::clicked, this, &ProgrammerCalculator::numberSystemDigitClicked);

    // Подключаем радиокнопки систем счисления
    connect(programmerUi->binRadio, &QRadioButton::clicked, this, &ProgrammerCalculator::onBaseGroupButtonClicked);
    connect(programmerUi->octRadio, &QRadioButton::clicked, this, &ProgrammerCalculator::onBaseGroupButtonClicked);
    connect(programmerUi->decRadio, &QRadioButton::clicked, this, &ProgrammerCalculator::onBaseGroupButtonClicked);
    connect(programmerUi->hexRadio, &QRadioButton::clicked, this, &ProgrammerCalculator::onBaseGroupButtonClicked);

    // Скобки
    connect(programmerUi->leftParenButton, &QPushButton::clicked, this, &ProgrammerCalculator::leftParenClicked);
    connect(programmerUi->rightParenButton, &QPushButton::clicked, this, &ProgrammerCalculator::rightParenClicked);
}


void ProgrammerCalculator::unaryOperatorClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton) return;

    QString operation = clickedButton->text();

    // Обновляем текущее число в выражении
    updateExpressionWithCurrentNumber();

    QString displayText = programmerUi->display->text();

    // Убираем "=" из отображения если есть
    if (displayText.contains("=")) {
        QStringList parts = displayText.split("=");
        if (parts.size() > 1) {
            displayText = parts.last().trimmed();
        }
    }

    bool ok;
    double operand = 0;

    // Преобразуем ввод из текущей системы счисления
    if (m_current_base == 10) {
        operand = displayText.toDouble(&ok);
    } else {
        long long intVal = displayText.toLongLong(&ok, m_current_base);
        operand = static_cast<double>(intVal);
    }

    if (!ok) {
        programmerUi->display->setText("Error");
        updateNumberSystemDisplays();
        return;
    }

    double result = 0.0;

    if (operation == "%") {
        result = percent(operand);
    }

    if (qIsNaN(result)) {
        programmerUi->display->setText("Error");
        updateNumberSystemDisplays();
        return;
    }

    // Преобразуем результат в текущую систему счисления
    QString formattedResult;
    if (m_current_base == 10) {
        formattedResult = formatNumberForDisplay(result);
    } else {
        long long intResult = static_cast<long long>(result);
        formattedResult = QString::number(intResult, m_current_base).toUpper();
    }

    // Отображаем результат
    programmerUi->display->setText(formattedResult);

    // Обновляем выражение
    if (!m_expression.isEmpty()) {
        m_expression.last().value = result;
    } else {
        m_expression.append({result, "", false});
    }

    updateHistoryDisplay();
    updateNumberSystemDisplays();

    m_waiting_for_operand = false;
    m_newCalculation = true;
}

void ProgrammerCalculator::clear()
{
    programmerUi->display->clear();
    m_waiting_for_operand = true;
    updateNumberSystemDisplays();
}

void ProgrammerCalculator::clearAll()
{
    programmerUi->display->setText("0");
    programmerUi->historyDisplay->setText("0");
    m_expression.clear();
    m_stored_value = 0.0;
    m_result = 0.0;
    m_waiting_for_operand = true;
    m_newCalculation = true;
    m_pending_operation = "";
    updateNumberSystemDisplays();
}

void ProgrammerCalculator::backspaceClicked()
{
    QString text = programmerUi->display->text();
    if (text.isEmpty() || text == "0" || m_waiting_for_operand) {
        programmerUi->display->setText("0");
        m_waiting_for_operand = true;
        updateNumberSystemDisplays();
        return;
    }

    text.chop(1);
    if (text.isEmpty()) {
        programmerUi->display->setText("0");
        m_waiting_for_operand = true;
    } else {
        programmerUi->display->setText(text);
    }
    updateNumberSystemDisplays();
}

void ProgrammerCalculator::equalClicked()
{
    updateExpressionWithCurrentNumber();

    if (m_expression.isEmpty()) {
        QString currentText = programmerUi->display->text();
        if (currentText.isEmpty() || currentText == "Error") {
            programmerUi->display->setText("0");
        }
        m_waiting_for_operand = true;
        m_newCalculation = true;
        updateNumberSystemDisplays();
        return;
    }

    double result = evaluateExpression(m_expression);

    if (qIsNaN(result) || qIsInf(result)) {
        programmerUi->display->setText("Error");
        programmerUi->historyDisplay->setText("Error");
        m_expression.clear();
        m_waiting_for_operand = true;
        m_newCalculation = true;
        updateNumberSystemDisplays();
        return;
    }

    // Преобразуем результат в текущую систему счисления
    QString formattedResult;
    if (m_current_base == 10) {
        formattedResult = formatNumberForDisplay(result);
    } else {
        long long intResult = static_cast<long long>(result);
        formattedResult = QString::number(intResult, m_current_base).toUpper();
    }

    programmerUi->display->setText(formattedResult);

    // Показываем всё выражение и результат в истории
    QString historyText;
    for (const auto& node : qAsConst(m_expression)) {
        if (node.isOperator) {
            if (node.operation == "(" || node.operation == ")") {
                historyText += node.operation;
            } else {
                historyText += " " + node.operation + " ";
            }
        } else {
            // Форматируем числа в истории согласно текущей системе
            if (m_current_base == 10) {
                historyText += formatNumberForDisplay(node.value);
            } else {
                long long intVal = static_cast<long long>(node.value);
                historyText += QString::number(intVal, m_current_base).toUpper();
            }
        }
    }
    historyText += " = " + formattedResult;
    programmerUi->historyDisplay->setText(historyText.trimmed());

    // Сохраняем результат для возможного продолжения
    m_result = result;
    m_expression.clear();
    m_expression.append({result, "", false});

    m_waiting_for_operand = true;
    m_newCalculation = true;
    updateNumberSystemDisplays();
}

void ProgrammerCalculator::bitwiseOperationClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton) return;

    QString operation = clickedButton->text();

    bool ok;
    long currentValue = programmerUi->display->text().toLong(&ok, m_current_base);
    if (!ok) {
        programmerUi->display->setText("Error");
        updateNumberSystemDisplays();
        return;
    }

    long storedValue = static_cast<long>(m_stored_value);
    long result = 0;

    if (operation == "AND") {
        result = currentValue & storedValue;
    }
    else if (operation == "OR") {
        result = currentValue | storedValue;
    }
    else if (operation == "XOR") {
        result = currentValue ^ storedValue;
    }
    else if (operation == "NOT") {
        result = ~currentValue;
    }
    else if (operation == "<<") {
        result = currentValue << 1;
    }
    else if (operation == ">>") {
        result = currentValue >> 1;
    }

    programmerUi->display->setText(QString::number(result, m_current_base).toUpper());
    updateNumberSystemDisplays();
}

void ProgrammerCalculator::digitClicked()
{
    CalculatorBase::digitClicked();
    updateNumberSystemDisplays();
}

void ProgrammerCalculator::numberSystemDigitClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton) return;

    QString digit = clickedButton->text();

    // Проверяем, допустима ли цифра для текущей системы счисления
    bool isValid = false;
    if (digit >= "0" && digit <= "9") {
        int num = digit.toInt();
        isValid = (num < m_current_base);
    } else if (digit >= "A" && digit <= "F") {
        isValid = (m_current_base == 16);
    }

    if (!isValid) {
        return;
    }

    handleDigitClick(digit);
    updateNumberSystemDisplays();
}

void ProgrammerCalculator::pointClicked()
{
    QString currentText = programmerUi->display->text();

    if (currentText.isEmpty() || currentText == "0" || m_waiting_for_operand) {
        programmerUi->display->setText("0,");
    } else if (!currentText.contains(',')) {
        programmerUi->display->setText(currentText + ",");
    }

    updateNumberSystemDisplays();
}

void ProgrammerCalculator::changeSignClicked()
{
    QString text = programmerUi->display->text();
    if (text.isEmpty()) {
        text = "0";
    }

    bool ok;
    double value = 0;

    if (m_current_base == 10) {
        value = text.toDouble(&ok);
    } else {
        long long intVal = text.toLongLong(&ok, m_current_base);
        value = static_cast<double>(intVal);
    }

    if (!ok) {
        programmerUi->display->setText("Error");
        updateNumberSystemDisplays();
        return;
    }

    double result = changeSign(value);

    QString formattedResult;
    if (m_current_base == 10) {
        formattedResult = formatNumberForDisplay(result);
    } else {
        long long intResult = static_cast<long long>(result);
        formattedResult = QString::number(intResult, m_current_base).toUpper();
    }

    programmerUi->display->setText(formattedResult);
    updateNumberSystemDisplays();
}

void ProgrammerCalculator::onBaseGroupButtonClicked()
{
    if (programmerUi->binRadio->isChecked()) m_current_base = 2;
    else if (programmerUi->octRadio->isChecked()) m_current_base = 8;
    else if (programmerUi->decRadio->isChecked()) m_current_base = 10;
    else if (programmerUi->hexRadio->isChecked()) m_current_base = 16;

    QString currentText = programmerUi->display->text();
    if (currentText != "0" && currentText != "Error") {
        bool ok;
        long long value = currentText.toLongLong(&ok, m_current_base);
        if (ok) {
            if (m_current_base == 10) {
                programmerUi->display->setText(QString::number(value));
            } else {
                programmerUi->display->setText(QString::number(value, m_current_base).toUpper());
            }
        }
    } else {
        programmerUi->display->setText("0");
    }

    updateNumberSystemButtons();
    updateNumberSystemDisplays();
}

void ProgrammerCalculator::updateNumberSystemDisplays()
{
    QString input = programmerUi->display->text();

    if (input.isEmpty() || input == "Error" || input == "0") {
        programmerUi->display_bin->setText("0");
        programmerUi->display_oct->setText("0");
        programmerUi->display_dec->setText("0");
        programmerUi->display_hex->setText("0");
        return;
    }

    bool ok;
    long long num = 0;

    if (input.startsWith("-")) {
        QString absInput = input.mid(1);
        num = absInput.toLongLong(&ok, m_current_base);
        if (ok) num = -num;
    } else {
        num = input.toLongLong(&ok, m_current_base);
    }

    if (!ok) {
        programmerUi->display_bin->setText("Error");
        programmerUi->display_oct->setText("Error");
        programmerUi->display_dec->setText("Error");
        programmerUi->display_hex->setText("Error");
        return;
    }

    programmerUi->display_bin->setText(QString::number(num, 2));
    programmerUi->display_oct->setText(QString::number(num, 8));
    programmerUi->display_dec->setText(QString::number(num, 10));
    programmerUi->display_hex->setText(QString::number(num, 16).toUpper());
}

void ProgrammerCalculator::updateNumberSystemButtons()
{
    bool hexMode = (m_current_base == 16);
    bool octMode = (m_current_base == 8);
    bool binMode = (m_current_base == 2);

    // Включаем/выключаем кнопки A-F
    programmerUi->buttonA->setEnabled(hexMode);
    programmerUi->buttonB->setEnabled(hexMode);
    programmerUi->buttonC->setEnabled(hexMode);
    programmerUi->buttonD->setEnabled(hexMode);
    programmerUi->buttonE->setEnabled(hexMode);
    programmerUi->buttonF->setEnabled(hexMode);

    // Включаем/выключаем цифровые кнопки
    QList<QPushButton*> digitButtons = {
        programmerUi->button0, programmerUi->button1, programmerUi->button2, programmerUi->button3, programmerUi->button4,
        programmerUi->button5, programmerUi->button6, programmerUi->button7, programmerUi->button8, programmerUi->button9
    };

    for (int i = 0; i < digitButtons.size(); ++i) {
        if (binMode) {
            digitButtons[i]->setEnabled(i <= 1);
        } else if (octMode) {
            digitButtons[i]->setEnabled(i <= 7);
        } else {
            digitButtons[i]->setEnabled(true);
        }
    }
}

void ProgrammerCalculator::updateExpressionWithCurrentNumber()
{
    QString currentText = programmerUi->display->text();
    if (currentText.isEmpty() || currentText == "Error") {
        return;
    }

    bool ok;
    double value = 0;

    if (m_current_base == 10) {
        value = currentText.toDouble(&ok);
    } else {
        long long intVal = currentText.toLongLong(&ok, m_current_base);
        value = static_cast<double>(intVal);
    }

    if (!ok) {
        return;
    }

    if (!m_expression.isEmpty() && !m_expression.last().isOperator) {
        m_expression.last().value = value;
    } else {
        m_expression.append({value, "", false});
    }
}

void ProgrammerCalculator::doubleOperandClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton) return;

    QString operation = clickedButton->text();
    handleOperatorClick(operation);

    // Для программистского калькулятора также обновляем дисплеи
    updateNumberSystemDisplays();
}

void ProgrammerCalculator::leftParenClicked()
{
    CalculatorBase::leftParenClicked();
    updateNumberSystemDisplays();
}

void ProgrammerCalculator::rightParenClicked()
{
    CalculatorBase::rightParenClicked();
    updateNumberSystemDisplays();
}
