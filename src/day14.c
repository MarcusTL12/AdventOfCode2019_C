#include <ctype.h>
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

static int ingredient_cmp(const void *a_, const void *b_, void *udata) {
    const ingredient_t *a = (ingredient_t *)a_, *b = (ingredient_t *)b_;

    if (a->name_len == b->name_len) {
        return memcmp(a->name, b->name, a->name_len);
    } else {
        return a->name_len - b->name_len;
    }
}

static uint64_t ingredient_hash(const void *ing_, uint64_t s0, uint64_t s1) {
    const ingredient_t *ing = (ingredient_t *)ing_;

    return hashmap_sip(ing->name, ing->name_len, s0, s1);
}

typedef struct {
    ingredient_t output;
    vec_t input;  // Element type: ingredient_t
} recipe_entry_t;

static int entry_cmp(const void *a_, const void *b_, void *udata) {
    const recipe_entry_t *a = (recipe_entry_t *)a_, *b = (recipe_entry_t *)b_;

    return ingredient_cmp(&a->output, &b->output, udata);
}

static uint64_t entry_hash(const void *entry_, uint64_t s0, uint64_t s1) {
    const recipe_entry_t *entry = (recipe_entry_t *)entry_;

    return ingredient_hash(&entry->output, s0, s1);
}

static void entry_free(void *entry_) {
    recipe_entry_t *entry = (recipe_entry_t *)entry_;
    vec_free(&entry->input);
}

static struct hashmap *parse_recipe(char *s) {
    struct hashmap *recipe =
        hashmap_new(sizeof(recipe_entry_t), 0, 3, 14, entry_hash, entry_cmp,
                    entry_free, NULL);

    recipe_entry_t entry;

    while (*s && *s != '\n') {
        entry.input = vec_create(sizeof(ingredient_t));
        while (*s != '=') {
            ingredient_t ing;
            ing.amt = atoll(s);
            while (isdigit(*s)) s++;
            s++;
            ing.name = s;
            ing.name_len = 0;
            while (isalpha(*(s++))) ing.name_len++;
            while (*s == ' ') s++;
            vec_push(&entry.input, &ing);
        }

        s += 3;

        entry.output.amt = atoll(s);
        while (isdigit(*s)) s++;
        s++;
        entry.output.name = s;
        entry.output.name_len = 0;
        while (isalpha(*(s++))) entry.output.name_len++;
        hashmap_set(recipe, &entry);
    }

    return recipe;
}

static void make_rec(struct hashmap *curprod, struct hashmap *cursurplus,
                     struct hashmap *recipe, ingredient_t chemical) {
    ingredient_t *ing, ore;
    ore.name = "ORE";
    ore.name_len = 3;
    if (!ingredient_cmp(&ore, &chemical, NULL)) {
        ing = hashmap_get(curprod, &ore);
        ing->amt += chemical.amt;
    } else {
        recipe_entry_t *currecipe = hashmap_get(recipe, &chemical);
        int64_t amtrecipies =
            (chemical.amt + currecipe->output.amt - 1) / currecipe->output.amt;

        ing = hashmap_get(cursurplus, &chemical);
        ing->amt += amtrecipies * currecipe->output.amt - chemical.amt;

        ing = hashmap_get(curprod, &chemical);
        ing->amt += amtrecipies * currecipe->output.amt;

        for (size_t i = 0; i < currecipe->input.len; i++) {
            ingredient_t inp = *(ingredient_t *)vec_get(&currecipe->input, i);
            int64_t need = inp.amt * amtrecipies;

            ing = hashmap_get(cursurplus, &inp);
            if (need < ing->amt) {
                ing->amt -= need;
            } else {
                need -= ing->amt;
                ing->amt = 0;
                inp.amt = need;
                make_rec(curprod, cursurplus, recipe, inp);
            }
        }
    }
}

static int64_t makefuel(struct hashmap *recipe, int64_t amt) {
    struct hashmap *curprod =
        hashmap_new(sizeof(ingredient_t), hashmap_count(recipe), 123, 456,
                    ingredient_hash, ingredient_cmp, NULL, NULL);
    struct hashmap *cursurplus =
        hashmap_new(sizeof(ingredient_t), hashmap_count(recipe), 123, 456,
                    ingredient_hash, ingredient_cmp, NULL, NULL);

    recipe_entry_t *entry;
    ingredient_t ing;
    ing.amt = 0;
    for (size_t i = 0; hashmap_iter(recipe, &i, (void **)&entry);) {
        ing.name = entry->output.name;
        ing.name_len = entry->output.name_len;
        hashmap_set(curprod, &ing);
        hashmap_set(cursurplus, &ing);
    }

    ing.name = "ORE";
    ing.name_len = 3;

    hashmap_set(curprod, &ing);
    hashmap_set(cursurplus, &ing);

    ing.name = "FUEL";
    ing.name_len = 4;
    ing.amt = amt;

    make_rec(curprod, cursurplus, recipe, ing);

    ing.name = "ORE";
    ing.name_len = 3;

    ingredient_t *ore = hashmap_get(curprod, &ing);

    int64_t ans = ore->amt;

    hashmap_free(curprod);
    hashmap_free(cursurplus);

    return ans;
}

void d14p1() {
    char *inp = file_read_full("input/day14/input");
    struct hashmap *recipe = parse_recipe(inp);

    int64_t ans = makefuel(recipe, 1);
    printf("Need %li ore\n", ans);

    hashmap_free(recipe);
    free(inp);
}

void d14p2() {
    char *inp = file_read_full("input/day14/input");
    struct hashmap *recipe = parse_recipe(inp);

    int64_t available_ore = 1000000000000;

    int64_t low = 1, high = 2;

    while (makefuel(recipe, high) < available_ore) {
        low = high;
        high <<= 1;
    }

    while (high - low > 1) {
        int64_t mid = (high + low) / 2;
        int64_t ore = makefuel(recipe, mid);
        if (ore > available_ore) {
            high = mid;
        } else if (ore < available_ore) {
            low = mid;
        } else {
            low = mid;
            break;
        }
    }

    printf("Can make %li fuel\n", low);

    hashmap_free(recipe);
    free(inp);
}
