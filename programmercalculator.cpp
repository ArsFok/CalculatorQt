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

    for(int i = 0; i < 10; ++i) {
        if (!m_digitButtons[i]) {
            m_digitButtons[i] = createButton(QString::number(i), SLOT(numberSystemDigitClicked()));
        }
    }

    m_buttonA = createButton("A", SLOT(numberSystemDigitClicked()));
    m_buttonB = createButton("B", SLOT(numberSystemDigitClicked()));
    m_buttonC = createButton("C", SLOT(numberSystemDigitClicked()));
    m_buttonD = createButton("D", SLOT(numberSystemDigitClicked()));
    m_buttonE = createButton("E", SLOT(numberSystemDigitClicked()));
    m_buttonF = createButton("F", SLOT(numberSystemDigitClicked()));

    m_buttonA->setEnabled(false);
    m_buttonB->setEnabled(false);
    m_buttonC->setEnabled(false);
    m_buttonD->setEnabled(false);
    m_buttonE->setEnabled(false);
    m_buttonF->setEnabled(false);

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

    connect(baseGroup, SIGNAL(buttonClicked(QAbstractButton*)),
            this, SLOT(onBaseGroupButtonClicked(QAbstractButton*)));

    updateNumberSystemDisplays();
    updateNumberSystemButtons();

    qDebug() << "Programmer Calculator UI setup complete";
}

void ProgrammerCalculator::leftParenClicked()
{
    if (m_waiting_for_operand) {
        m_display->clear();
        m_waiting_for_operand = false;
    }
    m_display->setText(m_display->text() + "(");
}

void ProgrammerCalculator::rightParenClicked()
{
    if (m_waiting_for_operand) {
        return;
    }
    m_display->setText(m_display->text() + ")");
}

void ProgrammerCalculator::onBaseGroupButtonClicked(QAbstractButton* button)
{
    if (!button) return;

    int newBase = button->property("base").toInt();
    qDebug() << "Switching to base:" << newBase;

    m_current_base = newBase;

    m_display->setText("0");
    m_historyDisplay->setText("0");
    m_waiting_for_operand = true;
    m_newCalculation = true;

    updateNumberSystemButtons();
    updateNumberSystemDisplays();
}

void ProgrammerCalculator::numberSystemDigitClicked()
{
    MyButton *clickedButton = qobject_cast<MyButton*>(sender());
    if (!clickedButton) return;

    QString digit = clickedButton->text().toUpper();

    bool isDigitValid = false;
    if (m_current_base <= 10) {
        isDigitValid = digit[0].isDigit() && digit.toInt(nullptr, m_current_base) < m_current_base;
    } else {
        isDigitValid = (digit >= "0" && digit <= "9") || (digit >= "A" && digit <= "F");
    }

    if (!isDigitValid) {
        qDebug() << "Недопустимая цифра" << digit << "для системы с основанием" << m_current_base;
        return;
    }

    QString currentText = m_display->text();
    if (currentText == "0" || m_waiting_for_operand || m_newCalculation) {
        currentText = digit;
        m_waiting_for_operand = false;
        m_newCalculation = false;
    } else {
        currentText += digit;
    }

    m_display->setText(currentText);
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
    qDebug() << "Обновление отображений для числа:" << input;

    if (input.isEmpty() || input == "Error") {
        if (m_display_bin) m_display_bin->setText("0");
        if (m_display_oct) m_display_oct->setText("0");
        if (m_display_dec) m_display_dec->setText("0");
        if (m_display_hex) m_display_hex->setText("0");
        return;
    }

    bool ok;
    long long num = 0;

    if (input.startsWith("-")) {
        // Отрицательное число
        QString absInput = input.mid(1);
        num = absInput.toLongLong(&ok, m_current_base);
        if (ok) {
            num = -num;
        }
    } else {
        num = input.toLongLong(&ok, m_current_base);
    }

    if (!ok) {
        qDebug() << "Ошибка преобразования числа:" << input;
        if (m_display_bin) m_display_bin->setText("Error");
        if (m_display_oct) m_display_oct->setText("Error");
        if (m_display_dec) m_display_dec->setText("Error");
        if (m_display_hex) m_display_hex->setText("Error");
        return;
    }

    // Для отрицательных чисел используем дополнительный код
    if (num < 0) {
        unsigned long long unsignedNum;

        if (m_current_base == 2) {
            // Для двоичной системы - 32-битное представление
            unsignedNum = static_cast<unsigned int>(static_cast<int>(num));
            if (m_display_bin) m_display_bin->setText(QString::number(unsignedNum, 2).rightJustified(32, '0'));
        } else {
            unsignedNum = static_cast<unsigned long long>(num);
            if (m_display_bin) m_display_bin->setText(QString::number(unsignedNum, 2));
        }

        if (m_display_oct) m_display_oct->setText(QString::number(unsignedNum, 8));
        if (m_display_dec) m_display_dec->setText(QString::number(num, 10));
        if (m_display_hex) m_display_hex->setText(QString::number(unsignedNum, 16).toUpper());
    } else {
        // Положительные числа
        if (m_display_bin) m_display_bin->setText(QString::number(num, 2));
        if (m_display_oct) m_display_oct->setText(QString::number(num, 8));
        if (m_display_dec) m_display_dec->setText(QString::number(num, 10));
        if (m_display_hex) m_display_hex->setText(QString::number(num, 16).toUpper());
    }
}

void ProgrammerCalculator::updateNumberSystemButtons()
{
    bool hexMode = (m_current_base == 16);

    if (m_buttonA) m_buttonA->setEnabled(hexMode);
    if (m_buttonB) m_buttonB->setEnabled(hexMode);
    if (m_buttonC) m_buttonC->setEnabled(hexMode);
    if (m_buttonD) m_buttonD->setEnabled(hexMode);
    if (m_buttonE) m_buttonE->setEnabled(hexMode);
    if (m_buttonF) m_buttonF->setEnabled(hexMode);

    for (int i = 0; i < 10; ++i) {
        if (m_digitButtons[i]) {
            m_digitButtons[i]->setEnabled(i < m_current_base);
        }
    }
}
