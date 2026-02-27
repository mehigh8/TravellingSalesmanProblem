// Rosu Mihai Cosmin 343C1
#include <stdio.h>
#include <time.h>
#include "serial_helpers.h"

int main(void)
{
	int node_count, start_node;
	graph_t *graph;

	scanf("%d%d", &node_count, &start_node);
	graph = create_graph(node_count, start_node);
	read_graph(graph);

	clock_t start, end;
	start = clock();

	printf("%d\n", solve_tsp(graph));

	end = clock();
	printf("Duration: %lf seconds\n", (double) (end - start) / CLOCKS_PER_SEC);

	free_graph(graph);
	return 0;
}
