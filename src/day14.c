#include <stdio.h>
#include <string.h>

#include "aoc_util/file_util.h"
#include "aoc_util/hashmap.h"
#include "aoc_util/vector.h"

typedef struct {
    char *name;
    size_t name_len;
    int64_t amt;
} ingredient_t;

typedef struct {
    ingredient_t output;  // Key
    vec_t input;          // Val
} recipe_entry_t;

static int entry_cmp(const void *a_, const void *b_, void *udata) {
    const recipe_entry_t *a = (recipe_entry_t *)a_, *b = (recipe_entry_t *)b_;

    if (a->output.name_len == b->output.name_len) {
        return memcmp(a->output.name, b->output.name, a->output.name_len);
    } else {
        return a->output.name_len - b->output.name_len;
    }
}

static uint64_t entry_hash(const void *item, uint64_t s0, uint64_t s1) {
    const recipe_entry_t *entry = (recipe_entry_t *)item;

    return hashmap_sip(entry->output.name, entry->output.name_len, s0, s1);
}

static struct hashmap *parse_recipe(char *s) {
    struct hashmap *recipe = hashmap_new(sizeof(recipe_entry_t), 0, 3, 14,
                                         entry_hash, entry_cmp, NULL, NULL);

    return recipe;
}

void d14p1() {
    char *inp = file_read_full("input/day14/ex1");
    struct hashmap *recipe = parse_recipe(inp);

    hashmap_free(recipe);
    free(inp);
}

void d14p2() {}
