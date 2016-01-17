// Creation of AI through genetic algorithm for a single-deck version of blackjack

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define NUM_RANKS 13
#define NUM_SUITS 4
#define DECK_SIZE NUM_RANKS * NUM_SUITS
#define NUM_SHUFFLE 200 	// number of swaps used to shuffle the deck

#define INITIAL_HAND_SIZE 2
#define MAX_HAND_SIZE 12 	// for a single deck, it's impossible to go over 12 cards in a hand without busting

#define NUM_GENERATIONS 1000 	// number of times to repeat the "natural selection" for the genetic algorithm
#define NUM_GAMES_PER_EVALUATION 100	// number of games used to evaluate fitness
#define POOL_SIZE 100	// number of members in each generation

#define HASH_ARRAY_SIZE 21	// hash by the possible sums (before busting)

typedef struct {
	int rank, suit;
} cardType;

typedef struct {
	int sum, handSize;	// key used to lookup the hand in the hash table
	cardType *cards[MAX_HAND_SIZE];	// array that points to cards
} handType;

struct nodeType {
	long long int key;
	int value;	// value is the response of the AI
	struct nodeType *next;
};
typedef struct nodeType nodeType;

// Hash table for efficient lookup of the AI's responses: the keys will correspond the hand composition of the AI, and
// and the values will be the response (0 to stand, 1 to hit). The hash table is implemented as an array of linked lists.
typedef struct {
	nodeType *heads[HASH_ARRAY_SIZE];
} hashTableType;

typedef struct {
	handType hand;
	int fitness;	// measure of how good the AI is
	hashTableType response;
} AIType;


/********************************************
 	 MISCELLANEOUS HELPING FUNCTIONS
 ********************************************/

// randInt: returns a random integer between min and max inclusive
int randInt (int min, int max) {
	return rand() % (max - min + 1) + min;
}

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
 	 FUNCTIONS FOR THE CARD HAND
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

/********************************************
 	 HASH TABLE FUNCTIONS
 ********************************************/

nodeType* listInsert (nodeType *head, long long int key, int value) {
	if (head == NULL) {
		head = malloc(sizeof(nodeType));
		if (head == NULL) {
			printf ("Memory allocation for head failed\n");
		}
		head->key = key;
		head->value = value;
		head->next = NULL;
	}
	else if (head->key != key) {
		head->next = listInsert (head->next, key, value);
	}

	return head;
}

// listPrint: prints out the contents of a list for debugging
void listPrint (nodeType *head) {
	if (head == NULL) {
		printf ("\n\n");
	}
	else {
		printf ("(%lli,%d) ", head->key, head->value);
		listPrint (head->next);
	}
}

// handToKey: computes the key for a given hand
long long int handToKey (handType *hand) {
	long long int key = 0;

	for (int i = 0; i < hand->handSize; i++) {
		if (hand->cards[i]->rank < 9) {
			key = key * 10 + hand->cards[i]->rank;
		}
		else {
			key = key * 10 + 9;	// ten, jack, queen, king have same value in blackjack, so we won't differentiate for the key
		}
	}

	return key;
}

// handToIndex: finds the index in the hash table for a given hand
int handToIndex (handType *hand) {
	int rawSum = 0;

	for (int i = 0; i < hand->handSize; i++) {
		if (hand->cards[i]->rank < 9) {
			rawSum += hand->cards[i]->rank + 1;
		}
		else {
			rawSum += 10;
		}
	}

	return rawSum;
}

void hashTableInsert (hashTableType *table, handType *hand, int response) {
	int index = handToIndex(hand);
	long long int key = handToKey(hand);
	table->heads[index] = listInsert(table->heads[index], key, response);

	printf ("hashTableInsert: index %d\n", index);
	listPrint (table->heads[index]);
}

void hashTableInitAllKeys (hashTableType *table, handType *hand, int* ranks, int curRank) {
	printf ("hashTableInitAllKeys: ");
	for (int i = 0; i < hand->handSize; i++) {
		printf ("%d ", hand->cards[i]->rank);
	}
	printf ("\n");

	hashTableInsert (table, hand, randInt(0, 1));	// choose an initially random response for the AI

	if (ranks[curRank] >= NUM_SUITS) {
		curRank += 1;
	}

	for (int i = curRank; i < NUM_RANKS; i++) {
		hand->cards[hand->handSize]->rank = i;
		hand->handSize += 1;
		ranks[i] += 1;

		if (handFindSum (hand) > 21) {
			hand->handSize -= 1;
			ranks[i] -=1;
			break;
		}

		hashTableInitAllKeys (table, hand, ranks, i);
		hand->handSize -= 1;
		ranks[i] -= 1;
	}
}

void hashTableInit (hashTableType *table) {
	printf ("hashTableInit\n");

	handType hand;
	int ranks[NUM_RANKS] = {0};

	table = malloc (sizeof (hashTableType));
	if (table == NULL) {
		printf ("Memory allocation for hash table failed\n");
	}

	for (int i = 0; i < MAX_HAND_SIZE; i++) {
		hand.cards[i] = malloc (sizeof (cardType));
		if (hand.cards[i] == NULL) {
			printf ("Memory allocation for cards in hand failed\n");
		}
	}

	hand.handSize = 0;

	for (int i = 0; i < NUM_RANKS; i++) {
		hand.cards[hand.handSize]->rank = i;
		hand.handSize += 1;
		ranks[i] += 1;

		for (int j = i; j < NUM_RANKS; j++) {
			hand.cards[hand.handSize]->rank = j;
			hand.handSize += 1;
			ranks[j] += 1;

			hashTableInitAllKeys (table, &hand, ranks, j);

			hand.handSize -= 1;
			ranks[j] -= 1;
		}

		hand.handSize -=1;
		ranks[i] -= 1;
	}
}

/********************************************
 	 FUNCTIONS FOR THE AI
 ********************************************/

// calculateFitness: find the fitness of a given AI by seeing how many games it wins
int calculateFitness (AIType *AI) {
	handType dealer;
	for (int i = 0; i < NUM_GAMES_PER_EVALUATION; i++) {


	}
}

int main () {

	//cardType *deck[DECK_SIZE]; // make the deck to be an array of pointers so shuffling is more efficient

	srand((unsigned int)time(NULL));

	//createDeck(deck);
	//shuffleDeck(deck);
	//printCards(deck, DECK_SIZE);

	/*
	handType hand;
	hand.handSize = 10;
	for (int i = 0; i < hand.handSize; i++) {
		hand.cards[i] = deck[i];
	}
	printf ("%d %d\n\n", deck[5]->rank, deck[5]->suit);
	handRemove(&hand, deck[5]);
	printCards (hand.cards, hand.handSize);
	*/

	hashTableType table;
	hashTableInit (&table);



	return 0;

}


