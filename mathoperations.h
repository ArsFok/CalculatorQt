#ifndef MATHOPERATIONS_H
#define MATHOPERATIONS_H

#include <QObject>
#include <QtMath>
#include <limits>
#include <QVector>
#include <QStack>

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

    double add(double a, double b);
    double subtract(double a, double b);
    double multiply(double a, double b);
    double divide(double a, double b);

    double squareRoot(double x);
    double square(double x);
    double reciprocal(double x);
    double percent(double x);
    double changeSign(double x);
    double absolute(double x);

    double sin(double x);
    double cos(double x);
    double tan(double x);
    double ctan(double x);

    double log10(double x);
    double ln(double x);
    double powerOf10(double x);

    double factorial(int n);
    double power(double base, double exponent);
    double modulus(double a, double b);

    double evaluateExpression(const QList<CalculationNode>& expr);

    static constexpr double PI = 3.1415926535897932384626433832795;
    static constexpr double E = 2.7182818284590452353602874713527;

private:
    int getPriority(const QString& op);
    double applyOperation(double a, double b, const QString& op);
    bool isOperator(const QString& token);
};

#endif // MATHOPERATIONS_H
