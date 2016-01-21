// Creation of AI through genetic algorithm for a single-deck version of blackjack

#include "hashtable.h"
#include "ai.h"

int main () {
	srand((unsigned int)time(NULL));

	aiType ai;

	//aiInit (&ai);
	//calculateFitness (&ai);

	handType hand;
	hand.handSize = 10;
	for (int i = 0; i < hand.handSize; i++) {
		hand.cards[i] = malloc (sizeof (cardType));
	}
	hand.cards[0]->rank = 0;
	hand.cards[1]->rank = 0;
	hand.cards[2]->rank = 0;
	hand.cards[3]->rank = 0;
	hand.cards[4]->rank = 1;
	hand.cards[5]->rank = 1;
	hand.cards[6]->rank = 1;
	hand.cards[7]->rank = 1;
	hand.cards[8]->rank = 2;
	hand.cards[9]->rank = 4;

	hashTableType table;
	hashTableInit (&table);

	printf ("index: %d, key: %d\n", handToIndex(&hand), handToKey(&hand));
	printf ("hashTableLookup: %d\n", hashTableLookup (&table, &hand));

	return 0;
}


