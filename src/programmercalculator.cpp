#include "programmercalculator.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QButtonGroup>
#include <QRadioButton>
#include <QDebug>

ProgrammerCalculator::ProgrammerCalculator(QWidget *parent)
    : CalculatorBase(parent, false),
      m_current_base(10),
      m_display_bin(nullptr),
      m_display_oct(nullptr),
      m_display_dec(nullptr),
      m_display_hex(nullptr),
      m_buttonA(nullptr),
      m_buttonB(nullptr),
      m_buttonC(nullptr),
      m_buttonD(nullptr),
      m_buttonE(nullptr),
      m_buttonF(nullptr)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(5);

    m_historyDisplay = createHistoryDisplay();
    mainLayout->addWidget(m_historyDisplay);
    m_display = createDisplay();
    mainLayout->addWidget(m_display);

    setupProgrammerUI();
}

void ProgrammerCalculator::setupProgrammerUI()
{
    qDebug() << "Setting up Programmer Calculator UI";

    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(layout());
    if (!mainLayout) {
        qDebug() << "Failed to get main layout in ProgrammerCalculator";
        return;
    }

    setStyleSheet(
        "ProgrammerCalculator {"
        "   background: #fff0f5;"
        "   border: 3px solid #db7093;"
        "   border-radius: 12px;"
        "}"
    );

    QGridLayout *displaysLayout = new QGridLayout();
    displaysLayout->setSpacing(5);

    QLabel *binLabel = new QLabel("BIN:");
    binLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    m_display_bin = createDisplay();
    m_display_bin->setStyleSheet("font-size: 16px; background-color: #f5f5f5; padding: 5px;");
    m_display_bin->setReadOnly(true);

    QLabel *octLabel = new QLabel("OCT:");
    octLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    m_display_oct = createDisplay();
    m_display_oct->setStyleSheet("font-size: 16px; background-color: #f5f5f5; padding: 5px;");
    m_display_oct->setReadOnly(true);

    QLabel *decLabel = new QLabel("DEC:");
    decLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    m_display_dec = createDisplay();
    m_display_dec->setStyleSheet("font-size: 16px; background-color: #f5f5f5; padding: 5px;");
    m_display_dec->setReadOnly(true);

    QLabel *hexLabel = new QLabel("HEX:");
    hexLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
    m_display_hex = createDisplay();
    m_display_hex->setStyleSheet("font-size: 16px; background-color: #f5f5f5; padding: 5px;");
    m_display_hex->setReadOnly(true);

    displaysLayout->addWidget(binLabel, 0, 0);
    displaysLayout->addWidget(m_display_bin, 0, 1);
    displaysLayout->addWidget(octLabel, 1, 0);
    displaysLayout->addWidget(m_display_oct, 1, 1);
    displaysLayout->addWidget(decLabel, 2, 0);
    displaysLayout->addWidget(m_display_dec, 2, 1);
    displaysLayout->addWidget(hexLabel, 3, 0);
    displaysLayout->addWidget(m_display_hex, 3, 1);

    mainLayout->addLayout(displaysLayout);

    QButtonGroup *baseGroup = new QButtonGroup(this);
    QRadioButton *binRadio = new QRadioButton("BIN");
    QRadioButton *octRadio = new QRadioButton("OCT");
    QRadioButton *decRadio = new QRadioButton("DEC");
    QRadioButton *hexRadio = new QRadioButton("HEX");

    binRadio->setProperty("base", 2);
    octRadio->setProperty("base", 8);
    decRadio->setProperty("base", 10);
    hexRadio->setProperty("base", 16);

    baseGroup->addButton(binRadio);
    baseGroup->addButton(octRadio);
    baseGroup->addButton(decRadio);
    baseGroup->addButton(hexRadio);

    decRadio->setChecked(true);

    QHBoxLayout *baseLayout = new QHBoxLayout();
    baseLayout->addWidget(binRadio);
    baseLayout->addWidget(octRadio);
    baseLayout->addWidget(decRadio);
    baseLayout->addWidget(hexRadio);
    baseLayout->addStretch();

    mainLayout->addLayout(baseLayout);

    QGridLayout *programmerLayout = new QGridLayout();
    programmerLayout->setSpacing(5);
    programmerLayout->setContentsMargins(10, 10, 10, 10);

    // Создаем кнопки цифр с другим слотом
    for(int i = 0; i < 10; ++i) {
        m_digitButtons[i] = createButton(QString::number(i), SLOT(numberSystemDigitClicked()));
    }

    // Создаем кнопки A-F
    m_buttonA = createButton("A", SLOT(numberSystemDigitClicked()));
    m_buttonB = createButton("B", SLOT(numberSystemDigitClicked()));
    m_buttonC = createButton("C", SLOT(numberSystemDigitClicked()));
    m_buttonD = createButton("D", SLOT(numberSystemDigitClicked()));
    m_buttonE = createButton("E", SLOT(numberSystemDigitClicked()));
    m_buttonF = createButton("F", SLOT(numberSystemDigitClicked()));

    // Изначально отключаем A-F (только для DEC)
    m_buttonA->setEnabled(false);
    m_buttonB->setEnabled(false);
    m_buttonC->setEnabled(false);
    m_buttonD->setEnabled(false);
    m_buttonE->setEnabled(false);
    m_buttonF->setEnabled(false);

    // Остальные кнопки
    MyButton *leftParenButton = createButton("(", SLOT(leftParenClicked()));
    MyButton *rightParenButton = createButton(")", SLOT(rightParenClicked()));
    MyButton *percentButton = createButton("%", SLOT(unaryOperatorClicked()));
    MyButton *divisionButton = createButton("/", SLOT(doubleOperandClicked()));
    MyButton *timesButton = createButton("*", SLOT(doubleOperandClicked()));
    MyButton *minusButton = createButton("-", SLOT(doubleOperandClicked()));
    MyButton *plusButton = createButton("+", SLOT(doubleOperandClicked()));
    MyButton *equalButton = createButton("=", SLOT(equalClicked()));
    MyButton *pointButton = createButton(",", SLOT(pointClicked()));
    MyButton *changeSignButton = createButton("+/-", SLOT(changeSignClicked()));

    MyButton *clearAllButton = createButton("CE", SLOT(clearAll()));
    MyButton *clearButton = createButton("C", SLOT(clear()));
    MyButton *backspaceButton = createButton("⌫", SLOT(backspaceClicked()));

    QString digitButtonStyle =
        "QPushButton {"
        "   background-color: #87CEEB;"
        "   color: black;"
        "   border: 2px solid #4682b4;"
        "   border-radius: 6px;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "   min-width: 45px;"
        "   min-height: 35px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #4682b4;"
        "   color: white;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #4169e1;"
        "}"
        "QPushButton:disabled {"
        "   background-color: #cccccc;"
        "   color: #666666;"
        "   border: 2px solid #999999;"
        "}";

    QString hexButtonStyle = digitButtonStyle;

    QString operationButtonStyle =
        "QPushButton {"
        "   background-color: #ffa500;"
        "   color: black;"
        "   border: 2px solid #ff8c00;"
        "   border-radius: 6px;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   min-width: 45px;"
        "   min-height: 35px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #ff8c00;"
        "   color: white;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #ff7f50;"
        "}";

    QString functionButtonStyle =
        "QPushButton {"
        "   background-color: #ffb6c1;"
        "   color: black;"
        "   border: 2px solid #ff69b4;"
        "   border-radius: 6px;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   min-width: 45px;"
        "   min-height: 35px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #ff69b4;"
        "   color: white;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #ff1493;"
        "}";

    // Применяем стили
    for (int i = 0; i < 10; ++i) {
        m_digitButtons[i]->setStyleSheet(digitButtonStyle);
    }

    m_buttonA->setStyleSheet(hexButtonStyle);
    m_buttonB->setStyleSheet(hexButtonStyle);
    m_buttonC->setStyleSheet(hexButtonStyle);
    m_buttonD->setStyleSheet(hexButtonStyle);
    m_buttonE->setStyleSheet(hexButtonStyle);
    m_buttonF->setStyleSheet(hexButtonStyle);

    QList<MyButton*> operationButtons = {
        divisionButton, timesButton, minusButton, plusButton, equalButton,
        percentButton
    };
    for (MyButton *btn : operationButtons) {
        btn->setStyleSheet(operationButtonStyle);
    }

    QList<MyButton*> functionButtons = {
        clearAllButton, clearButton, backspaceButton, changeSignButton,
        leftParenButton, rightParenButton, pointButton
    };
    for (MyButton *btn : functionButtons) {
        btn->setStyleSheet(functionButtonStyle);
    }

    // Расположение кнопок
    programmerLayout->addWidget(m_buttonA, 0, 0);
    programmerLayout->addWidget(clearAllButton, 0, 1);
    programmerLayout->addWidget(clearButton, 0, 2);
    programmerLayout->addWidget(backspaceButton, 0, 3, 1, 2);

    programmerLayout->addWidget(m_buttonB, 1, 0);
    programmerLayout->addWidget(leftParenButton, 1, 1);
    programmerLayout->addWidget(rightParenButton, 1, 2);
    programmerLayout->addWidget(percentButton, 1, 3);
    programmerLayout->addWidget(divisionButton, 1, 4);

    programmerLayout->addWidget(m_buttonC, 2, 0);
    programmerLayout->addWidget(m_digitButtons[7], 2, 1);
    programmerLayout->addWidget(m_digitButtons[8], 2, 2);
    programmerLayout->addWidget(m_digitButtons[9], 2, 3);
    programmerLayout->addWidget(timesButton, 2, 4);

    programmerLayout->addWidget(m_buttonD, 3, 0);
    programmerLayout->addWidget(m_digitButtons[4], 3, 1);
    programmerLayout->addWidget(m_digitButtons[5], 3, 2);
    programmerLayout->addWidget(m_digitButtons[6], 3, 3);
    programmerLayout->addWidget(minusButton, 3, 4);

    programmerLayout->addWidget(m_buttonE, 4, 0);
    programmerLayout->addWidget(m_digitButtons[1], 4, 1);
    programmerLayout->addWidget(m_digitButtons[2], 4, 2);
    programmerLayout->addWidget(m_digitButtons[3], 4, 3);
    programmerLayout->addWidget(plusButton, 4, 4);

    programmerLayout->addWidget(m_buttonF, 5, 0);
    programmerLayout->addWidget(changeSignButton, 5, 1);
    programmerLayout->addWidget(m_digitButtons[0], 5, 2);
    programmerLayout->addWidget(pointButton, 5, 3);
    programmerLayout->addWidget(equalButton, 5, 4);

    mainLayout->addLayout(programmerLayout);

    // Подключаем смену системы счисления
    connect(baseGroup, SIGNAL(buttonClicked(QAbstractButton*)),
            this, SLOT(onBaseGroupButtonClicked(QAbstractButton*)));

    // Инициализируем отображения
    updateNumberSystemDisplays();
    updateNumberSystemButtons();

    qDebug() << "Programmer Calculator UI setup complete";
}

// Функции для скобок
void ProgrammerCalculator::leftParenClicked()
{
    qDebug() << "=== leftParenClicked START (Programmer) ===";
    qDebug() << "Display text:" << m_display->text();
    qDebug() << "New calculation:" << m_newCalculation;
    qDebug() << "Expression size:" << m_expression.size();
    qDebug() << "Current base:" << m_current_base;

    // Отладочный вывод всего выражения
    qDebug() << "Current expression:";
    for (int i = 0; i < m_expression.size(); ++i) {
        if (m_expression[i].isOperator) {
            qDebug() << "  [" << i << "] Operator:" << m_expression[i].operation;
        } else {
            qDebug() << "  [" << i << "] Value:" << m_expression[i].value;
        }
    }

    QString currentText = m_display->text();

    // Если дисплей показывает "Error" - очищаем всё
    if (currentText == "Error") {
        clearAll();
        currentText = "0";
    }

    // ВСЕГДА обновляем выражение с текущим числом из дисплея
    if (!currentText.isEmpty() && currentText != "0") {
        bool ok;
        double value = 0;

        // Преобразуем из текущей системы счисления
        if (m_current_base == 10) {
            value = currentText.toDouble(&ok);
        } else {
            long long intVal = currentText.toLongLong(&ok, m_current_base);
            value = static_cast<double>(intVal);
        }

        if (ok) {
            // Добавляем или обновляем число в выражении
            if (m_expression.isEmpty() || m_expression.last().isOperator) {
                m_expression.append({value, "", false});
            } else {
                m_expression.last().value = value;
            }
        }
    }

    // Если это новое вычисление ИЛИ выражение пустое
    if (m_newCalculation || m_expression.isEmpty()) {
        qDebug() << "Starting new expression with (";
        // Просто добавляем скобку
        m_expression.append({0.0, "(", true});
    }
    else {
        // Продолжаем существующее выражение
        // Если последний элемент - число, добавляем умножение
        if (!m_expression.last().isOperator) {
            m_expression.append({0.0, "×", true});
            qDebug() << "Added × before (";
        }
        // Добавляем скобку
        m_expression.append({0.0, "(", true});
    }

    // Обновляем отображение
    updateHistoryDisplay();

    // Очищаем дисплей для ввода внутри скобок
    m_display->clear();
    m_waiting_for_operand = true;
    m_newCalculation = false;

    // Обновляем дисплеи систем счисления
    updateNumberSystemDisplays();

    qDebug() << "=== leftParenClicked END ===";
    qDebug() << "New expression size:" << m_expression.size();
    qDebug() << "Waiting for operand:" << m_waiting_for_operand;
}

void ProgrammerCalculator::rightParenClicked()
{
    qDebug() << "=== rightParenClicked START (Programmer) ===";
    qDebug() << "Display text:" << m_display->text();

    // Обновляем текущее число в выражении
    updateExpressionWithCurrentNumber();

    // Проверяем баланс скобок
    int openCount = 0;
    int closeCount = 0;
    for (const auto& node : qAsConst(m_expression)) {
        if (node.isOperator) {
            if (node.operation == "(") openCount++;
            if (node.operation == ")") closeCount++;
        }
    }

    qDebug() << "Open parentheses:" << openCount << "Close parentheses:" << closeCount;

    // Нельзя добавить лишнюю закрывающую скобку
    if (closeCount >= openCount) {
        qDebug() << "Too many closing parentheses";
        return;
    }

    // Добавляем закрывающую скобку
    m_expression.append({0.0, ")", true});

    updateHistoryDisplay();

    // После закрывающей скобки можно вводить операторы
    m_display->clear();
    m_waiting_for_operand = false;

    // Обновляем дисплеи систем счисления
    updateNumberSystemDisplays();

    qDebug() << "=== rightParenClicked END ===";
}

void ProgrammerCalculator::numberSystemDigitClicked()
{
    MyButton *clickedButton = qobject_cast<MyButton*>(sender());
    if (!clickedButton) return;

    QString digit = clickedButton->text();
    qDebug() << "Number system digit clicked:" << digit << "base:" << m_current_base;

    // Проверяем, допустима ли цифра для текущей системы счисления
    bool isValid = false;
    if (digit >= "0" && digit <= "9") {
        int num = digit.toInt();
        isValid = (num < m_current_base);
    } else if (digit >= "A" && digit <= "F") {
        // Для 16-ричной системы
        isValid = (m_current_base == 16);
    }

    if (!isValid) {
        qDebug() << "Invalid digit" << digit << "for base" << m_current_base;
        return;
    }

    QString currentText = m_display->text();

    // Если ожидаем новый операнд или это новое вычисление
    if (m_waiting_for_operand || m_newCalculation) {
        m_display->setText(digit);
        m_waiting_for_operand = false;
        m_newCalculation = false;
    } else {
        m_display->setText(currentText + digit);
    }

    // Обновляем все дисплеи
    updateNumberSystemDisplays();
}

void ProgrammerCalculator::onBaseGroupButtonClicked(QAbstractButton* button)
{
    if (!button) return;

    int newBase = button->property("base").toInt();
    qDebug() << "Switching to base:" << newBase;

    // Преобразуем текущее значение в новую систему счисления
    QString currentText = m_display->text();
    if (currentText != "0" && currentText != "Error") {
        bool ok;
        long long value = currentText.toLongLong(&ok, m_current_base);
        if (ok) {
            if (newBase == 10) {
                m_display->setText(QString::number(value));
            } else {
                m_display->setText(QString::number(value, newBase).toUpper());
            }
        }
    } else {
        m_display->setText("0");
    }

    m_current_base = newBase;
    updateNumberSystemButtons();
    updateNumberSystemDisplays();
}

void ProgrammerCalculator::bitwiseOperationClicked()
{
    MyButton *clickedButton = qobject_cast<MyButton*>(sender());
    if (!clickedButton) return;

    QString operation = clickedButton->text();

    bool ok;
    long currentValue = m_display->text().toLong(&ok, m_current_base);
    if (!ok) {
        m_display->setText("Error");
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

    m_display->setText(QString::number(result, m_current_base).toUpper());
    updateNumberSystemDisplays();
}

void ProgrammerCalculator::updateNumberSystemDisplays()
{
    QString input = m_display->text();
    qDebug() << "Обновление отображений для числа:" << input << "base:" << m_current_base;

    if (input.isEmpty() || input == "Error" || input == "0") {
        if (m_display_bin) m_display_bin->setText("0");
        if (m_display_oct) m_display_oct->setText("0");
        if (m_display_dec) m_display_dec->setText("0");
        if (m_display_hex) m_display_hex->setText("0");
        return;
    }

    bool ok;
    long long num = 0;

    // Пробуем преобразовать из текущей системы счисления
    if (input.startsWith("-")) {
        QString absInput = input.mid(1);
        num = absInput.toLongLong(&ok, m_current_base);
        if (ok) num = -num;
    } else {
        num = input.toLongLong(&ok, m_current_base);
    }

    if (!ok) {
        qDebug() << "Ошибка преобразования числа:" << input << "from base:" << m_current_base;
        if (m_display_bin) m_display_bin->setText("Error");
        if (m_display_oct) m_display_oct->setText("Error");
        if (m_display_dec) m_display_dec->setText("Error");
        if (m_display_hex) m_display_hex->setText("Error");
        return;
    }

    // Обновляем все дисплеи
    if (m_display_bin) m_display_bin->setText(QString::number(num, 2));
    if (m_display_oct) m_display_oct->setText(QString::number(num, 8));
    if (m_display_dec) m_display_dec->setText(QString::number(num, 10));
    if (m_display_hex) m_display_hex->setText(QString::number(num, 16).toUpper());
}

void ProgrammerCalculator::updateNumberSystemButtons()
{
    bool hexMode = (m_current_base == 16);
    bool octMode = (m_current_base == 8);
    bool binMode = (m_current_base == 2);
    bool decMode = (m_current_base == 10);

    // Включаем/выключаем кнопки A-F только для HEX
    if (m_buttonA) m_buttonA->setEnabled(hexMode);
    if (m_buttonB) m_buttonB->setEnabled(hexMode);
    if (m_buttonC) m_buttonC->setEnabled(hexMode);
    if (m_buttonD) m_buttonD->setEnabled(hexMode);
    if (m_buttonE) m_buttonE->setEnabled(hexMode);
    if (m_buttonF) m_buttonF->setEnabled(hexMode);

    // Включаем/выключаем цифровые кнопки в зависимости от системы счисления
    for (int i = 0; i < 10; ++i) {
        if (m_digitButtons[i]) {
            if (binMode) {
                m_digitButtons[i]->setEnabled(i <= 1); // Только 0 и 1
            } else if (octMode) {
                m_digitButtons[i]->setEnabled(i <= 7); // 0-7
            } else if (decMode || hexMode) {
                m_digitButtons[i]->setEnabled(true); // Все цифры
            } else {
                m_digitButtons[i]->setEnabled(false);
            }
        }
    }
}

// Переопределенные методы

void ProgrammerCalculator::digitClicked()
{
    CalculatorBase::digitClicked(); // Вызываем родительский метод
    updateNumberSystemDisplays(); // ОБНОВЛЯЕМ ДИСПЛЕИ
}

void ProgrammerCalculator::equalClicked()
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

        // ОБНОВЛЯЕМ ДИСПЛЕИ
        updateNumberSystemDisplays();
        return;
    }

    qDebug() << "=== BEFORE EVALUATION (Programmer) ===";
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

        // ОБНОВЛЯЕМ ДИСПЛЕИ ДАЖЕ ПРИ ОШИБКЕ
        updateNumberSystemDisplays();
        return;
    }

    // Преобразуем результат в текущую систему счисления
    QString formattedResult;
    if (m_current_base == 10) {
        formattedResult = formatNumberForDisplay(result);
    } else {
        // Для других систем счисления преобразуем в целое число
        long long intResult = static_cast<long long>(result);
        formattedResult = QString::number(intResult, m_current_base).toUpper();
    }

    m_display->setText(formattedResult);

    // Показываем всё выражение и результат в истории
    QString historyText;
    for (const auto& node : m_expression) {
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
    m_historyDisplay->setText(historyText.trimmed());

    // Сохраняем результат для возможного продолжения
    m_result = result;
    m_expression.clear();
    m_expression.append({result, "", false});

    m_waiting_for_operand = true;
    m_newCalculation = true;

    // ОБНОВЛЯЕМ ДИСПЛЕИ ПОСЛЕ ВЫЧИСЛЕНИЯ
    updateNumberSystemDisplays();
}

void ProgrammerCalculator::unaryOperatorClicked()
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
    double operand = 0;

    // Преобразуем ввод из текущей системы счисления
    if (m_current_base == 10) {
        operand = displayText.toDouble(&ok);
    } else {
        long long intVal = displayText.toLongLong(&ok, m_current_base);
        operand = static_cast<double>(intVal);
    }

    if (!ok) {
        m_display->setText("Error");
        updateNumberSystemDisplays();
        return;
    }

    double result = 0.0;

    if (operation == "%") {
        result = m_mathOps.percent(operand);
    }

    if (qIsNaN(result)) {
        m_display->setText("Error");
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
    m_display->setText(formattedResult);

    m_expression.clear();
    m_expression.append({result, "", false});

    updateHistoryDisplay();
    updateNumberSystemDisplays(); // ОБНОВЛЯЕМ ДИСПЛЕИ

    m_waiting_for_operand = false;
    m_newCalculation = true;
}

void ProgrammerCalculator::changeSignClicked()
{
    QString text = m_display->text();
    if (text.isEmpty()) {
        text = "0";
    }

    bool ok;
    double value = 0;

    // Преобразуем ввод из текущей системы счисления
    if (m_current_base == 10) {
        value = text.toDouble(&ok);
    } else {
        long long intVal = text.toLongLong(&ok, m_current_base);
        value = static_cast<double>(intVal);
    }

    if (!ok) {
        m_display->setText("Error");
        updateNumberSystemDisplays();
        return;
    }

    double result = m_mathOps.changeSign(value);

    // Преобразуем результат в текущую систему счисления
    QString formattedResult;
    if (m_current_base == 10) {
        formattedResult = formatNumberForDisplay(result);
    } else {
        long long intResult = static_cast<long long>(result);
        formattedResult = QString::number(intResult, m_current_base).toUpper();
    }

    m_display->clear();
    m_display->setText(formattedResult);

    updateNumberSystemDisplays(); // ОБНОВЛЯЕМ ДИСПЛЕИ
}

void ProgrammerCalculator::clearAll()
{
    CalculatorBase::clearAll(); // Вызываем родительский метод
    updateNumberSystemDisplays(); // ОБНОВЛЯЕМ ДИСПЛЕИ
}

void ProgrammerCalculator::clear()
{
    CalculatorBase::clear(); // Вызываем родительский метод
    updateNumberSystemDisplays(); // ОБНОВЛЯЕМ ДИСПЛЕИ
}

void ProgrammerCalculator::backspaceClicked()
{
    CalculatorBase::backspaceClicked(); // Вызываем родительский метод
    updateNumberSystemDisplays(); // ОБНОВЛЯЕМ ДИСПЛЕИ
}

void ProgrammerCalculator::doubleOperandClicked()
{
    MyButton *clickedButton = qobject_cast<MyButton*>(sender());
    if (!clickedButton) return;

    QString operation = clickedButton->text();

    // Получаем текущее значение из дисплея
    QString displayText = m_display->text();

    // Преобразуем ввод из текущей системы счисления в double
    bool ok;
    double value = 0;

    if (displayText.isEmpty() || displayText == "Error") {
        value = 0;
    } else {
        if (m_current_base == 10) {
            value = displayText.toDouble(&ok);
        } else {
            // Для не-десятичных систем преобразуем в long long, затем в double
            long long intVal = displayText.toLongLong(&ok, m_current_base);
            value = static_cast<double>(intVal);
        }

        if (!ok) {
            m_display->setText("Error");
            updateNumberSystemDisplays();
            return;
        }
    }

    // Обновляем текущее число в выражении
    updateExpressionWithCurrentNumber();

    // Если выражение пустое
    if (m_expression.isEmpty()) {
        // Добавляем 0 и оператор
        m_expression.append({value, "", false});
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

    // Обновляем дисплеи систем счисления
    updateNumberSystemDisplays();
}


void ProgrammerCalculator::pointClicked()
{
    // В программистском калькуляторе точка не используется,
    // но можно использовать запятую для разделения
    QString currentText = m_display->text();

    if (currentText.isEmpty() || currentText == "0" || m_waiting_for_operand) {
        m_display->setText("0,");
    } else if (!currentText.contains(',')) {
        m_display->setText(currentText + ",");
    }

    updateNumberSystemDisplays(); // ОБНОВЛЯЕМ ДИСПЛЕИ
}

void ProgrammerCalculator::updateExpressionWithCurrentNumber()
{
    qDebug() << "=== updateExpressionWithCurrentNumber (Programmer) ===";
    qDebug() << "Display text:" << m_display->text();
    qDebug() << "Current base:" << m_current_base;

    QString currentText = m_display->text();
    if (currentText.isEmpty() || currentText == "Error") {
        qDebug() << "Skipping - empty or Error";
        return;
    }

    bool ok;
    double value = 0;

    if (m_current_base == 10) {
        value = currentText.toDouble(&ok);
    } else {
        // Для не-десятичных систем
        long long intVal = currentText.toLongLong(&ok, m_current_base);
        value = static_cast<double>(intVal);
    }

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
