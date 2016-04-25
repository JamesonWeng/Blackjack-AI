// Creation of AI through genetic algorithm for a single-deck version of blackjack
// Things to add later: consideration of dealer's face-up card, consideration of remaining deck composition

#include "hashtable.h"
#include "ai.h"
#include "constants.h"

int main() {
	srand((unsigned int) time(NULL));
	FILE *f = fopen("aiTable.txt", "w");

	aiType *ai = aiSimulate();

	aiToFile(ai, f);

	aiFree(ai);
	fclose(f);
	return 0;
}


