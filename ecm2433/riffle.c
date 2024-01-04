#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "riffle.h"

/**

  Performs one riffle shuffle on an array.
  @param L A pointer to the array to shuffle.
  @param len The number of elements in the array.
  @param size The size of each element in bytes.
  @param work A pointer to a temporary array to use for shuffling.
*/
void riffle_once(void *L, int len, int size, void *work) {
  srand(time(NULL));
  void *fhalf;
  void *shalf;

  // allocate enough space to store half of the inputted array in each half.
  fhalf = malloc((len/2) * size);
  shalf = malloc((len/2) * size);

  // split the input array into the halves and store the first half
  // second half in the arrays fhalf and shalf respectively.
  memcpy(fhalf, L, (len/2) * size); 
  memcpy(shalf, L + ((len/2)*size), (len/2) * size);


  int i;
  int first_index = 0;
  int second_index = 0;
  int probability = rand() % 2;
  void *chosen_array;


  for (i = 0; i < len; i++) {
    probability = rand() % 2;
    if (probability == 0) {
      if (first_index < (len/2)) {// this if statement is included so that one array is not selected more than the other.
        if (size == sizeof(char*)) { // if the inputted array contains strings, cast to (char**).
          chosen_array = fhalf;
          memcpy(work + (i * size), (char**)chosen_array + first_index, size);
          first_index++;
        } else {
        chosen_array = fhalf; // Else, continue but there's no need to cast for character and int arrays.
        memcpy(work + (i * size), chosen_array + (size * first_index), size);
        first_index++;
        }
      }
      else {
        chosen_array = shalf;
        memcpy(work + (i * size), chosen_array + (size * second_index), size);
        second_index++;
      }
    } else if (probability == 1) {
       if (second_index < (len/2)) { // this if statement is included so that one array is not selected more than the other.
         if (size == sizeof(char*)) { // if the inputted array contains strings, cast to (char**).
           chosen_array = shalf;
           memcpy(work + (i * size), (char**)chosen_array + second_index, size);
          second_index++;
         } else { // Else, continue but there's no need to cast for character and int arrays.
        chosen_array = shalf;
        memcpy(work + (i * size), chosen_array + (size * second_index), size);
        second_index++;
         }
       }
      else {
        chosen_array = fhalf;
      memcpy(work + (i * size), chosen_array + (size * first_index), size);
      first_index++;
      }
    }
  }
  memcpy(L, work, len * size); // copy the work array over to L to overwrite the inputted array with the new, shuffled array.
  free(fhalf); // free allocated memory to avoid memory leaks
  free(shalf);
}


/**

  Riffles the inputted array N times, by using riffle_once.
  @param L Pointer to the array to be riffled.
  @param len The length of the array.
  @param size The size of each element in the array.
  @param N The number of perfect shuffles to perform.
*/
void riffle(void *L, int len, int size, int N) {
    int i;
    void *work = malloc(len * size);
    for (i = 0; i < N; i++) { // executes riffle N number of times on the array L. 
      riffle_once(L, len, size, work);
  }
    free(work); // free allocated memory to avoid memory leaks
}

/**
 * Compares two int pointers
 * @param a A pointer to an integer to be compared
 * @param b A pointer to an integer to be compared
 * @return -1 if a is greater than b, 0 if a is equal to b or -1 if a is less than b.
*/
int cmp_int(void *a, void *b) {
    int x = *(int*)a;
    int y = *(int*)b;

    if (x > y)
      return -1;
    else if (x == y)
      return 0;
    else if (x < y)
      return 1;
}


/**
 * Compares two string pointers
 * @param a A pointer to a string to be compared
 * @param b A pointer to a string to be compared
 * @return -1 if a is greater than b, 0 if a is equal to b or -1 if a is less than b.
*/
int cmp_str(void *a, void *b) {
    char* x = *(char**)a;
    char* y = *(char**)b;

    return strcmp(y, x);
}


/**
 * Checks if a list is shuffled or not
 * @param L A pointer to the array to be checked.
 * @param len The number of elements in the array.
 * @param size The size of each element in bytes.
 * @return 1 if the list is shuffled, 0 if not.
*/
int check_shuffle(void *L, int len, int size, int (*cmp)(void *, void *)) {
    int i;
    int j;
    void *copy = malloc(len * size);
    memcpy(copy, L, len * size);

    riffle(L, len, size, 5);

    /*for (i = 0; i < len; i++) {
      printf("Original array: %d\n", ((int*)copy)[i]);
    }
    printf("\n\n\n");
    for (i = 0; i < len; i++) {
      printf("Shuffled array: %d\n", ((int*)L)[i]);
    }*/

    int result = 1;
    for (i = 0; i < len; i++) {
        for (j = 0; j < len; j++) {
          char *a = (char*) L + i * size;
          char *b = (char*) copy + i * size;
          if (cmp(a, b) != 0) {
              break;
          }
          if (j == len - 1) {
            free(copy);
            result = 0;
            return result;
          }
        }
    }
    free(copy);
    return result;
}

/**
 * Checks the quality of an array by iterating over each element and comparing them.
 * @param numbers A pointer to the array to be checked.
 * @param len The number of elements in the array.
 * @return The quality of the inputted array.
*/
float quality(int *numbers, int len) {
  int i;
  int counter = 0;

  for (i = 0; i < len-1; i++) {
    if(numbers[i] < numbers[i + 1]) {
      counter++;
    }
  }
  float quality = (float)counter / (float)(len-1);
  return quality;
}

/**
 * Checks the average quality of a shuffle of an array of integers for a specified number of trials
 * @param N The range of numbers to be shuffled (if N=20, an array of numbers 0-19 will be generated).
 * @param shuffles The number of times to shuffle the array.
 * @param trials The number of trials used to get the average over.
 * @return The average quality of the inputted array.
*/
float average_quality(int N, int shuffles, int trials) {
    int i;
    int *deck = malloc(N * sizeof(int));
    int *work = malloc(N * sizeof(int)); // the workspace
    for (i = 0; i < N; i++) {
        deck[i] = i;
    }

    float quality_score = 0;

    for (i = 0; i < trials; i++) {
        memcpy(work, deck, N * sizeof(int));
        riffle(work, N, sizeof(int), shuffles);
        quality_score += quality((int *)work, N);
    }

    free(deck);
    free(work);

    return (float)quality_score / (float)trials;
}
  