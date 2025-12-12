#include "MainWindow.h"
#include "calculatorbase.h"
#include "scientificcalculator.h"
#include "programmercalculator.h"
#include "mybutton.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QStackedWidget>
#include <QDebug>

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent),
      m_stackedWidget(nullptr),
      m_basicCalculator(nullptr),
      m_scientificCalculator(nullptr),
      m_programmerCalculator(nullptr),
      m_navButtonGroup(nullptr)
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
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    createNavigation();
    mainLayout->addWidget(m_navContainer);

    m_stackedWidget = new QStackedWidget();
    m_stackedWidget->setMinimumSize(580, 700);
    mainLayout->addWidget(m_stackedWidget);

    m_basicCalculator = new CalculatorBase();
    m_scientificCalculator = new ScientificCalculator();
    m_programmerCalculator = new ProgrammerCalculator();

    m_stackedWidget->addWidget(m_basicCalculator);
    m_stackedWidget->addWidget(m_scientificCalculator);
    m_stackedWidget->addWidget(m_programmerCalculator);

    connect(m_stackedWidget, &QStackedWidget::currentChanged,
            this, &Calculator::onStackedWidgetChanged);

    m_stackedWidget->setCurrentIndex(0);
    setWindowTitle("Калькулятор");

    qDebug() << "Calculator initialized. Stacked widget count:" << m_stackedWidget->count();
}

Calculator::~Calculator()
{
    qDebug() << "Calculator destroyed";
}

void Calculator::switchPage()
{
    MyButton *button = qobject_cast<MyButton*>(sender());
    if (!button) {
        qDebug() << "Switch page: sender is not a MyButton";
        return;
    }

    bool ok;
    int pageIndex = button->property("pageIndex").toInt(&ok);

    if (!ok) {
        qDebug() << "Switch page: invalid pageIndex property";
        return;
    }

    if (pageIndex >= 0 && pageIndex < m_stackedWidget->count()) {
        qDebug() << "Switching to page:" << pageIndex;
        m_stackedWidget->setCurrentIndex(pageIndex);

        m_stackedWidget->update();
        update();
    } else {
        qDebug() << "Invalid page index:" << pageIndex << "max:" << m_stackedWidget->count() - 1;
    }
}

void Calculator::onStackedWidgetChanged(int index)
{
    qDebug() << "Stacked widget changed to index:" << index;

    if (m_navButtonGroup) {
        QAbstractButton *button = m_navButtonGroup->button(index);
        if (button) {
            button->setChecked(true);
            qDebug() << "Set button" << index << "to checked";
        }
    }

    update();
}

void Calculator::createNavigation()
{
    m_navContainer = new QWidget();
    m_navContainer->setFixedHeight(50);

    QHBoxLayout *navLayout = new QHBoxLayout(m_navContainer);
    navLayout->setContentsMargins(0, 0, 0, 0);
    navLayout->setSpacing(5);

    m_navButtonGroup = new QButtonGroup(this);
    m_navButtonGroup->setExclusive(true);

    QVector<QPair<QString, int>> modes = {
        {"Обычный", 0},
        {"Инженерный", 1},
        {"Программист", 2}
    };

    for (const auto& mode : modes) {
        MyButton *btn = new MyButton(mode.first);
        btn->setFixedSize(150, 40);
        connect(btn, &MyButton::clicked, this, &Calculator::switchPage);
        btn->setProperty("pageIndex", mode.second);
        btn->setCheckable(true);
        m_navButtonGroup->addButton(btn, mode.second);
        navLayout->addWidget(btn);

        qDebug() << "Created button:" << mode.first << "with pageIndex:" << mode.second;
    }

    if (!m_navButtonGroup->buttons().isEmpty()) {
        m_navButtonGroup->button(0)->setChecked(true);
        qDebug() << "Set initial button to checked";
    }

    QString buttonStyle =
        "QPushButton {"
        "   background-color: #e0e0e0;"
        "   border: 2px solid #a0a0a0;"
        "   border-radius: 8px;"
        "   padding: 8px;"
        "   font-size: 14px;"
        "   font-weight: normal;"
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

    const auto buttons = m_navButtonGroup->buttons();
    for (QAbstractButton *btn : buttons) {
        btn->setStyleSheet(buttonStyle);
    }
}
