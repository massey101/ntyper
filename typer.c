#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>


/* ============= This stuff all has to go eventually =========== */
char * groups[] = {
        "qaz1!QAZ",
        "wsx2@WSX",
        "edc3#EDC",
        "rfvtgb45$%RFVTGB",
        "yhnujm67^&YHNUJM",
        "ik,8*IK<",
        "ol.9(OL>",
        "p;/0-=)_+P:?{}[]\"'\\|"
};

uint8_t * weights[8];
uint8_t weights0[] = {10, 10, 10, 5, 3, 3, 3, 3};
uint8_t weights1[] = {10, 10, 10, 5, 3, 3, 3, 3};
uint8_t weights2[] = {10, 10, 10, 5, 3, 3, 3, 3};
uint8_t weights3[] = {10, 10, 10, 10, 10, 10, 5, 5, 3, 3, 3, 3, 3, 3, 3, 3};
uint8_t weights4[] = {10, 10, 10, 10, 10, 10, 5, 5, 3, 3, 3, 3, 3, 3, 3, 3};
uint8_t weights5[] = {10, 10, 10, 5, 3, 3, 3, 3};
uint8_t weights6[] = {10, 10, 10, 5, 10, 3, 3, 3};
uint8_t weights7[] = {10, 10, 5, 5, 8, 8, 10, 8, 8, 3, 3, 3, 8, 8, 8, 8, 5, 5, 1, 1};

uint8_t finger_weights[] = {1, 1, 1, 2, 2, 1, 1, 2};

uint8_t weights_total[] = {0, 0, 0, 0, 0, 0, 0, 0};

struct hand_data {
        char ** groups;
        uint8_t ** weights;
        uint8_t * finger_weights;
        uint8_t * weights_total;
        uint8_t finger_weight_total;
};

void setup_hand(struct hand_data * data) {
        uint8_t i, j;

        data->groups = groups;
        data->weights = weights;
        data->weights[0] = weights0;
        data->weights[1] = weights1;
        data->weights[2] = weights2;
        data->weights[3] = weights3;
        data->weights[4] = weights4;
        data->weights[5] = weights5;
        data->weights[6] = weights6;
        data->weights[7] = weights7;
        data->weights_total = weights_total;
        data->finger_weights = finger_weights;
        data->finger_weight_total = 0;
        for (i = 0; i < 8; i++) {
                for (j = 0; j < strlen(groups[i]); j++) {
                        data->weights_total[i] += weights[i][j];
                }
                data->finger_weight_total += finger_weights[i];
        }
}
/* ========================================================================= */

/* generator
 * Currently just chooses a finger. In the future it will pick a finger, a
 * number of keys and the keys themselves.
 * Arguments: buffer - not used yet
 *            data - Contains the information of keys and weights.
 * Returns int - the finger that was chosen (will be void in the future)
 */
uint8_t generator(char * buffer, struct hand_data * data)
{
        int16_t i;
        uint8_t finger = -1;
        // Chose finger
        i = rand() % data->finger_weight_total + 1;
        while (i > 0) {
                finger++;
                i -= data->finger_weights[finger];
        }
        return finger;
}

/* main
 * currently just used to test the generator.
 *
 */
int main(int argc, char ** argv)
{
        struct hand_data data;
        char * buffer;
        uint16_t test[] = {0, 0, 0, 0, 0, 0, 0, 0};
        uint16_t i;
        setup_hand(&data);

        /* do 1100 tests. We exect around 100 for each weighting score on each
         * finger */      
        for (i = 0; i < 1100; i++) {
                test[generator(buffer, &data)] += 1;
        }

        for (i = 0; i < 8; i++) {
                printf(" %03u", test[i]);
        }
        printf("\n");

        return 0;
}
