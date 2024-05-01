#ifndef ARTIGEN_H
#define ARTIGEN_H

#include "lib/bin_tree.h"

struct ArticleStorage {
    const char* folder_name = NULL;
    FILE* file = NULL;
};

struct ArticleInfo {
    unsigned int max_dif_power = 0;
};

struct ArticleProject {
    ArticleStorage storage = {};

    ArticleInfo info = {};
};

void Article_ctor(ArticleProject* article, const char* dest_folder);
void Article_dtor(ArticleProject* article);
bool Article_is_fine(ArticleProject* article);

void describe_differentiation(ArticleProject* article, const Equation* equation, unsigned int power);

void describe_series(ArticleProject* article, const Equation* equation, double point, unsigned int power);

void describe_tangent(ArticleProject* article, const Equation* equation, double point);

#endif
