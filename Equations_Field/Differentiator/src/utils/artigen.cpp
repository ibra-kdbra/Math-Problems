#include "artigen.h"

#include <string.h>
#include <math.h>

#include "lib/util/util.h"

#include "lib/util/dbg/logger.h"
#include "lib/util/dbg/debug.h"

#include "config.h"

#define INIT_PRINT_BUFFER() static char formula_buffer[MAX_FORMULA_LENGTH] = "";    \
    memset(formula_buffer, 0, sizeof(formula_buffer));                              \
    caret_t formula_caret = formula_buffer

/**
 * @brief Print one of the transition phrases into the article.
 * 
 * @param article 
 */
static void put_transition(ArticleProject* article);

/**
 * @brief Replace variable under equation with equation derivative.
 * 
 * @param equation 
 */
void diff_in_place(Equation** equation);

void Article_ctor(ArticleProject* article, const char* dest_folder) {
    _LOG_FAIL_CHECK_(article, "error", ERROR_REPORTS, return, &errno, EFAULT);
    _LOG_FAIL_CHECK_(dest_folder, "error", ERROR_REPORTS, return, &errno, EFAULT);

    static char full_file_name[FILENAME_MAX] = "";
    strncpy(full_file_name, dest_folder, FILENAME_MAX - 1);
    strncat(full_file_name, ART_MAIN_NAME, strlen(full_file_name) - strlen(ART_MAIN_NAME));

    article->storage.folder_name = dest_folder;
    article->storage.file = fopen(full_file_name, "w");
    _LOG_FAIL_CHECK_(article->storage.file, "error", ERROR_REPORTS, return, &errno, ENOENT);

    fprintf(article->storage.file, "%s", ARTICLE_PREFIX);

    _LOG_FAIL_CHECK_(article->storage.file, "error", ERROR_REPORTS, return, &errno, ENOENT);
}

void Article_dtor(ArticleProject* article) {
    _LOG_FAIL_CHECK_(Article_is_fine(article), "error", ERROR_REPORTS, return, &errno, EINVAL);

    fprintf(article->storage.file, "%s", ARTICLE_POSTFIX);

    fclose(article->storage.file);
    article->storage.file =  NULL;

    article->storage.folder_name = "";
    article->info.max_dif_power = 0;
}

bool Article_is_fine(ArticleProject* article) {
    if (!article || !article->storage.file) return false;
    return true;
}

#define FILL_PRINT_BUFFER(eq) do {                      \
    formula_caret = formula_buffer;                     \
    Equation_write_as_tex(eq, &formula_caret, &errno);  \
    *formula_caret = '\0';                              \
} while (0)

//* Printf for article.
#define PUT(...) fprintf(article->storage.file, __VA_ARGS__)

void describe_differentiation(ArticleProject* article, const Equation* equation, unsigned int power) {
    _LOG_FAIL_CHECK_(Article_is_fine(article), "error", ERROR_REPORTS, return, &errno, EINVAL);
    _LOG_FAIL_CHECK_(!Equation_get_error(equation), "error", ERROR_REPORTS, return, &errno, EINVAL);

    Equation* current_stage = Equation_copy(equation);
    unsigned int cur_power = 0;

    while (cur_power < article->info.max_dif_power && cur_power < power) {
        diff_in_place(&current_stage);

        ++cur_power;
    }

    INIT_PRINT_BUFFER();

    if (cur_power > 0) {
        FILL_PRINT_BUFFER(equation);
        PUT("We have already proven the statement\n\\[(%s)^{(%ld)}=", formula_buffer, (long int)cur_power);

        FILL_PRINT_BUFFER(current_stage);
        PUT("%s\\]\n", formula_buffer);

        if (cur_power < power) {
            PUT("Let\'s now continue calculating derivatives.\\newline\n");
        }
    }

    while (cur_power < power) {
        put_transition(article);

        FILL_PRINT_BUFFER(current_stage);
        PUT("\\[(%s)'=", formula_buffer);

        diff_in_place(&current_stage);

        FILL_PRINT_BUFFER(current_stage);
        PUT("%s\\]\n", formula_buffer);

        ++cur_power;

        if(current_stage->type == TYPE_CONST) {
            PUT("As we know, any degree derivative of the constant is equal to "
                "zero, so we can stop differentiating at this point.\\newline\n");
            cur_power = (unsigned int)-1;
        }
    }

    Equation_dtor(&current_stage);

    if (cur_power > article->info.max_dif_power) article->info.max_dif_power = cur_power;
}

void describe_series(ArticleProject* article, const Equation* equation, double point, unsigned int power) {
    _LOG_FAIL_CHECK_(Article_is_fine(article), "error", ERROR_REPORTS, return, &errno, EINVAL);
    _LOG_FAIL_CHECK_(!Equation_get_error(equation), "error", ERROR_REPORTS, return, &errno, EINVAL);

    INIT_PRINT_BUFFER();

    PUT("Let's first calculate equation derivatives.\\newline\n");
    describe_differentiation(article, equation, power);

    PUT("Then we can just place values to previously calculated derivatives and paste them to the series formula.\n");

    FILL_PRINT_BUFFER(equation);
    PUT("\\[%s=", formula_buffer);

    unsigned long long divisor = 1;
    Equation* current_stage = Equation_copy(equation);
    unsigned int cur_power = 0;
    for (; cur_power < power; divisor *= ++cur_power + 1) {
        diff_in_place(&current_stage);

        double value = Equation_calculate(current_stage, 'x');

        if (is_equal(value, 0.0)) continue;

        double top = value;
        unsigned long long bottom = divisor;

        if (is_equal(top, round(top))) {
            unsigned long long div = gcd((unsigned long long)abs(top), bottom);
            top /= (double)div;
            bottom /= div;
        }

        if (cur_power) {
            if (bottom != 1) PUT("+\\frac{%lg}{%lld}", top, (long long)bottom);
            else if (!is_equal(value, 1.0)) PUT("%+lg", top);

            if (is_equal(point, 0.0)) PUT("x");
            else PUT("(x%+lg)", -point);

            if (cur_power > 1) PUT("^{%d}", (int)cur_power);
        } else {
            PUT("%lg", value);
        }
    }

    if (power > 0) {
        if (!is_equal(point, 0.0)) PUT("+o((x%+lg)", -point);
        else PUT("+o(x");
        if (power > 1) {
            PUT("^{%d}", (int)power);
        }
        PUT(")");
    } else {
        PUT("+o(1)");
    }
    
    Equation_dtor(&current_stage);

    PUT("\\]");
}

void describe_tangent(ArticleProject* article, const Equation* equation, double point) {
    _LOG_FAIL_CHECK_(Article_is_fine(article), "error", ERROR_REPORTS, return, &errno, EINVAL);
    _LOG_FAIL_CHECK_(!Equation_get_error(equation), "error", ERROR_REPORTS, return, &errno, EINVAL);

    INIT_PRINT_BUFFER();

    double value = Equation_calculate(equation, point);

    Equation* deriv = Equation_diff(equation, 'x');
    Equation_simplify(deriv);

    double slope_k = Equation_calculate(deriv, point);

    FILL_PRINT_BUFFER(equation);
    PUT("To find the tangent, first we need to calculate first derivative of the equation at point $x=%lg$.\n", point);
    PUT("\\[(%s)'=", formula_buffer);

    FILL_PRINT_BUFFER(deriv);
    PUT("%s=%lg\\]\n", formula_buffer, slope_k);

    double constant = 0.0;

    if (isinf(slope_k)) {
        PUT("As we can see, derivative at this point is reaching infinity, "
            "meaning, that tangent at this point is a vertical line $x=%lg$", point);
    } else {
        constant = value - slope_k * point;
        PUT("Using this data we can assume that the tangent at given point is $y=%lgx%+lg$\\newline\n\n",
            slope_k, constant);
    }

    PUT(ARTICLE_GRAPH_PREFIX_TEMPLATE, point, point);

    formula_caret = formula_buffer;
    Equation_write_as_formula(equation, &formula_caret, &errno);
    *formula_caret = '\0';
    PUT(ARTICLE_GRAPH_TEMPLATE, point, point, "blue", formula_buffer);

    const char* tangent_formula = dynamic_sprintf("%lf*x%+lf", slope_k, constant);
    PUT(ARTICLE_GRAPH_TEMPLATE, point, point, "red", tangent_formula);
    free((void*)tangent_formula);
    tangent_formula = NULL;

    PUT(ARTICLE_GRAPH_SUFFIX);

    Equation_dtor(&deriv);
}

static void put_transition(ArticleProject* article) {
    PUT("%s", TRANSITION_PHRASES[(unsigned int)rand() % TRANSITION_PHRASE_COUNT]);
}

void diff_in_place(Equation** equation) {
    _LOG_FAIL_CHECK_(!Equation_get_error(*equation), "error", ERROR_REPORTS, return, &errno, EINVAL);

    Equation* next_stage = Equation_diff(*equation, 'x');
    Equation_dtor(equation);
    *equation = next_stage;
    Equation_simplify(*equation);
}
