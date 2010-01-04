/*
 ============================================================================
 Name        : bag_record.c
 Author      : Michael Cook
 Version     : 1.0
 Copyright   : I own this code (c)
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bag_record.h"

bag_record *read_bag_record(char *line) {
	// First, we'll need a bag record

	bag_record *record = malloc(sizeof(bag_record));

	if (record == NULL) {
		printf("Unable to allocate memory for a new bag");
		exit(EXIT_FAILURE);
	}

	// Clear it out

	memset(record, 0, sizeof(bag_record));

	// Read stuff in

	int count = sscanf(line, "%li %8s %6s %3s %3s", &record->timestamp, record->luggage_id,
												record->flight_id, record->source,
												record->destination);

	if (count != 5) {
		printf("Reading in the bag only got us %d pieces of info", count);
		exit(EXIT_FAILURE);
	} else {
		// OK, we're going to do some sanity checks

		if ((strlen(record->source) != 3) || (strlen(record->destination) != 3)) {
			// OK, airport codes weren't the right length

			printf("One (or more) airport codes were of the wrong length");
			exit(EXIT_FAILURE);
		} else if (strlen(record->flight_id) < 3) {
			// Flight code wasn't two letters and at least 1 digit

			printf("The flight ID was too short to be valid");
			exit(EXIT_FAILURE);
		} else if (strlen(record->luggage_id) != 8) {
			// Luggage ID wasn't the right length

			printf("The luggage ID wasn't 8 characters as expected");
			exit(EXIT_FAILURE);
		}
	}

	// Now we need to get the comment string, if it exists. Find out where it would be

	char *pos = strstr(line, record->destination);

	if (pos == NULL) {
		printf("The line didn't contain the destination code it did before. Odd.");
		exit(EXIT_FAILURE);
	}

	pos = pos + strlen(record->destination);	// Make up for the destination it's self

	// Now that we know that, find the next start of something useful

	while ((*pos == ' ') || (*pos == '\t'))
		pos++;

	// OK, pos now points to the start of the comment. If it's \0 or \n, we're done. If not, copy it

	if ((*pos != '\0') && (*pos != '\n')) {
		int len = strlen(pos);

		if (pos[len - 1] == '\n')	// Ignore trailing new lines
			len--;

		// Copy out the comment so we can have a version that will stay around

		record->comment = malloc(len + 1);

		if (record->comment == NULL) {
			printf("Couldn't allocate memory to hold the comment");
			exit(EXIT_FAILURE);
		}

		memset(record->comment, '\0', len + 1);

		strncpy(record->comment, pos, len);

		// Make sure no ASCII control characters slipped through to annoy people (like the bell)
		// Replace them with a space

		int i;

		for (i = 0; i < len; i++) {
			if ((record->comment[i] < 32) || (record->comment[i] == 127))	// ASCII control character?
				record->comment[i] = ' ';									// Replace it with a space
		}
	}

	return record;
}

void print_bag_record(bag_record *record, char *buffer, int size) {
	// First, clear the buffer

	memset(buffer, '\0', size);

	// Now construct the string

	if (record->comment != NULL) {
		snprintf(buffer, size - 1, "%li %s %s %s %s %s", record->timestamp, record->luggage_id,
															record->flight_id, record->source,
															record->destination, record->comment);
	} else {
		snprintf(buffer, size - 1, "%li %s %s %s %s", record->timestamp, record->luggage_id,
														record->flight_id, record->source,
														record->destination);
	}
}

void cleanup_bag(bag_record *record) {
	// Clean up the comment field, if it exists

	if (record->comment != NULL) {
		free(record->comment);
	}

	// Clean up the record it's self

	free(record);
}
