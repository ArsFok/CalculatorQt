#include "MainWindow.h"
#include "calculatorbase.h"
#include "scientificcalculator.h"
#include "programmercalculator.h"
#include <QStackedWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
{
    // Инициализация указателей
    m_stackedWidget = nullptr;
    m_basicButton = nullptr;
    m_scientificButton = nullptr;
    m_programmerButton = nullptr;
    m_navButtonGroup = nullptr;

    setWindowTitle("Калькулятор");
    setMinimumSize(600, 800);

    // Создаем центральный виджет
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Главный layout
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    // Создаем навигацию
    mainLayout->addLayout(createNavigation());

    // Создаем stacked widget
    m_stackedWidget = new QStackedWidget();
    m_stackedWidget->setMinimumSize(580, 700);
    mainLayout->addWidget(m_stackedWidget);

    // Создаем калькуляторы
    CalculatorBase *basicCalculator = new CalculatorBase();
    ScientificCalculator *scientificCalculator = new ScientificCalculator();
    ProgrammerCalculator *programmerCalculator = new ProgrammerCalculator();

    // Инициализируем калькуляторы (после создания UI)
    basicCalculator->setupCalculator();
    scientificCalculator->setupCalculator();
    programmerCalculator->setupCalculator();

    // Добавляем в stacked widget
    m_stackedWidget->addWidget(basicCalculator);
    m_stackedWidget->addWidget(scientificCalculator);
    m_stackedWidget->addWidget(programmerCalculator);

    // Подключаем сигналы
    connect(m_stackedWidget, &QStackedWidget::currentChanged,
            this, &Calculator::onStackedWidgetChanged);

    // Устанавливаем начальный калькулятор
    m_stackedWidget->setCurrentIndex(0);
    if (m_basicButton) {
        m_basicButton->setChecked(true);
    }
}

Calculator::~Calculator()
{
}

QLayout* Calculator::createNavigation()
{
    QHBoxLayout *navLayout = new QHBoxLayout();
    navLayout->setSpacing(5);

    // Создаем кнопки навигации
    m_basicButton = new QPushButton("Обычный");
    m_scientificButton = new QPushButton("Инженерный");
    m_programmerButton = new QPushButton("Программист");

    // Делаем их переключаемыми
    m_basicButton->setCheckable(true);
    m_scientificButton->setCheckable(true);
    m_programmerButton->setCheckable(true);

    // Группа для exclusive выбора
    m_navButtonGroup = new QButtonGroup(this);
    m_navButtonGroup->setExclusive(true);
    m_navButtonGroup->addButton(m_basicButton, 0);
    m_navButtonGroup->addButton(m_scientificButton, 1);
    m_navButtonGroup->addButton(m_programmerButton, 2);

    // Подключаем сигналы
    connect(m_basicButton, &QPushButton::clicked, this, &Calculator::switchPage);
    connect(m_scientificButton, &QPushButton::clicked, this, &Calculator::switchPage);
    connect(m_programmerButton, &QPushButton::clicked, this, &Calculator::switchPage);

    // Стили для кнопок навигации
    QString buttonStyle =
        "QPushButton {"
        "   background-color: #e0e0e0;"
        "   border: 2px solid #a0a0a0;"
        "   border-radius: 8px;"
        "   padding: 8px;"
        "   font-size: 14px;"
        "   font-weight: normal;"
        "   min-width: 150px;"
        "   min-height: 40px;"
        "}"
        "QPushButton:checked {"
        "   background-color: #87CEEB;"
        "   border: 2px solid #4682b4;"
        "   font-weight: bold;"
        "   color: black;"
        "}"
        "QPushButton:hover {"
        "   background-color: #d0d0d0;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #c0c0c0;"
        "}";

    m_basicButton->setStyleSheet(buttonStyle);
    m_scientificButton->setStyleSheet(buttonStyle);
    m_programmerButton->setStyleSheet(buttonStyle);

    // Добавляем кнопки в layout
    navLayout->addWidget(m_basicButton);
    navLayout->addWidget(m_scientificButton);
    navLayout->addWidget(m_programmerButton);

    return navLayout;
}

void Calculator::switchPage()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    if (button == m_basicButton) {
        m_stackedWidget->setCurrentIndex(0);
    } else if (button == m_scientificButton) {
        m_stackedWidget->setCurrentIndex(1);
    } else if (button == m_programmerButton) {
        m_stackedWidget->setCurrentIndex(2);
    }
}

void Calculator::onStackedWidgetChanged(int index)
{
    switch (index) {
    case 0:
        if (m_basicButton) m_basicButton->setChecked(true);
        break;
    case 1:
        if (m_scientificButton) m_scientificButton->setChecked(true);
        break;
    case 2:
        if (m_programmerButton) m_programmerButton->setChecked(true);
        break;
    }
}
