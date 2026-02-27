// Rosu Mihai Cosmin 343C1
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include "pthreads_helpers.h"

int main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "Usage ./pthreads_tsp <num_threads>\n");
		return -1;
	}
	int num_threads = atoi(argv[1]);

	int node_count, start_node;
	graph_t *graph;

	scanf("%d%d", &node_count, &start_node);
	graph = create_graph(node_count, start_node);
	read_graph(graph);

	struct timespec start, finish;
	double elapsed;

	clock_gettime(CLOCK_MONOTONIC, &start);

	pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
	int *results = malloc(num_threads * sizeof(int));
	thread_data_t *thread_datas = malloc(num_threads * sizeof(thread_data_t));

	for (int i = 0; i < num_threads; i++) {
		thread_datas[i].graph = graph;
		thread_datas[i].results = results;
		thread_datas[i].thread_id = i;
		thread_datas[i].num_threads = num_threads;

		if (pthread_create(&threads[i], NULL, solve_tsp, &thread_datas[i])) {
			fprintf(stderr, "Error when creating thread %d\n", i);
			return -1;
		}
	}

	for (int i = 0; i < num_threads; i++)
		if (pthread_join(threads[i], NULL)) {
			fprintf(stderr, "Error when joining thread %d\n", i);
			return -1;
		}

	int best_cost = results[0];
	for (int i = 1; i < num_threads; i++)
		if (results[i] < best_cost)
			best_cost = results[i];

	printf("%d\n", best_cost);
	
	free(thread_datas);
	free(results);
	free(threads);

	clock_gettime(CLOCK_MONOTONIC, &finish);

	elapsed = finish.tv_sec - start.tv_sec;
	elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
	printf("Duration: %lf seconds\n", elapsed);

	free_graph(graph);
	return 0;
}
