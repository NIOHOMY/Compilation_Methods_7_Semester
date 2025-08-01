﻿#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <sstream>
#include <cctype>
#include <map>

struct Node {
    std::string value;
    std::vector<Node*> children;

    Node(std::string val) : value(val) {}
    ~Node() {
        for (auto child : children)
            delete child;
    }
};

void printTree(Node* root, int level = 0);

void tokenize(const char& expr);

int precedence(const std::string& op);

Node* parseExpression(const std::vector<std::string>& tokens);
