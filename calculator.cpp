#include "calculator.h"
#include "mybutton.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowFlags(windowFlags() & ~Qt::FramelessWindowHint);
    setMinimumSize(600, 800);

    setStyleSheet(
         "QMainWindow {"
         "   border: 5px solid #555555;"
         "   border-radius: 5px;"
         "   background: #f0f0f0;"
         "}"
    );

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    m_stackedWidget = new QStackedWidget();
    mainLayout->addWidget(m_stackedWidget);

    // Создаем калькуляторы
    m_basicCalculator = new CalculatorBase();
    m_scientificCalculator = new ScientificCalculator();
    m_programmerCalculator = new ProgrammerCalculator();

    m_stackedWidget->addWidget(m_basicCalculator);
    m_stackedWidget->addWidget(m_scientificCalculator);
    m_stackedWidget->addWidget(m_programmerCalculator);

    createNavigation();
    m_stackedWidget->setCurrentIndex(0);
    setWindowTitle("Калькулятор");
}

Calculator::~Calculator()
{
}

void Calculator::switchPage()
{
    MyButton *button = qobject_cast<MyButton*>(sender());
    if (!button) return;

    int pageIndex = button->property("pageIndex").toInt();
    m_stackedWidget->setCurrentIndex(pageIndex);
}

void Calculator::createNavigation()
{
    QWidget *navContainer = new QWidget();
    QHBoxLayout *navLayout = new QHBoxLayout(navContainer);
    navLayout->setContentsMargins(5, 5, 5, 5);
    navLayout->setSpacing(10);

    QVector<QPair<QString, int>> modes = {
        {"Обычный", 0},
        {"Инженерный", 1},
        {"Программист", 2}
    };

    QButtonGroup *modeGroup = new QButtonGroup(this);
    modeGroup->setExclusive(true);

    for (const auto& mode : modes) {
        MyButton *btn = new MyButton(mode.first);
        connect(btn, &MyButton::clicked, this, &Calculator::switchPage);
        btn->setProperty("pageIndex", mode.second);
        btn->setCheckable(true);
        modeGroup->addButton(btn);
        navLayout->addWidget(btn);
    }

    if (!modeGroup->buttons().isEmpty()) {
        modeGroup->buttons().at(0)->setChecked(true);
    }

    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout*>(centralWidget()->layout());
    if (mainLayout) {
        mainLayout->insertWidget(0, navContainer);
    }

    QString buttonStyle =
        "QPushButton {"
        "   background-color: #f0f0f0;"
        "   border: 1px solid #ccc;"
        "   padding: 5px 10px;"
        "   border-radius: 4px;"
        "}"
        "QPushButton:checked {"
        "   background-color: #d0d0ff;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: #e0e0e0;"
        "}";

    const auto buttons = modeGroup->buttons();
    for (QAbstractButton *btn : buttons) {
        btn->setStyleSheet(buttonStyle);
    }
}
