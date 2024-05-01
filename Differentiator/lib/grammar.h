#ifndef GRAMMAR_H
#define GRAMMAR_H

#include "bin_tree.h"

enum LexType {
    LEX_PLUS = OP_ADD,
    LEX_MINUS = OP_SUB,
    LEX_MUL = OP_MUL,
    LEX_DIV = OP_DIV,
    LEX_SIN = OP_SIN,
    LEX_COS = OP_COS,
    LEX_POW = OP_POW,
    LEX_LN = OP_LN,

    LEX_OP_TERM,

    LEX_OP_BRACKET,
    LEX_CL_BRACKET,

    LEX_VAR,
    LEX_NUM,
};

union LexValue {
    double dbl;
    char ch;
};

struct Lex {
    LexType type = LEX_OP_TERM;
    LexValue value = { .ch = 0 };
    const char* address = NULL;
};

struct LexStack {
    Lex* buffer = NULL;
    size_t size = 0;
    size_t capacity = 0;
};

void LexStack_ctor(LexStack* stack, size_t start_size);
void LexStack_dtor(LexStack* stack);

void LexStack_push(LexStack* stack, Lex lexeme);


LexStack lexify(const char* line);

#define GRAM_FUNCTION(name) Equation* name(const LexStack stack, int* caret);

//* G = eq
GRAM_FUNCTION(parse); 

//* eq = mult ([+-]mult)*
GRAM_FUNCTION(parse_expr);

//* mult = pow ([*/]pow)*
GRAM_FUNCTION(parse_mult);

//* pow = brackets ([^]brackets)*
GRAM_FUNCTION(parse_pow);

//* brackets = number | '('eq')'
GRAM_FUNCTION(parse_brackets);

//* function = number | ((sin|cos|ln)'('eq')')
GRAM_FUNCTION(parse_function);

//* number = std::double | std::alpha
GRAM_FUNCTION(parse_number);

#undef GRAM_FUNCTION

#endif