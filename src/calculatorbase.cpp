#include "calculatorbase.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QDebug>

CalculatorBase::CalculatorBase(QWidget *parent, bool setupDefaultUI)
    : QWidget(parent),
      m_mathOps(this),
      m_sum_in_memory(0.0),
      m_pending_operation(""),
      m_stored_value(0.0),
      m_waiting_for_operand(true),
      m_result(0.0),
      m_newCalculation(true),
      m_shouldUpdateHistory(false),
      m_uiInitialized(false)
{
    qDebug() << "=== CalculatorBase constructor ===";
    qDebug() << "Expression size in constructor start:" << m_expression.size();

    for (int i = 0; i < 10; ++i) {
        m_digitButtons[i] = nullptr;
    }

    m_expression.clear();

    qDebug() << "Expression size after clear:" << m_expression.size();

    if (setupDefaultUI) {
        setupUI();
    }

    qDebug() << "=== End CalculatorBase constructor ===";
}

void CalculatorBase::setupUI()
{
    if (m_uiInitialized) {
        qDebug() << "UI already initialized for CalculatorBase";
        return;
    }

    m_uiInitialized = true;

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(0);

    setStyleSheet(
        "CalculatorBase {"
        "   background: #f0f8ff;"
        "   border: 3px solid #4682b4;"
        "   border-radius: 12px;"
        "}"
    );

    m_historyDisplay = createHistoryDisplay();
    mainLayout->addWidget(m_historyDisplay);

    m_display = createDisplay();
    mainLayout->addWidget(m_display);

    QGridLayout *buttonsLayout = new QGridLayout();
    buttonsLayout->setSpacing(6);
    buttonsLayout->setContentsMargins(15, 15, 15, 15);

    for(int i = 0; i < 10; ++i) {
        m_digitButtons[i] = createButton(QString::number(i), SLOT(digitClicked()));
    }

    QString digitButtonStyle =
        "QPushButton {"
        "   background-color: #87CEEB;"
        "   color: black;"
        "   border: 2px solid #45a049;"
        "   border-radius: 8px;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   min-width: 60px;"
        "   min-height: 50px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #45a049;"
        "   color: white;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #3d8b40;"
        "}";

    QString functionButtonStyle =
        "QPushButton {"
        "   background-color: #ffb6c1;"
        "   color: black;"
        "   border: 2px solid #ff69b4;"
        "   border-radius: 8px;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "   min-width: 60px;"
        "   min-height: 50px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #ff69b4;"
        "   color: white;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #ff1493;"
        "}";

    QString operationButtonStyle =
        "QPushButton {"
        "   background-color: #ffa500;"
        "   color: black;"
        "   border: 2px solid #ff8c00;"
        "   border-radius: 8px;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   min-width: 60px;"
        "   min-height: 50px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #ff8c00;"
        "   color: white;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #ff7f50;"
        "}";

    for (int i = 0; i < 10; ++i) {
        m_digitButtons[i]->setStyleSheet(digitButtonStyle);
    }

    MyButton *percentButton = createButton("%", SLOT(unaryOperatorClicked()));
    MyButton *pointButton = createButton(".", SLOT(pointClicked()));
    MyButton *changeSignButton = createButton("±", SLOT(changeSignClicked()));
    MyButton *backspaceButton = createButton("⌫", SLOT(backspaceClicked()));
    MyButton *clearButton = createButton("C", SLOT(clear()));
    MyButton *clearAllButton = createButton("CE", SLOT(clearAll()));

    MyButton *divisionButton = createButton("÷", SLOT(doubleOperandClicked()));
    MyButton *timesButton = createButton("×", SLOT(doubleOperandClicked()));
    MyButton *minusButton = createButton("-", SLOT(doubleOperandClicked()));
    MyButton *plusButton = createButton("+", SLOT(doubleOperandClicked()));

    MyButton *squareButton = createButton("√", SLOT(unaryOperatorClicked()));
    MyButton *powerButton = createButton("x²", SLOT(unaryOperatorClicked()));
    MyButton *reciprocalButton = createButton("1/x", SLOT(unaryOperatorClicked()));
    MyButton *equalButton = createButton("=", SLOT(equalClicked()));

    MyButton *clearMemoryButton = createButton("MC", SLOT(clearMemory()));
    MyButton *readMemoryButton = createButton("MR", SLOT(readMemory()));
    MyButton *addToMemoryButton = createButton("M+", SLOT(addToMemory()));
    MyButton *minToMemoryButton = createButton("M-", SLOT(minToMemory()));

    QList<MyButton*> functionButtons = {
        percentButton, pointButton, changeSignButton, backspaceButton,
        clearButton, clearAllButton, squareButton, powerButton, reciprocalButton,
        clearMemoryButton, readMemoryButton, addToMemoryButton, minToMemoryButton
    };

    for (MyButton *btn : functionButtons) {
        btn->setStyleSheet(functionButtonStyle);
    }

    QList<MyButton*> operationButtons = {
        divisionButton, timesButton, minusButton, plusButton, equalButton
    };

    for (MyButton *btn : operationButtons) {
        btn->setStyleSheet(operationButtonStyle);
    }

    buttonsLayout->addWidget(clearMemoryButton, 0, 0);
    buttonsLayout->addWidget(readMemoryButton, 0, 1);
    buttonsLayout->addWidget(addToMemoryButton, 0, 2);
    buttonsLayout->addWidget(minToMemoryButton, 0, 3);

    buttonsLayout->addWidget(percentButton, 1, 0);
    buttonsLayout->addWidget(clearAllButton, 1, 1);
    buttonsLayout->addWidget(clearButton, 1, 2);
    buttonsLayout->addWidget(backspaceButton, 1, 3);

    buttonsLayout->addWidget(squareButton, 2, 0);
    buttonsLayout->addWidget(powerButton, 2, 1);
    buttonsLayout->addWidget(reciprocalButton, 2, 2);
    buttonsLayout->addWidget(divisionButton, 2, 3);

    buttonsLayout->addWidget(m_digitButtons[7], 3, 0);
    buttonsLayout->addWidget(m_digitButtons[8], 3, 1);
    buttonsLayout->addWidget(m_digitButtons[9], 3, 2);
    buttonsLayout->addWidget(timesButton, 3, 3);

    buttonsLayout->addWidget(m_digitButtons[4], 4, 0);
    buttonsLayout->addWidget(m_digitButtons[5], 4, 1);
    buttonsLayout->addWidget(m_digitButtons[6], 4, 2);
    buttonsLayout->addWidget(minusButton, 4, 3);

    buttonsLayout->addWidget(m_digitButtons[1], 5, 0);
    buttonsLayout->addWidget(m_digitButtons[2], 5, 1);
    buttonsLayout->addWidget(m_digitButtons[3], 5, 2);
    buttonsLayout->addWidget(plusButton, 5, 3);

    buttonsLayout->addWidget(changeSignButton, 6, 0);
    buttonsLayout->addWidget(m_digitButtons[0], 6, 1);
    buttonsLayout->addWidget(pointButton, 6, 2);
    buttonsLayout->addWidget(equalButton, 6, 3);

    mainLayout->addLayout(buttonsLayout);

    for (int i = 0; i < 7; ++i) {
        buttonsLayout->setRowStretch(i, 1);
    }
    for (int j = 0; j < 4; ++j) {
        buttonsLayout->setColumnStretch(j, 1);
    }

    qDebug() << "CalculatorBase UI initialized";
}

QLineEdit* CalculatorBase::createHistoryDisplay()
{
    QLineEdit *historyDisplay = new QLineEdit();
    historyDisplay->setReadOnly(true);
    historyDisplay->setAlignment(Qt::AlignRight);
    historyDisplay->setMaxLength(100);
    historyDisplay->setStyleSheet(
        "QLineEdit {"
        "   font-size: 16px;"
        "   padding: 5px;"
        "   border: 2px solid #888;"
        "   border-radius: 6px;"
        "   background-color: #f8f8f8;"
        "   color: #666;"
        "   min-height: 30px;"
        "   margin-bottom: 5px;"
        "}"
    );
    historyDisplay->setText("0");
    return historyDisplay;
}

QLineEdit* CalculatorBase::createDisplay()
{
    QLineEdit *display = new QLineEdit("0");
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setMaxLength(50);
    display->setStyleSheet(
        "QLineEdit {"
        "   font-size: 28px;"
        "   padding: 10px;"
        "   border: 3px solid #555;"
        "   border-radius: 8px;"
        "   background-color: #f8f8f8;"
        "   min-height: 40px;"
        "   margin-bottom: 10px;"
        "}"
    );
    return display;
}

MyButton* CalculatorBase::createButton(const QString &text, const char *member)
{
    MyButton *button = new MyButton(text);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}

void CalculatorBase::digitClicked()
{
    MyButton *clickedButton = qobject_cast<MyButton*>(sender());
    if (!clickedButton) return;

    QString digit = clickedButton->text();
    QString currentText = m_display->text();

    // Если дисплей показывает "Error" - очищаем
    if (currentText == "Error") {
        clearAll();
        currentText = "0";
    }

    // Если ожидаем новый операнд (после операции)
    if (m_waiting_for_operand) {
        m_display->clear();
        m_display->setText(digit);
        m_waiting_for_operand = false;

        // Число уже будет добавлено при нажатии операции
        updateHistoryDisplay();
        return;
    }

    // Если это новое вычисление
    if (m_newCalculation) {
        m_display->clear();
        m_display->setText(digit);
        m_expression.clear();
        m_newCalculation = false;
        m_waiting_for_operand = false;
        updateHistoryDisplay();
        return;
    }

    // Обычный ввод цифры
    if (currentText == "0") {
        m_display->setText(digit);
    } else {
        m_display->setText(currentText + digit);
    }

    updateHistoryDisplay();
}

void CalculatorBase::pointClicked()
{
    QString currentText = m_display->text();

    // Если дисплей показывает "Error" - очищаем
    if (currentText == "Error") {
        clearAll();
        currentText = "0";
    }

    // Если ожидаем новый операнд
    if (m_waiting_for_operand) {
        m_display->setText("0.");
        m_waiting_for_operand = false;
        updateHistoryDisplay();
        return;
    }

    // Если это новое вычисление
    if (m_newCalculation) {
        m_display->setText("0.");
        m_expression.clear();
        m_newCalculation = false;
        m_waiting_for_operand = false;
        updateHistoryDisplay();
        return;
    }

    // Проверяем, есть ли уже точка
    if (!currentText.contains('.')) {
        if (currentText.isEmpty()) {
            m_display->setText("0.");
        } else {
            m_display->setText(currentText + ".");
        }
        updateHistoryDisplay();
    }
}
QString CalculatorBase::formatNumberForDisplay(double value)
{
    if (qIsNaN(value) || qIsInf(value)) {
        return "Error";
    }

    // Проверяем, является ли число целым
    double intPart;
    double fracPart = std::modf(value, &intPart);

    // Если дробная часть равна нулю (или почти нулю), показываем как целое число
    if (qFuzzyIsNull(fracPart)) {
        return QString::number(static_cast<long long>(value));
    }

    // Форматируем с 15 знаками после запятой
    QString result = QString::number(value, 'f', 15);

    // Убираем лишние нули в конце
    if (result.contains('.')) {
        // Удаляем все нули с конца
        while (result.endsWith('0')) {
            result.chop(1);
        }
        // Если после удаления нулей осталась точка, удаляем и её
        if (result.endsWith('.')) {
            result.chop(1);
        }
    }

    // Для очень больших чисел используем научную нотацию
    double absValue = qAbs(value);
    if (absValue >= 1.0e12 || (absValue > 0 && absValue < 1.0e-10)) {
        QString sciNotation = QString::number(value, 'g', 12);
        return sciNotation;
    }

    return result;
}

void CalculatorBase::changeSignClicked()
{
    QString text = m_display->text();
    if (text.isEmpty()) {
        text = "0";
    }

    bool ok;
    double value = text.toDouble(&ok);
    if (!ok) {
        m_display->setText("Error");
        return;
    }
    double result = m_mathOps.changeSign(value);

    m_display->clear();
    m_display->setText(formatNumberForDisplay(result));
}

void CalculatorBase::backspaceClicked()
{
    qDebug() << "Backspace clicked. Expression size:" << m_expression.size();

    // Случай 1: Если выражение не пустое - удаляем из истории
    if (!m_expression.isEmpty()) {
        // Получаем последний элемент выражения
        CalculationNode lastNode = m_expression.last();

        // Если последний элемент - оператор
        if (lastNode.isOperator) {
            // Удаляем оператор из выражения
            m_expression.removeLast();

            // Обновляем историю
            updateHistoryDisplay();

            // Если после удаления оператора выражение не пустое
            if (!m_expression.isEmpty()) {
                // И последний элемент теперь - число, показываем его в основном дисплее
                if (!m_expression.last().isOperator) {
                    QString numStr = QString::number(m_expression.last().value, 'g', 10);
                    m_display->setText(numStr);
                    m_waiting_for_operand = false;
                } else {
                    // Если все еще оператор, очищаем дисплей
                    m_display->clear();
                    m_waiting_for_operand = true;
                }
            } else {
                // Выражение полностью очищено
                m_display->setText("0");
                m_waiting_for_operand = true;
                m_newCalculation = true;
            }
        }
        // Если последний элемент - число
        else {
            QString currentNum = QString::number(lastNode.value);

            // Если число имеет более одной цифры
            if (currentNum.length() > 1) {
                // Удаляем последнюю цифру
                currentNum.chop(1);
                lastNode.value = currentNum.toDouble();
                m_expression.last() = lastNode;

                // Обновляем дисплей и историю
                m_display->setText(currentNum);
                updateHistoryDisplay();
                m_waiting_for_operand = false;
            }
            // Если число состоит из одной цифры
            else {
                // Удаляем число из выражения
                m_expression.removeLast();

                // Обновляем историю
                updateHistoryDisplay();

                // Если выражение не пустое
                if (!m_expression.isEmpty()) {
                    // И последний элемент - оператор
                    if (m_expression.last().isOperator) {
                        m_display->clear();
                        m_waiting_for_operand = true;
                    } else {
                        // Показываем предыдущее число
                        QString numStr = QString::number(m_expression.last().value, 'g', 10);
                        m_display->setText(numStr);
                        m_waiting_for_operand = false;
                    }
                } else {
                    // Выражение пустое
                    m_display->setText("0");
                    m_waiting_for_operand = true;
                    m_newCalculation = true;
                }
            }
        }
        return;
    }

    // Случай 2: Если выражение пустое, обрабатываем обычный бекспейс для дисплея
    QString text = m_display->text();
    if (text.isEmpty() || text == "0" || m_waiting_for_operand) {
        m_display->setText("0");
        m_waiting_for_operand = true;
        return;
    }

    // Удаляем последний символ из дисплея
    text.chop(1);

    if (text.isEmpty()) {
        m_display->setText("0");
        m_waiting_for_operand = true;
    } else {
        m_display->setText(text);
    }
}

void CalculatorBase::clearAll()
{
    m_display->setText("0");
    resetHistoryDisplay();
    m_expression.clear();
    m_stored_value = 0.0;
    m_result = 0.0;
    m_waiting_for_operand = true;
    m_newCalculation = true;
    m_pending_operation = "";
}

void CalculatorBase::clear()
{
    m_display->clear();
    m_waiting_for_operand = true;
}

void CalculatorBase::unaryOperatorClicked()
{
    MyButton *clickedButton = qobject_cast<MyButton*>(sender());
    if (!clickedButton) return;

    QString operation = clickedButton->text();
    QString displayText = m_display->text();

    // Убираем "=" из отображения если есть
    if (displayText.contains("=")) {
        QStringList parts = displayText.split("=");
        if (parts.size() > 1) {
            displayText = parts.last().trimmed();
        }
    }

    bool ok;
    double operand = displayText.toDouble(&ok);
    if (!ok) {
        m_display->setText("Error");
        return;
    }

    double result = 0.0;

    if (operation == "1/x") {
        result = m_mathOps.reciprocal(operand);
    }
    else if (operation == "x²") {
        result = m_mathOps.square(operand);
    }
    else if (operation == "√") {
        result = m_mathOps.squareRoot(operand);
    }
    else if (operation == "%") {
        result = m_mathOps.percent(operand);
    }

    if (qIsNaN(result)) {
        m_display->setText("Error");
        return;
    }

    // Форматируем результат
    QString formattedResult = formatNumberForDisplay(result);

    // Отображаем результат
    m_display->setText(formattedResult);

    m_expression.clear();
    m_expression.append({result, "", false});

    updateHistoryDisplay();

    m_waiting_for_operand = false;
    m_newCalculation = true;
}
void CalculatorBase::doubleOperandClicked()
{
    MyButton *clickedButton = qobject_cast<MyButton*>(sender());
    if (!clickedButton) return;

    QString operation = clickedButton->text();

    // Обновляем текущее число в выражении
    updateExpressionWithCurrentNumber();

    // Если выражение пустое
    if (m_expression.isEmpty()) {
        // Добавляем 0 и оператор
        m_expression.append({0.0, "", false});
        m_expression.append({0.0, operation, true});
    }
    // Если последний элемент - оператор
    else if (m_expression.last().isOperator) {
        // Заменяем оператор (кроме скобок)
        if (m_expression.last().operation != "(" &&
            m_expression.last().operation != ")") {
            m_expression.last().operation = operation;
        } else {
            m_expression.append({0.0, operation, true});
        }
    }
    // Если последний элемент - число
    else {
        m_expression.append({0.0, operation, true});
    }

    m_pending_operation = operation;
    updateHistoryDisplay();

    m_display->clear();
    m_waiting_for_operand = true;
}

bool CalculatorBase::calculate(double operand)
{
    double result = 0.0;

    if (m_pending_operation == "+") {
        result = m_mathOps.add(m_stored_value, operand);
    }
    else if (m_pending_operation == "-") {
        result = m_mathOps.subtract(m_stored_value, operand);
    }
    else if (m_pending_operation == "×") {
        result = m_mathOps.multiply(m_stored_value, operand);
    }
    else if (m_pending_operation == "÷") {
        result = m_mathOps.divide(m_stored_value, operand);
    }

    if (qIsNaN(result)) {
        m_display->setText("Error");
        m_pending_operation.clear();
        return false;
    }

    m_stored_value = result;
    m_display->setText(QString::number(result));
    return true;
}

void CalculatorBase::equalClicked()
{
    // Обновляем текущее число в выражении
    updateExpressionWithCurrentNumber();

    if (m_expression.isEmpty()) {
        // Если выражение пустое, показываем текущее число
        QString currentText = m_display->text();
        if (currentText.isEmpty() || currentText == "Error") {
            m_display->setText("0");
        }
        m_waiting_for_operand = true;
        m_newCalculation = true;
        return;
    }

    qDebug() << "=== BEFORE EVALUATION ===";
    for (int i = 0; i < m_expression.size(); ++i) {
        if (m_expression[i].isOperator) {
            qDebug() << i << "Operator:" << m_expression[i].operation;
        } else {
            qDebug() << i << "Value:" << m_expression[i].value;
        }
    }

    double result = m_mathOps.evaluateExpression(m_expression);

    qDebug() << "Result from evaluateExpression:" << result;

    if (qIsNaN(result) || qIsInf(result)) {
        m_display->setText("Error");
        m_historyDisplay->setText("Error");
        m_expression.clear();
        m_waiting_for_operand = true;
        m_newCalculation = true;
        return;
    }

    QString formattedResult = formatNumberForDisplay(result);
    m_display->setText(formattedResult);

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
            historyText += formatNumberForDisplay(node.value);
        }
    }
    historyText += " = " + formattedResult;
    m_historyDisplay->setText(historyText.trimmed());

    // Сохраняем результат для возможного продолжения
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

    // Если ожидаем новый операнд, показываем оператор в конце
    if (m_waiting_for_operand && !m_expression.isEmpty() &&
        m_expression.last().isOperator && m_expression.last().operation != ")") {
        // Уже добавлен в цикле
    }

    historyText = historyText.trimmed();

    if (!historyText.isEmpty()) {
        m_historyDisplay->setText(historyText);
    } else {
        m_historyDisplay->setText("0");
    }
}
void CalculatorBase::resetHistoryDisplay()
{
    m_historyDisplay->setText("0");
}

void CalculatorBase::clearMemory()
{
    m_sum_in_memory = 0.0;
}

void CalculatorBase::readMemory()
{
    m_display->clear();
    m_display->setText(formatNumberForDisplay(m_sum_in_memory));
    m_waiting_for_operand = false;
}
void CalculatorBase::addToMemory()
{
    bool ok;
    double value = m_display->text().toDouble(&ok);
    if (!ok) {
        m_display->setText("Error");
        return;
    }
    m_sum_in_memory += value;
}

void CalculatorBase::minToMemory()
{
    bool ok;
    double value = m_display->text().toDouble(&ok);
    if (!ok) {
        m_display->setText("Error");
        return;
    }
    m_sum_in_memory -= value;
}

void CalculatorBase::updateExpressionWithCurrentNumber()
{
    qDebug() << "=== updateExpressionWithCurrentNumber ===";
    qDebug() << "Display text:" << m_display->text();

    QString currentText = m_display->text();
    if (currentText.isEmpty() || currentText == "Error") {
        qDebug() << "Skipping - empty or Error";
        return;
    }

    bool ok;
    double value = currentText.toDouble(&ok);
    if (!ok) {
        qDebug() << "Skipping - not a valid number";
        return;
    }

    if (!m_expression.isEmpty() && !m_expression.last().isOperator) {
        qDebug() << "Updating last value to:" << value;
        m_expression.last().value = value;
    } else {
        qDebug() << "Adding new value:" << value;
        m_expression.append({value, "", false});
    }
}
