#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../AoC_C_utils/src/file_util.h"
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

static bool print_nodes(const void *node, void *udata) {
    printf("%.*s -> %.*s;    depth: %i\n", 3, node, 3, node + 3,
           *((int *)(node + 6)));
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
            return 0;
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

    // hashmap_scan(graph, print_nodes, NULL);

    size_t amt_nodes = hashmap_count(graph);

    node_t **nodes = malloc(amt_nodes * sizeof(node_t *));
    {
        node_t **nnodes = nodes;
        hashmap_scan(graph, collect_keys, &nnodes);
    }

    int total = 0;

    for (size_t i = 0; i < amt_nodes; i++) {
        int l = traverse_from(graph, nodes[i]);

        // printf("l: %i\n", l);

        total += l + 1;
    }

    // hashmap_scan(graph, print_nodes, NULL);

    // hashmap_scan(graph, collect_keys, data);

    // node_t lookup, *node;

    // memcpy(lookup.name, "H\0\0", 3);

    // node = hashmap_get(graph, &lookup);

    // traverse_from(graph, node);

    printf("Total: %i\n", total);

    free(nodes);
    hashmap_free(graph);
    free(inp);
}

void d6p2() {}
