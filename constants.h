/*
 * constants.h
 *
 *  Created on: Jan 17, 2016
 *      Author: Jameson
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#define NUM_RANKS 13
#define NUM_APPRECIABLE_RANKS 10 // 10 to king have the same value
#define NUM_SUITS 4
#define DECK_SIZE NUM_RANKS * NUM_SUITS

#define MIN_HAND_SIZE 2
#define MAX_HAND_SIZE 12 // at 12 cards, any hand is definitely a bust
#define DEALER_MIN_SUM 17 // the dealer must keep hitting until he has a sum of at least 17

#define HASH_ARRAY_SIZE 23

#define POOL_SIZE 200 // make at least 10
#define POOL_RATIO 4 // reciprocal of ratio of pool kept each generation (e.g. if 4, only top 1/4 of pool kept)
#define NUM_GENERATIONS 1000
#define NUM_GAMES_PER_EVALUATION 800

#define MUTATION_CHANCE_1 1
#define MUTATION_CHANCE_2 10

#endif /* CONSTANTS_H_ */
