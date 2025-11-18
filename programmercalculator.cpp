#include "programmercalculator.h"
#include <QGridLayout>
#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QButtonGroup>
#include <QRadioButton>
#include <QDebug>
#include <QToolTip>

ProgrammerCalculator::ProgrammerCalculator(QWidget *parent)
    : CalculatorBase(parent),
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
    setupUI();
}

void ProgrammerCalculator::setupUI()
{
    QFrame *frame = new QFrame();
    frame->setFrameStyle(QFrame::Box | QFrame::Plain);
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
    layout->setSpacing(6);
    layout->setContentsMargins(8, 8, 8, 8);

    // Создаем дисплеи для систем счисления
    QVBoxLayout *displayLayout = new QVBoxLayout();

    QLabel *binLabel = new QLabel("BIN:");
    m_display_bin = createDisplay();
    m_display_bin->setStyleSheet("font-size: 20px;");
    displayLayout->addWidget(binLabel);
    displayLayout->addWidget(m_display_bin);

    QLabel *octLabel = new QLabel("OCT:");
    m_display_oct = createDisplay();
    m_display_oct->setStyleSheet("font-size: 20px;");
    displayLayout->addWidget(octLabel);
    displayLayout->addWidget(m_display_oct);

    QLabel *decLabel = new QLabel("DEC:");
    m_display_dec = createDisplay();
    m_display_dec->setStyleSheet("font-size: 20px;");
    displayLayout->addWidget(decLabel);
    displayLayout->addWidget(m_display_dec);

    QLabel *hexLabel = new QLabel("HEX:");
    m_display_hex = createDisplay();
    m_display_hex->setStyleSheet("font-size: 20px;");
    displayLayout->addWidget(hexLabel);
    displayLayout->addWidget(m_display_hex);

    layout->addLayout(displayLayout, 0, 0, 1, 5);

    // Основной дисплей
    m_display = createDisplay();
    layout->addWidget(m_display, 1, 0, 1, 5);

    // Кнопки управления
    layout->addWidget(createButton("C", SLOT(clear())), 2, 0);
    layout->addWidget(createButton("CE", SLOT(clearAll())), 2, 1);
    layout->addWidget(createButton("⌫", SLOT(backspaceClicked())), 2, 2);
    layout->addWidget(createButton("±", SLOT(changeSignClicked())), 2, 3);
    layout->addWidget(createButton("=", SLOT(equalClicked())), 2, 4);

    // Радио-кнопки для выбора системы счисления
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
    layout->addLayout(baseLayout, 3, 0, 1, 5);

    // Создаем цифровые кнопки (0-F)
    for (int i = 0; i < 16; ++i) {
        QString text = QString::number(i, 16).toUpper();
        m_digitButtons[i] = createButton(text, SLOT(numberSystemDigitClicked()));
        m_digitButtons[i]->setEnabled(i < 10); // По умолчанию только 0-9
    }

    // Создаем буквенные кнопки для HEX
    m_buttonA = createButton("A", SLOT(numberSystemDigitClicked()));
    m_buttonB = createButton("B", SLOT(numberSystemDigitClicked()));
    m_buttonC = createButton("C", SLOT(numberSystemDigitClicked()));
    m_buttonD = createButton("D", SLOT(numberSystemDigitClicked()));
    m_buttonE = createButton("E", SLOT(numberSystemDigitClicked()));
    m_buttonF = createButton("F", SLOT(numberSystemDigitClicked()));

    // Изначально отключаем буквенные кнопки
    m_buttonA->setEnabled(false);
    m_buttonB->setEnabled(false);
    m_buttonC->setEnabled(false);
    m_buttonD->setEnabled(false);
    m_buttonE->setEnabled(false);
    m_buttonF->setEnabled(false);

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
        "}"
        "QPushButton:disabled {"
        "   background-color: #cccccc;"
        "   color: #666666;"
        "   border: 2px solid #999999;"
        "}";

    for (int i = 0; i < 10; ++i) {
        m_digitButtons[i]->setStyleSheet(digitButtonStyle);
    }

    m_buttonA->setStyleSheet(digitButtonStyle);
    m_buttonB->setStyleSheet(digitButtonStyle);
    m_buttonC->setStyleSheet(digitButtonStyle);
    m_buttonD->setStyleSheet(digitButtonStyle);
    m_buttonE->setStyleSheet(digitButtonStyle);
    m_buttonF->setStyleSheet(digitButtonStyle);

    // Размещение кнопок
    // Первый ряд цифр
    layout->addWidget(m_digitButtons[7], 4, 1);
    layout->addWidget(m_digitButtons[8], 4, 2);
    layout->addWidget(m_digitButtons[9], 4, 3);
    layout->addWidget(createButton("AND", SLOT(bitwiseOperationClicked())), 4, 4);

    // Второй ряд цифр
    layout->addWidget(m_digitButtons[4], 5, 1);
    layout->addWidget(m_digitButtons[5], 5, 2);
    layout->addWidget(m_digitButtons[6], 5, 3);
    layout->addWidget(createButton("OR", SLOT(bitwiseOperationClicked())), 5, 4);

    // Третий ряд цифр
    layout->addWidget(m_digitButtons[1], 6, 1);
    layout->addWidget(m_digitButtons[2], 6, 2);
    layout->addWidget(m_digitButtons[3], 6, 3);
    layout->addWidget(createButton("XOR", SLOT(bitwiseOperationClicked())), 6, 4);

    // Четвертый ряд цифр
    layout->addWidget(m_digitButtons[0], 7, 3);
    layout->addWidget(createButton("NOT", SLOT(bitwiseOperationClicked())), 7, 4);

    // Буквенные кнопки (A-F)
    layout->addWidget(m_buttonA, 4, 0);
    layout->addWidget(m_buttonB, 5, 0);
    layout->addWidget(m_buttonC, 6, 0);
    layout->addWidget(m_buttonD, 7, 0);
    layout->addWidget(m_buttonE, 7, 1);
    layout->addWidget(m_buttonF, 7, 2);

    // Добавляем tooltip'ы
    m_buttonA->setToolTip("Шестнадцатеричная цифра A (10 в десятичной)");
    m_buttonB->setToolTip("Шестнадцатеричная цифра B (11 в десятичной)");
    m_buttonC->setToolTip("Шестнадцатеричная цифра C (12 в десятичной)");
    m_buttonD->setToolTip("Шестнадцатеричная цифра D (13 в десятичной)");
    m_buttonE->setToolTip("Шестнадцатеричная цифра E (14 в десятичной)");
    m_buttonF->setToolTip("Шестнадцатеричная цифра F (15 в десятичной)");

    // Исправленное подключение - используем старый стиль connect
    connect(baseGroup, SIGNAL(buttonClicked(QAbstractButton*)),
            this, SLOT(onBaseGroupButtonClicked(QAbstractButton*)));

    QVBoxLayout *verticalLayout = new QVBoxLayout(this);
    verticalLayout->addWidget(frame);
}

// Добавляем слот для обработки кликов по радио-кнопкам
void ProgrammerCalculator::onBaseGroupButtonClicked(QAbstractButton* button)
{
    if (!button) return;

    int newBase = button->property("base").toInt();
    qDebug() << "Switching to base:" << newBase;

    if (newBase != 2 && newBase != 8 && newBase != 10 && newBase != 16) {
        qDebug() << "Invalid base:" << newBase;
        return;
    }

    m_current_base = newBase;

    // Конвертируем текущее значение
    bool ok;
    QString currentText = m_display->text();
    long value = currentText.toLong(&ok, m_current_base);

    if (!ok) {
        qDebug() << "Conversion error for:" << currentText;
        value = 0;
    }

    updateNumberSystemButtons();
    m_display->setText(QString::number(value, m_current_base).toUpper());
    m_waiting_for_operand = false;
    m_newCalculation = false;

    updateNumberSystemDisplays();
}

void ProgrammerCalculator::numberSystemDigitClicked()
{
    MyButton *clickedButton = qobject_cast<MyButton*>(sender());
    if (!clickedButton) return;

    QString digit = clickedButton->text().toUpper();

    // Проверка допустимости цифры для текущей системы счисления
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
    if (currentText == "0" || m_waiting_for_operand) {
        currentText = digit;
        m_waiting_for_operand = false;
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

    bool ok;
    long num = input.toLong(&ok, m_current_base);

    if (!ok) {
        qDebug() << "Ошибка преобразования числа";
        if (m_display_bin) m_display_bin->setText("Error");
        if (m_display_oct) m_display_oct->setText("Error");
        if (m_display_dec) m_display_dec->setText("Error");
        if (m_display_hex) m_display_hex->setText("Error");
        return;
    }

    // Проверка на переполнение
    if (num < 0 || num > std::numeric_limits<long>::max()) {
        qDebug() << "Число вне допустимого диапазона";
        if (m_display_bin) m_display_bin->setText("Overflow");
        if (m_display_oct) m_display_oct->setText("Overflow");
        if (m_display_dec) m_display_dec->setText("Overflow");
        if (m_display_hex) m_display_hex->setText("Overflow");
        return;
    }

    if (m_display_bin) m_display_bin->setText(QString::number(num, 2));
    if (m_display_oct) m_display_oct->setText(QString::number(num, 8));
    if (m_display_dec) m_display_dec->setText(QString::number(num, 10));
    if (m_display_hex) m_display_hex->setText(QString::number(num, 16).toUpper());
}

void ProgrammerCalculator::updateNumberSystemButtons()
{
    bool hexMode = (m_current_base == 16);

    // Включение буквенных кнопок
    if (m_buttonA) m_buttonA->setEnabled(hexMode);
    if (m_buttonB) m_buttonB->setEnabled(hexMode);
    if (m_buttonC) m_buttonC->setEnabled(hexMode);
    if (m_buttonD) m_buttonD->setEnabled(hexMode);
    if (m_buttonE) m_buttonE->setEnabled(hexMode);
    if (m_buttonF) m_buttonF->setEnabled(hexMode);

    // Включение цифровых кнопок
    for (int i = 0; i < 10; ++i) {
        if (m_digitButtons[i]) {
            m_digitButtons[i]->setEnabled(i < m_current_base);
        }
    }
}
