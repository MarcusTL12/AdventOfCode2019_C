#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../AoC_C_utils/src/file_util.h"
#include "../AoC_C_utils/src/vector.h"
#include "../hashmap.c/hashmap.h"

typedef struct {
    char name[3], orbit[3];
    int orbit_depth;
} node_t;

static uint64_t node_hash(const void *node, uint64_t s0, uint64_t s1) {
    return hashmap_sip(node, 3, s0, s1);
}

static int node_cmp(const void *a, const void *b, void *udata) {
    return memcmp(a, b, 3);
}

static void show_node(void *node_) {
    node_t *node = *(node_t **)node_;
    printf("%.*s -> %.*s;    depth: %i\n", 3, node->name, 3, node->orbit,
           node->orbit_depth);
}

static bool print_nodes(const void *node_, void *udata) {
    node_t *node = (void *)node_;
    show_node(&node);
    return true;
}

static struct hashmap *make_graph(char *c) {
    struct hashmap *graph =
        hashmap_new(sizeof(node_t), 0, 0, 0, node_hash, node_cmp, NULL, NULL);

    while (*c) {
        node_t node;

        memset(node.orbit, 0, 3);
        int len = 0;
        while (c[len] != ')') len++;
        memcpy(node.orbit, c, len);
        c += len + 1;

        memset(node.name, 0, 3);
        len = 0;
        while (c[len] != '\n' && c[len]) len++;
        memcpy(node.name, c, len);
        c += len + 1;

        node.orbit_depth = 0;

        hashmap_set(graph, &node);
    }

    return graph;
}

static int traverse_from(struct hashmap *graph, node_t *node) {
    if (node->orbit_depth) {
        return node->orbit_depth;
    } else {
        node_t lookup;
        memcpy(lookup.name, node->orbit, 3);
        node_t *node2 = hashmap_get(graph, &lookup);
        if (node2) {
            node->orbit_depth = traverse_from(graph, node2) + 1;
            return node->orbit_depth;
        } else {
            return 1;
        }
    }
}

static bool collect_keys(const void *node, void *udata) {
    **(node_t ***)udata = (node_t *)node;
    (*(node_t ***)udata)++;

    return true;
}

void d6p1() {
    char *inp = file_read_full("input/day6/input");

    struct hashmap *graph = make_graph(inp);

    size_t amt_nodes = hashmap_count(graph);

    node_t **nodes = malloc(amt_nodes * sizeof(node_t *));
    {
        node_t **nnodes = nodes;
        hashmap_scan(graph, collect_keys, &nnodes);
    }

    int total = 0;

    for (size_t i = 0; i < amt_nodes; i++) {
        total += traverse_from(graph, nodes[i]);
    }

    printf("Total: %i\n", total);

    free(nodes);
    hashmap_free(graph);
    free(inp);
}

static node_t **get_path(struct hashmap *graph, node_t *node) {
    vec_t v = vec_create(sizeof(node_t *));

    node_t lookup;

    lookup.orbit_depth = 0;

    do {
        vec_push(&v, &node);
        memcpy(lookup.name, node->orbit, 3);
        node = hashmap_get(graph, &lookup);
    } while (node);

    node_t **rev_path = v.data, **path = malloc(sizeof(node_t *) * v.len);

    for (size_t i = 0, j = v.len - 1; i < v.len; i++, j--) {
        path[i] = rev_path[j];
    }

    vec_free(&v);

    return path;
}

void d6p2() {
    char *inp = file_read_full("input/day6/input");

    struct hashmap *graph = make_graph(inp);

    node_t lookup;
    memcpy(lookup.name, "YOU", 3);
    node_t *you = hashmap_get(graph, &lookup);
    node_t **path_you = get_path(graph, you);
    memcpy(lookup.name, "SAN", 3);
    node_t *san = hashmap_get(graph, &lookup);
    node_t **path_san = get_path(graph, hashmap_get(graph, &lookup));

    node_t *common;

    for (size_t i = 0; path_you[i] == path_san[i]; i++) {
        common = path_you[i];
    }

    free(path_you);
    free(path_san);

    int ans = traverse_from(graph, you) + traverse_from(graph, san) -
              2 * traverse_from(graph, common) - 2;

    printf("Dist: %i\n", ans);

    hashmap_free(graph);
    free(inp);
}
