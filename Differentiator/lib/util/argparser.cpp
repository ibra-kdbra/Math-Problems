#include "argparser.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "dbg/debug.h"

/**
 * @brief Print tag description to the console.
 * 
 * @param tag tag to describe
 */
static void print_description(const ActionTag& tag);

/**
 * @brief Check if tag was called.
 * 
 * @param str string to check
 * @param tag tag to compare string to
 * @return bool if tag could be called by str
 */
static inline bool match(const char* str, const ActionTag& tag);

void parse_args(const int argc, const char** argv, const int action_c, const struct ActionTag* actions) {
    for (int arg_id = 1; arg_id < argc; arg_id++) {
        const char* arg = argv[arg_id];

        if (arg[0] != '-' || strlen(arg) < 2)
            continue;
        
        for (int tag_id = 0; tag_id < action_c; tag_id++) {
            if (!(arg[1] == actions[tag_id].name.short_name || 
                (arg[1] == '-' && strcmp(arg + 2, actions[tag_id].name.long_name) == 0))) continue;
            //                                  ^ number of chars in "--"
            const GenericFunctionCall* call = &actions[tag_id].action;
            if (call->function)
                call->function(call->parameters_length, 
                call->parameters, arg + 2);
        }  //   number of chars in "--" ^

        if (    strcmp(arg, "--help") == 0 || 
                strcmp(arg, "-help") == 0 || 
                strcmp(arg, "-h") == 0 || 
                strcmp(arg, "-H") == 0) {
            printf("Valid tags:\n\n");
            printf("-H -h -help --help - print this message.\n\n");
            for (int tag_id = 0; tag_id < action_c; tag_id++) {
                print_description(actions[tag_id]);
            }
            //* Any program should stop if user only wants to know his options.
            exit(EXIT_SUCCESS);
        }
    }
}

void edit_int(const int argc, void** argv, const char* argument) {
    SILENCE_UNUSED(argc);
    *(int*)argv[0] = atoi(argument);
}

void edit_string(const int argc, void** argv, const char* argument) {
    SILENCE_UNUSED(argc);
    strcpy(*(char**)argv, argument);
}

void edit_double(const int argc, void** argv, const char* argument) {
    SILENCE_UNUSED(argc);
    *(double*)argv[0] = atof(argument);
}

void print_description(const ActionTag& tag) {
    if (*tag.name.long_name)
        printf("-%c --%s - %s\n\n", tag.name.short_name, tag.name.long_name, tag.description);
    else
        printf("-%c - %s\n\n", tag.name.short_name, tag.description);
}

bool match(const char* str, const ActionTag& tag) {
    if (*str != '-') return false;

    bool short_match = str[1] == tag.name.short_name;

    if (*tag.name.long_name && *str && str[1] == '-') {
        bool long_match = strcmp(str + 2, tag.name.long_name) == 0;
        return short_match || long_match;
    } else return short_match;
}