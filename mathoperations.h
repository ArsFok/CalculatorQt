#ifndef MATHOPERATIONS_H
#define MATHOPERATIONS_H

#include <QObject>
#include <QtMath>
#include <limits>

struct CalculationNode {
    double value;
    QString operation;
    bool isOperator;
};

class MathOperations : public QObject
{
    Q_OBJECT

public:
    explicit MathOperations(QObject *parent = nullptr);

    // Базовые операции
    double add(double a, double b);
    double subtract(double a, double b);
    double multiply(double a, double b);
    double divide(double a, double b);

    // Унарные операции
    double squareRoot(double x);
    double square(double x);
    double reciprocal(double x);
    double percent(double x);
    double changeSign(double x);
    double absolute(double x);

    // Тригонометрические функции
    double sin(double x);
    double cos(double x);
    double tan(double x);
    double ctan(double x);

    // Логарифмические функции
    double log10(double x);
    double ln(double x);
    double powerOf10(double x);

    // Специальные функции
    double factorial(int n);
    double power(double base, double exponent);
    double modulus(double a, double b);

    // Вычисление выражений
    double evaluateExpression(const QList<CalculationNode>& expr);

    // Константы
    static constexpr double PI = M_PI;
    static constexpr double E = M_E;

private:
    int getPriority(const QString& op);
};

#endif // MATHOPERATIONS_H
