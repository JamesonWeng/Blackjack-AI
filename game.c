// Creation of AI through genetic algorithm for a single-deck version of blackjack
// Things to add later: consideration of dealer's face-up card, consideration of remaining deck composition

#include "hashtable.h"
#include "ai.h"

int main () {
	srand((unsigned int)time(NULL));

	FILE *f = fopen ("hashtable.txt", "w");
	if (f == NULL) {
		return 5;
	}

	aiType ai;

	aiInit (&ai);

	hashTableToFile (&(ai.responses), f);
	calculateFitness (&ai);

	hashTableFree (&ai.responses);

	return 0;
}


