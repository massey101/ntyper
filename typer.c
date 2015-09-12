/*******************************************************************************
 * Author: Jack Massey
 * Email: jacknmassey@gmail.com
 * Date: 2015-07-11
 *
 * Licence:
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/


#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<string.h>

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

/* Helper function to skip whitespace characters
 * Arguments: buf - string to skip with
 * Returns: New position in string
 */
char * skip_white(char * buf)
{
        while (buf[0] == ' ' || buf[0] == '\t') {
                buf++;
        }
        return buf;
}

/* Populates a list of intergers from a space seperated string.
 * Note: May be seperated by any number of spaces.
 * Arguments: str - Space seperated string of intergers
 *            data - array to store in of at least size num.
              num - number of characters to copy.
 * Returns: The number copied on success or -1 on failure.
 */
int populate_int(char * str, uint8_t * data, uint8_t num) {
        uint16_t i = 0;
        char * end;
        long temp;
        while (*str != '\0') {
                if (i >= num) {
                        break;
                }
                temp = strtol(str, &end, 10);
                if (str == end || temp > 255 || temp < 0) {
                        return -1;
                }
                data[i] = temp;
                str = skip_white(end);
                i++;
        }
        return i;
}

/* Populates a list of characters from a space seperated string.
 * Note: May be seperated by any number of spaces.
 * Arguments: str - space seperated string of characters.
 *            data - array to store in of at least size num.
              num - number of characters to copy.
 * Returns: The number copied on success or -1 on failure.
 */
int populate_char(char * str, char * data, uint8_t num) {
        uint16_t i = 0;
        while (*str != '\0') {
                if (i >= num) {
                        break;
                }
                data[i] = *str;
                str = skip_white(str+1);
                i++;
        }
        return i;
}

/* Initialise the hands struct with a specified number of fingers.
 * Arguments: h - struct to init
 *            num_fingers - the number of fingers (duh)
 * Returns: 0 for success. (Currently always returns 0)
 */
int init_hands(struct hands * h, uint8_t num_fingers) {
        h->num = num_fingers;
        h->total = 0;
        h->fingers = calloc(num_fingers, sizeof(struct finger));
	if (h->fingers == NULL) {
		return -1;
	}
        h->weights = calloc(num_fingers, sizeof(uint8_t));
	if (h->weights == NULL) {
		return -1;
	}
	return 0;
}

/* Process a single line for a file in the custom format. There are 4 different
 * types of line that the function uses:
 *
 * [num]                        // type -1 specifies the number of fingers
 * [num]                        // type 0 specifies the fingers weight
 * [char] [char] [char] ...     // type 1 has a list of keys for the finger
 * [num] [num] [num] ..         // type 2 has a list of weights for each key
 *
 * types 0-2 then repeat for each finger.
 *
 * Arguments: buffer - single line to process
 *            h - data collected goes in this struct
 *            num - specifies the line number starting at -1
 * Returns: 0 for success and -1 otherwise.
 */
int process(char * buffer, struct hands * h, int num) {
        char * end, * str = buffer;
        uint8_t num_copied, num_fingers, i, sum = 0;
        long size;

        str = skip_white(str);

        if (num == -1) {
                size = strtol(str, &end, 10);
                if (str == end || size > 255 || size < 0) {
                        return -1;
                }
                if (init_hands(h, size)) {
			return -1;
		}
                return 0;
        }

        switch(num%3) {
        case 0:
                size = strtol(str, &end, 10);
                if (str == end || size > 255 || size < 0) {
                        return -1;
                }
                h->weights[num/3] = size;
                break;
        case 1:
                size = strtol(str, &end, 10);
                if (str == end || size > 255 || size < 0) {
                        return -1;
                }
                h->fingers[num/3].num = size;
                str = skip_white(end);

                h->fingers[num/3].keys = calloc(h->fingers[num/3].num, sizeof(char));
		if (h->fingers[num/3].keys == NULL) {
			return -1;
		}
                h->fingers[num/3].num = populate_char(str, h->fingers[num/3].keys,
                                h->fingers[num/3].num);
                if (h->fingers[num/3].num < 0) {
                        return -1;
                }
                break;
        case 2:
                h->fingers[num/3].weights = calloc(h->fingers[num/3].num, sizeof(uint8_t));
		if (h->fingers[num/3].weights == NULL) {
			return -1;
		}
                num_copied = populate_int(str, h->fingers[num/3].weights, h->fingers[num/3].num);
                if (num_copied != h->fingers[num/3].num) {
                        return -1;
                }
                for (i = 0; i < num_copied; i++) {
                        sum += h->fingers[num/3].weights[i];
                }
                h->fingers[num/3].total = sum;
                break;
        default:
                break;
        }
        return 0;
}

/* Setup the hands struct, loading the information from a file called
 * "example.txt".
 * Arguments: h - This empty struct will be initialised.
 * Returns: 0 for success and -1 otherwise.(This should be made WAY more
 *          detailed in the future
 */
int setup_hands(struct hands * h) {
        FILE * f = fopen("example.txt", "r");
        char buffer[1000];
        int i, num = -1;

        if (f == NULL) {
		return -1;
	}

        while(fgets(buffer, 1000, f) != NULL) {
                buffer[strcspn(buffer, "\n")] = '\0';
                if (buffer[0] == '\0') {
                        continue;
                }

                if (num > 3*h->num-1) {
                        break;
                }

                if (process(buffer, h, num) < 0) {
                       return -1;
                }
                num++;
        }
        if ((num+1)/3 != h->num) {
                return -1;
        }

        for (i = 0; i < h->num; i++) {
                h->total += h->weights[i];
        }

        return 0;
}



/* Helper function for the generator. Generates a random number with weighting
 * for specific numbers.
 * Arguments: data - Array of weights for each number.
 *            max - The sum of data (saves recalculation)
 * Returns: A random number between 0 and len(data)
 */
int weighted_rand(uint8_t * data, uint8_t max) {
        int16_t num = -1, i = rand() % max + 1;
        while (i > 0) {
                num++;
                i -= data[num];
        }
        return num;
}


/* Generates a random sequence. The sequence will all be on the same finger, of a
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
 * currently just used to test the generator and setup.
 *
 */
int main(int argc, char ** argv)
{
        struct hands data;
        char buffer[6];
        uint16_t i;
        if (setup_hands(&data)) {
		printf("Reading error!\n");
		return 1;
	}

        /* do 11 tests. */
        for (i = 0; i < 11; i++) {
                generator(buffer, &data);
                printf("%s ", buffer);
        }
        printf("\n");

        return 0;
}
