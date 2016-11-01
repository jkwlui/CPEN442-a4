/*
   CPEN442 Assignment 4 Problem 4 App patcher
   @author: Jonathan Lui 73003097

   Part of this program uses the ida_patcher C program created by Chris
   Eagle to apply a IDA .dif patch file to the target program, adapted
   to specifically change the password of the program into the user's
   choice.
*/

/*
   Source for ida_patcher
   Copyright (c) 2006 Chris Eagle cseagle at gmail.com

   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
   Software Foundation; either version 2 of the License, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
   more details.

   You should have received a copy of the GNU General Public License along with
   this program; if not, write to the Free Software Foundation, Inc., 59 Temple
   Place, Suite 330, Boston, MA 02111-1307 USA
   */

/*
 * This program reads an Ida Pro generated .dif file and applies
 * the differences to the original binary to create a patched binary
 * The program will infer the name of the binary from the content of
 * the dif file, or take the name as a command line option.
 * CAUTION: The program transforms the input binary so if you wish
 * to preserve the original binary make sure you make a copy and
 * transform the copy.
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sha1.h"
void usage(char **argv);

int main(int argc, char **argv) {
	char password[21];
	int password_len;
    BYTE hash[20];
	FILE *input = NULL;
    SHA1_CTX ctx;

	int i;

    memset(password, '\0', sizeof(password));
	for (i = 1; i < argc; i += 2) {
		if (!strcmp(argv[i], "-p")) {
			if ((i + 1) < argc) {
				password_len = strlen(argv[i+1]);
				if (password_len > 20) {
					fprintf(stderr, "Password too big (max: 20 characters)\n");
					exit(0);
				}
                printf("Password input: %s\n", argv[i+1]);
                strcpy(password, argv[i+1]);
			} else {
				usage(argv);
			}
		}
		else if (!strcmp(argv[i], "-i")) {
			if ((i + 1) < argc) {
				fprintf(stderr, "Opening %s\n", argv[i+1]);
				input = fopen(argv[i+1], "rb+");
				if (input == NULL) {
					fprintf(stderr, "Failed to open input file %s\n", argv[i+1]);
					exit(0);
				}
			}
		}
		else {
			usage(argv);
		}
	}

	if (password_len <= 0)
		usage(argv);
	if (!input)
		usage(argv);

    // Compute the SHA-1 hash
    sha1_init(&ctx);
    sha1_update(&ctx, password, password_len);
    sha1_final(&ctx, hash);

    printf("Computing SHA-1 hash for supplied password..done!\n");
    printf("SHA1(%s): ", password);
    for (i=0; i<20; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");

    // Start replacing the bytes in the program:
    unsigned int start = 0x1dfef;
    for (i=0; i<20; i++) {
        fseek(input, start+i, SEEK_SET);
        fputc(hash[i], input);
    }
	fclose(input);
    printf("Finished patching!\n");

    return 0;
}

void usage(char** argv) {
	fprintf(stderr, "usage:\n\t%s -i <binary> -p <password>\n", argv[0]);
	exit(0);
}
