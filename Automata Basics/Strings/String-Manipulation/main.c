#include <stdio.h>
#include <stdlib.h>

typedef struct CharNode {
    char data;
    struct CharNode *next;
} CharNode;

typedef struct String {
    CharNode *head; // Pointer to the first character of the string
    CharNode *tail; // Pointer to the last character of the string
    size_t length; // Length of the string
} String;

// Function to initialize a string
void initString(String *str) {
    str->head = str->tail = NULL;
    str->length = 0;
}

// Function to append a character to a string
void appendChar(String *str, char character) {
    CharNode *newNode = (CharNode *)malloc(sizeof(CharNode));
    newNode->data = character;
    newNode->next = NULL;

    if (str->head == NULL) {
        str->head = str->tail = newNode;
    } else {
        str->tail->next = newNode;
        str->tail = newNode;
    }

    str->length++;
}

// Function to read a string from user input
void readString(String *str) {
    printf("Enter a string: ");
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        appendChar(str, ch);
    }
}

// Function to concatenate two strings
void concatenateStrings(String *result, const String *str1, const String *str2) {
    CharNode *current = str1->head;
    while (current != NULL) {
        appendChar(result, current->data);
        current = current->next;
    }

    current = str2->head;
    while (current != NULL) {
        appendChar(result, current->data);
        current = current->next;
    }
}

// Function to free allocated memory
void cleanupString(String *str) {
    CharNode *current = str->head;
    while (current != NULL) {
        CharNode *next = current->next;
        free(current);
        current = next;
    }
    str->head = str->tail = NULL;
    str->length = 0;
}

// Function to print a given string
void printString(const String *str) {
    CharNode *current = str->head;
    while (current != NULL) {
        putchar(current->data);
        current = current->next;
    }
}

int main() {
    String string1, string2, concatenated;

    initString(&string1);
    initString(&string2);
    initString(&concatenated);

    readString(&string1);
    readString(&string2);

    concatenateStrings(&concatenated, &string1, &string2);

    printf("Concatenated string: ");
    printString(&concatenated);
    putchar('\n');

    cleanupString(&string1);
    cleanupString(&string2);
    cleanupString(&concatenated);

    return 0;
}
