#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "riffle.h"

int main() {
    srand(time(NULL)); // random seed set, based on the current time.
    int i;
    int L[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20}; // array of integers from 1-20.
    int *l_ptr = &L[0]; // pointer set to the first element of the integer array.
    int l_size = sizeof(int); // size of the integer array.
    int l_len = sizeof(L) / sizeof(L[0]); // length of the integer array.
    riffle(l_ptr, l_len, l_size, 5); // perform riffle on the integer array.

    // Output the result of the riffle shuffle on L.
    printf("Output of array L, shuffled:\n");
    for (i = 0; i < l_len; i++)
    {
      printf("%d\n", ((int*)l_ptr)[i]);
    }

    // Output the shuffle check of the riffle shuffle on L.
    printf("Shuffle check for int array: %i\n", check_shuffle(l_ptr, l_len, l_size, &cmp_int));

    // array of greek words
    char *greek[] = {"alpha", "beta", "gamma", "delta", "epsilon", "zeta", "eta", "theta", "iota", "kappa", "lambda", "mu"};
    char **greek_ptr = &greek[0]; // pointer set to the first element of the integer array.
    int greek_size = sizeof(greek[0]); // size of the greek array.
    int greek_len = sizeof(greek) / sizeof(greek[0]); // length of the greek array.
    riffle(greek, greek_len, greek_size, 5); // perform riffle on the greek array.

    // Output the result of the riffle shuffle on L.
    printf("Output of array greek, shuffled:\n");
    for (i = 0; i < greek_len; i++)
    {
      printf("%s\n", ((char**)greek)[i]);
    }

    // Output the shuffle check of the riffle shuffle on greek.
    printf("Shuffle check for greek array: %i\n", check_shuffle(greek_ptr, greek_len, greek_size, &cmp_str));

    return 0;
}
  