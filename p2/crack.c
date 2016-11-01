#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h> // C OpenMP for parallelisation<]
#include "sha1.h"

#define SALT "BC"
#define NUM_CHARSET 76
#define NUM_CHARACTERS 6

int main()
{
    // 85fefe6664c11b1e9b38559d420de7de957bb562
    BYTE hash[SHA1_BLOCK_SIZE] = {0x85,0xfe,0xfe,0x66,0x64,0xc1,0x1b,0x1e,0x9b,
                                  0x38,0x55,0x9d,0x42,0x0d,0xe7,0xde,0x95,0x7b,
                                  0xb5,0x62};

    /*BYTE hash[SHA1_BLOCK_SIZE] = {*/
        /*//dce9e9ff15bebf9591461b72e91090328a4527d8*/
        /*0xdc, 0xe9, 0xe9, 0xff, 0x15, 0xbe, 0xbf, 0x95, 0x91, 0x46, 0x1b,*/
        /*0x72, 0xe9, 0x10, 0x90, 0x32, 0x8a, 0x45, 0x27, 0xd8*/
    /*};*/

    char charset[NUM_CHARSET] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()_+-=";
    int done = 0;
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;
    int m = 0;
    int n = 0;
#pragma omp parallel for shared(done) collapse(6)
        for ( i=0; i < NUM_CHARSET; i++) {
            for ( j=0;j < NUM_CHARSET; j++) {
                for ( k=0; k < NUM_CHARSET; k++) {
                    for ( l=0; l < NUM_CHARSET; l++) {
                        for ( m=0; m < NUM_CHARSET; m++) {
                            for ( n=0; n < NUM_CHARSET; n++) {
                                BYTE text[NUM_CHARACTERS+3];
                                text[0] = 'B';
                                text[1] = 'C';
                                text[2] = charset[i];
                                text[3] = charset[j];
                                text[4] = charset[k];
                                text[5] = charset[l];
                                text[6] = charset[m];
                                text[7] = charset[n];
                                text[8] = '\0';

                                BYTE buf[SHA1_BLOCK_SIZE];
                                SHA1_CTX ctx;
                                sha1_init(&ctx);
                                sha1_update(&ctx, text, 8);
                                sha1_final(&ctx, buf);
                                if (!memcmp(hash, buf, SHA1_BLOCK_SIZE)) {
                                    printf("hash found!\n");
                                    printf("%s\n", text);
                                    done = 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    return 0;
}
