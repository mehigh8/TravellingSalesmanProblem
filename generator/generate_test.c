#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
	if (argc < 2) {
		fprintf(stderr, "Usage: ./generate_test <node_count> [output_name]\n");
		return -1;
	}
	srand(time(NULL));

	int node_count = atoi(argv[1]);
	FILE *out;
	if (argc == 2)
		out = fopen("test.in", "w");
	else
		out = fopen(argv[2], "w");
	fprintf(out, "%d %d\n", node_count, rand() % node_count);

	for (int i = 0; i < node_count; i++) {
		for (int j = 0; j < node_count; j++) {
			if (j == i)
				fprintf(out, "0 ");
			else
				fprintf(out, "%d ", rand() % 50 + 1);
		}
		fprintf(out, "\n");
	}

	fclose(out);
	return 0;
}
