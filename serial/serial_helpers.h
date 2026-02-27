// Rosu Mihai Cosmin 343C1
#ifndef __SERIAL_HELPERS_H__
#define __SERIAL_HELPERS_H__

typedef struct {
	int **cost_matrix;
	int node_count;
	int start_node;
} graph_t;

graph_t *create_graph(int node_count, int start_node);
void read_graph(graph_t *graph);
void free_graph(graph_t *graph);

int solve_tsp(graph_t *graph);
int calculate_cost(graph_t *graph, int *permutation);

#endif
