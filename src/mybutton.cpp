#include "mybutton.h"

MyButton::MyButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setStyleSheet("font-size: 18px; padding: 10px;");
}
