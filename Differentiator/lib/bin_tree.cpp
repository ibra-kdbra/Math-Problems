#include "bin_tree.h"

#include <sys/stat.h>
#include <cstring>
#include <time.h>
#include <ctype.h>
#include <math.h>

#include "util/util.h"
#include "util/dbg/debug.h"
#include "file_helper.h"
#include "alloc_tracker/alloc_tracker.h"

#include "tree_config.h"

#define OP_SWITCH_END default:                                                                                          \
        if (err_code) *err_code = EINVAL;                                                                               \
        log_printf(ERROR_REPORTS, "error",                                                                              \
            "Somehow Operation equation->value.op had an incorrect value of %d.\n", equation->value.op);                \
        break;                                                                                                          \

/**
 * @brief Print subtree to the .dot file.
 * 
 * @param equation subtree to print
 * @param file write destination
 * @param err_code variable to use as errno
 */
static void recursive_graph_dump(const Equation* equation, FILE* file, int* const err_code = &errno);

/**
 * @brief Collapse the equation to constant.
 * 
 * @param equation 
 */
static void wrap_constants(Equation* equation);

/**
 * @brief Resolve useless operations, such as multiplication by 1 or 0.
 * 
 * @param equation
 */
static void rm_useless(Equation* equation);

Equation* Equation_new(NodeType type, NodeValue value, Equation* left, Equation* right, int* const err_code) {
    Equation* equation = (Equation*) calloc(1, sizeof(*equation));
    _LOG_FAIL_CHECK_(equation, "error", ERROR_REPORTS, return NULL, err_code, ENOMEM);

    equation->type = type;
    equation->value = value;
    equation->left = left;
    equation->right = right;

    return equation;
}

void Equation_dtor(Equation** equation) {
    if (!equation)  return;
    if (!*equation) return;

    if ((*equation)->left)  Equation_dtor(&(*equation)->left);
    if ((*equation)->right) Equation_dtor(&(*equation)->right);
    free(*equation);
    *equation = NULL;
}

static size_t PictCount = 0;

void _Equation_dump_graph(const Equation* equation, unsigned int importance) {
    start_local_tracking();

    BinaryTree_status_t status = Equation_get_error(equation);
    _log_printf(importance, "tree_dump", "\tEquation at %p (status = %d):\n", equation, status);
    if (status) {
        for (size_t error_id = 0; error_id < TREE_REPORT_COUNT; ++error_id) {
            if (status & (1 << error_id)) {
                _log_printf(importance, "tree_dump", "\t%s\n", TREE_STATUS_DESCR[error_id]);
            }
        }
    }

    if (status & ~TREE_INV_CONNECTIONS) return_clean();

    FILE* temp_file = fopen(TREE_TEMP_DOT_FNAME, "w");
    
    _LOG_FAIL_CHECK_(temp_file, "error", ERROR_REPORTS, return_clean(), NULL, 0);

    fputs("digraph G {\n", temp_file);
    fputs(  "\trankdir=TB\n"
            "\tlayout=dot\n"
            , temp_file);

    recursive_graph_dump(equation, temp_file);

    fputc('}', temp_file);
    fclose(temp_file);

    _LOG_FAIL_CHECK_(!system("mkdir -p " TREE_LOG_ASSET_FOLD_NAME), "error", ERROR_REPORTS, return_clean(), NULL, EAGAIN);

    time_t raw_time = 0;
    time(&raw_time);

    const char* pict_name = dynamic_sprintf(TREE_LOG_ASSET_FOLD_NAME "/pict%04ld_%ld.png", (long int)++PictCount, raw_time);
    _LOG_FAIL_CHECK_(pict_name, "error", ERROR_REPORTS, return_clean(), NULL, EFAULT);
    track_allocation(pict_name, free_variable);

    const char* draw_request = dynamic_sprintf("dot -Tpng -o %s " TREE_TEMP_DOT_FNAME, pict_name);
    track_allocation(draw_request, free_variable);

    log_printf(STATUS_REPORTS, "status", "Draw request: %s.\n", draw_request);
    _LOG_FAIL_CHECK_(!system(draw_request), "error", ERROR_REPORTS, return_clean(), NULL, EAGAIN);

    _log_printf(importance, "tree_dump",
                "\n<details><summary>Graph</summary><img src=\"%s\"></details>\n", pict_name);
    
    return_clean();
}

#define in_brackets(left, code, right, condition) do {  \
    bool __cond = (condition);                          \
    if (__cond) caret_printf(caret, left);              \
    code                                                \
    if (__cond) caret_printf(caret, right);             \
} while (0)

void Equation_write_as_formula(const Equation* equation, caret_t* caret, int* const err_code) {
    _LOG_FAIL_CHECK_(!Equation_get_error(equation), "error", ERROR_REPORTS, return, err_code, EINVAL);
    _LOG_FAIL_CHECK_(caret, "error", ERROR_REPORTS, return, err_code, EINVAL);

    switch (equation->type) {
    case TYPE_VAR:
        caret_printf(caret, "%c", (char)equation->value.id);
        break;

    case TYPE_CONST:
        caret_printf(caret, "(%lg)", equation->value.dbl);
        break;

    case TYPE_OP: {
        switch (equation->value.op) {
        case OP_SIN:
        case OP_COS:
            caret_printf(caret, "%s(deg", OP_TEXT_REPS[equation->value.op]);
            in_brackets("(", { Equation_write_as_formula(equation->right, caret); }, "))", true);
            break;
        case OP_LN:
            caret_printf(caret, "%s", OP_TEXT_REPS[equation->value.op]);
            in_brackets("(", { Equation_write_as_formula(equation->right, caret); }, ")", true);
            break;
        case OP_ADD:
        case OP_SUB:
        case OP_MUL:
        case OP_DIV:
        case OP_POW:
            in_brackets("(", { Equation_write_as_formula(equation->left, caret); }, ")", true);
            caret_printf(caret, "%s", OP_TEXT_REPS[equation->value.op]);
            in_brackets("(", { Equation_write_as_formula(equation->right, caret); }, ")", true);
            break;
        OP_SWITCH_END
        }

        break;
    }
    default:
        log_printf(ERROR_REPORTS, "error", 
            "Somehow NodeType equation->type had an incorrect value of %d.\n", equation->type);
        break;
    }
}

void Equation_write_as_tex(const Equation* equation, caret_t* caret, int* const err_code) {
    _LOG_FAIL_CHECK_(!Equation_get_error(equation), "error", ERROR_REPORTS, return, err_code, EINVAL);
    _LOG_FAIL_CHECK_(caret, "error", ERROR_REPORTS, return, err_code, EINVAL);

    switch (equation->type) {
    case TYPE_VAR:
        caret_printf(caret, "%c", (char)equation->value.id);
        break;

    case TYPE_CONST:
        if (equation->value.dbl >= 0) {
            caret_printf(caret, "%lg", equation->value.dbl);
        } else {
            caret_printf(caret, "(%lg)", equation->value.dbl);
        }
        break;

    case TYPE_OP: {
        switch (equation->value.op) {
        case OP_POW:
            in_brackets("(", { Equation_write_as_tex(equation->left, caret); }, ")",
                equation->left->type == TYPE_OP);

            caret_printf(caret, "^");

            in_brackets("{", { Equation_write_as_tex(equation->right, caret); }, "}", true);

            break;

        case OP_DIV:
            caret_printf(caret, "\\frac");

            in_brackets("{", { Equation_write_as_tex(equation->left,  caret); }, "}", true);
            in_brackets("{", { Equation_write_as_tex(equation->right, caret); }, "}", true);

            break;

        case OP_MUL:
            in_brackets("(", { Equation_write_as_tex(equation->left, caret); }, ")",
                equation->left->type == TYPE_OP &&
                OP_PRIORITY[equation->left->value.op] < OP_PRIORITY[equation->value.op]);

            if (equation->right->type == TYPE_CONST)
                caret_printf(caret, "\\cdot");

            in_brackets("(", { Equation_write_as_tex(equation->right, caret); }, ")",
                equation->right->type == TYPE_OP &&
                OP_PRIORITY[equation->right->value.op] < OP_PRIORITY[equation->value.op]);
            
            break;

        case OP_SIN:
        case OP_COS:
        case OP_LN:
            caret_printf(caret, "\\%s", OP_TEXT_REPS[equation->value.op]);
            in_brackets("(", { Equation_write_as_tex(equation->right, caret); }, ")", true);
            break;
        
        case OP_ADD:
        case OP_SUB:
            in_brackets("(", { Equation_write_as_tex(equation->left, caret); }, ")",
                equation->left->type == TYPE_OP &&
                OP_PRIORITY[equation->left->value.op] < OP_PRIORITY[equation->value.op]);

            caret_printf(caret, "%s", OP_TEXT_REPS[equation->value.op]);

            in_brackets("(", { Equation_write_as_tex(equation->right, caret); }, ")",
                equation->right->type == TYPE_OP &&
                OP_PRIORITY[equation->right->value.op] < OP_PRIORITY[equation->value.op]);
            
            break;
        OP_SWITCH_END
        }

        break;
    }
    default:
        log_printf(ERROR_REPORTS, "error", 
            "Somehow NodeType equation->type had an incorrect value of %d.\n", equation->type);
        break;
    }
}

BinaryTree_status_t Equation_get_error(const Equation* equation) {
    BinaryTree_status_t status = 0;

    if (equation == NULL) return TREE_NULL;

    if (equation->type == TYPE_OP && (!equation->left || !equation->right)) status |= TREE_INV_CONNECTIONS;
    if (equation->type != TYPE_OP && ( equation->left ||  equation->right)) status |= TREE_INV_CONNECTIONS;

    #ifndef NDEBUG
        if (equation->left)  status |= Equation_get_error(equation->left);
        if (equation->right) status |= Equation_get_error(equation->right);
    #endif

    return status;
}

Equation* Equation_copy(const Equation* equation) {
    if (!equation) return NULL;
    return Equation_new(equation->type, equation->value, Equation_copy(equation->left), Equation_copy(equation->right));
}

#define eq_cL Equation_copy(equation->left)
#define eq_cR Equation_copy(equation->right)
#define eq_dL Equation_diff(equation->left,  var_id)
#define eq_dR Equation_diff(equation->right, var_id)

static inline Equation* eq_const(double val) { return Equation_new(TYPE_CONST, { .dbl = val }, NULL, NULL); }
static inline Equation* eq_var(char id) { return Equation_new(TYPE_VAR,    { .id = (unsigned long)id }, NULL, NULL); }
static inline Equation* eq_op(Operator op, Equation* left, Equation* right) {
    return Equation_new(TYPE_OP, { .op = op }, left, right); 
}

static inline Equation* eq_add(Equation* left, Equation* right) { return eq_op(OP_ADD, left, right); }
static inline Equation* eq_sub(Equation* left, Equation* right) { return eq_op(OP_SUB, left, right); }
static inline Equation* eq_mul(Equation* left, Equation* right) { return eq_op(OP_MUL, left, right); }
static inline Equation* eq_div(Equation* left, Equation* right) { return eq_op(OP_DIV, left, right); }
static inline Equation* eq_pow(Equation* left, Equation* right) { return eq_op(OP_POW, left, right); }
static inline Equation* eq_sin(Equation* arg)                   { return eq_op(OP_SIN, eq_const(0),  arg); }
static inline Equation* eq_cos(Equation* arg)                   { return eq_op(OP_COS, eq_const(0),  arg); }
static inline Equation* eq_neg(Equation* arg)                   { return eq_op(OP_MUL, eq_const(-1), arg); }
static inline Equation* eq_ln (Equation* arg)                   { return eq_op(OP_LN,  eq_const(0),  arg); }

Equation* Equation_diff(const Equation* equation, const uintptr_t var_id, int* const err_code) {
    if (!equation) return NULL;
    switch (equation->type) {
    case TYPE_VAR:
        if (equation->value.id == var_id)
            return eq_const(1);
        else
            return Equation_copy(equation);
    
    case TYPE_CONST: return eq_const(0);

    case TYPE_OP:
        switch (equation->value.op) {
        case OP_ADD: return eq_add(eq_dL, eq_dR);
        case OP_SUB: return eq_sub(eq_dL, eq_dR);

        case OP_MUL: return eq_add(eq_mul(eq_dL, eq_cR), eq_mul(eq_cL, eq_dR));
        case OP_DIV: return eq_div(eq_sub(eq_mul(eq_dL, eq_cR), eq_mul(eq_dR, eq_cL)), eq_pow(eq_cR, eq_const(2)));

        case OP_SIN: return eq_mul(eq_dR, eq_cos(eq_cR));
        case OP_COS: return eq_mul(eq_dR, eq_neg(eq_sin(eq_cR)));

        case OP_POW: return eq_mul(eq_pow(eq_cL, eq_sub(eq_cR, eq_const(1))),
                                   eq_add( eq_mul(eq_cR, eq_dL),  eq_mul(eq_mul(eq_cL, eq_dR), eq_ln(eq_cL)) ));
        case OP_LN:  return eq_div(eq_dR, eq_cR);
        OP_SWITCH_END
        }
        break;

    default:
        log_printf(ERROR_REPORTS, "error", 
            "Somehow NodeType equation->type had an incorrect value of %d.\n", equation->type);
        break;
    }
    return Equation_copy(equation);
}

static bool eq_t_const(Equation* eq) { return eq->type == TYPE_CONST;  }
static bool eq_t_op(Equation* eq)    { return eq->type == TYPE_OP;     }

#define eq_L ( equation->left )
#define eq_R ( equation->right )

void Equation_simplify(Equation* equation, int* const err_code) {
    if (!equation) return;
    _LOG_FAIL_CHECK_(!Equation_get_error(equation), "error", ERROR_REPORTS, return, err_code, EINVAL);

    if (!equation->type == TYPE_OP) return;

    Equation_simplify(eq_L, err_code);
    Equation_simplify(eq_R, err_code);

    wrap_constants(equation);

    rm_useless(equation);
}

double Equation_calculate(const Equation* equation, const double x_value, int* const err_code) {
    if (!equation) return 0.0;

    switch (equation->type) {

    case TYPE_CONST: return equation->value.dbl;
    case TYPE_VAR: return equation->value.id == 'x' ? x_value : 0.0;

    case TYPE_OP: {
        double alpha = Equation_calculate(equation->left, x_value);
        double beta  = Equation_calculate(equation->right, x_value);

        switch (equation->value.op) {

        case OP_ADD: return alpha + beta;
        case OP_SUB: return alpha - beta;
        case OP_MUL: return alpha * beta;
        case OP_DIV:
            _LOG_FAIL_CHECK_(!is_equal(beta, 0.0), "error", ERROR_REPORTS, return INFINITY, err_code, EINVAL);
            return alpha / beta;
        case OP_COS: return cos(beta);
        case OP_SIN: return sin(beta);
        case OP_POW: return pow(alpha, beta);
        case OP_LN:  return log(beta);
        OP_SWITCH_END

        }

        break;
    }
    default: break;
    }

    return 0.0;
}

void recursive_graph_dump(const Equation* equation, FILE* file, int* const err_code) {
    _LOG_FAIL_CHECK_(!(Equation_get_error(equation) & (~TREE_INV_CONNECTIONS)), 
        "error", ERROR_REPORTS, return, err_code, EINVAL);
    _LOG_FAIL_CHECK_(file, "error", ERROR_REPORTS, return, err_code, ENOENT);

    if (!equation || !file) return;
    fprintf(file, "\tV%p [shape=\"box\" label=\"", equation);
    switch (equation->type) {
        case TYPE_CONST: fprintf(file, "%lg", equation->value.dbl);             break;
        case TYPE_OP: fprintf(file, "%s", OP_TEXT_REPS[equation->value.op]);    break;
        case TYPE_VAR: fprintf(file, "%c", (char)equation->value.id);           break;
        default:
            log_printf(ERROR_REPORTS, "error", 
                "Somehow NodeType equation->type had an incorrect value of %d.\n", equation->type);
            break;
    }
    fprintf(file, "\"]\n");

    if (equation->left) {
        recursive_graph_dump(equation->left, file);
        fprintf(file, "\tV%p -> V%p [arrowhead=\"none\"]\n", equation, equation->left);
    }
    if (equation->right) {
        recursive_graph_dump(equation->right, file);
        fprintf(file, "\tV%p -> V%p [arrowhead=\"none\"]\n", equation, equation->right);
    }
}

#define eq_value ( equation->value.dbl )
#define get_value(eq) ( eq->value.dbl )
#define as_op(operation) eq_value = get_value(eq_L) operation get_value(eq_R); break

static void wrap_constants(Equation* equation) {
    if (!equation || !eq_t_op(equation)) return;
    if (!eq_t_const(eq_L) || !eq_t_const(eq_R)) return;

    if ( equation->value.op == OP_DIV && ( is_equal(get_value(eq_R), 0.0) ||
        !is_equal(get_value(eq_L) / get_value(eq_R), round(get_value(eq_L) / get_value(eq_R))) ) ) return;
    
    if ( equation->value.op == OP_POW && !is_equal(get_value(eq_R), round(get_value(eq_R))) ) return;
    
    switch (equation->value.op) {
    case OP_ADD: as_op(+);
    case OP_SUB: as_op(-);
    case OP_MUL: as_op(*);
    case OP_DIV: as_op(/);
    case OP_POW:
        eq_value = pow(get_value(eq_L), get_value(eq_R));
        break;
    case OP_LN:
        if (is_equal(get_value(eq_R), 1.0)) {
            eq_value = 0.0;
            break;
        } else {
            return;
        }
    case OP_SIN:
    case OP_COS:
    default: return;
    }

    equation->type = TYPE_CONST;

    Equation_dtor(&equation->left);
    Equation_dtor(&equation->right);
}


static inline void replace_node(Equation* alpha, Equation* beta) {    
    if (alpha->right != beta)
        Equation_dtor(&alpha->right);
    else if (alpha->left != beta)
        Equation_dtor(&alpha->left);
    alpha->type  = beta->type;
    alpha->left  = beta->left;
    alpha->right = beta->right;
    alpha->value = beta->value;
    free(beta);
}

static inline void set_to_const(Equation* equation, double value) {
    equation->type = TYPE_CONST;
    Equation_dtor(&equation->left);
    Equation_dtor(&equation->right);
    equation->value.dbl = value;
}

static void rm_useless(Equation* equation) {
    if (!equation || !eq_t_op(equation)) return;
    if (!eq_t_const(eq_L) && !eq_t_const(eq_R)) return;
    
    Equation* const_branch = eq_t_const(eq_L) ? eq_L : eq_R;
    Equation* undef_branch = eq_t_const(eq_L) ? eq_R : eq_L;

    switch (equation->value.op) {
    case OP_ADD:
    case OP_SUB:
        if (is_equal(const_branch->value.dbl, 0.0))
            replace_node(equation, undef_branch);
        break;
    case OP_MUL:
        if (is_equal(const_branch->value.dbl, 1.0))
            replace_node(equation, undef_branch);
        else if (is_equal(const_branch->value.dbl, 0.0))
            set_to_const(equation, 0.0);
        break;
    case OP_DIV:
        if (eq_R->type == TYPE_CONST && is_equal(eq_R->value.dbl, 1.0))
            replace_node(equation, eq_L);
        else if (eq_L->type == TYPE_CONST && is_equal(eq_L->value.dbl, 0.0))
            set_to_const(equation, 0.0);
        break;
    case OP_POW:
        if (eq_R->type == TYPE_CONST && is_equal(eq_R->value.dbl, 1.0))
            replace_node(equation, eq_L);
        else if (eq_R->type == TYPE_CONST && is_equal(eq_R->value.dbl, 0.0))
            set_to_const(equation, 1.0);
        else if (eq_L->type == TYPE_CONST && is_equal(eq_L->value.dbl, 1.0)) 
            set_to_const(equation, 1.0);
        else if (eq_L->type == TYPE_CONST && is_equal(eq_L->value.dbl, 0.0))
            set_to_const(equation, 0.0);
        break;
    case OP_LN:
    case OP_COS:
    case OP_SIN:
    default: return;
    }
}
