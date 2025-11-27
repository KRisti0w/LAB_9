#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <map>
#include <cmath>
#include <cctype>
#include <stdexcept>

class StringCalculator {
    std::string input;
    double x_value;
    bool has_variable;

    std::map<char, int> priority{{'+',1},{'-',1},{'*',2},{'/',2},{'^',3}};
    std::map<std::string, int> functions{{"sin",1},{"cos",1},{"tg",1},{"ctg",1},{"exp",1}};

public:
    StringCalculator() : x_value(0), has_variable(false) {}

    double calculate(const std::string& expr) {
        input = expr;
        has_variable = false;

        if (!validate()) throw std::invalid_argument("Invalid expression");

        if (has_variable) {
            std::cout << "Enter x=";
            std::cin >> x_value;
        }

        auto postfix = toPostfix();
        printPostfix(postfix);
        return evalPostfix(postfix);
    }

private:
    bool validate() {
        std::stack<char> brackets;
        bool last_op = true;

        for (size_t i = 0; i < input.length(); ++i) {
            char c = input[i];
            if (isspace(c)) continue;

            if (c == '(') { brackets.push(c); last_op = true; }
            else if (c == ')') {
                if (brackets.empty()) return false;
                brackets.pop(); last_op = false;
            }
            else if (isOp(c)) {
                if (last_op && c != '-') return false;
                last_op = true;
            }
            else if (isdigit(c) || c == '.' || c == 'x') {
                if (c == 'x') has_variable = true;
                last_op = false;
            }
            else if (isalpha(c)) {
                std::string func;
                while (i < input.length() && isalpha(input[i])) func += input[i++];
                i--;
                if (functions.find(func) == functions.end()) return false;
                last_op = true;
            }
        }
        return brackets.empty() && !last_op;
    }

    bool isOp(char c) { return c == '+' || c == '-' || c == '*' || c == '/' || c == '^'; }

    std::queue<std::string> toPostfix() {
        std::queue<std::string> output;
        std::stack<std::string> ops;

        for (size_t i = 0; i < input.length(); ++i) {
            char c = input[i];
            if (isspace(c)) continue;

            if (isdigit(c) || c == '.') {
                std::string num;
                while (i < input.length() && (isdigit(input[i]) || input[i] == '.'))
                    num += input[i++];
                i--; output.push(num);
            }
            else if (c == 'x') output.push("x");
            else if (isalpha(c)) {
                std::string func;
                while (i < input.length() && isalpha(input[i])) func += input[i++];
                i--; ops.push(func);
            }
            else if (c == '(') ops.push("(");
            else if (c == ')') {
                while (!ops.empty() && ops.top() != "(") {
                    output.push(ops.top()); ops.pop();
                }
                if (!ops.empty()) ops.pop();
                if (!ops.empty() && functions.count(ops.top())) {
                    output.push(ops.top()); ops.pop();
                }
            }
            else if (isOp(c)) {
                if (c == '-' && (i == 0 || input[i-1] == '(' || isOp(input[i-1])))
                    ops.push("u-");
                else {
                    while (!ops.empty() && ops.top() != "(" && priority[ops.top()[0]] >= priority[c]) {
                        output.push(ops.top()); ops.pop();
                    }
                    ops.push(std::string(1, c));
                }
            }
        }

        while (!ops.empty()) { output.push(ops.top()); ops.pop(); }
        return output;
    }

    double evalPostfix(std::queue<std::string> postfix) {
        std::stack<double> vals;

        while (!postfix.empty()) {
            std::string token = postfix.front();
            postfix.pop();

            if (isdigit(token[0]) || (token[0] == '.')) vals.push(std::stod(token));
            else if (token == "x") vals.push(x_value);
            else if (isOp(token[0])) {
                double b = vals.top(); vals.pop();
                double a = vals.top(); vals.pop();
                switch (token[0]) {
                    case '+': vals.push(a + b); break;
                    case '-': vals.push(a - b); break;
                    case '*': vals.push(a * b); break;
                    case '/':
                        if (b == 0) throw std::invalid_argument("Division by zero");
                        vals.push(a / b); break;
                    case '^': vals.push(pow(a, b)); break;
                }
            }
            else if (token == "u-") { double a = vals.top(); vals.pop(); vals.push(-a); }
            else {
                double arg = vals.top(); vals.pop();
                if (token == "sin") vals.push(sin(arg));
                else if (token == "cos") vals.push(cos(arg));
                else if (token == "tg") vals.push(tan(arg));
                else if (token == "ctg") vals.push(1.0 / tan(arg));
                else if (token == "exp") vals.push(exp(arg));
            }
        }

        if (vals.size() != 1) throw std::invalid_argument("Calculation error");
        return vals.top();
    }

    void printPostfix(std::queue<std::string> postfix) {
        std::cout << "Postfix: ";
        while (!postfix.empty()) {
            std::cout << postfix.front() << " ";
            postfix.pop();
        }
        std::cout << std::endl;
    }
};

int main() {
    StringCalculator calc;
    std::string expr;

    std::cout << "Enter expression: ";
    std::getline(std::cin, expr);

    try {
        double result = calc.calculate(expr);
        std::cout << "Result: " << result << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    return 0;
}