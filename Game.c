// creation of AI through genetic algorithm for a simple single-deck version of blackjack:

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define NUM_RANKS 13
#define NUM_SUITS 4
#define DECK_SIZE NUM_RANKS * NUM_SUITS
#define INITIAL_HAND_SIZE 2
#define MAX_HAND_SIZE 12 // for a single deck, it's impossible to go over 12 cards without busting
#define NUM_SHUFFLE 200
#define NUM_GENERATIONS 1000 // number of times to repeat the "natural selection" for the genetic algorithm
#define NUM_GAMES_PER_EVALUATION 100

typedef struct {
	int rank, suit;
} cardType;

typedef struct {
	int handSize; // current hand size
	cardType *cards[MAX_HAND_SIZE]; // points to cards in the deck
} handType;

// hash table for efficient lookup of the AI responses
// the keys will correspond the hand composition of the AI
// the values will be the response (0 to stand, 1 to hit)



typedef struct {
	handType hand;
	int fitness;
	int response[];
} AIType;


int randInt (int min, int max) {
	return rand() % (max - min + 1) + min;
}

/****************************
 ********************************************
 FUNCTIONS FOR THE CARD HAND
 ********************************************
 ****************************/

// inserts a card into an array of cards sorted in increasing order by rank
void insert (cardType *card, cardType **cards, int handSize) {
	int place = 0;
	while (cards[place]->rank < card->rank && place < handSize) {
		place += 1;
	}

	for (int i = handSize + 1; i > place; i--) {
		cards[i] = cards[i-1];
	}

	cards[place] = card;
}

// finds the largest sum under 21, it it's possible, for a given hand
int bestSum (cardType **cards, int handSize) {
	int numAces = 0;
	int sum = 0;

	// find the sum of the hand assuming the aces are 1
	for (int i = 0; i < handSize; i++) {
		if (cards[handSize]->rank == 0) {
			numAces += 1;
		}
		if (cards[handSize]->rank >= 0 && cards[handSize]->rank <= 9) {
			sum += cards[handSize]->rank + 1;
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

/****************************
 ********************************************
 FUNCTIONS FOR THE AI
 ********************************************
 ****************************/

int calculateFitness (AIType *AI) {
	handType dealer;
	for (int i = 0; i < NUM_GAMES_PER_EVALUATION; i++) {



	}
}


/****************************
 ********************************************
 FUNCTIONS FOR THE DECK
 ********************************************
 ****************************/

void createDeck (cardType **deck) {
	for (int i = 0; i < NUM_RANKS; i++) {
		for (int j = 0; j < NUM_SUITS; j++) {
			deck[i*NUM_SUITS + j] = malloc (sizeof (cardType));
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

// for debugging purposes
void printDeck (cardType **deck) {
	for (int i = 0; i < DECK_SIZE; i++) {
		printf ("%i %i\n", deck[i]->rank, deck[i]->suit);
	}
}

int main () {
	cardType *deck[DECK_SIZE]; // make the deck to be an array of pointers so shuffling is more efficient
	int deckIndex = 0;
	handType player, dealer;

	srand((unsigned int)time(NULL));

	createDeck(deck);
	shuffleDeck(deck);
	//printDeck(deck);

	for (int i = 0; i < INITIAL_HAND_SIZE; i++) {
		player.cards[i] = deck[deckIndex++];
	}
	for (int i = 0; i < INITIAL_HAND_SIZE; i++) {
		dealer.cards[i] = deck[deckIndex++];
	}



}


