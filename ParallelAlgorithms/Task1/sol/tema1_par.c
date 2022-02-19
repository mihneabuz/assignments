#include <stdlib.h>
#include <stdio.h>
#include "genetic_algorithm.h"

int main(int argc, char *argv[]) {
	sack_object *objects = NULL;
	int object_count = 0;
	int sack_capacity = 0;
	int generations_count = 0;
    int proccesor_count = 0;

	if (!read_input(&objects, &object_count, &sack_capacity, &generations_count, &proccesor_count, argc, argv)) {
		return 0;
	}

	run_genetic_algorithm(objects, object_count, generations_count, sack_capacity, proccesor_count);

	free(objects);

	return 0;
}
