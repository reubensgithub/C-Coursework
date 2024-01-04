#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pig.h"

int main() {
    char line[200]; // input space that can contain up to 200 characters for an inputted sentence.
    while (1) {
        printf("Enter a line: "); // prompts user to input a sentence.
        fgets(line, sizeof(line), stdin);
        if (line[0] == '\n') { // if an empty line is inputted, break out from the loop.
            break;
        }
        char *token = strtok(line, " \n");
        while (token != NULL) {
            char piglatin[200]; // workspace that can contain up to 200 characters for an inputted sentence.
            char *copy = strdup(token);
            strcpy(piglatin, pig(copy));
            printf("%s ", piglatin);
            free(copy);
            token = strtok(NULL, " \n");
        }
        printf("\n");
    }
    return 0;
}


