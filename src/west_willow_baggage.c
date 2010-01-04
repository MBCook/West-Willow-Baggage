/*
 ============================================================================
 Name        : west_willow_baggage.c
 Author      : Michael Cook
 Version     : 1.0
 Copyright   : I own this code (c)
 ============================================================================
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bag_carousel.h"
#include "bag_record.h"

// Forward definitions

void print_carousel(bag_carousel *carousel, char* buffer, int buffer_size,
							char *luggage, char* flight, char *source, char *destination);
bool filter_bag(bag_record *bag, char *luggage, char* flight, char *source, char *destination);

// Meat of the file

int main(int argc, char** argv) {
	// OK, first let's make sure our command line arguments are sane

	if (argc != 5) {
		printf("Usage: %s LUGGAGE_ID FLIGHT_ID DEPARTING ARRIVING\n", argv[0]);
		printf("\tEach argument is a search string for that piece of information\n");
		printf("\tSubstituting a hyphen for a search string acts as a wildcard\n");

		exit(EXIT_FAILURE);
	}

	// OK, our arguments are sane, let's move on and load in all the luggage data

	char buffer[1024];

	memset(&buffer, '\0', 1024);

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

	// OK, we're done, we'll need to dump bag history, based on our filter

	print_carousel(carousel, buffer, 1024, argv[1], argv[2], argv[3], argv[4]);

	cleanup_carousel(carousel);

	exit(EXIT_SUCCESS);
}

void print_carousel(bag_carousel *carousel, char* buffer, int buffer_size,
									char *luggage, char* flight, char *source, char *destination) {
	if (carousel->left != NULL)
		print_carousel(carousel->left, buffer, buffer_size, luggage, flight, source, destination);

	bag_bin *bin = carousel->bin;

	while (bin != NULL) {
		if (filter_bag(bin->bag, luggage, flight, source, destination)) {
			print_bag_record(bin->bag, buffer, 1024);

			printf("%s\n", buffer);

			memset(buffer, '\0', 1024);
		}

		bin = bin->next;
	}

	if (carousel->right != NULL)
		print_carousel(carousel->right, buffer, buffer_size, luggage, flight, source, destination);
}

bool filter_bag(bag_record *bag, char *luggage, char* flight, char *source, char *destination) {
	if (strncmp("-", luggage, 1) != 0) {
		// Filter on luggage ID

		if (strncmp(bag->luggage_id, luggage, 8) != 0)
			return false;
	}

	if (strncmp("-", flight, 1) != 0) {
		// Filter on flight ID

		if (strncmp(bag->flight_id, flight, 6) != 0)
			return false;
	}

	if (strncmp("-", source, 1) != 0) {
		// Filter on departing airport ID

		if (strncmp(bag->source, source, 3) != 0)
			return false;
	}

	if (strncmp("-", destination, 1) != 0) {
		// Filter on destination airport ID

		if (strncmp(bag->destination, destination, 3) != 0)
			return false;
	}

	return true;
}
