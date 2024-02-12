#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CODEWORD_LEN 6 // N
#define MESSAGE_LEN 3  // K
#define NUM_CHECKS 3   // M


// Function to encode the message
int* encode(int message[MESSAGE_LEN], int generator[MESSAGE_LEN][CODEWORD_LEN], int codeword[CODEWORD_LEN])
{
    for(int k = 0; k < MESSAGE_LEN; k++)
    {
        if(message[k] == 1)
        {
            for(int n = 0; n < CODEWORD_LEN; n++)
            {
                codeword[n] ^= generator[k][n];
            }
        }
    }
    return codeword;
}

int main()
{
    // int H[3][7] = {{1, 0, 1, 0, 1, 0, 1},
    //                {0, 1, 1, 0, 0, 1, 1},
    //                {0, 0, 0, 1, 1, 1, 1}};

    int G[MESSAGE_LEN][CODEWORD_LEN] = {{1, 0, 0, 1, 0, 1},
                                        {0, 1, 0, 1, 1, 1},
                                        {0, 0, 1, 0, 1, 1}};

    int message[8][MESSAGE_LEN] = {{0, 0, 0},
                                   {0, 0, 1},
                                   {0, 1, 0},
                                   {0, 1, 1},
                                   {1, 0, 0},
                                   {1, 0, 1},
                                   {1, 1, 0},
                                   {1, 1, 1}};
    int *codeword;

    codeword = (int*)calloc(CODEWORD_LEN, sizeof(int));

    for (int j = 0; j < 8; j++)
    {
        memset(codeword, 0, CODEWORD_LEN * sizeof(int));
        codeword = encode(message[j], G, codeword);

        printf("[ ");

        for(int i = 0; i < MESSAGE_LEN; i++)
            printf("%d ", message[j][i]);
        
        printf("] = [ ");

        for (int i = 0; i < CODEWORD_LEN; i++)
            printf("%d ", codeword[i]);
        
        printf("]\n");
    
    }

    return 0;
}