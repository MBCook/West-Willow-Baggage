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
#include "bag_carousel.h"
#include "bag_record.h"

void print_carousel(bag_carousel *carousel, char* buffer, int buffer_size);

int main(void) {
	char buffer[1024];

	memset(&buffer, '\0', 1024);

	printf("Please enter a bag record per line, bank or EOF to end:\n");

	bag_carousel *carousel = NULL;

	fflush(stdout);

	while (fgets(buffer, 1024 - 1, stdin) != NULL) {
		if (strlen(buffer) < 2)			// So it will catch new lines
			break;	// Done with stuff

		// Get the bag

		bag_record *bag = read_bag_record(buffer);

		// Handle it

		if (carousel == NULL) {
			// Create the carousel

			carousel = create_carousel(create_bin(bag));
		} else {
			// Make sure we're not replacing something

			bag_bin *bin = find_bin(carousel, bag->luggage_id);

			if (bin == NULL) {
				// Need to add it

				add_bin(carousel, create_bin(bag));
			} else {
				// Need to update it

				update_bin(bin, bag);
			}
		}

		memset(buffer, '\0', 1024);
	}

	// OK, we're done, we'll need to dump bag history

	printf("The most recent information is:\n");

	print_carousel(carousel, buffer, 1024);

	cleanup_carousel(carousel);

	exit(EXIT_SUCCESS);
}

void print_carousel(bag_carousel *carousel, char* buffer, int buffer_size) {
	if (carousel->left != NULL)
		print_carousel(carousel->left, buffer, buffer_size);

	bag_bin *bin = carousel->bin;

	while (bin != NULL) {
		print_bag_record(bin->bag, buffer, 1024);

		printf("%s\n", buffer);

		bin = bin->next;
	}

	if (carousel->right != NULL)
		print_carousel(carousel->right, buffer, buffer_size);
}
