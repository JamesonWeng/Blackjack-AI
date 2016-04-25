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

cardType *deckInit() {
	cardType *deck = malloc(DECK_SIZE * sizeof(cardType));
	for (int i = 0; i < NUM_RANKS; i++) {
		for (int j = 0; j < NUM_SUITS; j++) {
			deck[i * NUM_SUITS + j].rank = i;
			deck[i * NUM_SUITS + j].suit = j;
		}
	}
	return deck;
}

void deckShuffle(cardType *deck) {
	for (int i = 0; i < DECK_SIZE; i++) {
		cardType temp = deck[i];
		int swapIndex = randInt(i, DECK_SIZE - 1);

		temp = deck[i];
		deck[i] = deck[swapIndex];
		deck[swapIndex] = temp;
	}
}

void deckFree (cardType *deck) {
	free(deck);
}


// printCards: writes the contents of an array of card pointers to standard output - for debugging purposes
void printCards(cardType *cards, int numCards) {
	for (int i = 0; i < numCards; i++) {
		printf ("%i %i\n", cards[i].rank, cards[i].suit);
	}
}

/********************************************
 	 FUNCTIONS FOR A CARD HAND
 ********************************************/

handType *handInit() {
	handType *hand = malloc(sizeof(handType));
	hand->handSize = 0;
	hand->sum = 0;
	hand->hit = 0;
	hand->cards = malloc(MAX_HAND_SIZE * sizeof(cardType));
	return hand;
}

// handInsert: inserts a card into the hand, and increases handSize by 1
void handInsert (handType *hand, cardType *card) {
	int place = 0;

	// cards in hand are sorted in increasing order by rank
	while (place < hand->handSize && hand->cards[place].rank <= card->rank) {
		place += 1;
	}

	for (int i = hand->handSize; i > place; i--) {
		cardType temp = hand->cards[i - 1];

		hand->cards[i - 1] = hand->cards[i];
		hand->cards[i] = temp;
	}

	hand->cards[place] = *card;
	hand->handSize += 1;
}

// handRemove: removes a card from the hand
void handRemove (handType *hand, cardType *card) {
	int place = 0;
	while (place < hand->handSize && (hand->cards[place].rank != card->rank || hand->cards[place].suit != card->suit)) {
		place += 1;
	}

	if (place < hand->handSize) {
		hand->handSize -= 1;
		for (int i = place; i < hand->handSize; i++) {
			hand->cards[i] = hand->cards[i+1];
		}
	}
}


// handFindSum: tries to find the largest sum under 21 for a given hand (if bust, returned sum is over 21)
void handFindSum (handType *hand) {
	int numAces = 0;
	hand->sum = 0;

	// find the sum of the hand assuming the aces are 1
	for (int i = 0; i < hand->handSize; i++) {
		if (hand->cards[i].rank == 0) {
			numAces += 1;
		}

		if (hand->cards[i].rank >= 0 && hand->cards[i].rank <= 9) {
			hand->sum += hand->cards[i].rank + 1;
		}
		else {
			hand->sum += 10;
		}
	}

	// find the best possible sum (without busting) by changing the worth of the aces
	while (numAces > 0  && hand->sum <= 11) {
		hand->sum += 10;
		numAces -= 1;
	}
}

void handFree(handType *hand) {
	free(hand->cards);
	free(hand);
}


