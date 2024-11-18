#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "SyntaxAnalysis.h"
#include <iostream>
#include <stdio.h>
#include <ctype.h>
#include <conio.h>
#include <stack>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>


char S[100];
std::stack<char> brackets;
std::string expression;
int i;
int ST;
int CL;
int FIX;
int D[11][11] = {
    //       'Aa..Zz' '0..9'  '.'    'E'    '='   '+|-'  '/|*'   '('   ')'    ';'
    { -1,        1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1 }, // Start
    { -1,        1,     2,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1 }, // I1
    { -1,        1,     2,    -1,    -1,     3,    -1,    -1,    -1,    -1,    -1 }, // I2
    { -1,       -1,     5,    -1,    -1,    -1,     8,    -1,     4,    -1,    -1 }, // Eq
    { -1,       -1,     5,    -1,    -1,    -1,     8,    -1,     4,    -1,    -1 }, // A
    { -1,       -1,     5,     6,     7,    -1,     8,     8,    -1,     9,    10 }, // N
    { -1,       -1,     5,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1 }, // D
    { -1,       -1,    -1,    -1,    -1,    -1,     8,    -1,    -1,    -1,    -1 }, // E
    { -1,       -1,     5,    -1,    -1,    -1,    -1,    -1,     4,    -1,    -1 }, // O
    { -1,       -1,    -1,    -1,    -1,    -1,     8,     8,    -1,     9,    10 }, // B
    { -1,        1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    11 }  // Divider
};

int W[] = { 0, 1 };

std::string out[] = {
    "Error",    // error
    "Final",    // success
};

int sclass(char c) {
    switch (c) {
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
        return 2;
    case '.':
        return 3;
    case 'E':
        return 4;
    case '=':
        return 5;
    case '+': case '-':
        return 6;
    case '*': case '/':
        return 7;
    case '(': {
        brackets.push('(');
        return 8;
    }
    case ')': {
        if (brackets.empty()) {
            return 0;
        }
        else {
            brackets.pop();
        }
        return 9;
    }
    case ';': case '\0': {
        if (!brackets.empty()) {
            return 0;
        }
        while (!brackets.empty()) {
            brackets.pop();
        }
        return 10;
    }
    default:
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
            return 1;
        }
        return 0;
    }
}

void lexfile() {
    char name[30];
    FILE* inp;
    do {
        printf("file name:");
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = '\0';
    } while ((inp = fopen(name, "r")) == NULL);

    S[0] = '\0';
    i = 0;
    ST = 0;
    while (1) {
        if ((ST == 0) && (S[i] == '\0')) {
            if (fgets(S, 100, inp) == NULL) {
                printf("The end\n");
                std::cout << "Parse Tree:" << std::endl;
                drawTree(expression);
                return;
            }
            i = 0;
        }
        if (ST == 0) FIX = i;

        CL = sclass(S[i]);
        printf("%c[%d->", S[i], ST);
        ST = D[ST][CL];
        printf("%d]\n", ST);

        i++;

        if (ST < 0 || (ST == 11 && CL == 10))
        {
            if (!brackets.empty()) {
                ST = -1;
            }
            int state = ((ST + 1) % 10) / 2;
            int j;
            i = i - W[state] + state;
            printf("(%d)<- %s ", W[state], out[state].c_str());
            for (j = FIX; j < i; j++)
                putchar(S[j]);
            puts("");
            expression = std::string(S + FIX, i - FIX);
            ST = 0;
            S[0] = '\0';
            i = 0;
        }
    }
}

int main() {
    lexfile();
    _getch();
    return 0;
}
