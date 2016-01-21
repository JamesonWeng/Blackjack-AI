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
	hashTableInit (&(ai->response));
}

// calculateFitness: find the fitness of a given AI by seeing how many games it wins
int calculateFitness (aiType *ai) {
	handType dealerHand;
	cardType *deck[DECK_SIZE];
	int deckIndex;
	int gamesWon = 0;

	createDeck (deck);

	for (int i = 0; i < NUM_GAMES_PER_EVALUATION; i++) {
		shuffleDeck (deck);
		printCards (deck, DECK_SIZE);
		ai->hand.handSize = 0;
		dealerHand.handSize = 0;
		dealerHand.hit = 1;
		deckIndex = 0;

		for (int i = 0; i < MIN_HAND_SIZE; i++) {
			printf ("DeckIndex: %d\n", deckIndex);
			handInsert (&(ai->hand), deck[deckIndex++]);

		}
		for (int i = 0; i < MIN_HAND_SIZE; i++) {
			printf ("DeckIndex: %d\n", deckIndex);
			handInsert (&dealerHand, deck[deckIndex++]);
		}

		printCards (ai->hand.cards, ai->hand.handSize);
		printCards (dealerHand.cards, dealerHand.handSize);

		while (ai->hand.hit || dealerHand.hit) {
			if (ai->hand.hit) {
				printf ("AI hits\n");
				handInsert (&(ai->hand), deck[deckIndex++]);
				//printCards (ai->hand.cards, ai->hand.handSize);
			}
		}


		break;
	}
}
