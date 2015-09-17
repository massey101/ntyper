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

#include "generator.h"


/* main
 * currently just used to test the generator and setup.
 *
 */
int main(int argc, char ** argv)
{
        struct hands data;
        char buffer[6];
        uint16_t i;
        if (setup_hands("example.txt", &data)) {
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
