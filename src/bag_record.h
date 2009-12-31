/*
 ============================================================================
 Name        : bag_record.h
 Author      : Michael Cook
 Version     : 1.0
 Copyright   : I own this code (c)
 ============================================================================
 */

#ifndef BAG_RECORD_H_
#define BAG_RECORD_H_

// Used to keep track of bags

struct bag_record {
	long timestamp;			// Seconds since unix epoch
	char luggage_id[7];		// Luggage ID, with extra space for null termination (like all below)
	char flight_id[7];		// Flight ID
	char source[4];			// Source airport
	char destination[4];	// Destination airport
	char *comment;			// Comment field
};

// Given a line of text, extract the bag record from it
// Caller is responsible for calling cleanup_bag to cleanup the memory allocated for the comment field

bag_record *read_bag_record(char *line);

// Given a bag record and a buffer, fill the buffer with a text version of the bag record

void print_bag_record(bag_record *record, char *buffer, int size);

// Clean up the memory assoicated with a bag record

void cleanup_bag(bag_record *record);

#endif /* BAG_RECORD_H_ */
