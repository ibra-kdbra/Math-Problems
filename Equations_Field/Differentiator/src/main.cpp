#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <ctype.h>

#include "lib/util/dbg/debug.h"
#include "lib/util/argparser.h"
#include "lib/alloc_tracker/alloc_tracker.h"
#include "lib/file_helper.h"
#include "lib/speaker.h"

#include "utils/config.h"

#include "lib/bin_tree.h"
#include "lib/grammar.h"
#include "utils/artigen.h"

#include "utils/main_utils.h"

#define MAIN

#define MAKE_WRAPPER(name) void* __wrapper_##name[] = {&name}

int main(const int argc, const char** argv) {
    atexit(log_end_program);

    start_local_tracking();

    unsigned int log_threshold = STATUS_REPORTS;
    MAKE_WRAPPER(log_threshold);
    int differentiation_power = 2;
    MAKE_WRAPPER(differentiation_power);
    int series_power = 5;
    MAKE_WRAPPER(series_power);
    double series_point = 0.0;
    MAKE_WRAPPER(series_point);

    ActionTag line_tags[] = {
        #include "cmd_flags/main_flags.h"
    };
    const int number_of_tags = ARR_SIZE(line_tags);

    parse_args(argc, argv, number_of_tags, line_tags);
    log_init("program_log.html", log_threshold, &errno);
    print_label();

    const char* f_name = get_input_file_name(argc, argv, DEFAULT_DB_NAME);

    log_printf(STATUS_REPORTS, "status", "Opening file %s as the equation source.\n", f_name);

    char* source_equation = read_whole(f_name);

    log_printf(STATUS_REPORTS, "status", "File content:\n%s\n", source_equation);

    srand((unsigned int)get_simple_hash(source_equation, source_equation + strlen(source_equation)));

    LexStack eq_lex_stack = lexify(source_equation);
    int lex_caret = 0;

    log_printf(STATUS_REPORTS, "status", "Lexeme stack size = %ld.\n", (long int)eq_lex_stack.size);
    _log_printf(STATUS_REPORTS, "status", "Lexeme stack capacity = %ld.\n", (long int)eq_lex_stack.capacity);

    Equation* equation = parse(eq_lex_stack, &lex_caret);
    track_allocation(equation, Equation_dtor);

    LexStack_dtor(&eq_lex_stack);

    free(source_equation);

    Equation_dump(equation, ABSOLUTE_IMPORTANCE);
    _LOG_FAIL_CHECK_(!Equation_get_error(equation), "error", ERROR_REPORTS, return_clean(EXIT_FAILURE), &errno, EAGAIN);

    ArticleProject article = {};
    Article_ctor(&article, "./");
    track_allocation(article, Article_dtor);

    fprintf(article.storage.file, "\n\\subsection{Derivatives}\n\n");

    describe_differentiation(&article, equation, (unsigned int)max(0, differentiation_power));

    fprintf(article.storage.file, "\n\\subsection{Series representation}\n\n");

    describe_series(&article, equation, series_point, (unsigned int)max(0, series_power));

    fprintf(article.storage.file, "\n\\subsection{Tangent at $X=%lg$}\n\n", series_point);

    describe_tangent(&article, equation, series_point);

    return_clean(errno == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}
