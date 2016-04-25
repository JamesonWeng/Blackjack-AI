/*
 * hashtable.c
 *
 *  Created on: Jan 17, 2016
 *      Author: Jameson
 */

#include "hashtable.h"

/********************************************
 	 HASH TABLE FUNCTIONS
 ********************************************/

// listInsert: insert into a linked list, overwrites duplicate keys
nodeType *listInsert(nodeType *head, long long int key, int value) {
	if (!head) {
		head = malloc(sizeof(nodeType));
		head->key = key;
		head->value = value;
		head->next = NULL;
	}
	else if (head->key != key)
		head->next = listInsert (head->next, key, value);
	else
		head->value = value;

	return head;
}

nodeType *listPrepend(nodeType *lst, long long int key, int value) {
	nodeType *head = malloc(sizeof(nodeType));
	head->key = key;
	head->value = value;
	head->next = lst;
	return head;
}

int listLength (nodeType *lst) {
	int len = 0;
	while (lst) {
		len++;
		lst = lst->next;
	}
	return len;
}

// listLookup: given a key, finds the node containing the response (either 0 or 1)
// if no such node (should not occur), will return -1
int listLookup(nodeType *current, long long int key) {
	while (current) {
		if (current->key == key)
			return current->value;

		current = current->next;
	}
	return -1;
}

// listPrint: prints out the contents of a list
void listPrint(nodeType *head) {
	nodeType *current = head;
	while (current) {
		printf ("(%lli,%d) ", current->key, current->value);
		current = current->next;
	}
	printf ("\n\n");
}

// freeList: frees all the pointers in a linked list
void listFree(nodeType *head) {
	nodeType *current = head;
	nodeType *temp;
	while (current) {
		temp = current;
		current = current->next;
		free(temp);
	}
}

// handToKey: computes the key for a given hand
long long int handToKey(handType *hand) {
	long long int key = 0;

	for (int i = 0; i < hand->handSize; i++) {
		if (hand->cards[i].rank < 9)
			key = key * 10 + hand->cards[i].rank;
		else // ten to king have same value in blackjack, so we won't differentiate for the key
			key = key * 10 + 9;
	}

	return key;
}

// handToIndex: finds the index in the hash table for a given hand
int handToIndex(handType *hand) {
	return (int) (handToKey(hand) % HASH_ARRAY_SIZE);
}

void hashTableInsert(hashTableType *table, handType *hand, int response) {
	int index = handToIndex(hand);
	long long int key = handToKey(hand);
	table->heads[index] = listInsert(table->heads[index], key, response);
}

int hashTableLookup(hashTableType *table, handType *hand) {
	int index = handToIndex(hand);
	long long int key = handToKey(hand);

	return listLookup(table->heads[index], key);
}

static void hashTableInitAllKeys(hashTableType *table, handType *hand, int ranks[NUM_RANKS], int curRank) {
	hashTableInsert(table, hand, randInt(0, 1));	// choose an initially random response for the AI

	if (ranks[curRank] >= NUM_SUITS) {
		curRank += 1;
	}

	for (int i = curRank; i < NUM_RANKS; i++) {
		hand->cards[hand->handSize].rank = i;
		hand->handSize += 1;
		ranks[i] += 1;

		handFindSum(hand);
		if (hand->sum > 21) {
			hand->handSize -= 1;
			ranks[i] -=1;
			break;
		}

		hashTableInitAllKeys(table, hand, ranks, i);
		hand->handSize -= 1;
		ranks[i] -= 1;
	}
}

hashTableType *hashTableInit() {
	int *ranks = malloc(NUM_RANKS * sizeof(int)); // records number of cards of each rank in hand
	for (int i = 0; i < NUM_RANKS; i++)
		ranks[i] = 0;

	handType *hand = handInit();

	hashTableType *table = malloc(sizeof(hashTableType));
	for (int i = 0; i < HASH_ARRAY_SIZE; i++)
		table->heads[i] = NULL;

	for (int i = 0; i < NUM_RANKS; i++) {
		hand->cards[hand->handSize].rank = i; // we don't care about suit here
		hand->handSize += 1;
		ranks[i] += 1;

		for (int j = i; j < NUM_RANKS; j++) {
			hand->cards[hand->handSize].rank = j;
			hand->handSize += 1;
			ranks[j] += 1;

			hashTableInitAllKeys(table, hand, ranks, j);	 // guarantees that hand will have at least two cards

			hand->handSize -= 1;
			ranks[j] -= 1;
		}

		hand->handSize -=1;
		ranks[i] -= 1;
	}

	handFree(hand);
	free(ranks);
	return table;
}

void hashTableToFile (hashTableType *table, FILE *f) {
	nodeType *current;
	for (int i = 0; i < HASH_ARRAY_SIZE; i++) {
		current = table->heads[i];
		fprintf (f, "Index %i: ", i);

		while (current) {
			fprintf (f, "(%lli,%i) ", current->key, current->value);
			current = current->next;
		}

		fprintf (f, "\n\n");
	}
}

void hashTableFree(hashTableType *table) {
	for (int i = 0; i < HASH_ARRAY_SIZE; i++) {
		listFree(table->heads[i]);
	}
	free(table);
}

