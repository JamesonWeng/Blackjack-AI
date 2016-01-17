/*
 * cards.c
 *
 *  Created on: Jan 17, 2016
 *      Author: Jameson
 */

#include "cards.h"

/********************************************
 	 FUNCTIONS FOR A DECK
 ********************************************/

void createDeck (cardType **deck) {
	for (int i = 0; i < NUM_RANKS; i++) {
		for (int j = 0; j < NUM_SUITS; j++) {
			deck[i*NUM_SUITS + j] = malloc (sizeof (cardType));
			if (deck[i*NUM_SUITS + j] == NULL) {
				printf ("Memory allocation for deck failed \n");
			}
			deck[i*NUM_SUITS + j]->rank = i;
			deck[i*NUM_SUITS + j]->suit = j;
		}
	}
}

void shuffleDeck (cardType **deck) {
	int i1, i2;
	cardType *temp;
	for (int i = 0; i < NUM_SHUFFLE; i++) {
		i1 = randInt (0, DECK_SIZE - 2);
		i2 = randInt (i1 + 1, DECK_SIZE - 1);

		temp = deck[i1];
		deck[i1] = deck[i2];
		deck[i2] = temp;

	}
}

void freeDeck (cardType **deck) {
	for (int i = 0; i < DECK_SIZE; i++) {
		free (deck[i]);
	}
}

// printCards: writes the contents of an array of card pointers to standard output - for debugging purposes
void printCards (cardType **cards, int numCards) {
	for (int i = 0; i < numCards; i++) {
		printf ("%i %i\n", cards[i]->rank, cards[i]->suit);
	}
}

/********************************************
 	 FUNCTIONS FOR A CARD HAND
 ********************************************/

// handInsert: inserts a card into an array of cards sorted in increasing order by rank, and increases handSize by 1
void handInsert (handType *hand, cardType *card) {
	int place = 0;
	while (place < hand->handSize && hand->cards[place]->rank <= card->rank) {
		place += 1;
	}

	for (int i = hand->handSize; i > place; i--) {
		hand->cards[i] = hand->cards[i-1];
	}

	hand->cards[place] = card;
	hand->handSize += 1;
}

// handRemove: removes a card from the hand
void handRemove (handType *hand, cardType *card) {

	int place = 0;
	while (place < hand->handSize && hand->cards[place]->rank != card->rank) {
		place += 1;
	}

	hand->handSize -= 1;
	for (int i = place; i < hand->handSize; i++) {
		hand->cards[i] = hand->cards[i+1];
	}
}


// handFindSum: tries to find the largest sum under 21 for a given hand (if the hand is a bust, the sum will be over 21)
int handFindSum (handType *hand) {
	int numAces = 0;
	int sum = 0;

	// find the sum of the hand assuming the aces are 1
	for (int i = 0; i < hand->handSize; i++) {
		if (hand->cards[i]->rank == 0) {
			numAces += 1;
		}

		if (hand->cards[i]->rank >= 0 && hand->cards[i]->rank <= 9) {
			sum += hand->cards[i]->rank + 1;
		}
		else {
			sum += 10;
		}
	}

	// find the best possible sum (without busting) by changing the worth of the aces
	while (numAces > 0  && sum <= 11) {
		sum += 10;
		numAces -= 1;
	}

	return sum;
}

