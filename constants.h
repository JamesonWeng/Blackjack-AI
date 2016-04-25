/*
 * constants.h
 *
 *  Created on: Jan 17, 2016
 *      Author: Jameson
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define NUM_RANKS 13
#define NUM_SUITS 4
#define DECK_SIZE NUM_RANKS * NUM_SUITS

#define MIN_HAND_SIZE 2
#define MAX_HAND_SIZE 12
#define DEALER_MIN_SUM 17 // the dealer must keep hitting until he has a sum of at least 17

#define HASH_ARRAY_SIZE 23

#define POOL_SIZE 500 // make at least 10
#define POOL_RATIO 4 // reciprocal of precentage of pool kept
#define NUM_GENERATIONS 1000
#define NUM_GAMES_PER_EVALUATION 500

#define MUTATION_CHANCE 1

#endif /* CONSTANTS_H_ */
