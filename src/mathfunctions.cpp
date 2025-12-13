#include "mathfunctions.h"
#include <QStack>
#include <QDebug>
#include <cmath>

double lnFunc(double x) {
    if (x <= 0)
        return std::numeric_limits<double>::quiet_NaN();
    return std::log(x);
}

double ctanDeg(double x) {
    double radians = qDegreesToRadians(x);
    if (qFuzzyCompare(std::sin(radians), 0.0))
        return std::numeric_limits<double>::quiet_NaN();
    return 1.0 / std::tan(radians);
}

double factorial(int n) {
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

int getPriority(const QString& op) {
    if (op == "(" || op == ")") return 0;
    if (op == "+" || op == "-") return 1;
    if (op == "×" || op == "÷" || op == "mod") return 2;
    if (op == "^") return 3;
    return -1;
}

double applyOperation(double a, double b, const QString& op) {
    if (op == "+") return add(a, b);
    if (op == "-") return subtract(a, b);
    if (op == "×") return multiply(a, b);
    if (op == "÷") return divide(a, b);
    if (op == "mod") return modulus(a, b);
    if (op == "^") return power(a, b);
    return std::numeric_limits<double>::quiet_NaN();
}

bool isMathOperator(const QString& token) {
    return token == "+" || token == "-" || token == "×" || token == "÷" ||
           token == "mod" || token == "^" || token == "(" || token == ")";
}

double evaluateExpression(const QList<CalculationNode>& expr) {
    if (expr.isEmpty()) {
        return 0.0;
    }

    QList<CalculationNode> processedExpr;
    for (int i = 0; i < expr.size(); ++i) {
        processedExpr.append(expr[i]);

        if (i < expr.size() - 1 &&
            !expr[i].isOperator &&
            expr[i+1].isOperator && expr[i+1].operation == "(") {
            processedExpr.append({0.0, "×", true});
        }

        if (i < expr.size() - 1 &&
            expr[i].isOperator && expr[i].operation == ")" &&
            !expr[i+1].isOperator) {
            processedExpr.append({0.0, "×", true});
        }

        if (i < expr.size() - 1 &&
            expr[i].isOperator && expr[i].operation == ")" &&
            expr[i+1].isOperator && expr[i+1].operation == "(") {
            processedExpr.append({0.0, "×", true});
        }
    }

    QList<QString> output;
    QStack<QString> stack;

    for (const auto& node : qAsConst(processedExpr)) {
        if (!node.isOperator) {
            output.append(QString::number(node.value, 'g', 15));
        } else {
            QString op = node.operation;

            if (op == "(") {
                stack.push(op);
            } else if (op == ")") {
                while (!stack.isEmpty() && stack.top() != "(") {
                    output.append(stack.pop());
                }
                if (!stack.isEmpty() && stack.top() == "(") {
                    stack.pop();
                } else {
                    return std::numeric_limits<double>::quiet_NaN();
                }
            } else {
                while (!stack.isEmpty() && stack.top() != "(" &&
                       getPriority(stack.top()) >= getPriority(op)) {
                    output.append(stack.pop());
                }
                stack.push(op);
            }
        }
    }

    while (!stack.isEmpty()) {
        if (stack.top() == "(") {
            return std::numeric_limits<double>::quiet_NaN();
        }
        output.append(stack.pop());
    }

    QStack<double> values;

    for (const QString& token : output) {
        if (isMathOperator(token) && token != "(" && token != ")") {
            if (values.size() < 2) {
                return std::numeric_limits<double>::quiet_NaN();
            }

            double b = values.pop();
            double a = values.pop();
            double result = applyOperation(a, b, token);

            if (qIsNaN(result)) {
                return result;
            }

            values.push(result);
        } else {
            bool ok;
            double value = token.toDouble(&ok);
            if (!ok) {
                return std::numeric_limits<double>::quiet_NaN();
            }
            values.push(value);
        }
    }

    if (values.size() != 1) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    return values.top();
}