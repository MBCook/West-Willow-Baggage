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
	// TODO: THIS IS NOT SAFE, we need to make sure things fit
	int count = sscanf(line, "%li %s %s %s %s", &record->timestamp, record->luggage_id,
												record->flight_id, record->source,
												record->destination);

	if (count != 5) {
		printf("Reading in the bag only got us %d pieces of info", count);
		exit(EXIT_FAILURE);
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

	// OK, pos now points to the start of the comment. If it's \0, we're done. If not, copy it

	if ((*pos != '\0') && (*pos != '\n')) {
		int len = strlen(pos);

		if (pos[len - 1] == '\n')	// Ignore trailing new lines
			len--;

		record->comment = malloc(len + 1);

		memset(record->comment, '\0', len + 1);

		if (record->comment == NULL) {
			printf("Couldn't allocate memory to hold the comment");
			exit(EXIT_FAILURE);
		}

		strncpy(record->comment, pos, len);
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
