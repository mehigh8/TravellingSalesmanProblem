// Rosu Mihai Cosmin 343C1
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "mpi_pthreads_helpers.h"

int main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "Usage: mpirun -np <num_procs> mpi_openmp_tsp <num_threads>\n");
		return -1;
	}
	int num_threads = atoi(argv[1]);
	int num_procs, rank;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int node_count, start_node;

	if (rank == ROOT)
		scanf("%d%d", &node_count, &start_node);
	
	MPI_Bcast(&node_count, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
	MPI_Bcast(&start_node, 1, MPI_INT, ROOT, MPI_COMM_WORLD);

	graph_t *graph = create_graph(node_count, start_node);
	read_graph(graph, rank);

	double start, end;
	start = MPI_Wtime();

	int *send_counts = malloc(num_procs * sizeof(int));
	int *displs = malloc(num_procs * sizeof(int));

	int scatter_count = (node_count < num_procs) ? num_procs : node_count;
	int *all_nodes = malloc(scatter_count * sizeof(int));
	int *nodes_to_check = malloc(scatter_count * sizeof(int));

	calculate_scatter(send_counts, displs, scatter_count, num_procs);
	if (rank == ROOT)
		for (int i = 0; i < scatter_count; i++)
			all_nodes[i] = i;

	MPI_Scatterv(all_nodes, send_counts, displs, MPI_INT, nodes_to_check, scatter_count, MPI_INT, ROOT, MPI_COMM_WORLD);

	int best_cost = solve_tsp(num_threads, graph, nodes_to_check, send_counts[rank]);

	if (rank == ROOT) {
		MPI_Status status;
		int recv_cost;
		for (int i = 1; i < num_procs; i++) {
			MPI_Recv(&recv_cost, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
			if (recv_cost < best_cost)
				best_cost = recv_cost;
		}

		printf("%d\n", best_cost);
	} else {
		MPI_Send(&best_cost, 1, MPI_INT, ROOT, 0, MPI_COMM_WORLD);
	}

	free(nodes_to_check);
	free(all_nodes);
	free(displs);
	free(send_counts);

	end = MPI_Wtime();
	if (rank == ROOT)
		printf("Duration: %lf seconds\n", end - start);

	free_graph(graph);

	MPI_Finalize();
	return 0;
}
