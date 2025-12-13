#include "calculatorbase.h"
#include "mybutton.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QDebug>

CalculatorBase::CalculatorBase(QWidget *parent, bool setupDefaultUI)
    : QWidget(parent),
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

    for (int i = 0; i < 10; ++i) {
        m_digitButtons[i] = nullptr;
    }

    // Инициализация всех указателей на кнопки
    m_percentButton = m_pointButton = m_changeSignButton = m_backspaceButton =
    m_clearButton = m_clearAllButton = m_squareButton = m_powerButton =
    m_reciprocalButton = m_divisionButton = m_timesButton = m_minusButton =
    m_plusButton = m_equalButton = m_clearMemoryButton = m_readMemoryButton =
    m_addToMemoryButton = m_minToMemoryButton = nullptr;

    m_expression.clear();

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

    // Создаем все кнопки
    createAllButtons();

    // Настраиваем стили кнопок
    setupButtonStyles();

    // Располагаем кнопки в макете
    arrangeButtonsInLayout(buttonsLayout);

    mainLayout->addLayout(buttonsLayout);

    for (int i = 0; i < 7; ++i) {
        buttonsLayout->setRowStretch(i, 1);
    }
    for (int j = 0; j < 4; ++j) {
        buttonsLayout->setColumnStretch(j, 1);
    }

    qDebug() << "CalculatorBase UI initialized";
}

void CalculatorBase::createAllButtons()
{
    qDebug() << "Creating all buttons...";

    // Цифровые кнопки
    for(int i = 0; i < 10; ++i) {
        m_digitButtons[i] = createButton(QString::number(i), SLOT(digitClicked()));
        qDebug() << "Created digit button:" << i;
    }

    // Функциональные кнопки
    m_percentButton = createButton("%", SLOT(unaryOperatorClicked()));
    m_pointButton = createButton(".", SLOT(pointClicked()));
    m_changeSignButton = createButton("±", SLOT(changeSignClicked()));
    m_backspaceButton = createButton("⌫", SLOT(backspaceClicked()));
    m_clearButton = createButton("C", SLOT(clear()));
    m_clearAllButton = createButton("CE", SLOT(clearAll()));
    m_squareButton = createButton("√", SLOT(unaryOperatorClicked()));
    m_powerButton = createButton("x²", SLOT(unaryOperatorClicked()));
    m_reciprocalButton = createButton("1/x", SLOT(unaryOperatorClicked()));

    // Операторные кнопки
    m_divisionButton = createButton("÷", SLOT(doubleOperandClicked()));
    m_timesButton = createButton("×", SLOT(doubleOperandClicked()));
    m_minusButton = createButton("-", SLOT(doubleOperandClicked()));
    m_plusButton = createButton("+", SLOT(doubleOperandClicked()));
    m_equalButton = createButton("=", SLOT(equalClicked()));

    // Кнопки памяти
    m_clearMemoryButton = createButton("MC", SLOT(clearMemory()));
    m_readMemoryButton = createButton("MR", SLOT(readMemory()));
    m_addToMemoryButton = createButton("M+", SLOT(addToMemory()));
    m_minToMemoryButton = createButton("M-", SLOT(minToMemory()));

    qDebug() << "All buttons created successfully";
}

void CalculatorBase::setupButtonStyles()
{
    qDebug() << "Setting up button styles...";

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

    // Применяем стили к цифровым кнопкам
    for (int i = 0; i < 10; ++i) {
        if (m_digitButtons[i]) {
            m_digitButtons[i]->setStyleSheet(digitButtonStyle);
        }
    }

    // Применяем стили к функциональным кнопкам
    QList<MyButton*> functionButtons = {
        m_percentButton, m_pointButton, m_changeSignButton, m_backspaceButton,
        m_clearButton, m_clearAllButton, m_squareButton, m_powerButton, m_reciprocalButton,
        m_clearMemoryButton, m_readMemoryButton, m_addToMemoryButton, m_minToMemoryButton
    };

    for (MyButton *btn : functionButtons) {
        if (btn) {
            btn->setStyleSheet(functionButtonStyle);
        }
    }

    // Применяем стили к операторным кнопкам
    QList<MyButton*> operationButtons = {
        m_divisionButton, m_timesButton, m_minusButton, m_plusButton, m_equalButton
    };

    for (MyButton *btn : operationButtons) {
        if (btn) {
            btn->setStyleSheet(operationButtonStyle);
        }
    }

    qDebug() << "Button styles applied";
}

void CalculatorBase::arrangeButtonsInLayout(QGridLayout *buttonsLayout)
{
    qDebug() << "Arranging buttons in layout...";

    // Первый ряд: кнопки памяти
    buttonsLayout->addWidget(m_clearMemoryButton, 0, 0);
    buttonsLayout->addWidget(m_readMemoryButton, 0, 1);
    buttonsLayout->addWidget(m_addToMemoryButton, 0, 2);
    buttonsLayout->addWidget(m_minToMemoryButton, 0, 3);

    // Второй ряд: функциональные кнопки
    buttonsLayout->addWidget(m_percentButton, 1, 0);
    buttonsLayout->addWidget(m_clearAllButton, 1, 1);
    buttonsLayout->addWidget(m_clearButton, 1, 2);
    buttonsLayout->addWidget(m_backspaceButton, 1, 3);

    // Третий ряд: математические функции
    buttonsLayout->addWidget(m_squareButton, 2, 0);
    buttonsLayout->addWidget(m_powerButton, 2, 1);
    buttonsLayout->addWidget(m_reciprocalButton, 2, 2);
    buttonsLayout->addWidget(m_divisionButton, 2, 3);

    // Четвертый ряд: 7, 8, 9, умножение
    buttonsLayout->addWidget(m_digitButtons[7], 3, 0);
    buttonsLayout->addWidget(m_digitButtons[8], 3, 1);
    buttonsLayout->addWidget(m_digitButtons[9], 3, 2);
    buttonsLayout->addWidget(m_timesButton, 3, 3);

    // Пятый ряд: 4, 5, 6, вычитание
    buttonsLayout->addWidget(m_digitButtons[4], 4, 0);
    buttonsLayout->addWidget(m_digitButtons[5], 4, 1);
    buttonsLayout->addWidget(m_digitButtons[6], 4, 2);
    buttonsLayout->addWidget(m_minusButton, 4, 3);

    // Шестой ряд: 1, 2, 3, сложение
    buttonsLayout->addWidget(m_digitButtons[1], 5, 0);
    buttonsLayout->addWidget(m_digitButtons[2], 5, 1);
    buttonsLayout->addWidget(m_digitButtons[3], 5, 2);
    buttonsLayout->addWidget(m_plusButton, 5, 3);

    // Седьмой ряд: смена знака, 0, точка, равно
    buttonsLayout->addWidget(m_changeSignButton, 6, 0);
    buttonsLayout->addWidget(m_digitButtons[0], 6, 1);
    buttonsLayout->addWidget(m_pointButton, 6, 2);
    buttonsLayout->addWidget(m_equalButton, 6, 3);

    qDebug() << "Buttons arranged in layout";
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

    QString digit = clickedButton->text();
    QString currentText = m_display->text();

    // Если дисплей показывает "Error" - очищаем
    if (currentText == "Error") {
        clearAll();
        currentText = m_display->text();
    }

    // Если ожидаем новый операнд (после операции)
    if (m_waiting_for_operand) {
        m_display->clear();
        m_display->setText(digit);
        m_waiting_for_operand = false;
        m_newCalculation = false;

        // Обновляем выражение
        updateExpressionWithCurrentNumber();
        updateHistoryDisplay();
        return;
    }

    // Если это новое вычисление (например, после = или clearAll)
    if (m_newCalculation) {
        qDebug() << "CASE 2: New calculation";
        m_display->clear();
        m_display->setText(digit);
        m_expression.clear();
        m_newCalculation = false;
        m_waiting_for_operand = false;

        updateExpressionWithCurrentNumber();
        updateHistoryDisplay();

        return;
    }

    // Если текущий текст "0", заменяем его
    if (currentText == "0" && digit != "0") {
        m_display->setText(digit);
    }
    // Если текущий текст "0" и нажата "0", ничего не делаем
    else if (currentText == "0" && digit == "0") {
        return;
    }
    // Обычное добавление цифры
    else {
        QString newText = currentText + digit;
        m_display->setText(newText);
    }

    // Обновляем текущее число в выражении
    updateExpressionWithCurrentNumber();
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
        updateExpressionWithCurrentNumber();
        updateHistoryDisplay();
        return;
    }

    // Если это новое вычисление
    if (m_newCalculation) {
        m_display->setText("0.");
        m_expression.clear();
        m_newCalculation = false;
        m_waiting_for_operand = false;
        updateExpressionWithCurrentNumber();
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
        updateExpressionWithCurrentNumber();
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
    double result = changeSign(value);

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
    updateExpressionWithCurrentNumber();
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
        result = add(m_stored_value, operand);
    }
    else if (m_pending_operation == "-") {
        result = subtract(m_stored_value, operand);
    }
    else if (m_pending_operation == "×") {
        result = multiply(m_stored_value, operand);
    }
    else if (m_pending_operation == "÷") {
        result = divide(m_stored_value, operand);
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

    double result = evaluateExpression(m_expression);

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
    qDebug() << "\n=== updateExpressionWithCurrentNumber START ===";
    qDebug() << "Display text:" << m_display->text();
    qDebug() << "Expression size:" << m_expression.size();

    QString currentText = m_display->text();
    if (currentText.isEmpty() || currentText == "Error" || currentText == "0") {
        qDebug() << "Skipping - empty, Error, or 0";
        qDebug() << "=== updateExpressionWithCurrentNumber END ===";
        return;
    }

    bool ok;
    double value = currentText.toDouble(&ok);
    if (!ok) {
        qDebug() << "Skipping - not a valid number";
        qDebug() << "=== updateExpressionWithCurrentNumber END ===";
        return;
    }

    qDebug() << "Value to add/update:" << value;

    if (m_expression.isEmpty()) {
        qDebug() << "Expression empty - adding first value";
        m_expression.append({value, "", false});
    } else if (!m_expression.last().isOperator) {
        qDebug() << "Updating last value from" << m_expression.last().value << "to" << value;
        m_expression.last().value = value;
    } else {
        qDebug() << "Last is operator - adding new value";
        m_expression.append({value, "", false});
    }

    qDebug() << "Expression after update (size:" << m_expression.size() << "):";
    for (int i = 0; i < m_expression.size(); ++i) {
        if (m_expression[i].isOperator) {
            qDebug() << "  [" << i << "] Operator:" << m_expression[i].operation;
        } else {
            qDebug() << "  [" << i << "] Value:" << m_expression[i].value;
        }
    }
    qDebug() << "=== updateExpressionWithCurrentNumber END ===\n";
}
