/*
 * utils.c
 *
 *  Created on: Jan 17, 2016
 *      Author: Jameson
 */

// randInt: returns a random integer between min and max inclusive
int randInt(int min, int max) {
	return rand() % (max - min + 1) + min;
}



