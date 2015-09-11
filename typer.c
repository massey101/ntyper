#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#define NUM_KEYS 32


struct finger {
        char * keys;
        uint8_t * weights;
        uint8_t total;
        uint8_t num;
};


struct hands {
        struct finger * fingers;
        uint8_t * weights;
        uint8_t total;
        uint8_t num;
};

char * skip_white(char * buf)
{
        while (buf[0] == ' ' || buf[0] == '\t') {
                buf++;
        }
        return buf;
}

int populate_int(char * str, uint8_t * data) {
        uint16_t i = 0;
        char * end;
        while (*str != '\0') {
                data[i] = strtol(str, &end, 10);
                if (str == end) {
                        return -1;
                }
                str = skip_white(end);
                i++;
        }
        return i;
}

int populate_char(char * str, char * data) {
        uint16_t i = 0;
        while (*str != '\0') {
                data[i] = *str;
                str = skip_white(str+1);
                i++;
        }
        return i;
}

int init_hands(struct hands * h, uint8_t num_fingers) {
        h->num = num_fingers;
        h->total = 0;
        h->fingers = calloc(num_fingers, sizeof(struct finger));
        h->weights = calloc(num_fingers, sizeof(uint8_t));
}

int process(char * buffer, struct hands * h, uint8_t num) {
        char * end, * str = buffer;
        uint8_t temp, i, sum = 0;

        str = skip_white(str);
        if (num == 0) {
                temp = strtol(str, &end, 10);
                if (str == end) {
                        return -1;
                }
                init_hands(h, temp);
                return 0;
        }
        switch((num-1)%3) {
        case 0:
                h->weights[(num-1)/3] = strtol(str, &end, 10);
                if (str == end) {
                        return -1;
                }
                break;
        case 1:
                h->fingers[(num-1)/3].keys = calloc(NUM_KEYS, sizeof(char));
                h->fingers[(num-1)/3].num = populate_char(str, h->fingers[(num-1)/3].keys);
                if (h->fingers[(num-1)/3].num < 0) {
                        return -1;
                }
                break;
        case 2:
                h->fingers[(num-1)/3].weights = calloc(NUM_KEYS, sizeof(uint8_t));
                temp = populate_int(str, h->fingers[(num-1)/3].weights);
                if (temp < 0 || temp != h->fingers[(num-1)/3].num) {
                        return -1;
                }
                for (i = 0; i < temp; i++) {
                        sum += h->fingers[(num-1)/3].weights[i];
                }
                h->fingers[(num-1)/3].total = sum;
                break;
        default:
                break;
        }
        return 0;
}

int setup_hands(struct hands * h) {
        FILE * f = fopen("example.txt", "r");
        char buffer[1000];
        uint8_t i, num = 0;

        if (f == NULL) {
                return -1;
        }

        while(fgets(buffer, 1000, f) != NULL) {
                buffer[strcspn(buffer, "\n")] = '\0';
                if (buffer[0] == '\0') {
                        continue;
                }
                if (process(buffer, h, num) < 0) {
                       return -1;
                }
                num++;
        }

        for (i = 0; i < h->num; i++) {
                h->total += h->weights[i];
        }

        return 0;
}




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
void generator(char * buffer, struct hands * h)
{
        int16_t i;
        uint8_t finger = -1, key, num;
        // Chose finger
        finger = weighted_rand(h->weights, h->total);

        // Choose a number of keys
        num = rand() % 4 + 2;

        // Choose some keys
        for (i = 0; i < num; i++) {
                key = weighted_rand(h->fingers[finger].weights,
                                    h->fingers[finger].total);
                buffer[i] = h->fingers[finger].keys[key];
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
        struct hands data;
        char buffer[6];
        uint16_t i;
        setup_hands(&data);

        /* do 11 tests. */
        for (i = 0; i < 11; i++) {
                generator(buffer, &data);
                printf("%s ", buffer);
        }
        printf("\n");

        return 0;
}
