#include "mathoperations.h"
#include <cmath>
#include <QDebug>

MathOperations::MathOperations(QObject *parent) : QObject(parent)
{
}

double MathOperations::add(double a, double b)
{
    return a + b;
}

double MathOperations::subtract(double a, double b)
{
    return a - b;
}

double MathOperations::multiply(double a, double b)
{
    return a * b;
}

double MathOperations::divide(double a, double b)
{
    if (qFuzzyIsNull(b))
        return std::numeric_limits<double>::quiet_NaN();
    return a / b;
}

double MathOperations::squareRoot(double x)
{
    if (x < 0.0)
        return std::numeric_limits<double>::quiet_NaN();
    return std::sqrt(x);
}

double MathOperations::square(double x)
{
    double result = x * x;

    // Проверяем на переполнение
    if (qIsInf(result)) {
        return std::numeric_limits<double>::infinity();
    }

    return result;
}

double MathOperations::reciprocal(double x)
{
    if (qFuzzyIsNull(x))
        return std::numeric_limits<double>::quiet_NaN();
    return 1.0 / x;
}

double MathOperations::percent(double x)
{
    return x * 0.01;
}

double MathOperations::changeSign(double x)
{
    return -x;
}

double MathOperations::absolute(double x)
{
    return std::fabs(x);
}

double MathOperations::sin(double x)
{
    return std::sin(qDegreesToRadians(x));
}

double MathOperations::cos(double x)
{
    return std::cos(qDegreesToRadians(x));
}

double MathOperations::tan(double x)
{
    double radians = qDegreesToRadians(x);
    if (qFuzzyCompare(std::cos(radians), 0.0))
        return std::numeric_limits<double>::quiet_NaN();
    return std::tan(radians);
}

double MathOperations::ctan(double x)
{
    double radians = qDegreesToRadians(x);
    if (qFuzzyCompare(std::sin(radians), 0.0))
        return std::numeric_limits<double>::quiet_NaN();
    return 1.0 / std::tan(radians);
}

double MathOperations::log10(double x)
{
    if (x <= 0)
        return std::numeric_limits<double>::quiet_NaN();
    return std::log10(x);
}

double MathOperations::ln(double x)
{
    if (x <= 0)
        return std::numeric_limits<double>::quiet_NaN();
    return std::log(x);
}

double MathOperations::powerOf10(double x)
{
    return std::pow(10.0, x);
}

double MathOperations::factorial(int n)
{
    if (n < 0)
        return std::numeric_limits<double>::quiet_NaN();

    if (n > 170)
        return std::numeric_limits<double>::infinity();

    double result = 1.0;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

double MathOperations::power(double base, double exponent)
{
    qDebug() << "Power called: base =" << base << "exponent =" << exponent;

    // Особый случай: 0^0
    if (qFuzzyIsNull(base) && qFuzzyIsNull(exponent)) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    // Проверка для отрицательного основания и дробной степени
    if (base < 0 && std::fmod(exponent, 1.0) != 0.0) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    // Проверка деления на ноль
    if (qFuzzyIsNull(base) && exponent < 0) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    double result = std::pow(base, exponent);
    qDebug() << "Power result:" << result;

    return result;
}


double MathOperations::modulus(double a, double b)
{
    if (qFuzzyIsNull(b))
        return std::numeric_limits<double>::quiet_NaN();
    return std::fmod(a, b);
}

int MathOperations::getPriority(const QString& op)
{
    if (op == "(" || op == ")") return 0;
    if (op == "+" || op == "-") return 1;
    if (op == "×" || op == "÷" || op == "mod") return 2;
    if (op == "mod") return 2;
    if (op == "^") return 3;
    return -1;
}

bool MathOperations::isOperator(const QString& token)
{
    return token == "+" || token == "-" || token == "×" || token == "÷" ||
           token == "mod" || token == "^" || token == "(" || token == ")";
}

double MathOperations::applyOperation(double a, double b, const QString& op)
{
    if (op == "+") return add(a, b);
    if (op == "-") return subtract(a, b);
    if (op == "×") return multiply(a, b);
    if (op == "÷") return divide(a, b);
    if (op == "mod") return modulus(a, b);
    if (op == "^") return power(a, b);

    return std::numeric_limits<double>::quiet_NaN();
}

double MathOperations::evaluateExpression(const QList<CalculationNode>& expr)
{
    if (expr.isEmpty())
        return 0.0;

    qDebug() << "=== EVALUATING EXPRESSION ===";
    for (int i = 0; i < expr.size(); ++i) {
        if (expr[i].isOperator) {
            qDebug() << i << "Operator:" << expr[i].operation;
        } else {
            qDebug() << i << "Value:" << expr[i].value;
        }
    }

    // Преобразуем инфиксную нотацию в обратную польскую (RPN)
    QList<QString> output;
    QStack<QString> stack;

    for (const auto& node : expr) {
        if (!node.isOperator) {
            // Число - добавляем в выход
            output.append(QString::number(node.value, 'g', 15));
        } else {
            QString op = node.operation;

            if (op == "(") {
                stack.push(op);
            } else if (op == ")") {
                // Выталкиваем все операторы до открывающей скобки
                while (!stack.isEmpty() && stack.top() != "(") {
                    output.append(stack.pop());
                }
                if (!stack.isEmpty() && stack.top() == "(") {
                    stack.pop(); // Убираем "("
                } else {
                    qDebug() << "Mismatched parentheses";
                    return std::numeric_limits<double>::quiet_NaN();
                }
            } else {
                // Обычный оператор
                while (!stack.isEmpty() && stack.top() != "(" &&
                       getPriority(stack.top()) >= getPriority(op)) {
                    output.append(stack.pop());
                }
                stack.push(op);
            }
        }
    }

    // Выталкиваем оставшиеся операторы из стека
    while (!stack.isEmpty()) {
        if (stack.top() == "(") {
            qDebug() << "Mismatched parentheses";
            return std::numeric_limits<double>::quiet_NaN();
        }
        output.append(stack.pop());
    }

    qDebug() << "RPN expression:" << output;

    // Вычисляем RPN выражение
    QStack<double> values;

    for (const QString& token : output) {
        if (isOperator(token) && token != "(" && token != ")") {
            if (values.size() < 2) {
                qDebug() << "Not enough values for operator:" << token;
                return std::numeric_limits<double>::quiet_NaN();
            }

            double b = values.pop();
            double a = values.pop();
            double result = applyOperation(a, b, token);

            if (qIsNaN(result)) {
                qDebug() << "Error in operation:" << a << token << b;
                return result;
            }

            values.push(result);
            qDebug() << "Applied operation:" << a << token << b << "=" << result;
        } else {
            // Это число
            bool ok;
            double value = token.toDouble(&ok);
            if (!ok) {
                qDebug() << "Invalid number:" << token;
                return std::numeric_limits<double>::quiet_NaN();
            }
            values.push(value);
            qDebug() << "Pushed value:" << value;
        }
    }

    if (values.size() != 1) {
        qDebug() << "Invalid expression - stack size:" << values.size();
        for (int i = 0; i < values.size(); ++i) {
            qDebug() << "Value" << i << ":" << values[i];
        }
        return std::numeric_limits<double>::quiet_NaN();
    }

    double result = values.top();
    qDebug() << "Final result:" << result;
    return result;
}
