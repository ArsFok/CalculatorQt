#include "mathoperations.h"

MathOperations::MathOperations(QObject *parent) : QObject(parent)
{
}

double MathOperations::add(double a, double b) { return a + b; }
double MathOperations::subtract(double a, double b) { return a - b; }
double MathOperations::multiply(double a, double b) { return a * b; }
double MathOperations::divide(double a, double b)
{
    if (qFuzzyIsNull(b)) return std::numeric_limits<double>::quiet_NaN();
    return a / b;
}

double MathOperations::squareRoot(double x)
{
    if (x < 0.0) return std::numeric_limits<double>::quiet_NaN();
    return std::sqrt(x);
}

double MathOperations::square(double x) { return x * x; }
double MathOperations::reciprocal(double x)
{
    if (qFuzzyIsNull(x)) return std::numeric_limits<double>::quiet_NaN();
    return 1.0 / x;
}

double MathOperations::percent(double x) { return x * 0.01; }
double MathOperations::changeSign(double x) { return -x; }
double MathOperations::absolute(double x) { return qAbs(x); }

double MathOperations::sin(double x) { return std::sin(qDegreesToRadians(x)); }
double MathOperations::cos(double x) { return std::cos(qDegreesToRadians(x)); }
double MathOperations::tan(double x)
{
    double radians = qDegreesToRadians(x);
    if (qFuzzyCompare(std::cos(radians), 0.0)) return std::numeric_limits<double>::quiet_NaN();
    return std::tan(radians);
}

double MathOperations::ctan(double x)
{
    double radians = qDegreesToRadians(x);
    if (qFuzzyCompare(std::sin(radians), 0.0)) return std::numeric_limits<double>::quiet_NaN();
    return 1.0 / std::tan(radians);
}

double MathOperations::log10(double x)
{
    if (x <= 0) return std::numeric_limits<double>::quiet_NaN();
    return std::log10(x);
}

double MathOperations::ln(double x)
{
    if (x <= 0) return std::numeric_limits<double>::quiet_NaN();
    return std::log(x);
}

double MathOperations::powerOf10(double x) { return std::pow(10, x); }

double MathOperations::factorial(int n)
{
    if (n < 0 || n > 20) return std::numeric_limits<double>::quiet_NaN();

    long long result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return static_cast<double>(result);
}

double MathOperations::power(double base, double exponent) { return std::pow(base, exponent); }
double MathOperations::modulus(double a, double b) { return std::fmod(a, b); }

double MathOperations::evaluateExpression(const QList<CalculationNode>& expr)
{
    if (expr.isEmpty()) return 0.0;

    double result = expr.first().value;
    QString lastOp;

    for (int i = 1; i < expr.size(); ++i) {
        const auto& node = expr.at(i);
        if (node.isOperator) {
            lastOp = node.operation;
        } else {
            if (lastOp == "+") result += node.value;
            else if (lastOp == "-") result -= node.value;
            else if (lastOp == "×") result *= node.value;
            else if (lastOp == "÷") {
                if (qFuzzyIsNull(node.value)) {
                    return std::numeric_limits<double>::quiet_NaN();
                }
                result /= node.value;
            }
        }
    }

    return result;
}

int MathOperations::getPriority(const QString& op)
{
    if (op == "×" || op == "÷") return 2;
    if (op == "+" || op == "-") return 1;
    return 0;
}
