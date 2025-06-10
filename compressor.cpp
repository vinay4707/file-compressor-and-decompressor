int fd1 = open(“sample.txt”, O_RDONLY);
if (fd1 == -1) {
    perror("Open Failed For Input File:\n");
    exit(1);
}

// Creating output file in write mode
int fd2 = open(“sample - compressed.txt”,
               O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
if (fd2 == -1) {
    perror("Open Failed For Output File:\n");
    exit(1);
}
#include <iostream>
#include <utility>
#include <vector>

using namespace std;

// Structure for tree nodes
struct Node {
    char character;
    int freq;
    Node *l, *r;

    Node(char c, int f)
        : character(c)
        , freq(f)
        , l(nullptr)
        , r(nullptr)
    {
    }
};

// Structure for min heap
struct Min_Heap {
    int size;
    vector<Node*> array;

    Min_Heap(int s)
        : size(s)
        , array(s)
    {
    }
};

// Function to create min heap
Min_Heap* createAndBuildMin_Heap(char arr[], int freq[],
                                 int unique_size)
{
    int i;

    // Initializing heap
    Min_Heap* Min_Heap = new Min_Heap(unique_size);

    // Initializing the array of pointers in minheap.
    // Pointers pointing to new nodes of character
    // and their frequency
    for (i = 0; i < unique_size; ++i) {
        Min_Heap->array[i] = new Node(arr[i], freq[i]);
    }

    int n = Min_Heap->size - 1;
    for (i = (n - 1) / 2; i >= 0; --i) {
        // Standard function for Heap creation
        Heapify(Min_Heap, i);
    }

    return Min_Heap;
}
// Function to build Huffman Tree
struct Node* buildHuffmanTree(char arr[], int freq[],
                              int unique_size)
{
    struct Node *l, *r, *top;
    while (!isSizeOne(Min_Heap)) {
        l = extractMinFromMin_Heap(Min_Heap);
        r = extractMinFromMin_Heap(Min_Heap);
        top = newNode('$', l->freq + r->freq);
        top->l = l;
        top->r = r;
        insertIntoMin_Heap(Min_Heap, top);
    }
    return extractMinFromMin_Heap(Min_Heap);
}
// Structure to store codes in compressed file
typedef struct code {
    char k;
    int l;
    int code_arr[16];
    struct code* p;
} code;

// Function to print codes into file
void printCodesIntoFile(int fd2, struct Node* root,
                        int t[], int top = 0)
{
    int i;
    if (root->l) {
        t[top] = 0;
        printCodesIntoFile(fd2, root->l, t, top + 1);
    }

    if (root->r) {
        t[top] = 1;
        printCodesIntoFile(fd2, root->r, t, top + 1);
    }

    if (isLeaf(root)) {
        data = (code*)malloc(sizeof(code));
        tree = (Tree*)malloc(sizeof(Tree));
        data->p = NULL;
        data->k = root->character;
        tree->g = root->character;
        write(fd2, &tree->g, sizeof(char));
        for (i = 0; i < top; i++) {
            data->code_arr[i] = t[i];
        }
        tree->len = top;
        write(fd2, &tree->len, sizeof(int));
        tree->dec
            = convertBinaryToDecimal(data->code_arr, top);
        write(fd2, &tree->dec, sizeof(int));
        data->l = top;
        data->p = NULL;
        if (k == 0) {
            front = rear = data;
            k++;
        }
        else {
            rear->p = data;
            rear = rear->p;
        }
    }
}
// Function to compress file
void compressFile(int fd1, int fd2, unsigned char a)
{
    char n;
    int h = 0, i;

    // Codes are written into file in bit by bit format
    while (read(fd1, &n, sizeof(char)) != 0) {
        rear = front;
        while (rear->k != n && rear->p != NULL) {
            rear = rear->p;
        }
        if (rear->k == n) {
            for (i = 0; i < rear->l; i++) {
                if (h < 7) {
                    if (rear->code_arr[i] == 1) {
                        a++;
                        a = a << 1;
                        h++;
                    }
                    else if (rear->code_arr[i] == 0) {
                        a = a << 1;
                        h++;
                    }
                }
                else if (h == 7) {
                    if (rear->code_arr[i] == 1) {
                        a++;
                        h = 0;
                    }
                    else {
                        h = 0;
                    }
                    write(fd2, &a, sizeof(char));
                    a = 0;
                }
            }
        }
    }
    for (i = 0; i < 7 - h; i++) {
        a = a << 1;
    }
    write(fd2, &a, sizeof(char));
}
