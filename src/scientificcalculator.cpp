#include "scientificcalculator.h"
#include "mybutton.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QLabel>

ScientificCalculator::ScientificCalculator(QWidget *parent)
    : CalculatorBase(parent, false),
      m_sinButton(nullptr), m_cosButton(nullptr), m_tanButton(nullptr), m_ctanButton(nullptr),
      m_piButton(nullptr), m_logButton(nullptr), m_lnButton(nullptr), m_powerButton(nullptr),
      m_tenPowerButton(nullptr), m_eButton(nullptr), m_factorialButton(nullptr),
      m_sqrtButton(nullptr), m_squareButton(nullptr), m_reciprocalButton(nullptr),
      m_absButton(nullptr), m_leftParenButton(nullptr), m_rightParenButton(nullptr),
      m_modButton(nullptr), m_percentButton(nullptr), m_divisionButton(nullptr),
      m_timesButton(nullptr), m_minusButton(nullptr), m_plusButton(nullptr),
      m_equalButton(nullptr), m_pointButton(nullptr), m_changeSignButton(nullptr),
      m_backspaceButton(nullptr), m_clearButton(nullptr), m_clearAllButton(nullptr)
{
    m_expression.clear();
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(5);

    m_historyDisplay = createHistoryDisplay();
    mainLayout->addWidget(m_historyDisplay);

    m_display = createDisplay();
    m_display->setText("0");
    mainLayout->addWidget(m_display);

    m_waiting_for_operand = true;
    m_newCalculation = true;
    m_pending_operation = "";

    setupScientificUI();
}

void ScientificCalculator::setupScientificUI()
{
    qDebug() << "Setting up Scientific Calculator UI";

    setStyleSheet(
        "ScientificCalculator {"
        "   background: #f0fff0;"
        "   border: 3px solid #32cd32;"
        "   border-radius: 12px;"
        "}"
    );

    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(layout());
    if (!mainLayout) {
        qDebug() << "No main layout found";
        return;
    }

    // Очищаем существующие кнопки цифр, если они есть
    for(int i = 0; i < 10; ++i) {
        if (m_digitButtons[i]) {
            delete m_digitButtons[i];
            m_digitButtons[i] = nullptr;
        }
    }

    // Создаем новые кнопки цифр
    for(int i = 0; i < 10; ++i) {
        m_digitButtons[i] = createButton(QString::number(i), SLOT(digitClicked()));
    }

    createScientificButtons();
    setupScientificButtonStyles();
    arrangeScientificButtons();

    qDebug() << "Scientific Calculator UI setup complete";
}

void ScientificCalculator::createScientificButtons()
{
    m_sinButton = createButton("sin", SLOT(sinClicked()));
    m_cosButton = createButton("cos", SLOT(cosClicked()));
    m_tanButton = createButton("tan", SLOT(tanClicked()));
    m_ctanButton = createButton("ctan", SLOT(ctanClicked()));
    m_piButton = createButton("π", SLOT(piClicked()));
    m_logButton = createButton("log", SLOT(logClicked()));
    m_lnButton = createButton("ln", SLOT(lnClicked()));
    m_powerButton = createButton("x^y", SLOT(powerClicked()));
    m_tenPowerButton = createButton("10^x", SLOT(TenInXClicked()));
    m_eButton = createButton("e", SLOT(eClicked()));
    m_factorialButton = createButton("n!", SLOT(factorialClicked()));
    m_sqrtButton = createButton("√", SLOT(unaryOperatorClicked()));
    m_squareButton = createButton("x²", SLOT(unaryOperatorClicked()));
    m_reciprocalButton = createButton("1/x", SLOT(unaryOperatorClicked()));
    m_absButton = createButton("|x|", SLOT(modulClicked()));
    m_leftParenButton = createButton("(", SLOT(leftParenClicked()));
    m_rightParenButton = createButton(")", SLOT(rightParenClicked()));
    m_modButton = createButton("mod", SLOT(modClicked()));
    m_percentButton = createButton("%", SLOT(unaryOperatorClicked()));
    m_divisionButton = createButton("÷", SLOT(doubleOperandClicked()));
    m_timesButton = createButton("×", SLOT(doubleOperandClicked()));
    m_minusButton = createButton("-", SLOT(doubleOperandClicked()));
    m_plusButton = createButton("+", SLOT(doubleOperandClicked()));
    m_equalButton = createButton("=", SLOT(equalClicked()));
    m_pointButton = createButton(".", SLOT(pointClicked()));
    m_changeSignButton = createButton("±", SLOT(changeSignClicked()));
    m_backspaceButton = createButton("⌫", SLOT(backspaceClicked()));
    m_clearButton = createButton("C", SLOT(clear()));
    m_clearAllButton = createButton("CE", SLOT(clearAll()));
}

void ScientificCalculator::setupScientificButtonStyles()
{
    QString scientificButtonStyle =
        "QPushButton {"
        "   background-color: #98fb98;"
        "   color: black;"
        "   border: 2px solid #32cd32;"
        "   border-radius: 6px;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   min-width: 50px;"
        "   min-height: 35px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #32cd32;"
        "   color: white;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #228b22;"
        "}";

    QString operationButtonStyle =
        "QPushButton {"
        "   background-color: #ffa500;"
        "   color: black;"
        "   border: 2px solid #ff8c00;"
        "   border-radius: 6px;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "   min-width: 50px;"
        "   min-height: 35px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #ff8c00;"
        "   color: white;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #ff7f50;"
        "}";

    QString digitButtonStyle =
        "QPushButton {"
        "   background-color: #87CEEB;"
        "   color: black;"
        "   border: 2px solid #4682b4;"
        "   border-radius: 6px;"
        "   font-size: 16px;"
        "   font-weight: bold;"
        "   min-width: 50px;"
        "   min-height: 35px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #4682b4;"
        "   color: white;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #4169e1;"
        "}";

    QString functionButtonStyle =
        "QPushButton {"
        "   background-color: #ffb6c1;"
        "   color: black;"
        "   border: 2px solid #ff69b4;"
        "   border-radius: 6px;"
        "   font-size: 14px;"
        "   font-weight: bold;"
        "   min-width: 50px;"
        "   min-height: 35px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #ff69b4;"
        "   color: white;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #ff1493;"
        "}";

    // Применяем стили к научным кнопкам
    QList<MyButton*> scientificButtons = {
        m_sinButton, m_cosButton, m_tanButton, m_ctanButton, m_piButton,
        m_logButton, m_lnButton, m_powerButton, m_tenPowerButton, m_eButton,
        m_factorialButton, m_sqrtButton, m_squareButton, m_reciprocalButton, m_absButton,
        m_leftParenButton, m_rightParenButton, m_modButton, m_percentButton
    };

    for (MyButton *btn : scientificButtons) {
        if (btn) btn->setStyleSheet(scientificButtonStyle);
    }

    // Применяем стили к операторным кнопкам
    QList<MyButton*> operationButtons = {
        m_divisionButton, m_timesButton, m_minusButton, m_plusButton, m_equalButton
    };

    for (MyButton *btn : operationButtons) {
        if (btn) btn->setStyleSheet(operationButtonStyle);
    }

    // Применяем стили к цифровым кнопкам
    for (int i = 0; i < 10; ++i) {
        if (m_digitButtons[i]) {
            m_digitButtons[i]->setStyleSheet(digitButtonStyle);
        }
    }

    // Применяем стили к функциональным кнопкам
    QList<MyButton*> functionButtons = {
        m_pointButton, m_changeSignButton, m_backspaceButton, m_clearButton, m_clearAllButton
    };

    for (MyButton *btn : functionButtons) {
        if (btn) btn->setStyleSheet(functionButtonStyle);
    }
}

void ScientificCalculator::arrangeScientificButtons()
{
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(layout());
    if (!mainLayout) return;

    QGridLayout *scientificLayout = new QGridLayout();
    scientificLayout->setSpacing(5);
    scientificLayout->setContentsMargins(5, 5, 5, 5);

    // Первый ряд
    scientificLayout->addWidget(m_sinButton, 0, 0);
    scientificLayout->addWidget(m_cosButton, 0, 1);
    scientificLayout->addWidget(m_tanButton, 0, 2);
    scientificLayout->addWidget(m_ctanButton, 0, 3);
    scientificLayout->addWidget(m_piButton, 0, 4);

    // Второй ряд
    scientificLayout->addWidget(m_logButton, 1, 0);
    scientificLayout->addWidget(m_lnButton, 1, 1);
    scientificLayout->addWidget(m_powerButton, 1, 2);
    scientificLayout->addWidget(m_tenPowerButton, 1, 3);
    scientificLayout->addWidget(m_eButton, 1, 4);

    // Третий ряд
    scientificLayout->addWidget(m_factorialButton, 2, 0);
    scientificLayout->addWidget(m_sqrtButton, 2, 1);
    scientificLayout->addWidget(m_squareButton, 2, 2);
    scientificLayout->addWidget(m_reciprocalButton, 2, 3);
    scientificLayout->addWidget(m_absButton, 2, 4);

    // Четвертый ряд
    scientificLayout->addWidget(m_leftParenButton, 3, 0);
    scientificLayout->addWidget(m_rightParenButton, 3, 1);
    scientificLayout->addWidget(m_modButton, 3, 2);
    scientificLayout->addWidget(m_percentButton, 3, 3);
    scientificLayout->addWidget(m_divisionButton, 3, 4);

    // Пятый ряд
    scientificLayout->addWidget(m_digitButtons[7], 4, 0);
    scientificLayout->addWidget(m_digitButtons[8], 4, 1);
    scientificLayout->addWidget(m_digitButtons[9], 4, 2);
    scientificLayout->addWidget(m_backspaceButton, 4, 3);
    scientificLayout->addWidget(m_timesButton, 4, 4);

    // Шестой ряд
    scientificLayout->addWidget(m_digitButtons[4], 5, 0);
    scientificLayout->addWidget(m_digitButtons[5], 5, 1);
    scientificLayout->addWidget(m_digitButtons[6], 5, 2);
    scientificLayout->addWidget(m_clearAllButton, 5, 3);
    scientificLayout->addWidget(m_minusButton, 5, 4);

    // Седьмой ряд
    scientificLayout->addWidget(m_digitButtons[1], 6, 0);
    scientificLayout->addWidget(m_digitButtons[2], 6, 1);
    scientificLayout->addWidget(m_digitButtons[3], 6, 2);
    scientificLayout->addWidget(m_clearButton, 6, 3);
    scientificLayout->addWidget(m_plusButton, 6, 4);

    // Восьмой ряд
    scientificLayout->addWidget(m_changeSignButton, 7, 0);
    scientificLayout->addWidget(m_digitButtons[0], 7, 1);
    scientificLayout->addWidget(m_pointButton, 7, 2);
    scientificLayout->addWidget(m_equalButton, 7, 3, 1, 2);

    mainLayout->addLayout(scientificLayout);
}


void ScientificCalculator::sinClicked()
{
    bool ok;
    double value = m_display->text().toDouble(&ok);
    if (!ok) {
        m_display->setText("Error");
        return;
    }
    double result = sin(value);
    if (qIsNaN(result)) {
        m_display->setText("Error");
        return;
    }
    QString formattedResult = formatNumberForDisplay(result);
    m_display->setText(formattedResult);

    m_expression.clear();
    m_expression.append({result, "", false});

    updateHistoryDisplay();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::cosClicked()
{
    bool ok;
    double value = m_display->text().toDouble(&ok);
    if (!ok) {
        m_display->setText("Error");
        return;
    }
    double result = cos(value);
    if (qIsNaN(result)) {
        m_display->setText("Error");
        return;
    }
    QString formattedResult = formatNumberForDisplay(result);
    m_display->setText(formattedResult);

    m_expression.clear();
    m_expression.append({result, "", false});

    updateHistoryDisplay();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::tanClicked()
{
    bool ok;
    double value = m_display->text().toDouble(&ok);
    if (!ok) {
        m_display->setText("Error");
        return;
    }
    double result = tan(value);
    if (qIsNaN(result)) {
        m_display->setText("Error");
        return;
    }
    QString formattedResult = formatNumberForDisplay(result);
    m_display->setText(formattedResult);

    m_expression.clear();
    m_expression.append({result, "", false});

    updateHistoryDisplay();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::ctanClicked()
{
    bool ok;
    double value = m_display->text().toDouble(&ok);
    if (!ok) {
        m_display->setText("Error");
        return;
    }
    double result = ctan(value);
    if (qIsNaN(result)) {
        m_display->setText("Error");
        return;
    }
    QString formattedResult = formatNumberForDisplay(result);
    m_display->setText(formattedResult);

    m_expression.clear();
    m_expression.append({result, "", false});

    updateHistoryDisplay();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::logClicked()
{
    bool ok;
    double value = m_display->text().toDouble(&ok);
    if (!ok) {
        m_display->setText("Error");
        return;
    }
    double result = log10(value);
    if (qIsNaN(result)) {
        m_display->setText("Error");
        return;
    }
    QString formattedResult = formatNumberForDisplay(result);
    m_display->setText(formattedResult);

    m_expression.clear();
    m_expression.append({result, "", false});

    updateHistoryDisplay();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::lnClicked()
{
    bool ok;
    double value = m_display->text().toDouble(&ok);
    if (!ok) {
        m_display->setText("Error");
        return;
    }
    double result = ln(value);
    if (qIsNaN(result)) {
        m_display->setText("Error");
        return;
    }
    QString formattedResult = formatNumberForDisplay(result);
    m_display->setText(formattedResult);

    m_expression.clear();
    m_expression.append({result, "", false});

    updateHistoryDisplay();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::factorialClicked()
{
    bool ok;
    int value = m_display->text().toInt(&ok);
    if (!ok || value < 0) {
        m_display->setText("Error");
        return;
    }
    double result = factorial(value);
    if (qIsNaN(result)) {
        m_display->setText("Error");
        return;
    }
    QString formattedResult = formatNumberForDisplay(result);
    m_display->setText(formattedResult);

    m_expression.clear();
    m_expression.append({result, "", false});

    updateHistoryDisplay();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::powerClicked()
{
    QString text = m_display->text();
    if (text.isEmpty()) {
        text = "0";
    }

    bool ok;
    double base = text.toDouble(&ok);
    if (!ok) {
        m_display->setText("Error");
        return;
    }

    // Если выражение пустое или последний элемент - оператор
    if (m_expression.isEmpty() || m_expression.last().isOperator) {
        m_expression.append({base, "", false});
    } else {
        // Обновляем последнее число
        m_expression.last().value = base;
    }

    // Добавляем оператор возведения в степень
    m_expression.append({0.0, "^", true});

    updateHistoryDisplay();
    m_display->clear();
    m_waiting_for_operand = true;
    m_newCalculation = false;
}

void ScientificCalculator::TenInXClicked()
{
    bool ok;
    double exponent = m_display->text().toDouble(&ok);
    if (!ok) {
        m_display->setText("Error");
        return;
    }
    double result = powerOf10(exponent);
    QString formattedResult = formatNumberForDisplay(result);
    m_display->setText(formattedResult);

    m_expression.clear();
    m_expression.append({result, "", false});

    updateHistoryDisplay();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::modulClicked()
{
    bool ok;
    double value = m_display->text().toDouble(&ok);
    if (!ok) {
        m_display->setText("Error");
        return;
    }
    double result = absolute(value);
    QString formattedResult = formatNumberForDisplay(result);
    m_display->setText(formattedResult);

    m_expression.clear();
    m_expression.append({result, "", false});

    updateHistoryDisplay();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::modClicked()
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

    // Если выражение пустое или последний элемент - оператор
    if (m_expression.isEmpty() || m_expression.last().isOperator) {
        m_expression.append({value, "", false});
    } else {
        // Обновляем последнее число
        m_expression.last().value = value;
    }

    // Добавляем оператор mod
    m_expression.append({0.0, "mod", true});

    updateHistoryDisplay();
    m_display->clear();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::piClicked()
{
    // ВСЕГДА начинаем новое вычисление с π
    m_display->clear();
    m_expression.clear();

    double piValue = MATH_PI;
    QString piStr = formatNumberForDisplay(piValue);
    m_display->setText(piStr);

    // Добавляем π в выражение
    m_expression.append({piValue, "", false});

    updateHistoryDisplay();
    m_newCalculation = false;
    m_waiting_for_operand = false;
}

void ScientificCalculator::eClicked()
{
    // ВСЕГДА начинаем новое вычисление с e
    m_display->clear();
    m_expression.clear();

    double eValue = MATH_PI;
    QString eStr = formatNumberForDisplay(eValue);
    m_display->setText(eStr);

    // Добавляем e в выражение
    m_expression.append({eValue, "", false});

    updateHistoryDisplay();
    m_newCalculation = false;
    m_waiting_for_operand = false;
}

void ScientificCalculator::leftParenClicked()
{
    qDebug() << "=== leftParenClicked START ===";
    qDebug() << "Display text:" << m_display->text();
    qDebug() << "New calculation:" << m_newCalculation;
    qDebug() << "Expression size:" << m_expression.size();

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
        double value = currentText.toDouble(&ok);
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

    qDebug() << "=== leftParenClicked END ===";
    qDebug() << "New expression size:" << m_expression.size();
    qDebug() << "Waiting for operand:" << m_waiting_for_operand;
}
void ScientificCalculator::rightParenClicked()
{
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

    // Нельзя добавить лишнюю закрывающую скобку
    if (closeCount >= openCount) {
        return;
    }

    // Добавляем закрывающую скобку
    m_expression.append({0.0, ")", true});

    updateHistoryDisplay();

    // После закрывающей скобки можно вводить операторы
    m_display->clear();
    m_waiting_for_operand = false;
}
