/*
 ============================================================================
 Name        : bag_carousel.c
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

bag_bin *create_bin(bag_record *bag) {
	bag_bin *bin = malloc(sizeof(bag_bin));

	if (bin == NULL) {
		printf("Unable to allocate memory for a new bin");
		exit(EXIT_FAILURE);
	}

	// Clear it out

	memset(bin, 0, sizeof(bag_bin));

	// Set it up

	bin->bag = bag;

	return bin;
}

void update_bin(bag_bin *bin, bag_record *new_bag) {
	// We're going to have to figure out if this is a new step, or replaces an old step

	while (bin != NULL) {
		// See if we need to replace this bag

		if (strncmp(new_bag->source, bin->bag->source, 3) == 0) {
			// We need to replace this record. Clean it up, and the rest of the bin

			cleanup_bag(bin->bag);

			if (bin->next != NULL) {
				cleanup_bin(bin->next);

				bin->next = NULL;
			}

			bin->bag = new_bag;

			return;	// We're done
		} else if (bin->next == NULL) {
			// OK, we are going to have add a new bin for this to go next, since there isn't one to replace

			bin->next = create_bin(new_bag);

			return; // We're done
		}

		// Look at the next part of the bin next

		bin = bin->next;
	}
}

void cleanup_bin(bag_bin *bin) {
	// Clean up the bag if it exists, and next bag, if it exists, etc.

	while (bin != NULL) {
		// Clear the bag, if it's there

		if (bin->bag != NULL)
			free(bin->bag);

		// Hold onto a pointer to us, point to the next bag, delete us

		bag_bin *us = bin;

		bin = bin->next;

		free(us);
	}
}

bag_carousel *create_carousel(bag_bin *bin) {
	bag_carousel *carousel = malloc(sizeof(bag_carousel));

	if (carousel == NULL) {
		printf("Unable to allocate memory for a new carousel");
		exit(EXIT_FAILURE);
	}

	// Clear it out

	memset(carousel, 0, sizeof(bag_carousel));

	// Set it up

	carousel->bin = bin;

	return carousel;
}

bag_bin *find_bin(bag_carousel *carousel, char *luggage_id) {
	while (carousel != NULL) {
		int ret_val = strncmp(carousel->bin->bag->luggage_id, luggage_id, 8);

		if (ret_val == 0) {
			// Found it

			return carousel->bin;
		} else if (ret_val < 0) {
			// It's to our left

			carousel = carousel->left;
		} else {
			// It's to our right

			carousel = carousel->right;
		}
	}

	// We didn't find the right bin

	return NULL;
}

void add_bin(bag_carousel *carousel, bag_bin *bin) {
	// Find the right spot for this

	while (carousel != NULL) {
		// Is this the right place?

		int ret_val = strncmp(carousel->bin->bag->luggage_id, bin->bag->luggage_id, 8);

		if (ret_val == 0) {
			// We need to replace this bin here

			cleanup_bin(carousel->bin);

			carousel->bin = bin;
		} else if (ret_val < 0) {
			// Go left!

			if (carousel->left == NULL) {
				carousel->left = create_carousel(bin);

				return;
			}

			carousel = carousel->left;
		} else {
			// Go right!

			if (carousel->right == NULL) {
				carousel->right = create_carousel(bin);

				return;
			}

			carousel = carousel->right;
		}
	}
}

void cleanup_carousel(bag_carousel *carousel) {
	// Clean up the left side, then the right, the the bin, then us
	// We're going to assume we won't have to recurse too deeply

	if (carousel->left != NULL)
		cleanup_carousel(carousel->left);
	if (carousel->right != NULL)
		cleanup_carousel(carousel->right);
	if (carousel->bin != NULL)
		cleanup_bin(carousel->bin);

	free(carousel);
}
