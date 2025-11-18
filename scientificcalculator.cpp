#include "scientificcalculator.h"
#include <QGridLayout>
#include <QFrame>

ScientificCalculator::ScientificCalculator(QWidget *parent)
    : CalculatorBase(parent)
{
    setupUI();
}

void ScientificCalculator::setupUI()
{
    QFrame *frame = new QFrame();
    frame->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    frame->setStyleSheet(
        "QFrame {"
        "   background: #f0f8ff;"
        "   border: 3px solid #4682b4;"
        "   border-radius: 12px;"
        "   padding: 10px;"
        "   box-shadow: inset 0 0 10px rgba(0,0,0,0.1);"
        "}"
    );

    QGridLayout *mainLayout = new QGridLayout(frame);
    mainLayout->setSpacing(6);
    mainLayout->setContentsMargins(12, 12, 12, 12);

    m_display = createDisplay();
    mainLayout->addWidget(m_display, 0, 0, 1, 5);

    for(int i = 0; i < 10; ++i) {
        m_digitButtons[i] = createButton(QString::number(i), SLOT(digitClicked()));
    }

    QString digitButtonStyle =
        "QPushButton {"
        "   background-color: #87CEEB;"
        "   color:  black;"
        "   border: 2px solid #45a049;"
        "   border-radius: 5px;"
        "   font-size: 18px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #45a049;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #3d8b40;"
        "}";

    for (int i = 0; i < 10; ++i) {
        m_digitButtons[i]->setStyleSheet(digitButtonStyle);
    }

    // Первый ряд
    mainLayout->addWidget(createButton("sin", SLOT(sinClicked())), 1, 0);
    mainLayout->addWidget(createButton("cos", SLOT(cosClicked())), 1, 1);
    mainLayout->addWidget(createButton("tan", SLOT(tanClicked())), 1, 2);
    mainLayout->addWidget(createButton("ctan", SLOT(ctanClicked())), 1, 3);
    mainLayout->addWidget(createButton("log", SLOT(logClicked())), 1, 4);

    // Второй ряд
    mainLayout->addWidget(createButton("x^y", SLOT(powerClicked())), 2, 0);
    mainLayout->addWidget(createButton("π", SLOT(piClicked())), 2, 1);
    mainLayout->addWidget(createButton("e", SLOT(eClicked())), 2, 2);
    mainLayout->addWidget(createButton("n!", SLOT(factorialClicked())), 2, 3);
    mainLayout->addWidget(createButton("ln", SLOT(lnClicked())), 2, 4);

    // Третий ряд
    mainLayout->addWidget(createButton("10^x", SLOT(TenInXClicked())), 3, 0);
    mainLayout->addWidget(createButton("√", SLOT(unaryOperatorClicked())), 3, 1);
    mainLayout->addWidget(createButton("x²", SLOT(unaryOperatorClicked())), 3, 2);
    mainLayout->addWidget(createButton("1/x", SLOT(unaryOperatorClicked())), 3, 3);
    mainLayout->addWidget(createButton("|x|", SLOT(modulClicked())), 3, 4);

    // Четвертый ряд
    mainLayout->addWidget(createButton("(", SLOT(leftParenClicked())), 4, 0);
    mainLayout->addWidget(createButton(")", SLOT(rightParenClicked())), 4, 1);
    mainLayout->addWidget(createButton("C", SLOT(clear())), 4, 2);
    mainLayout->addWidget(createButton("CE", SLOT(clearAll())), 4, 3);
    mainLayout->addWidget(createButton("⌫", SLOT(backspaceClicked())), 4, 4);

    // Пятый ряд
    mainLayout->addWidget(m_digitButtons[7], 5, 0);
    mainLayout->addWidget(m_digitButtons[8], 5, 1);
    mainLayout->addWidget(m_digitButtons[9], 5, 2);
    mainLayout->addWidget(createButton("%", SLOT(unaryOperatorClicked())), 5, 3);
    mainLayout->addWidget(createButton("mod", SLOT(modClicked())), 5, 4);

    // Шестой ряд
    mainLayout->addWidget(m_digitButtons[4], 6, 0);
    mainLayout->addWidget(m_digitButtons[5], 6, 1);
    mainLayout->addWidget(m_digitButtons[6], 6, 2);
    mainLayout->addWidget(createButton("×", SLOT(doubleOperandClicked())), 6, 3);
    mainLayout->addWidget(createButton("÷", SLOT(doubleOperandClicked())), 6, 4);

    // Седьмой ряд
    mainLayout->addWidget(m_digitButtons[1], 7, 0);
    mainLayout->addWidget(m_digitButtons[2], 7, 1);
    mainLayout->addWidget(m_digitButtons[3], 7, 2);
    mainLayout->addWidget(createButton("+", SLOT(doubleOperandClicked())), 7, 3);
    mainLayout->addWidget(createButton("-", SLOT(doubleOperandClicked())), 7, 4);

    // Восьмой ряд
    mainLayout->addWidget(createButton("±", SLOT(changeSignClicked())), 8, 0);
    mainLayout->addWidget(m_digitButtons[0], 8, 1);
    mainLayout->addWidget(createButton(".", SLOT(pointClicked())), 8, 2);
    mainLayout->addWidget(createButton("=", SLOT(equalClicked())), 8, 3, 1, 2);

    for (int i = 0; i < 9; ++i) {
        mainLayout->setRowStretch(i, 1);
    }
    for (int j = 0; j < 5; ++j) {
        mainLayout->setColumnStretch(j, 1);
    }

    QVBoxLayout *verticalLayout = new QVBoxLayout(this);
    verticalLayout->addWidget(frame);
}

void ScientificCalculator::sinClicked()
{
    bool ok;
    double value = m_display->text().toDouble(&ok);
    if (!ok) {
        m_display->setText("Error");
        return;
    }
    double result = m_mathOps.sin(value);
    if (qIsNaN(result)) {
        m_display->setText("Error");
        return;
    }
    m_display->setText(QString::number(result));
    m_waiting_for_operand = true;
}

void ScientificCalculator::cosClicked()
{
    bool ok;
    double value = m_display->text().toDouble(&ok);
    if (!ok) {
        m_display->setText("Error");
        return;
    }
    double result = m_mathOps.cos(value);
    if (qIsNaN(result)) {
        m_display->setText("Error");
        return;
    }
    m_display->setText(QString::number(result));
    m_waiting_for_operand = true;
}

void ScientificCalculator::tanClicked()
{
    bool ok;
    double value = m_display->text().toDouble(&ok);
    if (!ok) {
        m_display->setText("Error");
        return;
    }
    double result = m_mathOps.tan(value);
    if (qIsNaN(result)) {
        m_display->setText("Error");
        return;
    }
    m_display->setText(QString::number(result));
    m_waiting_for_operand = true;
}

void ScientificCalculator::ctanClicked()
{
    bool ok;
    double value = m_display->text().toDouble(&ok);
    if (!ok) {
        m_display->setText("Error");
        return;
    }
    double result = m_mathOps.ctan(value);
    if (qIsNaN(result)) {
        m_display->setText("Error");
        return;
    }
    m_display->setText(QString::number(result));
    m_waiting_for_operand = true;
}

void ScientificCalculator::logClicked()
{
    bool ok;
    double value = m_display->text().toDouble(&ok);
    if (!ok) {
        m_display->setText("Error");
        return;
    }
    double result = m_mathOps.log10(value);
    if (qIsNaN(result)) {
        m_display->setText("Error");
        return;
    }
    m_display->setText(QString::number(result));
    m_waiting_for_operand = true;
}

void ScientificCalculator::lnClicked()
{
    bool ok;
    double value = m_display->text().toDouble(&ok);
    if (!ok) {
        m_display->setText("Error");
        return;
    }
    double result = m_mathOps.ln(value);
    if (qIsNaN(result)) {
        m_display->setText("Error");
        return;
    }
    m_display->setText(QString::number(result));
    m_waiting_for_operand = true;
}

void ScientificCalculator::factorialClicked()
{
    bool ok;
    int value = m_display->text().toInt(&ok);
    if (!ok) {
        m_display->setText("Error");
        return;
    }
    double result = m_mathOps.factorial(value);
    if (qIsNaN(result)) {
        m_display->setText("Error");
        return;
    }
    m_display->setText(QString::number(result));
    m_waiting_for_operand = true;
}

void ScientificCalculator::powerClicked()
{
    bool ok;
    double base = m_display->text().toDouble(&ok);
    if (!ok) {
        m_display->setText("Error");
        return;
    }
    m_stored_value = base;
    m_pending_operation = "^";
    m_waiting_for_operand = true;
}

void ScientificCalculator::piClicked()
{
    m_display->setText(QString::number(MathOperations::PI));
    m_waiting_for_operand = false;
}

void ScientificCalculator::eClicked()
{
    m_display->setText(QString::number(MathOperations::E));
    m_waiting_for_operand = false;
}

void ScientificCalculator::TenInXClicked()
{
    bool ok;
    double exponent = m_display->text().toDouble(&ok);
    if (!ok) {
        m_display->setText("Error");
        return;
    }
    double result = m_mathOps.powerOf10(exponent);
    m_display->setText(QString::number(result));
    m_waiting_for_operand = true;
}

void ScientificCalculator::modulClicked()
{
    bool ok;
    double value = m_display->text().toDouble(&ok);
    if (!ok) {
        m_display->setText("Error");
        return;
    }
    double result = m_mathOps.absolute(value);
    m_display->setText(QString::number(result));
    m_waiting_for_operand = true;
}

void ScientificCalculator::modClicked()
{
    bool ok;
    double value = m_display->text().toDouble(&ok);
    if (!ok) {
        m_display->setText("Error");
        return;
    }
    m_stored_value = value;
    m_pending_operation = "mod";
    m_waiting_for_operand = true;
}

void ScientificCalculator::leftParenClicked()
{
    if (m_waiting_for_operand) {
        m_display->clear();
        m_waiting_for_operand = false;
    }
    m_display->setText(m_display->text() + "(");
}

void ScientificCalculator::rightParenClicked()
{
    if (m_waiting_for_operand) {
        return;
    }
    m_display->setText(m_display->text() + ")");
}
