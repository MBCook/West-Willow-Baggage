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
#include <string.h>
#include "bag_record.h"

int main(void) {
	char buffer[1024];

	memset(&buffer, '\0', 1024);

	printf("Please enter a bag record:\n");

	fflush(stdout);

	if (fgets(buffer, 1024 - 1, stdin) == NULL) {
		printf("Input was empty. Dang.");
		exit(EXIT_FAILURE);
	}

	bag_record *bag = read_bag_record(buffer);

	printf("The bag we got is:\n");

	print_bag_record(bag, buffer, 1024);

	printf("%s\n", buffer);

	cleanup_bag(bag);

	exit(EXIT_SUCCESS);
}
