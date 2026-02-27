// Rosu Mihai Cosmin 343C1
#ifndef __MPI_OPENMP_HELPERS_H__
#define __MPI_OPENMP_HELPERS_H__

#define ROOT 0

typedef struct {
	int **cost_matrix;
	int node_count;
	int start_node;
} graph_t;

graph_t *create_graph(int node_count, int start_node);
void read_graph(graph_t *graph, int rank);
void free_graph(graph_t *graph);

int calculate_cost(graph_t *graph, int *permutation);
int solve_tsp(graph_t *graph, int *nodes_to_check, int n2c_count);

void calculate_scatter(int *send_counts, int *displs, int node_count, int num_procs);

#endif
