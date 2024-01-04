#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


// I have included strsep() into this file because I was having
// issues with my string.h header file apparently not having
// strsep() defined in it. The pig() function should still work
// without it, provided that the string.h file in your system includes it.
// I would recommend keeping it in case, but if pig() works without strsep() being
// defined in this file then feel free to remove it.
char *strsep(char **stringp, const char *delim) {
    char *start = *stringp;
    char *end = start ? strpbrk(start, delim) : NULL;
    if (end) {
        *end++ = '\0';
        *stringp = end;
    } else {
        *stringp = NULL;
    }
    return start;
}


/**

    This function takes a word and translates it into Pig Latin.
    It returns the resulting Pig Latin word as a string.
    @param word A pointer to the string representing the original word.
    @return A pointer to the string representing the Pig Latin word.
*/
char *pig(char *word) {
    char *copy = strdup(word);
    char *result = malloc(strlen(word) + 3);
    result[0] = '\0';
    char *point = NULL;

    if (*word == 'a' || *word == 'e' || // if word begins with a vowel, add "way" to the end.
        *word == 'i' || *word == 'o' ||
        *word == 'u') {
      strcpy(result, word);
      strcat(result, "way");
    } else if (*word == 'y') {
        copy = strsep(&copy, "aeiou"); // if word begins with a y, treat the word
                                       // as if it began with a vowel.
        point = word;
        strcpy(result, point);
        strcat(result, copy);
        strcat(result, "ay");
    } else {
        copy = strsep(&copy, "aeiouy"); // if the word begins with a consonant, it will take
                                        // the first consonant cluster before the first vowel
                                        // and add it to the end of the word, followed by "ay". 
        point = word;
        strcpy(result, point);
        strcat(result, copy);
        strcat(result, "ay");
    }

    if (*result == 'y') {
        char *vowel = strpbrk(result+1, "aeiouy"); // if the result begins with a y, remove it to get the
                                                  // Pig Latin translation.
        if (vowel) {
            memmove(result, vowel, strlen(vowel)+1);
        }
    } else {
        char *vowel = strpbrk(result, "aeiouy"); // if the result begins with any other letter, remove the
                                                 // first consonant cluster to get the Pig Latin translation.
        if (vowel) {
            memmove(result, vowel, strlen(vowel)+1);
        }
    }

    free(copy);
    return result;
}
