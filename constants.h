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
#define NUM_SHUFFLE 200

#define MIN_HAND_SIZE 2
#define MAX_HAND_SIZE 12

#define HASH_ARRAY_SIZE 22	// hash by the possible sums (before busting)

#define POOL_SIZE 200
#define NUM_GENERATIONS 1000
#define NUM_GAMES_PER_EVALUATION 100


#endif /* CONSTANTS_H_ */
