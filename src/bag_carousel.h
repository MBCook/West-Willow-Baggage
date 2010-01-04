/*
 ============================================================================
 Name        : bag_carousel.h
 Author      : Michael Cook
 Version     : 1.0
 Copyright   : I own this code (c)
 ============================================================================
 */

#include "bag_record.h"

#ifndef BAG_CAROUSEL_H_
#define BAG_CAROUSEL_H_

// Temporary definitions so things compile

struct bag_bin_struct;
struct bag_carousel_struct;

typedef struct bag_bin_struct bag_bin;
typedef struct bag_carousel_struct bag_carousel;

// The data structures we use to keep bags organized in memory.

// Bin stores the bag and a pointer to the next bag. This is what our tree will hold.
// We call it a "bin" because you can find bins of bags at airports. Each bin (which is
// basically a singly linked list) holds all current records for the given bag.

struct bag_bin_struct {
	bag_record *bag;
	bag_bin *next;
};

// Now the node for our tree. Holds the a left pointer (bag with luggage ID less than us),
// a right bag pointer (bag with luggage ID more than us), and a bag_bin to hold the schedule
// for our luggage ID.

struct bag_carousel_struct {
	bag_carousel *left;
	bag_bin *bin;
	bag_carousel *right;
};

// Functions for working with bins

bag_bin *create_bin(bag_record *bag);
void update_bin(bag_bin *bin, bag_record *new_bag);
void cleanup_bin(bag_bin *bin);

// Functions for working with the carousel (which, again, is actually a tree)

bag_carousel *create_carousel(bag_bin *bin);
bag_bin *find_bin(bag_carousel *carousel, char *luggage_id);
void add_bin(bag_carousel *carousel, bag_bin *bin);
void cleanup_carousel(bag_carousel *carousel);

#endif /* BAG_CAROUSEL_H_ */
