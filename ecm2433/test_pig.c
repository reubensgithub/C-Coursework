#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pig.h"

int main() {
    // A character array of 7 words, each with a maximum length of 10 characters
    // Maximum length is set to 10 characters because pig() can add a maximum of
    // 3 new characters to these words.
    char input[7][10] = {"happy", "duck", "glove", "evil", "eight", "yowler", "crystal"};


    // Iterates over the input array, assigns a character pointer to the output of pig
    // from the ith word of input to the (len-1)th input.
    int i;
    for (i = 0; i < 7; i++) {
        char *result = pig(input[i]);
        printf("%s => %s\n", input[i], result);
        free(result);
    }

    return 0;
}