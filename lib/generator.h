/*******************************************************************************
 * Author: Jack Massey
 * Email: jacknmassey@gmail.com
 * Date: 2015-07-11
 * Licence: See typer.c
 ******************************************************************************/

#include<stdint.h>

#ifndef GENERATOR_H
#define GENERATOR_H

/* =============================== Data structs ============================= */

/* Defines the keys and weights for a signle finger num defines the number of
 * chracters on the key and total defines the total of the weights to save
 * calculating it every time.
 */
struct finger {
        char * keys;
        uint8_t * weights;
        uint8_t total;
        uint8_t num;
};

/* Defines the fingers that are used and the weights for each finger.
 * num is the number of fingers and total is the total of the weights.
 */
struct hands {
        struct finger * fingers;
        uint8_t * weights;
        uint8_t total;
        uint8_t num;
};


/* ================================ Functions =============================== */

int setup_hands(char *, struct hands *);
int generator(char *, struct hands *);

#endif
