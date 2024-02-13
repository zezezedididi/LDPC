#include <time.h>
#include "encoding.h"
#include "decoding.h"
#include "display_variables.h"
#include "defs.h"

void BSC_noise(int *codeword, float p)
{
    srand(time(NULL));
    for (int i = 0; i < CODEWORD_LEN; i++)
    {
        float r = (float)rand() / RAND_MAX;
        printf("r = %f\n", r);
        if (r < p)
        {
            codeword[i] = !codeword[i];
        }
    }
}

int main()
{
    int H[NUM_CHECKS][CODEWORD_LEN] = {{1, 1, 0, 1, 0, 0},
                                       {0, 1, 1, 0, 1, 0},
                                       {1, 1, 1, 0, 0, 1}};

    // int H[NUM_CHECKS][CODEWORD_LEN] = {{1, 1, 0, 1, 0, 0},
    //                                    {0, 1, 1, 0, 1, 0},
    //                                    {1, 0, 0, 0, 1, 1},
    //                                    {0, 0, 1, 1, 0, 1}};

    int G[MESSAGE_LEN][CODEWORD_LEN] = {{1, 0, 0, 1, 0, 1},
                                        {0, 1, 0, 1, 1, 1},
                                        {0, 0, 1, 0, 1, 1}};

    int message[MESSAGE_LEN] = {1, 0, 1};
    
    int *codeword_encoded = (int*)malloc(CODEWORD_LEN*sizeof(int));

    int *codeword_decoded = (int*)malloc(CODEWORD_LEN*sizeof(int));

    int test_codeword[CODEWORD_LEN] = {1, 1, 1, 1, 1, 1};

    encode(message, G, codeword_encoded);

    print_vector_int(codeword_encoded, CODEWORD_LEN);

    //memcpy(codeword_encoded, test_codeword, CODEWORD_LEN*sizeof(int));

    BSC_noise(codeword_encoded, BSC_ERROR_RATE);

    print_vector_int(codeword_encoded, CODEWORD_LEN);
    
    decode(H, codeword_encoded, codeword_decoded);

    if(codeword_decoded == NULL)
    {
        printf("Not a valid codeword\n");
        return 0;
    }
    print_vector_int(codeword_decoded, CODEWORD_LEN);

    free(codeword_encoded);
    free(codeword_decoded);

    //check_possible_codewords(H);
    return 0;
}