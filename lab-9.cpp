#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <map>
#include <cctype>

class StringCalculator {
    std::string input;
    std::map<char, int> priority{{'+',1},{'-',1},{'*',2},{'/',2},{'^',3}};
    std::map<std::string, int> functions{{"sin",1},{"cos",1},{"tg",1},{"ctg",1},{"exp",1}};

public:
    void printPostfix(const std::string& expr) {
        input = expr;
        auto postfix = toPostfix();

        while (!postfix.empty()) {
            std::cout << postfix.front() << " ";
            postfix.pop();
        }
        std::cout << std::endl;
    }

private:
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
};

int main() {
    StringCalculator calc;
    std::string expr;

    std::cout << "Enter expression: ";
    std::getline(std::cin, expr);

    calc.printPostfix(expr);
    return 0;
}
