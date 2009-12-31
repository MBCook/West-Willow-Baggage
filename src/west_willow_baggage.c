/*
 ============================================================================
 Name        : west_willow_baggage.c
 Author      : Michael Cook
 Version     : 1.0
 Copyright   : I own this code (c)
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "bag_record.h"

int main(void) {
	char buffer[1024];

	memset(&buffer, '\0', 1024);

	if (fgets(buffer, 1024 - 1, stdin) == NULL) {
		printf("Input was empty. Dang.");
		exit(EXIT_FAILURE);
	}

	bag_record *bag = read_bag_record(&buffer);

	printf("The bag we got is:\n");

	memset(&buffer, '\0', 1024);

	print_bag_record(bag, &buffer, 1024);

	cleanup_bag(record);

	exit(EXIT_SUCCESS);
}
