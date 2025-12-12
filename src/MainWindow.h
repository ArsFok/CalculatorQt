#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

// Forward declarations
class QStackedWidget;
class CalculatorBase;
class ScientificCalculator;
class ProgrammerCalculator;
class QButtonGroup;
class QWidget;

class Calculator : public QMainWindow
{
    Q_OBJECT

public:
    explicit Calculator(QWidget *parent = nullptr);
    ~Calculator();

private slots:
    void switchPage();
    void onStackedWidgetChanged(int index);

private:
    void createNavigation();

    QStackedWidget *m_stackedWidget;
    CalculatorBase *m_basicCalculator;
    ScientificCalculator *m_scientificCalculator;
    ProgrammerCalculator *m_programmerCalculator;

    QButtonGroup *m_navButtonGroup;
    QWidget *m_navContainer;
};

#endif // MAINWINDOW_H
