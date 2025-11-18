#include "calculatorbase.h"
#include <QGridLayout>
#include <QFrame>

CalculatorBase::CalculatorBase(QWidget *parent)
    : QWidget(parent),
      m_mathOps(this),
      m_sum_in_memory(0.0),
      m_pending_operation(""),
      m_stored_value(0.0),
      m_waiting_for_operand(true),
      m_result(0.0),
      m_newCalculation(true)
{
    setupUI();
}

void CalculatorBase::setupUI()
{
    QFrame *frame = new QFrame();
    frame->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
    frame->setLineWidth(2);
    frame->setStyleSheet(
         "QFrame {"
         "   background: #f0f8ff;"
         "   border: 3px solid #4682b4;"
         "   border-radius: 12px;"
         "   padding: 10px;"
         "   box-shadow: inset 0 0 10px rgba(0,0,0,0.1);"
         "}"
    );

    QGridLayout *layout = new QGridLayout(frame);
    layout->setSpacing(8);
    layout->setContentsMargins(10, 10, 10, 10);

    m_display = createDisplay();
    layout->addWidget(m_display, 0, 0, 1, 4);

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

    // Размещение кнопок памяти
    layout->addWidget(clearMemoryButton, 1, 0);
    layout->addWidget(readMemoryButton, 1, 1);
    layout->addWidget(addToMemoryButton, 1, 2);
    layout->addWidget(minToMemoryButton, 1, 3);

    // Второй ряд
    layout->addWidget(percentButton, 2, 0);
    layout->addWidget(clearButton, 2, 2);
    layout->addWidget(clearAllButton, 2, 1);
    layout->addWidget(backspaceButton, 2, 3);

    // Третий ряд
    layout->addWidget(squareButton, 3, 0);
    layout->addWidget(powerButton, 3, 1);
    layout->addWidget(reciprocalButton, 3, 2);
    layout->addWidget(divisionButton, 3, 3);

    // Четвертый ряд
    layout->addWidget(m_digitButtons[7], 4, 0);
    layout->addWidget(m_digitButtons[8], 4, 1);
    layout->addWidget(m_digitButtons[9], 4, 2);
    layout->addWidget(timesButton, 4, 3);

    // Пятый ряд
    layout->addWidget(m_digitButtons[4], 5, 0);
    layout->addWidget(m_digitButtons[5], 5, 1);
    layout->addWidget(m_digitButtons[6], 5, 2);
    layout->addWidget(minusButton, 5, 3);

    // Шестой ряд
    layout->addWidget(m_digitButtons[1], 6, 0);
    layout->addWidget(m_digitButtons[2], 6, 1);
    layout->addWidget(m_digitButtons[3], 6, 2);
    layout->addWidget(plusButton, 6, 3);

    // Седьмой ряд
    layout->addWidget(changeSignButton, 7, 0);
    layout->addWidget(m_digitButtons[0], 7, 1);
    layout->addWidget(pointButton, 7, 2);
    layout->addWidget(equalButton, 7, 3);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(frame);
}

// Реализация методов CalculatorBase...
// (остальные методы аналогичны вашему исходному коду)

QLineEdit* CalculatorBase::createDisplay()
{
    QLineEdit *display = new QLineEdit("0");
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setStyleSheet(
            "QLineEdit {"
            "   font-size: 30px;"
            "   padding: 5px;"
            "   border: 2px solid #555;"
            "   background-color: #f8f8f8;"
            "   min-height: 30px;"
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

    if (m_waiting_for_operand || m_newCalculation) {
        m_display->clear();
        m_newCalculation = false;
        m_waiting_for_operand = false;
        if (m_newCalculation) {
            m_expression.clear();
        }
    }

    m_display->setText(m_display->text() + digit);
}

void CalculatorBase::pointClicked()
{
    if (m_waiting_for_operand) {
        m_display->setText("0");
    }
    if (!m_display->text().contains('.')) {
        m_display->setText(m_display->text() + ".");
    }
    m_waiting_for_operand = false;
}

void CalculatorBase::changeSignClicked()
{
    QString text = m_display->text();
    bool ok;
    double value = text.toDouble(&ok);
    if (!ok) {
        m_display->setText("Error");
        return;
    }
    double result = m_mathOps.changeSign(value);
    m_display->setText(QString::number(result));
}

void CalculatorBase::backspaceClicked()
{
    if (m_waiting_for_operand) return;

    QString text = m_display->text();
    if (text.isEmpty()) return;

    if (text.endsWith(' ')) {
        text.chop(3);
        if (!m_expression.isEmpty() && m_expression.last().isOperator) {
            m_expression.removeLast();
        }
    } else {
        text.chop(1);
    }

    m_display->setText(text.isEmpty() ? "0" : text);
}

void CalculatorBase::clear()
{
    m_display->setText("0");
    m_waiting_for_operand = true;
}

void CalculatorBase::clearAll()
{
    m_display->setText("0");
    m_expression.clear();
    m_stored_value = 0.0;
    m_result = 0.0;
    m_waiting_for_operand = true;
    m_newCalculation = true;
    m_pending_operation = "";
}

void CalculatorBase::unaryOperatorClicked()
{
    MyButton *clickedButton = qobject_cast<MyButton*>(sender());
    if (!clickedButton) return;

    QString operation = clickedButton->text();
    QString displayText = m_display->text();

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
    QString operationText;

    if (operation == "1/x") {
        result = m_mathOps.reciprocal(operand);
        operationText = QString("1/(%1)").arg(operand);
    }
    else if (operation == "x²") {
        result = m_mathOps.square(operand);
        operationText = QString("(%1)²").arg(operand);
    }
    else if (operation == "√") {
        result = m_mathOps.squareRoot(operand);
        operationText = QString("√(%1)").arg(operand);
    }
    else if (operation == "%") {
        result = m_mathOps.percent(operand);
        operationText = QString("%1%").arg(operand);
    }

    if (qIsNaN(result)) {
        m_display->setText("Error");
        return;
    }

    m_display->setText(QString("%1 = %2").arg(operationText).arg(result));
    m_waiting_for_operand = true;
    m_newCalculation = true;
    m_result = result;
}

void CalculatorBase::doubleOperandClicked()
{
    MyButton *clickedButton = qobject_cast<MyButton*>(sender());
    if (!clickedButton) return;

    QString operation = clickedButton->text();
    bool ok;
    double operand = m_display->text().toDouble(&ok);
    if (!ok) {
        m_display->setText("Error");
        return;
    }

    if (!m_newCalculation) {
        m_stored_value = operand;
    } else {
        m_stored_value = m_result;
    }

    m_expression.append({m_stored_value, "", false});
    m_expression.append({0.0, operation, true});

    m_pending_operation = operation;
    m_display->setText(QString("%1 %2 ").arg(m_stored_value).arg(operation));
    m_newCalculation = true;
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
    bool ok;
    double operand = m_display->text().toDouble(&ok);
    if (!ok) {
        m_display->setText("Error");
        return;
    }

    m_expression.append({operand, "", false});

    QString exprStr;
    for (const auto& node : qAsConst(m_expression)) {
        if (node.isOperator) {
            exprStr += " " + node.operation + " ";
        } else {
            exprStr += QString::number(node.value);
        }
    }
    exprStr += " =";

    double result = m_mathOps.evaluateExpression(m_expression);
    if (qIsNaN(result)) {
        m_display->setText("Error");
        return;
    }

    m_display->setText(exprStr + " " + QString::number(result));
    m_result = result;
    m_expression.clear();
    m_waiting_for_operand = true;
    m_newCalculation = true;
}

void CalculatorBase::clearMemory()
{
    m_sum_in_memory = 0.0;
}

void CalculatorBase::readMemory()
{
    m_display->setText(QString::number(m_sum_in_memory));
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

void CalculatorBase::showOperation(const QString &operationText, double result)
{
    QString displayText = QString("%1 = %2")
                        .arg(operationText)
                        .arg(result);
    m_display->setText(displayText);
}
