/*
 * cards.h
 *
 *  Created on: Jan 17, 2016
 *      Author: Jameson
 */


#ifndef CARDS_H_
#define CARDS_H_

#include "constants.h"
#include <stdio.h>

typedef struct {
	int rank, suit;
} cardType;

typedef struct {
	int sum, handSize;	// key used to lookup the hand in the hash table
	cardType *cards[MAX_HAND_SIZE];	// array that points to cards
} handType;

void createDeck (cardType **deck);

void shuffleDeck (cardType **deck);

void freeDeck (cardType **deck);

void printCards (cardType **cards, int numCards);

void handInsert (handType *hand, cardType *card);

void handRemove (handType *hand, cardType *card);

int handFindSum (handType *hand);

#endif /* CARDS_H_ */
