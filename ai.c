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
aiType *aiInit() {
	aiType *ai = malloc(sizeof(aiType));
	ai->hand = handInit();
	ai->responses = hashTableInit();
	return ai;
}

void aiFree(aiType *ai) {
	hashTableFree(ai->responses);
	handFree(ai->hand);
	free(ai);
}

// aiFindResponse: finds the response of the AI, and stores it in the hit field
static void aiFindResponse(aiType *ai, cardType *dealerCard) {
	ai->hand->hit = hashTableLookup(ai->responses, ai->hand, dealerCard->rank);
	if (ai->hand->hit == -1)
		printf("aiFindResponse: No response from lookup\n");
}

// calculateFitness: find the fitness of a given AI by seeing how many games it wins
static void aiCalculateFitness(aiType *ai) {
	handType *dealerHand = handInit();
	cardType *deck = deckInit();

	int deckIndex = 0;
	int gamesWon = 0;

	for (int i = 0; i < NUM_GAMES_PER_EVALUATION; i++) {
		deckShuffle(deck);

		ai->hand->handSize = 0;
		dealerHand->handSize = 0;
		dealerHand->hit = 1;
		deckIndex = 0;

		for (int i = 0; i < MIN_HAND_SIZE; i++) {
			handInsert(ai->hand, deck + deckIndex++);
			handInsert(dealerHand, deck + deckIndex++);
		}

		aiFindResponse(ai, dealerHand->cards); // we suppose the first card in dealer's hand is face up
		handFindSum(ai->hand);
		handFindSum(dealerHand);

		while (ai->hand->sum <= 21 && dealerHand->sum <= 21 && (ai->hand->hit || dealerHand->hit)) {
			if (ai->hand->hit) {
				handInsert(ai->hand, deck + deckIndex++);
				handFindSum(ai->hand);

				if (ai->hand->sum <= 21)
					aiFindResponse(ai, dealerHand->cards);
			}

			if (dealerHand->hit) {
				handInsert(dealerHand, deck + deckIndex++);
				handFindSum(dealerHand);

				if (dealerHand->sum >= DEALER_MIN_SUM)
					dealerHand->hit = 0;
			}
		}

		if (ai->hand->sum <= 21 && (ai->hand->sum > dealerHand->sum || dealerHand->sum > 21))
			gamesWon++;
	}

	ai->fitness = gamesWon;

	deckFree(deck);
	handFree(dealerHand);
}

// aiMutate: randomly mutates the response table of an AI
static void aiMutate (aiType *ai) {
	for (int i = 0; i < HASH_ARRAY_SIZE; i++) {
		nodeType *cur = ai->responses->heads[i];

		while (cur) {
			if (randInt(1, 100) <= MUTATION_CHANCE_1) {
				unsigned long int response = 0;
				for (int i = 0; i < NUM_APPRECIABLE_RANKS; i++) {
					if (randInt(1, 100) <= MUTATION_CHANCE_2)
						response = response * 10 + randInt(0,1);
					else
						response = response * 10 + getDigit(cur->value, NUM_APPRECIABLE_RANKS - i - 1);
				}
				cur->value = response;
			}
			cur = cur->next;
		}
	}
}

// aiMate: creates a new AI by joining two existing ones
static aiType *aiMate (aiType *a1, aiType *a2) {
	aiType *aiNew = malloc(sizeof(aiType));
	aiNew->hand = handInit();
	aiNew->responses = malloc(sizeof(hashTableType));

	for (int i = 0; i < HASH_ARRAY_SIZE; i++) {
		nodeType *n1 = a1->responses->heads[i];
		nodeType *n2 = a2->responses->heads[i];
		aiNew->responses->heads[i] = NULL;

		while (n1 && n2) {
			unsigned long int response = 0;
			for (int i = 0; i < NUM_APPRECIABLE_RANKS; i++)
				response = response * 10 + getDigit(randInt(0,1)? n1->value : n2->value, NUM_APPRECIABLE_RANKS - i - 1);

			if (n1->key != n2->key) {
				printf("aiMate: different keys\n");
			}


			aiNew->responses->heads[i] = listInsert(aiNew->responses->heads[i], n1->key, response);
			n1 = n1->next;
			n2 = n2->next;
		}

		if (n1 || n2)
			printf("aiMate: lists were not of the same length\n");

	}

	return aiNew;
}

static void aiQuickSort(aiType **aiGen, int len) {
	if (len < 2)
		return;

	aiType *partition = aiGen[len/2];
	int left = 0, right = len - 1;

	while (left < right) {
		while (aiGen[left]->fitness > partition->fitness)
			left++;
		while (aiGen[right]->fitness < partition->fitness)
			right--;

		if (left < right) {
			aiType *temp = aiGen[right];
			aiGen[right] = aiGen[left];
			aiGen[left] = temp;
			left++;
			right--;
		}
	}

	aiQuickSort(aiGen, left);
	aiQuickSort(aiGen + left, len - left);
}

// aiSortByFitness: uses quick sort to sort the AI generation by decreasing fitness
static void aiSortByFitness(aiType **aiGen) {
	aiQuickSort(aiGen, POOL_SIZE);
}

// aiNewGeneration: mutates the old generation to create the next one
void aiNewGeneration(aiType **aiGen) {
	for (int i = 0; i < POOL_SIZE; i++)
		aiCalculateFitness(aiGen[i]);

	aiSortByFitness(aiGen);

	for (int i = POOL_SIZE / POOL_RATIO + 1; i < POOL_SIZE; i++) {
		aiFree(aiGen[i]);

		int parent1 = randInt(0, POOL_SIZE / POOL_RATIO - 1);
		int parent2 = randInt(parent1 + 1, POOL_SIZE / POOL_RATIO);
		aiGen[i] = aiMate(aiGen[parent1], aiGen[parent2]);
	}

	for (int i = 0; i < POOL_SIZE; i++)
		aiMutate(aiGen[i]);
}

// aiSimulate: runs the simulation, and produces the resulting AI
aiType *aiSimulate() {
	aiType **aiGen = malloc(POOL_SIZE * sizeof(aiType*));

	for (int i = 0; i < POOL_SIZE; i++)
		aiGen[i] = aiInit();

	for (int i = 0; i < NUM_GENERATIONS; i++) {
		aiNewGeneration(aiGen);
		printf("Simulating generation %i... best fitness: %i\n", i + 1, aiGen[0]->fitness);
	}

	aiType *res = aiGen[0];

	for (int i = 1; i < POOL_SIZE; i++)
		aiFree(aiGen[i]);

	free(aiGen);
	return res;
}

static void printHandFromKey(unsigned long long int key, FILE *f) {
	static const int base = 11;
	if (key) {
		printHandFromKey(key / base, f);
		fprintf(f, "%i ", (int) (key % base));
	}
}

void aiToFile(aiType *ai, FILE *f) {
	for (int i = 0; i < HASH_ARRAY_SIZE; i++) {
		nodeType *cur = ai->responses->heads[i];

		while (cur) {
			unsigned long int value = cur->value;

			fprintf(f, "Hand: ");
			printHandFromKey(cur->key, f);

			fprintf(f, "\n");
			for (int i = 0; i < NUM_APPRECIABLE_RANKS; i++) {
				fprintf(f, "Dealer's card: %2i %15s %5s\n", i + 1, "Response: ", (value % 2)? "hit":"pass");
				value /= 2;
			}
			fprintf(f, "\n");

			cur = cur->next;
		}

	}
}

