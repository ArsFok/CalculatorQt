#include "scientificcalculator.h"
#include "forms/ui_scientificcalculator.h"
#include <QPushButton>
#include <QDebug>

ScientificCalculator::ScientificCalculator(QWidget *parent)
    : CalculatorBase(parent),
      ui(new Ui::ScientificCalculator)
{
}

ScientificCalculator::~ScientificCalculator()
{
    delete ui;
}

void ScientificCalculator::initialize()
{
    setupConnections();
}

void ScientificCalculator::setupUI()
{
    ui->setupUi(this);

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
    if (ui->sinButton) {
        connect(ui->sinButton, &QPushButton::clicked, this, &ScientificCalculator::sinClicked);
    }
    if (ui->cosButton) {
        connect(ui->cosButton, &QPushButton::clicked, this, &ScientificCalculator::cosClicked);
    }
    if (ui->tanButton) {
        connect(ui->tanButton, &QPushButton::clicked, this, &ScientificCalculator::tanClicked);
    }
    if (ui->ctanButton) {
        connect(ui->ctanButton, &QPushButton::clicked, this, &ScientificCalculator::ctanClicked);
    }

    if (ui->logButton) {
        connect(ui->logButton, &QPushButton::clicked, this, &ScientificCalculator::logClicked);
    }
    if (ui->lnButton) {
        connect(ui->lnButton, &QPushButton::clicked, this, &ScientificCalculator::lnClicked);
    }

    if (ui->factorialButton) {
        connect(ui->factorialButton, &QPushButton::clicked, this, &ScientificCalculator::factorialClicked);
    }
    if (ui->powerButton) {
        connect(ui->powerButton, &QPushButton::clicked, this, &ScientificCalculator::powerClicked);
    }
    if (ui->tenPowerButton) {
        connect(ui->tenPowerButton, &QPushButton::clicked, this, &ScientificCalculator::TenInXClicked);
    }
    if (ui->absButton) {
        connect(ui->absButton, &QPushButton::clicked, this, &ScientificCalculator::modulClicked);
    }
    if (ui->modButton) {
        connect(ui->modButton, &QPushButton::clicked, this, &ScientificCalculator::modClicked);
    }

    if (ui->piButton) {
        connect(ui->piButton, &QPushButton::clicked, this, &ScientificCalculator::piClicked);
    }
    if (ui->eButton) {
        connect(ui->eButton, &QPushButton::clicked, this, &ScientificCalculator::eClicked);
    }

    if (ui->leftParenButton) {
        connect(ui->leftParenButton, &QPushButton::clicked, this, &ScientificCalculator::leftParenClicked);
    }
    if (ui->rightParenButton) {
        connect(ui->rightParenButton, &QPushButton::clicked, this, &ScientificCalculator::rightParenClicked);
    }
}

void ScientificCalculator::sinClicked()
{
    bool ok;
    double value = ui->display->text().toDouble(&ok);
    if (!ok) {
        ui->display->setText("Error");
        return;
    }
    double result = sinDeg(value);
    if (qIsNaN(result)) {
        ui->display->setText("Error");
        return;
    }
    QString formattedResult = formatNumberForDisplay(result);
    ui->display->setText(formattedResult);

    m_expression.clear();
    m_expression.append({result, "", false});

    updateHistoryDisplay();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::cosClicked()
{
    bool ok;
    double value = ui->display->text().toDouble(&ok);
    if (!ok) {
        ui->display->setText("Error");
        return;
    }
    double result = cosDeg(value);
    if (qIsNaN(result)) {
        ui->display->setText("Error");
        return;
    }
    QString formattedResult = formatNumberForDisplay(result);
    ui->display->setText(formattedResult);

    m_expression.clear();
    m_expression.append({result, "", false});

    updateHistoryDisplay();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::tanClicked()
{
    bool ok;
    double value = ui->display->text().toDouble(&ok);
    if (!ok) {
        ui->display->setText("Error");
        return;
    }
    double result = tanDeg(value);
    if (qIsNaN(result)) {
        ui->display->setText("Error");
        return;
    }
    QString formattedResult = formatNumberForDisplay(result);
    ui->display->setText(formattedResult);

    m_expression.clear();
    m_expression.append({result, "", false});

    updateHistoryDisplay();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::ctanClicked()
{
    bool ok;
    double value = ui->display->text().toDouble(&ok);
    if (!ok) {
        ui->display->setText("Error");
        return;
    }
    double result = ctanDeg(value);
    if (qIsNaN(result)) {
        ui->display->setText("Error");
        return;
    }
    QString formattedResult = formatNumberForDisplay(result);
    ui->display->setText(formattedResult);

    m_expression.clear();
    m_expression.append({result, "", false});

    updateHistoryDisplay();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::logClicked()
{
    bool ok;
    double value = ui->display->text().toDouble(&ok);
    if (!ok) {
        ui->display->setText("Error");
        return;
    }
    double result = log10Func(value);
    if (qIsNaN(result)) {
        ui->display->setText("Error");
        return;
    }
    QString formattedResult = formatNumberForDisplay(result);
    ui->display->setText(formattedResult);

    m_expression.clear();
    m_expression.append({result, "", false});

    updateHistoryDisplay();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::lnClicked()
{
    bool ok;
    double value = ui->display->text().toDouble(&ok);
    if (!ok) {
        ui->display->setText("Error");
        return;
    }
    double result = lnFunc(value);
    if (qIsNaN(result)) {
        ui->display->setText("Error");
        return;
    }
    QString formattedResult = formatNumberForDisplay(result);
    ui->display->setText(formattedResult);

    m_expression.clear();
    m_expression.append({result, "", false});

    updateHistoryDisplay();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::factorialClicked()
{
    bool ok;
    int value = ui->display->text().toInt(&ok);
    if (!ok || value < 0) {
        ui->display->setText("Error");
        return;
    }
    double result = factorial(value);
    if (qIsNaN(result)) {
        ui->display->setText("Error");
        return;
    }
    QString formattedResult = formatNumberForDisplay(result);
    ui->display->setText(formattedResult);

    m_expression.clear();
    m_expression.append({result, "", false});

    updateHistoryDisplay();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::powerClicked()
{
    QString text = ui->display->text();
    if (text.isEmpty()) {
        text = "0";
    }

    bool ok;
    double base = text.toDouble(&ok);
    if (!ok) {
        ui->display->setText("Error");
        return;
    }

    if (m_expression.isEmpty() || m_expression.last().isOperator) {
        m_expression.append({base, "", false});
    } else {
        m_expression.last().value = base;
    }

    m_expression.append({0.0, "^", true});

    updateHistoryDisplay();
    ui->display->clear();
    m_waiting_for_operand = true;
    m_newCalculation = false;
}

void ScientificCalculator::TenInXClicked()
{
    bool ok;
    double exponent = ui->display->text().toDouble(&ok);
    if (!ok) {
        ui->display->setText("Error");
        return;
    }
    double result = powerOf10(exponent);
    QString formattedResult = formatNumberForDisplay(result);
    ui->display->setText(formattedResult);

    m_expression.clear();
    m_expression.append({result, "", false});

    updateHistoryDisplay();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::modulClicked()
{
    bool ok;
    double value = ui->display->text().toDouble(&ok);
    if (!ok) {
        ui->display->setText("Error");
        return;
    }
    double result = absolute(value);
    QString formattedResult = formatNumberForDisplay(result);
    ui->display->setText(formattedResult);

    m_expression.clear();
    m_expression.append({result, "", false});

    updateHistoryDisplay();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::modClicked()
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

    if (m_expression.isEmpty() || m_expression.last().isOperator) {
        m_expression.append({value, "", false});
    } else {
        m_expression.last().value = value;
    }

    m_expression.append({0.0, "mod", true});

    updateHistoryDisplay();
    ui->display->clear();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void ScientificCalculator::piClicked()
{
    ui->display->clear();
    m_expression.clear();

    double piValue = MATH_PI;
    QString piStr = formatNumberForDisplay(piValue);
    ui->display->setText(piStr);

    m_expression.append({piValue, "", false});

    updateHistoryDisplay();
    m_newCalculation = false;
    m_waiting_for_operand = false;
}

void ScientificCalculator::eClicked()
{
    ui->display->clear();
    m_expression.clear();

    double eValue = MATH_E;
    QString eStr = formatNumberForDisplay(eValue);
    ui->display->setText(eStr);

    m_expression.append({eValue, "", false});

    updateHistoryDisplay();
    m_newCalculation = false;
    m_waiting_for_operand = false;
}

void ScientificCalculator::leftParenClicked()
{
    QString currentText = ui->display->text();

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
    ui->display->clear();
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
    ui->display->clear();
    m_waiting_for_operand = false;
}
