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
int calculateFitness (aiType *ai) {
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
				printf ("\nAI hits\n");
				handInsert (&(ai->hand), deck[deckIndex++]);
				handFindSum (&(ai->hand));
				if (ai->hand.sum <= 21) {
					aiFindResponse(ai);
				}

				printCards (ai->hand.cards, ai->hand.handSize);
			}

			if (dealerHand.hit) {
				printf ("\nDealer hits\n");
				handInsert (&dealerHand, deck[deckIndex++]);
				handFindSum (&dealerHand);
				if (dealerHand.sum >= DEALER_MIN_SUM) {
					dealerHand.hit = 0;
				}

				printCards (dealerHand.cards, dealerHand.handSize);
			}
		}

		printf ("\nAI Hand:\n");
		printCards(ai->hand.cards, ai->hand.handSize);
		printf ("\nDealer Hand:\n");
		printCards(dealerHand.cards, dealerHand.handSize);



		break;
	}

	freeCards (deck, DECK_SIZE);

	return gamesWon;
}
