#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define SYSTEM 8 //(2/8/10/16)

int validateNumber(int c) {
    if (SYSTEM == 2) {
        if ((c != '0' && c != '1') && !isspace(c)) {
            return 0;
        } else {
            return 1;
        }
    } else if (SYSTEM == 10 || SYSTEM == 8) {
        if (!(c >= '0' && c <= '9') && !isspace(c)) {
            return 0;
        } else {
            return 1;
        }
    } else if (SYSTEM == 16) {
        if (!isxdigit(c) && !isspace(c)) {
            return 0;
        } else {
            return 1;
        }
    }
}

char *getLiner(FILE *infile, char** lines, int numLines) {
    char *line;
    int size = 0;
    int capacity = 1;
    int c;
    char *temporary;
    if ((temporary = malloc(sizeof (char) * capacity)) == NULL) {
        fprintf(stderr, "Memory allocation error");
        fclose(infile);
        exit(1);
    }
    line = temporary;//malloc check na line
    int wrongCount = 0;

    while ((c = fgetc(infile)) != '\n' && c != EOF) {
        if (size == 0 && c == '0') {
            continue;
        }
        if (validateNumber(c) == 0) {
            wrongCount++;
            goto labeler;
        }
        if (!isspace(c)) {
            line[size++] = c;
        } else if (size > 0) {
            break;
        }
        labeler:
        if (size >= capacity) {
            if (capacity == 1) {
                capacity = 10;
            } else if (capacity < INT_MAX / 2) {
                capacity *= 2;
            } else if (capacity < INT_MAX - 100) {
                capacity += 100;
            } else {
                fprintf(stderr, "Stack overflow");
                return NULL;
            }
            char *temp;
            if ((temp = realloc(line, sizeof(char) * capacity)) == NULL) {
                fprintf(stderr, "Memory allocation error");
                free(line);//!!!!
                fclose(infile);
                exit(1);
            }

            line = temp;
        }
    }
    if (size == 0 && c == EOF) {
        free(line);
        return NULL;
    }
    line[size] = '\0';
    if (wrongCount != 0) {
        fprintf(stderr, "Wrong input number");
        for(int i = 0; i < numLines; i++){
            free(lines[i]);
        }
        free(lines);
        free(line);
        fclose(infile);
        exit(1);
    } else {
        return line;
    }
}

int hexCharToDecimal(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'A' && c <= 'F') {
        return c - 'A' + 10;
    } else if (c >= 'a' && c <= 'f') {
        return c - 'a' + 10;
    }
    return 0;
}

void addNumbers(FILE* infile, char* num_0, char* num_1, int numLines, char** lines) {
    char* result = NULL;
    long int length0 = strlen(num_0);
    long int length1 = strlen(num_1);
    long int max_length = (length0 > length1) ? length0 : length1;
    char* temporary;
    if ((temporary = (char*)realloc(result, (max_length + 2) * sizeof(char))) == NULL) {
        fprintf(stderr, "Memory allocation error");
        for (int h = 2; h < numLines; h++) {
            free(lines[h]);
        }
        free(lines);
        free(num_0);
        free(num_1);
        free(result);
        fclose(infile);
        exit(1);
    }
    result = temporary;

    for (int j = 0; j < max_length + 2; j++) {
        result[j] = 0;
    }
    int carry = 0;
    long int p = length0 - 1;
    long int n = length1 - 1;
    long int k = max_length;

    while (p >= 0 || n >= 0) {
        int bit1 = (p >= 0) ? hexCharToDecimal(num_0[p]) : 0;
        int bit2 = (n >= 0) ? hexCharToDecimal(num_1[n]) : 0;

        int sum = bit1 + bit2 + carry;
        result[k] = sum % SYSTEM;
        carry = sum / SYSTEM;

        p--;
        n--;
        k--;
    }

    if (carry) {
        result[k] = carry;
    } else {
        k++;
    }
    for (int j = 2; j < numLines; j++) {
        char* num = lines[j];
        if (num == NULL) {
            fprintf(stderr, "Error reading number from the file\n");
            for (int h = 2; h < numLines; h++) {
                free(lines[h]);
            }
            free(lines);
            free(num_0);
            free(num_1);
            free(result);
            fclose(infile);
            exit(1);
        }

        long int lengthNum = strlen(num);
        long int lengthResult = max_length + 1;
        max_length = (lengthNum > lengthResult) ? lengthNum : lengthResult;

        char* tmp;
        if ((tmp = (char*)realloc(result, (max_length + 2) * sizeof(char))) == NULL) {
            fprintf(stderr, "Memory allocation error");
            for (int h = 2; h < numLines; h++) {
                free(lines[h]);
            }
            free(lines);
            free(num_0);
            free(num_1);
            free(result);
            fclose(infile);
            exit(1);
        }
        result = tmp;

        carry = 0;
        p = lengthNum - 1;
        n = lengthResult - 1;
        k = max_length;

        while (p >= 0 || n >= 0) {
            int bit1 = (p >= 0) ? hexCharToDecimal(num[p]) : 0;
            int bit2 = (n >= 0) ? result[n] : 0;
            int sum = bit1 + bit2 + carry;
            result[k] = sum % SYSTEM;
            carry = sum / SYSTEM;
            p--;
            n--;
            k--;
        }
        if (carry) {
            result[k] = carry;
        } else {
            k++;
        }
    }

    printf("\n\n\nThe number is ");
    int zerosBeggining = 0;

    for (int z = 0; z < max_length; z++) {
        if (result[z] == 0) {
            zerosBeggining++;
        }
        else {
            break;
        }
    }
    for (int s = zerosBeggining; s < max_length + 1; s++) {
        printf("%X", result[s]); // Print hexadecimal digits
    }
    free(result);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Bad input.");
        return EXIT_FAILURE;
    }
    FILE *infile = fopen(argv[1], "r");
    if (infile == NULL) {
        printf("Bad allocation.");
        return EXIT_FAILURE;
    }
    char *line;
    int numLines = 0;
    char **lines = NULL;
    while ((line = getLiner(infile, lines, numLines)) != NULL) {
        char **temp;
        if ((temp = realloc(lines, (numLines + 1) * sizeof(*lines))) == NULL) {
            printf("lines allocation error");
            for (int i = 0; i < numLines; i++) {
                free(lines[i]);
            }
            free(lines);
            if (fclose(infile) != 0) {
                printf("Error closing the file");
                return EXIT_FAILURE;
            }
            return EXIT_FAILURE;
        }
        lines = temp;
        lines[numLines] = line;
        numLines++;
    }
    if (numLines < 2) {
        fprintf(stderr, "\nIncorrect input error\n");
        for (int h = 0; h < numLines; h++) {
            free(lines[h]);
        }
        free(lines);
        fclose(infile);
        return EXIT_FAILURE;
    }
    char *num_0 = lines[0];
    if (num_0 == NULL) {
        fprintf(stderr, "Error reading the first number from the file\n");
        for (int h = 0; h < numLines; h++) {
            free(lines[h]);
        }
        free(lines);
        fclose(infile);
        return EXIT_FAILURE;
    }
    char *num_1 = lines[1];
    if (num_1 == NULL) {
        fprintf(stderr, "Error reading the second number from the file\n");
        for (int h = 0; h < numLines; h++) {
            free(lines[h]);
        }
        free(lines);
        free(num_0);
        fclose(infile);
        return EXIT_FAILURE;
    }

    addNumbers(infile, num_0, num_1, numLines, lines);

    for (int i = 2; i < numLines; i++) {
        free(lines[i]);
    }
    free(lines);
    free(num_0);
    free(num_1);
    fclose(infile);
    return 0;
}