#include "encoding.h"


// Function to encode the message
void encode(int message[MESSAGE_LEN], int generator[MESSAGE_LEN][CODEWORD_LEN], int* codeword)
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
}