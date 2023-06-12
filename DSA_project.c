#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Structure to store character frequency and Huffman code
struct Node {
    char character;
    int frequency;
    struct Node* left;
    struct Node* right;
};

// Structure to store encoded character and corresponding Huffman code
struct EncodedChar {
    char character;
    char code[100];
};

// Function to create a new node
struct Node* createNode(char character, int frequency) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->character = character;
    node->frequency = frequency;
    node->left = node->right = NULL;
    return node;
}

// Function to create a min heap node
void minHeapify(struct Node** minHeap, int size, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < size && minHeap[left]->frequency < minHeap[smallest]->frequency)
        smallest = left;

    if (right < size && minHeap[right]->frequency < minHeap[smallest]->frequency)
        smallest = right;

    if (smallest != index) {
        struct Node* temp = minHeap[index];
        minHeap[index] = minHeap[smallest];
        minHeap[smallest] = temp;
        minHeapify(minHeap, size, smallest);
    }
}

// Function to build min heap
void buildMinHeap(struct Node** minHeap, int size) {
    int i = (size - 1) / 2;
    while (i >= 0) {
        minHeapify(minHeap, size, i);
        i--;
    }
}

// Function to insert a node in min heap
void insertNode(struct Node** minHeap, int* size, struct Node* node) {
    (*size)++;
    int i = *size - 1;
    while (i && node->frequency < minHeap[(i - 1) / 2]->frequency) {
        minHeap[i] = minHeap[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap[i] = node;
}

// Function to extract minimum frequency node from min heap
struct Node* extractMin(struct Node** minHeap, int* size) {
    struct Node* node = minHeap[0];
    minHeap[0] = minHeap[(*size) - 1];
    (*size)--;
    minHeapify(minHeap, *size, 0);
    return node;
}

// Function to assign Huffman codes to the characters
void assignHuffmanCodes(struct Node* root, char code[], int top, struct EncodedChar encodedChars[]) {
    if (root->left) {
        code[top] = '0';
        assignHuffmanCodes(root->left, code, top + 1, encodedChars);
    }

    if (root->right) {
        code[top] = '1';
        assignHuffmanCodes(root->right, code, top + 1, encodedChars);
    }

    if (!root->left && !root->right) {
        int i;
        for (i = 0; i < top; i++) {
            encodedChars[root->character].code[i] = code[i];
        }
        encodedChars[root->character].code[top] = '\0';
    }
}

// Function to encode the given sentence
void encodeSentence(char* sentence, struct EncodedChar encodedChars[], char* encodedSentence) { 
    int i, j;
    for (i = 0; i < strlen(sentence); i++) {
        for (j = 0; j < strlen(encodedChars[sentence[i]].code); j++) {
            strcat(encodedSentence, &encodedChars[sentence[i]].code[j]);
        }
    }
}

// Function to decode the encoded sentence
void decodeSentence(char* encodedSentence, struct EncodedChar encodedChars[], char* decodedSentence) {
    int i, j;
    char code[100];
    int codeIndex = 0;

    for (i = 0; i < strlen(encodedSentence); i++) {
        code[codeIndex++] = encodedSentence[i];
        code[codeIndex] = '\0';

        for (j = 0; j < 256; j++) {
            if (strcmp(encodedChars[j].code, code) == 0) {
                strcat(decodedSentence, &encodedChars[j].character);
                codeIndex = 0;
                break;
            }
        }
    }
}

// Function to count and display the frequency of each alphabet
void countAndDisplayFrequency(char* sentence) {
    int frequency[26] = { 0 };  // Frequency array for each alphabet

    // Convert all characters to lowercase
    int i;
    for (i = 0; i < strlen(sentence); i++) {
        char c = tolower(sentence[i]);
        if (c >= 'a' && c <= 'z') {
            frequency[c - 'a']++;
        }
    }

    // Display the frequency of each alphabet
    for (i = 0; i < 26; i++) {
        printf("%c: %d\n", 'a' + i, frequency[i]);
    }
}

int main() {
    char sentence[1000];
    printf("-------------Encoder/Decoder-----------\n");

    printf("Enter the sentence to encode: ");
    fgets(sentence, sizeof(sentence), stdin);
    sentence[strcspn(sentence, "\n")] = '\0';

    // Calculate character frequencies
    int frequencies[256] = { 0 };
    int i;
    for (i = 0; i < strlen(sentence); i++) {
        frequencies[(int)sentence[i]]++;
    }

    // Create min heap for Huffman encoding
    struct Node* minHeap[256];
    int size = 0;
    for (i = 0; i < 256; i++) {
        if (frequencies[i] > 0) {
            minHeap[size] = createNode(i, frequencies[i]);
            size++;
        }
    }
    buildMinHeap(minHeap, size);

    // Build Huffman tree
    while (size > 1) {
        struct Node* left = extractMin(minHeap, &size);
        struct Node* right = extractMin(minHeap, &size);

        struct Node* newNode = createNode('$', left->frequency + right->frequency);
        newNode->left = left;
        newNode->right = right;

        insertNode(minHeap, &size, newNode);
    }

    // Assign Huffman codes to characters
    struct EncodedChar encodedChars[256];
    char code[100];
    assignHuffmanCodes(minHeap[0], code, 0, encodedChars);

    // Encode the sentence
    char encodedSentence[1000] = "";
    encodeSentence(sentence, encodedChars, encodedSentence);
    printf("\n\n***********************************************\n\n");
    printf("Encoded sentence: %s ", encodedSentence);
    printf("\n\n***********************************************\n\n");
    // Decode the encoded sentence
    char decodedSentence[1000] = "";
    decodeSentence(encodedSentence, encodedChars, decodedSentence);
    printf("Decoded sentence: %s\n", sentence);

    // Count and display the frequency of each alphabet
    printf("\n\n************ Frequency Table ******************\n\n");
    countAndDisplayFrequency(sentence);
    printf("\n\n********************** THANK YOU *************************\n\n");
    return 0;
}
