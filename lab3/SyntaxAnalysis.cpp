#pragma once
#include "SyntaxAnalysis.h"

static std::vector<std::string> tokens;
static std::string token;

void printTree(Node* root, int level) {
    if (!root) return;
    std::cout << std::string(level * 2, ' ') << root->value << std::endl;
    for (auto child : root->children) {
        printTree(child, level + 1);
    }
}

void tokenize(const char& c) {
    if (c == ';') {
        Node* root = parseExpression(tokens);

        printTree(root);

        delete root;
        tokens.clear();
        token.clear();
    } else {
        if (std::isspace(c)) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else if (std::isdigit(c) || c == '.' || c == 'E' || c == '+' || c == '-' || std::isalpha(c)) { // числа E название
            token += c;
        }
        else if (c == '(' || c == ')' || c == '+' || c == '-' || c == '*' || c == '/') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            token += c;
            tokens.push_back(token);
            token.clear();
        }
        else if (c == '=') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            token += c;
            tokens.push_back(token);
            token.clear();
        }
    }
}

int precedence(const std::string& op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    return 0;
}

Node* parseExpression(const std::vector<std::string>& tokens) {
    std::stack<Node*> operands;
    std::stack<std::string> operators;

    Node* assignmentNode = nullptr;

    for (const auto& token : tokens) {
        if (token == "=") {
            continue;
        }
        else if (token == "+" || token == "-" || token == "*" || token == "/") {

            while (!operators.empty() && precedence(operators.top()) >= precedence(token)) {
                std::string op = operators.top();
                operators.pop();

                Node* right = operands.top();
                operands.pop();
                Node* left = operands.top();
                operands.pop();

                Node* opNode = new Node(op);
                opNode->children.push_back(left);
                opNode->children.push_back(right);

                operands.push(opNode);
            }
            operators.push(token);
        }
        else if (token == "(") {
            operators.push(token);
        }
        else if (token == ")") {
            while (!operators.empty() && operators.top() != "(") {
                std::string op = operators.top();
                operators.pop();

                Node* right = operands.top();
                operands.pop();
                Node* left = operands.top();
                operands.pop();

                Node* opNode = new Node(op);
                opNode->children.push_back(left);
                opNode->children.push_back(right);

                operands.push(opNode);
            }
            operators.pop();  // - (
        }
        else {
            operands.push(new Node(token));
        }
    }

    while (!operators.empty()) {
        std::string op = operators.top();
        operators.pop();

        Node* right = operands.top();
        operands.pop();
        Node* left = operands.top();
        operands.pop();

        Node* opNode = new Node(op);
        opNode->children.push_back(left);
        opNode->children.push_back(right);

        operands.push(opNode);
    }

    if (!operands.empty()) {
        assignmentNode = operands.top();
        operands.pop();
        Node* root = new Node(tokens[0]);
        root->children.push_back(new Node("="));
        root->children.push_back(assignmentNode);
        return root;
    }

    return nullptr;
}