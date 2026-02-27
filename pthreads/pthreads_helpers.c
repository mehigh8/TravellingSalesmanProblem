// Rosu Mihai Cosmin 343C1
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "pthreads_helpers.h"

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

int calculate_cost(graph_t *graph, int *permutation)
{
	int cost = 0;
	for (int i = 0; i < graph->node_count - 1; i++)
		cost += graph->cost_matrix[permutation[i]][permutation[i+1]];

	cost += graph->cost_matrix[permutation[graph->node_count - 1]][permutation[0]];

	return cost;
}

static double min(double a, double b)
{
	return a < b ? a : b;
}

void *solve_tsp(void *arg)
{
	thread_data_t *data = (thread_data_t *)arg;
	data->results[data->thread_id] = INT_MAX;

	int start = data->thread_id * (double)data->graph->node_count / data->num_threads;
	int end = min((data->thread_id + 1) * (double)data->graph->node_count / data->num_threads, data->graph->node_count);

	int *permutation = malloc(data->graph->node_count * sizeof(int));
	int *used = calloc(data->graph->node_count, sizeof(int));

	for (int second_node = start; second_node < end; second_node++) {
		if (second_node == data->graph->start_node)
			continue;

		for (int i = 0; i < data->graph->node_count; i++) {
			permutation[i] = -1;
			used[i] = 0;
		}

		permutation[0] = data->graph->start_node;
		used[data->graph->start_node] = 1;
		permutation[1] = second_node;
		used[second_node] = 1;

		int node_idx = 2;
		while (node_idx > 1) {
			if (node_idx == data->graph->node_count) {
				int new_cost = calculate_cost(data->graph, permutation);
				if (new_cost < data->results[data->thread_id])
					data->results[data->thread_id] = new_cost;

				node_idx--;
				continue;
			}

			int new_val = permutation[node_idx];

			do
			{
				new_val++;
				if (new_val == data->graph->node_count)
					break;
			} while (used[new_val] == 1);

			if (new_val == data->graph->node_count) {
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
	}

	free(permutation);
	free(used);

	return NULL;
}
