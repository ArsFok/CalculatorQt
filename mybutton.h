#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QPushButton>

class MyButton : public QPushButton
{
    Q_OBJECT

public:
    explicit MyButton(const QString &text, QWidget *parent = nullptr);
};

#endif // MYBUTTON_H
