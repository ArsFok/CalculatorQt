#include "scientificcalculator.h"
#include "ui_scientificcalculator.h"
#include <QPushButton>
#include <QDebug>

ScientificCalculator::ScientificCalculator(QWidget *parent)
    : CalculatorBase(parent),
      scientificUi(new Ui::ScientificCalculator)
{
}

ScientificCalculator::~ScientificCalculator()
{
    delete scientificUi;
}

void ScientificCalculator::setupUI()
{
    scientificUi->setupUi(this);


    setStyleSheet(
        "ScientificCalculator {"
        "   background: #f0fff0;"
        "   border: 3px solid #32cd32;"
        "   border-radius: 12px;"
        "}"
    );
}

void ScientificCalculator::setupConnections()
{
    // Наследуем базовые подключения
    CalculatorBase::setupConnections();

    // Научные функции
    if (scientificUi->sinButton) {
        connect(scientificUi->sinButton, &QPushButton::clicked, this, &ScientificCalculator::sinClicked);
    }
    if (scientificUi->cosButton) {
        connect(scientificUi->cosButton, &QPushButton::clicked, this, &ScientificCalculator::cosClicked);
    }
    if (scientificUi->tanButton) {
        connect(scientificUi->tanButton, &QPushButton::clicked, this, &ScientificCalculator::tanClicked);
    }
    if (scientificUi->ctanButton) {
        connect(scientificUi->ctanButton, &QPushButton::clicked, this, &ScientificCalculator::ctanClicked);
    }

    if (scientificUi->logButton) {
        connect(scientificUi->logButton, &QPushButton::clicked, this, &ScientificCalculator::logClicked);
    }
    if (scientificUi->lnButton) {
        connect(scientificUi->lnButton, &QPushButton::clicked, this, &ScientificCalculator::lnClicked);
    }

    if (scientificUi->factorialButton) {
        connect(scientificUi->factorialButton, &QPushButton::clicked, this, &ScientificCalculator::factorialClicked);
    }
    if (scientificUi->powerButton) {
        connect(scientificUi->powerButton, &QPushButton::clicked, this, &ScientificCalculator::powerClicked);
    }
    if (scientificUi->tenPowerButton) {
        connect(scientificUi->tenPowerButton, &QPushButton::clicked, this, &ScientificCalculator::TenInXClicked);
    }
    if (scientificUi->absButton) {
        connect(scientificUi->absButton, &QPushButton::clicked, this, &ScientificCalculator::modulClicked);
    }
    if (scientificUi->modButton) {
        connect(scientificUi->modButton, &QPushButton::clicked, this, &ScientificCalculator::modClicked);
    }

    if (scientificUi->piButton) {
        connect(scientificUi->piButton, &QPushButton::clicked, this, &ScientificCalculator::piClicked);
    }
    if (scientificUi->eButton) {
        connect(scientificUi->eButton, &QPushButton::clicked, this, &ScientificCalculator::eClicked);
    }

    if (scientificUi->leftParenButton) {
        connect(scientificUi->leftParenButton, &QPushButton::clicked, this, &ScientificCalculator::leftParenClicked);
    }
    if (scientificUi->rightParenButton) {
        connect(scientificUi->rightParenButton, &QPushButton::clicked, this, &ScientificCalculator::rightParenClicked);
    }
}

void ScientificCalculator::sinClicked()
{
    bool ok;
    double value = scientificUi->display->text().toDouble(&ok);
    if (!ok) {
        scientificUi->display->setText("Error");
        return;
    }
    double result = sinDeg(value);
    if (qIsNaN(result)) {
        scientificUi->display->setText("Error");
        return;
    }
    QString formattedResult = formatNumberForDisplay(result);
    scientificUi->display->setText(formattedResult);

    m_expression.clear();
    m_expression.append({result, "", false});

    updateHistoryDisplay();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::cosClicked()
{
    bool ok;
    double value = scientificUi->display->text().toDouble(&ok);
    if (!ok) {
        scientificUi->display->setText("Error");
        return;
    }
    double result = cosDeg(value);
    if (qIsNaN(result)) {
        scientificUi->display->setText("Error");
        return;
    }
    QString formattedResult = formatNumberForDisplay(result);
    scientificUi->display->setText(formattedResult);

    m_expression.clear();
    m_expression.append({result, "", false});

    updateHistoryDisplay();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::tanClicked()
{
    bool ok;
    double value = scientificUi->display->text().toDouble(&ok);
    if (!ok) {
        scientificUi->display->setText("Error");
        return;
    }
    double result = tanDeg(value);
    if (qIsNaN(result)) {
        scientificUi->display->setText("Error");
        return;
    }
    QString formattedResult = formatNumberForDisplay(result);
    scientificUi->display->setText(formattedResult);

    m_expression.clear();
    m_expression.append({result, "", false});

    updateHistoryDisplay();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::ctanClicked()
{
    bool ok;
    double value = scientificUi->display->text().toDouble(&ok);
    if (!ok) {
        scientificUi->display->setText("Error");
        return;
    }
    double result = ctanDeg(value);
    if (qIsNaN(result)) {
        scientificUi->display->setText("Error");
        return;
    }
    QString formattedResult = formatNumberForDisplay(result);
    scientificUi->display->setText(formattedResult);

    m_expression.clear();
    m_expression.append({result, "", false});

    updateHistoryDisplay();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::logClicked()
{
    bool ok;
    double value = scientificUi->display->text().toDouble(&ok);
    if (!ok) {
        scientificUi->display->setText("Error");
        return;
    }
    double result = log10Func(value);
    if (qIsNaN(result)) {
        scientificUi->display->setText("Error");
        return;
    }
    QString formattedResult = formatNumberForDisplay(result);
    scientificUi->display->setText(formattedResult);

    m_expression.clear();
    m_expression.append({result, "", false});

    updateHistoryDisplay();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::lnClicked()
{
    bool ok;
    double value = scientificUi->display->text().toDouble(&ok);
    if (!ok) {
        scientificUi->display->setText("Error");
        return;
    }
    double result = lnFunc(value);
    if (qIsNaN(result)) {
        scientificUi->display->setText("Error");
        return;
    }
    QString formattedResult = formatNumberForDisplay(result);
    scientificUi->display->setText(formattedResult);

    m_expression.clear();
    m_expression.append({result, "", false});

    updateHistoryDisplay();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::factorialClicked()
{
    bool ok;
    int value = scientificUi->display->text().toInt(&ok);
    if (!ok || value < 0) {
        scientificUi->display->setText("Error");
        return;
    }
    double result = factorial(value);
    if (qIsNaN(result)) {
        scientificUi->display->setText("Error");
        return;
    }
    QString formattedResult = formatNumberForDisplay(result);
    scientificUi->display->setText(formattedResult);

    m_expression.clear();
    m_expression.append({result, "", false});

    updateHistoryDisplay();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::powerClicked()
{
    QString text = scientificUi->display->text();
    if (text.isEmpty()) {
        text = "0";
    }

    bool ok;
    double base = text.toDouble(&ok);
    if (!ok) {
        scientificUi->display->setText("Error");
        return;
    }

    if (m_expression.isEmpty() || m_expression.last().isOperator) {
        m_expression.append({base, "", false});
    } else {
        m_expression.last().value = base;
    }

    m_expression.append({0.0, "^", true});

    updateHistoryDisplay();
    scientificUi->display->clear();
    m_waiting_for_operand = true;
    m_newCalculation = false;
}

void ScientificCalculator::TenInXClicked()
{
    bool ok;
    double exponent = scientificUi->display->text().toDouble(&ok);
    if (!ok) {
        scientificUi->display->setText("Error");
        return;
    }
    double result = powerOf10(exponent);
    QString formattedResult = formatNumberForDisplay(result);
    scientificUi->display->setText(formattedResult);

    m_expression.clear();
    m_expression.append({result, "", false});

    updateHistoryDisplay();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::modulClicked()
{
    bool ok;
    double value = scientificUi->display->text().toDouble(&ok);
    if (!ok) {
        scientificUi->display->setText("Error");
        return;
    }
    double result = absolute(value);
    QString formattedResult = formatNumberForDisplay(result);
    scientificUi->display->setText(formattedResult);

    m_expression.clear();
    m_expression.append({result, "", false});

    updateHistoryDisplay();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::modClicked()
{
    QString text = scientificUi->display->text();
    if (text.isEmpty()) {
        text = "0";
    }

    bool ok;
    double value = text.toDouble(&ok);
    if (!ok) {
        scientificUi->display->setText("Error");
        return;
    }

    if (m_expression.isEmpty() || m_expression.last().isOperator) {
        m_expression.append({value, "", false});
    } else {
        m_expression.last().value = value;
    }

    m_expression.append({0.0, "mod", true});

    updateHistoryDisplay();
    scientificUi->display->clear();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::piClicked()
{
    scientificUi->display->clear();
    m_expression.clear();

    double piValue = MATH_PI;
    QString piStr = formatNumberForDisplay(piValue);
    scientificUi->display->setText(piStr);

    m_expression.append({piValue, "", false});

    updateHistoryDisplay();
    m_newCalculation = false;
    m_waiting_for_operand = false;
}

void ScientificCalculator::eClicked()
{
    scientificUi->display->clear();
    m_expression.clear();

    double eValue = MATH_E;
    QString eStr = formatNumberForDisplay(eValue);
    scientificUi->display->setText(eStr);

    m_expression.append({eValue, "", false});

    updateHistoryDisplay();
    m_newCalculation = false;
    m_waiting_for_operand = false;
}

void ScientificCalculator::leftParenClicked()
{
    QString currentText = scientificUi->display->text();

    if (currentText == "Error") {
        clearAll();
        currentText = "0";
    }

    if (!currentText.isEmpty() && currentText != "0") {
        bool ok;
        double value = currentText.toDouble(&ok);
        if (ok) {
            if (m_expression.isEmpty() || m_expression.last().isOperator) {
                m_expression.append({value, "", false});
            } else {
                m_expression.last().value = value;
            }
        }
    }

    if (m_newCalculation || m_expression.isEmpty()) {
        m_expression.append({0.0, "(", true});
    }
    else {
        if (!m_expression.last().isOperator) {
            m_expression.append({0.0, "×", true});
        }
        m_expression.append({0.0, "(", true});
    }

    updateHistoryDisplay();
    scientificUi->display->clear();
    m_waiting_for_operand = true;
    m_newCalculation = false;
}

void ScientificCalculator::rightParenClicked()
{
    updateExpressionWithCurrentNumber();

    int openCount = 0;
    int closeCount = 0;
    for (const auto& node : qAsConst(m_expression)) {
        if (node.isOperator) {
            if (node.operation == "(") openCount++;
            if (node.operation == ")") closeCount++;
        }
    }

    if (closeCount >= openCount) {
        return;
    }

    m_expression.append({0.0, ")", true});

    updateHistoryDisplay();
    scientificUi->display->clear();
    m_waiting_for_operand = false;
}
