#define CLOVE_ENABLE_AUTODISCOVERY
#include "clove.h"


CLOVE_RUNNER()

/*
int main() {
	__clove_vector_t vect;
	__clove_vector_init(&vect, sizeof(int));

	int* value1 = (int*)__clove_vector_next(&vect);
	*value1 = 1;

	int* value2 = (int*)__clove_vector_next(&vect);
	*value2 = 2;

	int* values = (int*)vect.items;

	int count = __clove_vector_count(&vect);
	for (int i = 0; i < count; i++) {
		printf("%d) %d\n", i, values[i]);
	}


    puts("GET");
    value1 = (int*) __clove_vector_get(&vect, 0);
    value2 = (int*)__clove_vector_get(&vect, 1);
    printf("%d) %d\n", 0, *value1);
    printf("%d) %d\n", 1, *value2);
}
*/