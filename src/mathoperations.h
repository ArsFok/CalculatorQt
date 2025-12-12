#ifndef MATHOPERATIONS_H
#define MATHOPERATIONS_H

#include <QtMath>
#include <limits>
#include <QList>
#include <QString>

struct CalculationNode {
    double value;
    QString operation;
    bool isOperator;
};

// Константы
constexpr double MATH_PI = 3.1415926535897932384626433832795;
constexpr double MATH_E = 2.7182818284590452353602874713527;
constexpr int MAX_SHOWING_FACTORIAL = 170;

// Базовые арифметические операции
inline double add(double a, double b) { return a + b; }
inline double subtract(double a, double b) { return a - b; }
inline double multiply(double a, double b) { return a * b; }
inline double divide(double a, double b) {
    if (qFuzzyIsNull(b))
        return std::numeric_limits<double>::quiet_NaN();
    return a / b;
}

// Унарные операции
inline double squareRoot(double x) {
    if (x < 0.0)
        return std::numeric_limits<double>::quiet_NaN();
    return std::sqrt(x);
}

inline double square(double x) {
    double result = x * x;
    if (qIsInf(result)) {
        return std::numeric_limits<double>::infinity();
    }
    return result;
}

inline double reciprocal(double x) {
    if (qFuzzyIsNull(x))
        return std::numeric_limits<double>::quiet_NaN();
    return 1.0 / x;
}

inline double percent(double x) { return x * 0.01; }
inline double changeSign(double x) { return -x; }
inline double absolute(double x) { return std::fabs(x); }

// Тригонометрические функции (в градусах)
inline double sinDeg(double x) { return std::sin(qDegreesToRadians(x)); }
inline double cosDeg(double x) { return std::cos(qDegreesToRadians(x)); }

inline double tanDeg(double x) {
    double radians = qDegreesToRadians(x);
    if (qFuzzyCompare(std::cos(radians), 0.0))
        return std::numeric_limits<double>::quiet_NaN();
    return std::tan(radians);
}

inline double ctan(double x) {
    double radians = qDegreesToRadians(x);
    if (qFuzzyCompare(std::sin(radians), 0.0))
        return std::numeric_limits<double>::quiet_NaN();
    return 1.0 / std::tan(radians);
}

// Логарифмические и экспоненциальные
inline double log10Func(double x) {
    if (x <= 0)
        return std::numeric_limits<double>::quiet_NaN();
    return std::log10(x);
}

inline double ln(double x) {
    if (x <= 0)
        return std::numeric_limits<double>::quiet_NaN();
    return std::log(x);
}

inline double powerOf10(double x) { return std::pow(10.0, x); }

// Факториал
inline double factorial(int n) {
    if (n < 0)
        return std::numeric_limits<double>::quiet_NaN();
    if (n > MAX_SHOWING_FACTORIAL)
        return std::numeric_limits<double>::infinity();

    double result = 1.0;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

// Возведение в степень
inline double power(double base, double exponent) {
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
    return std::pow(base, exponent);
}

// Остаток от деления
inline double modulus(double a, double b) {
    if (qFuzzyIsNull(b))
        return std::numeric_limits<double>::quiet_NaN();
    return std::fmod(a, b);
}

// Функции для работы с выражениями (не inline, будут в .cpp)
double evaluateExpression(const QList<CalculationNode>& expr);
int getPriority(const QString& op);
double applyOperation(double a, double b, const QString& op);
bool isMathOperator(const QString& token);

#endif // MATHOPERATIONS_H
