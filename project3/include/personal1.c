
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CORRECT_UNICODE "🟩"
#define PRESENT_UNICODE "🟨"
#define WRONG_UNICODE "⬛"
#define QCORRECT_UNICODE "🟢"
#define QPRESENT_UNICODE "🟡"

typedef enum {
    CORRECT,
    WRONGPOS,
    WRONG,
    QUANTUMCORRECT,
    QUANTUMWRONGPOS,
} feedback_result;

#define CHAR_TO_INDEX(c) ((int)c - (int)'a')
#define alphabetsize 26

typedef struct Trie {
    struct Trie *child[alphabetsize];
    bool end;
} Trie;

struct Trie *create() {
    struct Trie *new = NULL;
    new = (struct Trie *)malloc(sizeof(struct Trie));

    if (new) {
        new->end = false;
        for (int i = 0; i < alphabetsize; i++) {
            new->child[i] = NULL;
        }
    }
    return new;
}

void insert(struct Trie *dict, char *str) {
    int length = strlen(str);
    int index;
    struct Trie *current = dict;
    for (int i = 0; i < length; i++) {
        index = CHAR_TO_INDEX(str[i]);
        if (!current->child[index]) {
            current->child[index] = create();
        }
        current = current->child[index];
    }
    current->end = true;
}

bool lookup(Trie *dict, char *str) {
    int index;
    int length = strlen(str);
    struct Trie *current = dict;
    if (str == NULL) {
        return false;
    }
    if (current == NULL) {
        return false;
    }
    for (int i = 0; i < length; i++) {
        index = CHAR_TO_INDEX(str[i]);
        if (!current->child[index]) {
            return false;
        }
        current = current->child[index];
    }

    return current->end;
}

void destroy(Trie *dict) {
    if (dict == NULL) {
        return;
    }
    for (int i = 0; i < alphabetsize; i++) {
        destroy(dict->child[i]);
    }
    free(dict);
}

void select_word(int k, char *word, Trie *dict) {
    Trie *current = dict;
    if (word != NULL) {
        for (int x = 0; x < k; x++) {
            int j =26*( drand48());
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

Trie *generateDict(char *filename, int k, char *selected1, char *selected2) {
    FILE *f;
    f = fopen(filename, "r");
    if (f == NULL) exit(EXIT_FAILURE);
    Trie *dict = create();

    char *input = malloc(((k + 1) * sizeof(char)));
    while (!feof(f)) {
        fscanf(f, "%s", input);
        
            insert(dict, input);
        
    }
    fclose(f);
    int wordcount[26];

    for (int i = 0; i < 26; i++) {
        wordcount[i] = 0;
    }

    select_word(k, selected1, dict);
    for (int i = 0; i < k; i++) {
        int index1 = CHAR_TO_INDEX(selected1[i]);
        wordcount[index1] += 1;
    }

    if (selected2 != NULL) {
        char *word2 = malloc((k + 1) * sizeof(char));
        Trie *dict2 = create();
        while (1) {
            
            f = fopen(filename, "r");
            if (f == NULL) exit(EXIT_FAILURE);
            
            while (!feof(f)) {
                fscanf(f, "%s", input);
                
                    bool found = true;
                    for (int i = 0; i < 26; i++) {
                        if (wordcount[i] > 0) {
                            char j[2];
                            *j = 'a' + i;
                            j[1]='\0';

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

            if (dict2!=NULL) {
                select_word(k, selected2, dict2);
                break;
            }
            //select something else for the first word;
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
        if (fscanf( stdin, "%s", word2)) {
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

    for (int count = 0; count < k; count++) {
        feedback[count] = WRONG;
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

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage: %s [k] [file] [quantum]\n", argv[0]);
        printf("  k       - word length\n");
        printf("  file    - textfile containing the words one per line\n");
        printf(
            "  quantum - whether quantum wordle should be played (y/n), "
            "default false\n");
        exit(EXIT_FAILURE);
    }

    // initialize randomness using current time
    srand48(time(NULL));
    int k = atoi(argv[1]);
    char *filename = argv[2];
    bool quantum = false;
    if (argc > 3) {
        if (argv[3][0] == 'y' || argv[3][0] == 'Y' || argv[3][0] == '1' ||
            argv[3][0] == 't' || argv[3][0] == 'T') {
            quantum = true;
        }
    }
    // allocate space to store the randomly selected word
    // +1 for null terminator
    char *selected = malloc(k + 1);
    char *selected2 = quantum ? malloc(k + 1) : NULL;
    // read file containing all words
    Trie *set = generateDict(filename, k, selected, selected2);
    printf("The selected word is \"%s\". (Do not tell anyone)\n", selected);
    if (quantum) {
        printf("The selected dual word is \"%s\". (Do not tell anyone)\n",
               selected2);
    }

    int rounds = 0;
    bool check = false;
    while (!check) {
        char *word = guess(set, k);
        feedback_result *result = getFeedback(word, selected, selected2, k);
        printFeedback(result, k);
        check = checkWin(result, k);
        rounds += 1;
    }
    printf("You needed %d attempts.\n", rounds);
    // frees all memory of the dictionary
    destroy(set);
    free(selected);
    if (selected2) free(selected2);

    return EXIT_SUCCESS;
}
