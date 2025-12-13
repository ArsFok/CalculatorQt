#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QStackedWidget;
class QPushButton;
class QButtonGroup;
class QVBoxLayout;
class QHBoxLayout;

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
    QLayout* createNavigation();

    QStackedWidget *m_stackedWidget;
    QPushButton *m_basicButton;
    QPushButton *m_scientificButton;
    QPushButton *m_programmerButton;
    QButtonGroup *m_navButtonGroup;
};

#endif // MAINWINDOW_H
