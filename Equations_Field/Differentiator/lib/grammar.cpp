#include "grammar.h"

#include <ctype.h>
#include <cstring>

#include "util/dbg/debug.h"

#define GRAM(name) Equation* name(const LexStack stack, int* caret)
#define CHECK_INPUT() do {                                                                          \
    _LOG_FAIL_CHECK_(stack.buffer, "error", ERROR_REPORTS, return NULL, NULL, EINVAL);              \
    _LOG_FAIL_CHECK_(*caret < (int)stack.size, "error", ERROR_REPORTS, return NULL, NULL, EINVAL);  \
    log_printf(STATUS_REPORTS, "status", "Called parsing of lexeme at index %d (type: %d).\n",      \
                                          *caret, (int)stack.buffer[*caret].type);                  \
}while (0)
#define CERROR(...) do {                                                        \
    log_dup(ERROR_REPORTS, "error", "%-20s\n", stack.buffer[*caret].address);   \
    log_dup(ERROR_REPORTS, "error", __VA_ARGS__);                               \
    return value;                                                               \
}while (0)

void LexStack_ctor(LexStack* stack, size_t start_size) {
    stack->capacity = start_size;
    stack->buffer = (Lex*) calloc(stack->capacity, sizeof(*stack->buffer));
}

void LexStack_dtor(LexStack* stack) {
    free(stack->buffer);
    stack->capacity = 0;
    stack->size = 0;
}

void LexStack_push(LexStack* stack, Lex lexeme) {
    if (stack->size + 1 >= stack->capacity) {
        stack->capacity *= 2;
        Lex* new_buf = (Lex*)calloc(stack->capacity, sizeof(*stack->buffer));
        _LOG_FAIL_CHECK_(new_buf, "error", ERROR_REPORTS, return, &errno, ENOMEM);
        memcpy(new_buf, stack->buffer, stack->size * sizeof(*stack->buffer));
        free(stack->buffer);
        stack->buffer = new_buf;
        for (size_t id = stack->size; id < stack->capacity; ++id) {
            stack->buffer[id] = {};
        }
    }
    stack->buffer[stack->size++] = lexeme;
    log_printf(STATUS_REPORTS, "status", "Pushed lexeme of type %d {.value = { .ch = %d, .dbl = %lg }} "
                "at \"%-10s\" to index %ld.\n",
                (int) lexeme.type, (int) lexeme.value.ch, lexeme.value.dbl, lexeme.address, (long int)stack->size - 1);
}


LexStack lexify(const char* line) {
    LexStack stack = {};
    LexStack_ctor(&stack, 1);

    const char* iter = line;

    for (int shift = 1; *iter != '\n' && *iter != '\0' && *iter != EOF; iter += shift, shift = 1) {
        char letter = *iter;

        if (isblank(letter)) continue;

        for (size_t id = 0; id < OP_TYPE_COUNT; id++) {
            if (strncmp(iter, OP_TEXT_REPS[id], strlen(OP_TEXT_REPS[id])))
                continue;
            LexStack_push(&stack, { .type = (LexType)id, .address = iter });
            shift = (int)strlen(OP_TEXT_REPS[id]);
            break;
        }

        if (isalpha(letter)) {
            if (!isalpha(iter[1]))
                LexStack_push(&stack, { .type = LEX_VAR, .value = { .ch = letter }, .address = iter });
            else
                while (isalpha(iter[shift])) ++shift;
        }

        if (isdigit(letter)) {
            const char* dbl_end = iter;  // v~~~~ Why the F this function demands non-const pointers?
            double value = strtod(iter, (char**)&dbl_end);
            LexStack_push(&stack, { .type = LEX_NUM, .value = { .dbl = value }, .address = iter });
            shift = (int)(dbl_end - iter);
        }

        if (letter == '(')
            LexStack_push(&stack, { .type = LEX_OP_BRACKET, .address = iter });
        if (letter == ')')
            LexStack_push(&stack, { .type = LEX_CL_BRACKET, .address = iter });
    }
    LexStack_push(&stack, { .type = LEX_OP_TERM, .address = iter });

    return stack;
}


#define ASSIGN_AND_CHECK(variable, value) do {                                      \
    variable = value;                                                               \
    _LOG_FAIL_CHECK_(variable, "error", ERROR_REPORTS, return NULL, NULL, EAGAIN);  \
} while (0);

GRAM(parse) {
    Equation* value = NULL;
    ASSIGN_AND_CHECK(value, parse_expr(stack, caret));

    if (stack.buffer[*caret].type != LEX_OP_TERM) {
        CERROR("Redundant lexeme detected after equation parsing has ended.\n");
    }

    return value;
}

GRAM(parse_expr) {
    CHECK_INPUT();
    Equation* value = NULL;
    ASSIGN_AND_CHECK(value, parse_mult(stack, caret));
    while (stack.buffer[*caret].type == LEX_PLUS || stack.buffer[*caret].type == LEX_MINUS) {
        LexType type = stack.buffer[*caret++].type;
        Equation* next_arg = NULL;
        ASSIGN_AND_CHECK(next_arg, parse_mult(stack, caret));
        value = Equation_new(TYPE_OP, { .op = type==LEX_PLUS ? OP_ADD : OP_SUB }, value, next_arg);
    }
    return value;
}

GRAM(parse_mult) {
    CHECK_INPUT();
    Equation* value = NULL;
    ASSIGN_AND_CHECK(value, parse_pow(stack, caret));
    while (stack.buffer[*caret].type == LEX_MUL || stack.buffer[*caret].type == LEX_DIV) {
        LexType type = stack.buffer[*caret++].type;
        Equation* next_arg = NULL;
        ASSIGN_AND_CHECK(next_arg, parse_pow(stack, caret));
        value = Equation_new(TYPE_OP, { .op = type==LEX_MUL ? OP_MUL : OP_DIV }, value, next_arg);
    }
    return value;
}

GRAM(parse_pow) {
    CHECK_INPUT();
    Equation* value = NULL;
    ASSIGN_AND_CHECK(value, parse_brackets(stack, caret));
    
    while (stack.buffer[*caret].type == LEX_POW) {
        ++*caret;
        Equation* next_arg = NULL;
        ASSIGN_AND_CHECK(next_arg, parse_brackets(stack, caret));
        value = Equation_new(TYPE_OP, { .op = OP_POW }, value, next_arg);
    }
    return value;
}

GRAM(parse_brackets) {
    CHECK_INPUT();
    Equation* value = NULL;
    if (stack.buffer[*caret].type == LEX_OP_BRACKET) {
        ++*caret;
        ASSIGN_AND_CHECK(value, parse_expr(stack, caret));
        if (stack.buffer[*caret].type != LEX_CL_BRACKET) CERROR("Expected closing bracket.\n");
        ++*caret;
    } else {
        ASSIGN_AND_CHECK(value, parse_function(stack, caret));
    }
    return value;
}

GRAM(parse_function) {
    CHECK_INPUT();
    Equation* value = NULL;
    LexType type = stack.buffer[*caret].type;
    if (type == LEX_SIN || type == LEX_COS || type == LEX_LN) {
        ++*caret;
        if (stack.buffer[*caret].type == LEX_OP_BRACKET) {
            ++*caret;
            ASSIGN_AND_CHECK(value, parse_expr(stack, caret));
            if (stack.buffer[*caret].type != LEX_CL_BRACKET) CERROR("Expected closing bracket.\n");
            value = Equation_new(TYPE_OP, { .op = (Operator)type }, Equation_new(TYPE_CONST, {}, NULL, NULL), value);
            ++*caret;
        } else
            CERROR("Expected opening bracket.\n");
    } else if (type == LEX_NUM || type == LEX_VAR) {
        ASSIGN_AND_CHECK(value, parse_number(stack, caret));
    }
    return value;
}

GRAM(parse_number) {
    CHECK_INPUT();
    Equation* value = NULL;
    if (stack.buffer[*caret].type == LEX_NUM) {
        ASSIGN_AND_CHECK(value, Equation_new(TYPE_CONST, { .dbl = stack.buffer[*caret].value.dbl }, NULL, NULL));
        ++*caret;
    } else if (stack.buffer[*caret].type == LEX_VAR) {
        ASSIGN_AND_CHECK(value, Equation_new(TYPE_VAR, { .id = (unsigned long)stack.buffer[*caret].value.ch },
            NULL, NULL));
        ++*caret;
    } else {
        CERROR("Expected number or variable, got lexeme of type %d instead.\n", (int)stack.buffer[*caret].type);
    }
    return value;
}
