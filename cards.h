/*
 * cards.h
 *
 *  Created on: Jan 17, 2016
 *      Author: Jameson
 */


#ifndef CARDS_H_
#define CARDS_H_

#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "utils.h"

typedef enum {
	heart = 0,
	club = 1,
	diamond = 2,
	spade = 3,
} suitType;

typedef struct {
	int rank;
	suitType suit;
} cardType;

typedef struct {
	int sum, handSize, hit;	// key used to lookup the hand in the hash table
	cardType *cards[MAX_HAND_SIZE];	// array that points to cards
} handType;

void deckCreate (cardType **deck);

void deckShuffle (cardType **deck);

void freeCards (cardType **cards, int numCards);

void printCards (cardType **cards, int numCards);

void handInsert (handType *hand, cardType *card);

void handRemove (handType *hand, cardType *card);

void handFindSum (handType *hand);

#endif /* CARDS_H_ */
