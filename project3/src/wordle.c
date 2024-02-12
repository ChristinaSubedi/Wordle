#include "wordle.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void select_word(int k, char *word, Trie *dict) {
    Trie *current = dict;
    if (word != NULL) {
        for (int x = 0; x < k; x++) {
            int j = 26 * (drand48());
            for (int y = 0; y < 26; y++) {
                if (current->child[j] == NULL) {
                    j = (j + 5) % 26;

                } else {
                    word[x] = 'a' + j;
                    current = current->child[j];
                    break;
                }
            }
        }
    }
    word[k] = '\0';
}

Trie *generateDict(char *filename, int k, /*@out@*/ char *selected1,
                   /*@out@*/ char *selected2) {
    FILE *f;
    f = fopen(filename, "r");
    if (f == NULL) exit(EXIT_FAILURE);
    Trie *dict = create();

    char *input = malloc(((k + 1) * sizeof(char)));
    while (1) {
        if (fscanf(f, "%s", input) == EOF) {
            break;
        }

        insert(dict, input);
    }
    fclose(f);

    select_word(k, selected1, dict);

    if (selected2 != NULL) {
        int wordcount[26];

        for (int i = 0; i < 26; i++) {
            wordcount[i] = 0;
        }
        for (int i = 0; i < k; i++) {
            int index1 = CHAR_TO_INDEX(selected1[i]);
            wordcount[index1] += 1;
        }
        Trie *dict2 = create();
        while (1) {
            f = fopen(filename, "r");
            if (f == NULL) exit(EXIT_FAILURE);

            while (1) {
                if (fscanf(f, "%s", input) == EOF) {
                    break;
                }

                bool found = true;
                for (int i = 0; i < 26; i++) {
                    if (wordcount[i] > 0) {
                        char j[2];
                        *j = 'a' + i;
                        j[1] = '\0';

                        if (strstr(input, j)) {
                            found = false;
                            break;
                        }
                    }
                }
                if (found) {
                    insert(dict2, input);
                }
            }
            fclose(f);

            if (dict2 != NULL) {
                select_word(k, selected2, dict2);
                break;
            }
            // select something else for the first word;
            select_word(k, selected1, dict);
        }
        destroy(dict2);
    }

    free(input);
    return dict;
}

char *guess(Trie *dict, int k) {
    char *word = malloc(sizeof(char) * (k + 1));
    char *word2 = malloc(sizeof(char) * 5000);
    printf("Please input your guess: ");

    while (1) {
        if (fscanf(stdin, "%s", word2)) {
            if ((*(word2 + k) == '\0') & (strlen(word2) == k)) {
                for (int i = 0; i < k; ++i) {
                    word2[i] = tolower((unsigned char)word2[i]);
                }
                if (dict == NULL) {
                    break;
                }
                bool check = lookup(dict, word2);
                if (check) {
                    break;
                }
            }
        }
        printf("Invalid word. Try again: ");
    }

    snprintf(word, (k + 1), "%s", word2);
    free(word2);
    return word;
}

feedback_result *getFeedback(char *guess, char *word1, char *word2, int k) {
    feedback_result *feedback = malloc(sizeof(feedback) * k);
    // stores address of the guessed word

    // array to check how many occurences of a word has occured

    int j = 1;
    int count = 0;

    for (int l = 0; l < k; l++) {
        feedback[l] = WRONG;
    }
    if (word2 != NULL) {
        j = 2;
    }
    char *word = word1;
    while (j) {
        bool isfirst = false;
        int wordcount[26];

        // initaializing the word array
        for (int i = 0; i < 26; i++) {
            wordcount[i] = 0;
        }

        // storing contents in the array and initializing result
        for (int i = 0; i < k; i++) {
            int index1 = CHAR_TO_INDEX(word[i]);

            wordcount[index1] += 1;
        }

        // for the green boxes
        for (int i = 0; i < k; i++) {
            int guessed = CHAR_TO_INDEX(guess[i]);
            int index1 = CHAR_TO_INDEX(word[i]);
            if (guessed == index1) {
                feedback[i] = CORRECT;
                wordcount[index1] -= 1;
                isfirst = true;
            }
        }

        // for the yellow boxes
        for (int i = 0; i < k; i++) {
            int guessed = CHAR_TO_INDEX(guess[i]);
            if ((feedback[i] != CORRECT) & (wordcount[guessed] > 0)) {
                {
                    feedback[i] = WRONGPOS;
                    wordcount[guessed] -= 1;
                    isfirst = true;
                }
            }
        }
        j = j - 1;
        word = word2;
        if (isfirst) {
            count += 1;
        }
    }

    if (count == 2) {
        for (int i = 0; i < k; i++) {
            if (feedback[i] == CORRECT) {
                feedback[i] = QUANTUMCORRECT;
            }
            if (feedback[i] == WRONGPOS) {
                feedback[i] = QUANTUMWRONGPOS;
            }
        }
    }

    return feedback;
}

void printFeedback(feedback_result *feedback, int k) {
    printf("Result: ");
    for (int i = 0; i < k; i++) {
        switch (feedback[i]) {
            case CORRECT:
                printf("%s", CORRECT_UNICODE);
                break;
            case WRONGPOS:
                printf("%s", PRESENT_UNICODE);
                break;
            case WRONG:
                printf("%s", WRONG_UNICODE);
                break;
            case QUANTUMCORRECT:
                printf("%s", QCORRECT_UNICODE);
                break;
            case QUANTUMWRONGPOS:
                printf("%s", QPRESENT_UNICODE);
                break;
            default:
                break;
        }
    }
    printf("\n");
}

bool checkWin(feedback_result *feedback, int k) {
    int count = 0;
    for (int i = 0; i < k; i++) {
        if (feedback[i] == CORRECT) {
            count += 1;
        }
    }
    if (count == k) {
        return true;
    } else {
        return false;
    }
}
