// Rosu Mihai Cosmin 343C1
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <mpi.h>
#include "mpi_helpers.h"

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

void read_graph(graph_t *graph, int rank)
{
	for (int i = 0; i < graph->node_count; i++) {
		for (int j = 0; j < graph->node_count; j++)
			if (rank == ROOT)
				scanf("%d", &(graph->cost_matrix[i][j]));
		MPI_Bcast(graph->cost_matrix[i], graph->node_count, MPI_INT, ROOT, MPI_COMM_WORLD);
	}
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

int solve_tsp(graph_t *graph, int *nodes_to_check, int n2c_count)
{
	int best_cost = INT_MAX;
	int *permutation = malloc(graph->node_count * sizeof(int));
	int *used = calloc(graph->node_count, sizeof(int));

	for (int i = 0; i < n2c_count; i++) {
		int second_node = nodes_to_check[i];
		if (second_node >= graph->node_count || second_node == graph->start_node)
			continue;

		for (int i = 0; i < graph->node_count; i++) {
			permutation[i] = -1;
			used[i] = 0;
		}

		permutation[0] = graph->start_node;
		used[graph->start_node] = 1;
		permutation[1] = second_node;
		used[second_node] = 1;

		int node_idx = 2;
		while (node_idx > 1) {
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
	}

	free(permutation);
	free(used);

	return best_cost;
}

void calculate_scatter(int *send_counts, int *displs, int node_count, int num_procs)
{
	int remainder = node_count % num_procs;
	int displacement_sum = 0;

	for (int i = 0; i < num_procs; i++) {
		send_counts[i] = node_count/num_procs;
		if (remainder > 0) {
			send_counts[i]++;
			remainder--;
		}

		displs[i] = displacement_sum;
		displacement_sum += send_counts[i];
	}
}
