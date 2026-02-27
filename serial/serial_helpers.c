// Rosu Mihai Cosmin 343C1
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "serial_helpers.h"

graph_t *create_graph(int node_count, int start_node)
{
	graph_t *graph = malloc(sizeof(graph_t));

	graph->node_count = node_count;
	graph->start_node = start_node;

	graph->cost_matrix = malloc(node_count * sizeof(int *));
	for (int i = 0; i < node_count; i++)
		graph->cost_matrix[i] = malloc(node_count * sizeof(int));

	return graph;
}

void read_graph(graph_t *graph)
{
	for (int i = 0; i < graph->node_count; i++)
		for (int j = 0; j < graph->node_count; j++)
			scanf("%d", &(graph->cost_matrix[i][j]));
}

void free_graph(graph_t *graph)
{
	for (int i = 0; i < graph->node_count; i++)
		free(graph->cost_matrix[i]);

	free(graph->cost_matrix);
	free(graph);
}

int solve_tsp(graph_t *graph)
{
	int best_cost = INT_MAX;
	int *permutation = malloc(graph->node_count * sizeof(int));
	int *used = calloc(graph->node_count, sizeof(int));

	for (int i = 0; i < graph->node_count; i++)
		permutation[i] = -1;

	permutation[0] = graph->start_node;
	used[graph->start_node] = 1;

	int node_idx = 1;
	while (node_idx > 0) {
		if (node_idx == graph->node_count) {
			int new_cost = calculate_cost(graph, permutation);
			if (new_cost < best_cost)
				best_cost = new_cost;

			node_idx--;
			continue;
		}

		int new_val = permutation[node_idx];

		do
		{
			new_val++;
			if (new_val == graph->node_count)
				break;
		} while (used[new_val] == 1);

		if (new_val == graph->node_count) {
			used[permutation[node_idx]] = 0;
			permutation[node_idx] = -1;
			node_idx--;
		} else {
			if (permutation[node_idx] != -1)
				used[permutation[node_idx]] = 0;

			permutation[node_idx] = new_val;
			used[new_val] = 1;
			node_idx++;
		}
	}

	free(permutation);
	free(used);
	return best_cost;
}

int calculate_cost(graph_t *graph, int *permutation)
{
	int cost = 0;
	for (int i = 0; i < graph->node_count - 1; i++)
		cost += graph->cost_matrix[permutation[i]][permutation[i+1]];

	cost += graph->cost_matrix[permutation[graph->node_count - 1]][permutation[0]];

	return cost;
}
