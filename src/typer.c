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

#include <stdio.h>
#include <stdint.h>
#include <ncurses.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "generator.h"

#define NUM_OF_TESTS 10
#define DEFAULT_FILE "example.txt"

char * parse_cli(int argc, char ** argv)
{
        char * filename;
        if (argc == 1) {
                filename = malloc(strlen(DEFAULT_FILE)+1);
                strcpy(filename, DEFAULT_FILE);
        } else if (argc == 2) {
                filename = malloc(strlen(argv[1])+1);
                strcpy(filename, argv[1]);
        } else {
                return NULL;
        }

        return filename;
}

/* Populates a line of randomly generated characters and gets the user to
 * reproduce them.
 * Arguments: data - Data used to generate characters
 */
void run(struct hands * data)
{
        char buffer[MAX_LEN*NUM_OF_TESTS+1], ch;
        uint16_t i, j = 0;

        for (i = 0; i < NUM_OF_TESTS-1; i++) {
                j += generator(buffer + j, data);
                buffer[j] = ' ';
                j++;
        }
        j += generator(buffer + j, data);
        buffer[j] = '\0';
        printw("%s", buffer);
        move(1, 0);
        refresh();

        i = 0;
        while (i < j) {
                ch = getch();
                if (ch == buffer[i]) {
                        addch(ch);
                        i++;
                }
        }
}

/* Main function for ntyper. Sets up the ncurses window, sets up the hand data
 * and then repeatedly runs the task for the user.
 */
int main(int argc, char ** argv)
{
        struct hands data;
        char * filename;

        srand(time(NULL));
        if ((filename = parse_cli(argc, argv)) == NULL) {
                printf("Usage: ntyper [filename]\n");
                return 1;
        }

        if (setup_hands(filename, &data)) {
                printf("Reading error!\n");
                return 1;
        }

        initscr();
        cbreak();
        noecho();

        while(1) {
                run(&data);
                clear();
        }

        endwin();

        return 0;
}
