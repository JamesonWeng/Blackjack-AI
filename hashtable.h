/*
 * hashtable.h
 *
 *  Created on: Jan 17, 2016
 *      Author: Jameson
 */

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "cards.h"
#include "utils.h"

struct nodeType {
	unsigned long long int key;
	unsigned long int value;	 // if dealer's face-up card has rank i, value's i-th digit is AI's response (we let digit 0 be rightmost digit)
	struct nodeType *next;
};

typedef struct nodeType nodeType;

// Hash table for efficient lookup of the AI's responses: the keys will correspond the hand composition of the AI, and
// and the values will be the response (0 to stand, 1 to hit). The hash table is implemented as an array of linked lists.
typedef struct {
	nodeType *heads[HASH_ARRAY_SIZE];
} hashTableType;

// listInsert: insert into a linked list
nodeType* listInsert(nodeType *head, unsigned long long int key, unsigned long int value);

nodeType *listPrepend(nodeType *lst, unsigned long long int key, unsigned long int value);

int listLength (nodeType *lst);

// listLookup: given a key, finds the node containing the response (either 0 or 1)
// if no such node (should not occur), will return -1
int listLookup(nodeType *current, unsigned long long int key, int digit);

// listPrint: prints out the contents of a list for debugging
void listPrint(nodeType *head);

void listFree(nodeType *head);

// handToKey: computes the key for a given hand
unsigned long long int handToKey(handType *hand);

// handToIndex: finds the index in the hash table for a given hand
int handToIndex(handType *hand);

void hashTableInsert(hashTableType *table, handType *hand, unsigned long int response);

int hashTableLookup(hashTableType *table, handType *hand, int digit);

// hashTableInit: generates a random response table
hashTableType *hashTableInit();

void hashTableToFile(hashTableType *table, FILE *f);

void hashTableFree(hashTableType *table);

#endif /* HASHTABLE_H_ */
