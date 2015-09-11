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

int weighted_rand(uint8_t * data, uint8_t max) {
        int16_t num = -1, i = rand() % max + 1;
        while (i > 0) {
                num++;
                i -= data[num];
        }
        return num;
}


/* generator
 * Generates a random sequence. The sequence will all be on the same finger, of a
 * random length between 2 and 5. and taking into account the respective weights
 * of keys and fingers.
 * Arguments: buffer - Sequence will return here. Minimum length of 6.
 *            data - Contains the information of keys and weights.
 * Returns: None
 */
void generator(char * buffer, struct hand_data * data)
{
        int16_t i;
        uint8_t finger = -1, key, num;
        // Chose finger
        finger = weighted_rand(data->finger_weights, data->finger_weight_total);

        // Choose a number of keys
        num = rand() % 4 + 2;
       
        // Choose some keys
        for (i = 0; i < num; i++) {
                key = weighted_rand(data->weights[finger],
                                    data->weights_total[finger]);
                buffer[i] = data->groups[finger][key];
        }
        buffer[i] = '\0';
        return;
}

/* main
 * currently just used to test the generator.
 *
 */
int main(int argc, char ** argv)
{
        struct hand_data data;
        char buffer[6];
        uint16_t i;
        setup_hand(&data);

        /* do 11 tests. */ 
        for (i = 0; i < 11; i++) {
                generator(buffer, &data);
                printf("%s ", buffer);
        }
        printf("\n");

        return 0;
}
