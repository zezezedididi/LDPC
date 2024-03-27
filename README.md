# Notes

## Encoding

Operation:
 - Matrix Multiplication

 - How to respresent message data
 - How to obtain the generator matrix

 MATRIX H (PARITY CHECK MATRIX) - m x n where m is the number of constraints and n is length of the codewords
HcT = 0 where cT is the transpose of the codeword

 MATRIX G (GENERATOR MATRIX) - k x n where k is the length of the message bits and n is the length of the codeword
c = uG where u is the message bits

- How to obtain G from H?
H = [A, I_{n-k}]
G = [I_k, AT]

## Decoding

Operation:
 - Sum-product decoding

RECEIVED VECTOR: y
CROSSOVER PROBABILITY: p
A PRIORI PROBABILITIES VECTOR: r 

r_i = log (p/(1-p)) if y_i = 1,
    = log ((1-p)/p) if y_1 = 0

MESSAGES FROM BIT NODE TO CHECK NODE MATRIX: M_{j,i} where j number of check nodes and i is the number of i nodes

M_{j,i} = sum_{j' belongs to A and j' =/= j}(E_{j',i} + r_i)

Initialization: M_{j,i} = r_i (ATTENTION CHECK THIS INITIALIZATION I BELIEVE THERE IS A CONDITION MISSING WHERE, FOR EACH COLUMN, ONLY THE ROWS THAT HAVE ONE (WHERE THAT BIT NODE IS USED) THE r is substituted and not the whole column as it is presented)

EXTRINSIC PROBABILITIES MATRIX: E

E_{j,i} = (page 32 eq 2.7)

L is the total LLR for each iteration and is given by

L_i = r_i + sum(E_{i,j}) for every checknode j that contains i

z is the current codeword after each iteration where z_i = 1 if L_i < 0
                                                     z_i = 0 if L_i > 0

- How to test if the obtained value is or isn't a code word

s = zH' and s must be a vector with only 0's

H' is HT

## Matrix representation

As matrizes neste momento têm este formato num ficheiro txt:
    >1st line: number_rows number_columns type
    >2nd-end : matrix 
    
Como o valor para as matrizes the paridade são 1 ou 0 só é necessário guardar os valores dos indices
Isto é inefeciente e têm que ser alterado já está no TODO
a maneira melhor seria guardar isto exatamente como está guardado dentro do programa (binario).

Dentro do programa em si as matrizes estão gruardadas numa estrutura pchk definida no defs
a estrutura é basicamente o que está no ficheiro



## Example for encoding

[1 0 0 1 0 1]
[0 1 0 1 1 1]
[0 0 1 0 1 1]


[0 0 0] = [0 0 0 0 0 0]
[0 0 1] = [0 0 1 0 1 1]
[0 1 0] = [0 1 0 1 1 1]
[0 1 1] = [0 1 1 1 0 0]
[1 0 0] = [1 0 0 1 0 1]
[1 0 1] = [1 0 1 1 1 0]
[1 1 0] = [1 1 0 0 1 0]
[1 1 1] = [1 1 1 0 0 1]

Ativa as colunas em que a mensagem está a 1 e dá-lhes xor.

Implementação do Mackday por exemplo para o [0 1 1]:
pega na segunda linha e copia-a para a codeword
[0 1 0 1 1 1]
depois pega na terceira linha e da xor das duas
[0 1 0 1 1 1] xor [0 0 1 0 1 1] = [0 1 1 1 0 0]


## TODO 

-implementar um sistema de ficheiros melhor que txt
    >vou tentar roubar a idea feita nas noutras implementações por parecer ser bem optimizado
-implementar matrizes esparsas
    >leitura da matrix
    >adaptar encoding e decoding
    >o encoding passa a ser o if no lado da matriz
-remover coisas fixas no ficheiro defs:
    >CODEWORD_LEN
    >MESSAGE_LEN
    >NUM_CHECKS

-check_possible_codewords() is a function in the decoding header that is not implemented => is this a issue???
