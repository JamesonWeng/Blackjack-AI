/*
 * ai.h
 *
 *  Created on: Jan 17, 2016
 *      Author: Jameson
 */

#ifndef AI_H_
#define AI_H_

#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "cards.h"
#include "hashtable.h"

typedef struct {
	handType *hand;
	int fitness;	// measure of how good the AI is
	hashTableType *responses;
} aiType;

aiType *aiInit();

void aiFree(aiType *ai);

void aiNewGeneration(aiType **aiGen);

aiType *aiSimulate();

void aiToFile(aiType *ai, FILE *f);

#endif /* AI_H_ */
