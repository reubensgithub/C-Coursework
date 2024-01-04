#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "riffle.h"

int main(){
    srand(time(NULL)); // random seed set based on time.

    int i;
    float avg_qualities[15]; // array containing average quality for every number of shuffles.

    for (i = 0; i < 15; i++) {
        avg_qualities[i] = average_quality(50, i+1, 30); // Store the average quality of each shuffle into the
                                                         // avg_qualities array.
    }

    FILE *file_writer;
    file_writer = fopen("quality.txt", "w");

    for (i = 0; i < 15; i++) {
		fprintf(file_writer, "Number of Shuffles = %i, Average Quality = %f\n", i+1, avg_qualities[i]);
    }
    fclose(file_writer);

    /* Based off of my results after running average_quality() to shuffle an array
       of length 50 to achieve a good shuffle, 7 shuffles is the necessary amount
       of shuffles needed to achieve a good shuffle, as the shuffle score after 7
       shuffles is closest to 0.5, after this, the quality score does not get any
       better after shuffling more than 7 times. */
}