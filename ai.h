/*
 * ai.h
 *
 *  Created on: Jan 17, 2016
 *      Author: Jameson
 */

#ifndef AI_H_
#define AI_H_

#include "constants.h"
#include "cards.h"
#include "hashtable.h"

typedef struct {
	handType hand;
	int fitness;	// measure of how good the AI is
	hashTableType responses;
} aiType;

void aiInit (aiType *ai);

void calculateFitness (aiType *ai);

#endif /* AI_H_ */
