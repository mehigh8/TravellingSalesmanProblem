// Rosu Mihai Cosmin 343C1
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "openmp_helpers.h"

int main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "Usage: ./openmp_tsp <num_threads>\n");
		return -1;
	}
	int num_threads = atoi(argv[1]);

	int node_count, start_node;
	graph_t *graph;

	scanf("%d%d", &node_count, &start_node);
	graph = create_graph(node_count, start_node);
	read_graph(graph);

	double start, end;
	start = omp_get_wtime();

	omp_set_num_threads(num_threads);

	printf("%d\n", solve_tsp(graph));

	end = omp_get_wtime();
	printf("Duration: %lf seconds\n", end - start);

	free_graph(graph);
	return 0;
}
