#ifndef ARGPARSER_H
#define ARGPARSER_H

#include <cstddef>

/**
 * @brief Name of the command line argument
 * 
 * @param short_name one-character name of the argument
 * @param long_name full name of the parameter ("" if should not be recognised)
 */
struct ActionName {
    char short_name = 0;
    const char* long_name = "";
};

struct GenericFunctionCall {
    void** parameters = NULL;
    int parameters_length = 0;
    void (*function)(const int argc, void** argv, const char* argument);
};

/**
 * @brief Structure to store line arguments.
 * 
 * @param name name of the parameter
 * @param action function to execute on call
 * @param description description to print on --help function
 */
struct ActionTag {
    struct ActionName name;
    struct GenericFunctionCall action;
    const char* description = "no information provided.";
};

/**
 * @brief Parse command line arguments and execute actions.
 * 
 * @param argc number of arguments
 * @param argv arguments
 * @param actions_count number of tags
 * @param actions tags
 */
void parse_args(const int argc, const char** argv, const int actions_count, const struct ActionTag* actions);

/**
 * @brief Set integer value (first pointer) to parsed value of argument.
 * 
 * @param argc number of arguments
 * @param argv pointers to arguments (1-st element should be int*)
 * @param argument argument as string
 */
void edit_int(const int argc, void** argv, const char* argument);

/**
 * @brief Set string value to the value of the argument.
 * 
 * @param argc number of arguments
 * @param argv pointers to arguments (1-st element should be const char**)
 * @param argument argument as string
 */
void edit_string(const int argc, void** argv, const char* argument);

/**
 * @brief Set double value (first pointer) to parsed value of argument.
 * 
 * @param argc number of arguments
 * @param argv pointers to arguments (1-st element should be double*)
 * @param argument argument as string
 */
void edit_double(const int argc, void** argv, const char* argument);

#endif