/*
 * ai.c
 *
 *  Created on: Jan 17, 2016
 *      Author: Jameson
 */

#include "ai.h"

/********************************************
 	 FUNCTIONS FOR THE AI
 ********************************************/

// aiInit: initializes the AI
void aiInit (aiType *ai) {
	hashTableInit (&(ai->responses));
}

static void aiFindResponse (aiType *ai) {
	ai->hand.hit = hashTableLookup (&(ai->responses), &(ai->hand));
	if (ai->hand.hit == -1) {
		printf ("No response from lookup\n");
	}
}

// calculateFitness: find the fitness of a given AI by seeing how many games it wins
void calculateFitness (aiType *ai) {
	handType dealerHand;
	cardType *deck[DECK_SIZE];
	int deckIndex;
	int gamesWon = 0;

	deckCreate (deck);

	for (int i = 0; i < NUM_GAMES_PER_EVALUATION; i++) {
		deckShuffle (deck);
		ai->hand.handSize = 0;
		dealerHand.handSize = 0;
		dealerHand.hit = 1;
		deckIndex = 0;

		for (int i = 0; i < MIN_HAND_SIZE; i++) {
			handInsert (&(ai->hand), deck[deckIndex++]);
		}
		for (int i = 0; i < MIN_HAND_SIZE; i++) {
			handInsert (&dealerHand, deck[deckIndex++]);
		}

		aiFindResponse (ai);
		handFindSum(&(ai->hand));
		handFindSum(&dealerHand);

		while (ai->hand.sum <= 21 && dealerHand.sum <= 21 && (ai->hand.hit || dealerHand.hit)) {
			if (ai->hand.hit) {
				handInsert (&(ai->hand), deck[deckIndex++]);
				handFindSum (&(ai->hand));
				if (ai->hand.sum <= 21) {
					aiFindResponse(ai);
				}
			}

			if (dealerHand.hit) {
				handInsert (&dealerHand, deck[deckIndex++]);
				handFindSum (&dealerHand);
				if (dealerHand.sum >= DEALER_MIN_SUM) {
					dealerHand.hit = 0;
				}
			}
		}

		if (ai->hand.sum <= 21 && (ai->hand.sum > dealerHand.sum || dealerHand.sum > 21)) {
			gamesWon++;
		}
	}

	freeCards (deck, DECK_SIZE);

	printf ("Games won: %i\n", gamesWon);
	ai->fitness = gamesWon;
}

void aiMutate (aiType *ai) {
	if (randInt(1, 100) >= MUTATION_CHANCE) {

	}
}

void aiMate (aiType *ai1, aiType *ai2, aiType *aiNew) {
	for (int i = 0; i < HASH_ARRAY_SIZE; i++) {

	}
}

