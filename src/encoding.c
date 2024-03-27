#include "encoding.h"


// Function to encode the message
void encode(int message[MESSAGE_LEN], pchk generator, int* codeword)
{
    for(int k = 0; k < generator.n_row; k++)
    {
        if(message[k] == 1)
        {
            for(int n = 0; n < generator.n_col; n++)
            {
                codeword[n] ^= generator.A[k][n];
            }
        }
    }
}