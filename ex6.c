#include "ex6.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>

# define INT_BUFFER 128
/*******************
 *Name: Chen Bujor
 *ID: 322996976
 * Assignment: Ex6
 ******************/
// ================================================
// Basic struct definitions from ex6.h assumed:
//   PokemonData { int id; char *name; PokemonType TYPE; int hp; int attack; EvolutionStatus CAN_EVOLVE; }
//   PokemonNode { PokemonData* data; PokemonNode* left, *right; }
//   OwnerNode   { char* ownerName; PokemonNode* pokedexRoot; OwnerNode *next, *prev; }
//   OwnerNode* ownerHead;
//   const PokemonData pokedex[];
// ================================================
void clearBuffer()
{
    scanf("%*[^\n]");
    scanf("%*c");
}
// --------------------------------------------------------------
// 1) Safe integer reading
// --------------------------------------------------------------
void trimWhitespace(char *str)
{
    // Remove leading spaces/tabs/\r
    int start = 0;
    while (str[start] == ' ' || str[start] == '\t' || str[start] == '\r')
        start++;

    if (start > 0)
    {
        int idx = 0;
        while (str[start])
            str[idx++] = str[start++];
        str[idx] = '\0';
    }

    // Remove trailing spaces/tabs/\r
    int len = (int)strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\t' || str[len - 1] == '\r'))
    {
        str[--len] = '\0';
    }
}

char *myStrdup(const char *src)
{
    if (!src)
        return NULL;
    size_t len = strlen(src);
    char *dest = (char *)malloc(len + 1);
    if (!dest)
    {
        printf("Memory allocation failed in myStrdup.\n");
        return NULL;
    }
    strcpy(dest, src);
    return dest;
}

int readIntSafe(const char *prompt)
{
    char buffer[INT_BUFFER];
    int value;
    int success = 0;
    while (!success)
    {
        printf("%s", prompt);

        // If we fail to read, treat it as invalid
        if (!fgets(buffer, sizeof(buffer), stdin))
        {
            printf("Invalid input.\n");
            clearerr(stdin);
            continue;
        }
        // 1) Strip any trailing \r or \n
        //    so "123\r\n" becomes "123"
        size_t len = strlen(buffer);
        if (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r'))
            buffer[--len] = '\0';
        if (len > 0 && (buffer[len - 1] == '\r' || buffer[len - 1] == '\n'))
            buffer[--len] = '\0';
        // 2) Check if empty after stripping
        if (len == 0)
        {
            printf("Invalid input.\n");
            continue;
        }

        // 3) Attempt to parse integer with strtol
        char *endptr;
        value = (int)strtol(buffer, &endptr, 10);

        // If endptr didn't point to the end => leftover chars => invalid
        // or if buffer was something non-numeric
        if (*endptr != '\0')
        {
            printf("Invalid input.\n");
        }
        else
        {
            // We got a valid integer
            success = 1;
        }
    }
    return value;
}

// --------------------------------------------------------------
// 2) Utility: Get type name from enum
// --------------------------------------------------------------
const char *getTypeName(PokemonType type)
{
    switch (type)
    {
    case GRASS:
        return "GRASS";
    case FIRE:
        return "FIRE";
    case WATER:
        return "WATER";
    case BUG:
        return "BUG";
    case NORMAL:
        return "NORMAL";
    case POISON:
        return "POISON";
    case ELECTRIC:
        return "ELECTRIC";
    case GROUND:
        return "GROUND";
    case FAIRY:
        return "FAIRY";
    case FIGHTING:
        return "FIGHTING";
    case PSYCHIC:
        return "PSYCHIC";
    case ROCK:
        return "ROCK";
    case GHOST:
        return "GHOST";
    case DRAGON:
        return "DRAGON";
    case ICE:
        return "ICE";
    default:
        return "UNKNOWN";
    }
}

// --------------------------------------------------------------
// Utility: getDynamicInput (for reading a line into malloc'd memory)
// --------------------------------------------------------------
char *getDynamicInput()
{
    char *input = NULL;
    size_t size = 0, capacity = 1;
    input = (char *)malloc(capacity);
    if (!input)
    {
        printf("Memory allocation failed.\n");
        return NULL;
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        if (size + 1 >= capacity)
        {
            capacity *= 2;
            char *temp = (char *)realloc(input, capacity);
            if (!temp)
            {
                printf("Memory reallocation failed.\n");
                free(input);
                return NULL;
            }
            input = temp;
        }
        input[size++] = (char)c;
    }
    input[size] = '\0';

    // Trim any leading/trailing whitespace or carriage returns
    trimWhitespace(input);

    return input;
}
//
// Creating quotes for printing in Level order
//

//create the queue
Queue* createQueue() {
    Queue *queue = (Queue*)malloc(sizeof(Queue));
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}
//check if queue is empty
int isEmpty( Queue *queue)
{
    return (queue->front == NULL);
}
//add element to end of queue
void enQueue(Queue *queue, PokemonNode *node) {
    QueueNode *newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->next = NULL;
    newNode->data = node;
    if (queue->rear == NULL) {
        queue->front = newNode;
        queue->rear = newNode;
        return;
    }
    queue->rear->next = newNode;
    queue->rear = newNode;
}
PokemonNode* deQueue(Queue *queue) {
    if (isEmpty(queue))
    {
        printf("Queue Empty\n");
        return NULL;
    }
    QueueNode *temp = queue->front;
    queue->front = queue->front->next;
    if (queue->front == NULL)
        queue->rear = NULL;
    PokemonNode *node = temp->data;
    free(temp);
    return node;
}
//iterate over the entire queue and free it
void freeQueue(Queue *queue) {
    while (queue->front != NULL) {
        deQueue(queue);
    }
    free(queue);
}
// Function to print a single Pokemon node
void printPokemonNode(PokemonNode *node)
{
    if (!node)
        return;
    printf("ID: %d, Name: %s, Type: %s, HP: %d, Attack: %d, Can Evolve: %s\n",
           node->data->id,
           node->data->name,
           getTypeName(node->data->TYPE),
           node->data->hp,
           node->data->attack,
           (node->data->CAN_EVOLVE == CAN_EVOLVE) ? "Yes" : "No");
}

PokemonNode *searchPokemonBFS(PokemonNode *root, int id)
{
    if (!root)
        return NULL;
    Queue *queue = createQueue();
    enQueue(queue, root);

    while (queue->front != NULL)
    {
        PokemonNode *current = deQueue(queue);
        if (current->data->id == id)
        {
            freeQueue(queue);
            return current;
        }
        if(current->left)
            enQueue(queue, current->left);
        if(current->right)
            enQueue(queue, current->right);
    }
    free(queue);
    return NULL;
}
PokemonNode* findMax(PokemonNode *root)
{
    if (root == NULL)
        return NULL;
    PokemonNode *current = root;
    while (current && current->right != NULL)
        current = current->right;
    return current->right;
}
PokemonNode *insertPokemonNode(PokemonNode *root, PokemonNode *newNode)
{
    int newID = newNode->data->id;
    PokemonNode *cur = root;
    while (1)
    {
        if (newID == cur->data->id)
            break;
        if (newID < cur->data->id)
        {
            if(cur->left == NULL)
            {
                cur->left = newNode;
                return root;
            }
            else
                cur = cur->left;
        }
        else if (newID > cur->data->id)
        {
            if(cur->right == NULL)
            {
                cur->right = newNode;
                return root;
            }
            else
                cur = cur->right;
        }
    }
    return root;
}
int compareByNameNode(const void *a, const void *b) {
    PokemonNode *nodeA = *(PokemonNode **)a;
    PokemonNode *nodeB = *(PokemonNode **)b;
    // Compare the names alphabetically
    return strcmp(nodeA->data->name, nodeB->data->name);
}
PokemonNode *removeNodeBST(PokemonNode *root, int id)
{
    if (root==NULL)
        return root;
    if(id < root->data->id)
        root->left = removeNodeBST(root->left, id);
    else if(id > root->data->id)
        root->right = removeNodeBST(root->right, id);
    else
    {
        if(root->left == NULL)
        {
            PokemonNode *temp = root->right;
            freePokemonNode(root);
            return temp;
        }
        else if(root->right == NULL)
        {
            PokemonNode *temp = root->left;
            freePokemonNode(root);
            return temp;
        }
        PokemonNode *successor = root->right;
        while (successor->left != NULL)
            successor = successor->left;
        root->data->id = successor->data->id;
        free(root->data->name);
        root->data->name = myStrdup(successor->data->name);
        root->data->hp = successor->data->hp;
        root->data->attack = successor->data->attack;
        root->data->TYPE = successor->data->TYPE;
        root->data->CAN_EVOLVE = successor->data->CAN_EVOLVE;

        root->right = removeNodeBST(successor->right, successor->right->data->id);
        }
    return root;
}
void pokemonFight(OwnerNode *owner)
{
    if (!owner)
        return;
    if (!owner->pokedexRoot)
    {
        printf("Pokedex is empty.\n");
        return;
    }
    int first = readIntSafe("Enter ID of the first Pokemon: ");
    int second = readIntSafe("Enter ID of the second Pokemon: ");
    PokemonNode *fighterOne = searchPokemonBFS(owner->pokedexRoot, first);
    PokemonNode *fighterTwo = searchPokemonBFS(owner->pokedexRoot, second);
    if (!fighterOne || !fighterTwo)
    {
        printf("One or both Pokemon IDs not found.\n");
        return;
    }
    float PowerOne = (fighterOne->data->attack * 1.5) + (fighterOne->data->hp * 1.2);
    float PowerTwo = (fighterTwo->data->attack * 1.5) + (fighterTwo->data->hp * 1.2);
    printf("Pokemon 1: %s (Score = %.2f)\n", fighterOne->data->name, PowerOne);
    printf("Pokemon 2: %s (Score = %.2f)\n", fighterTwo->data->name, PowerTwo);
    float result = PowerOne - PowerTwo;
    if (result == 0)
        printf("It's a tie!\n");
    if(result > 0)
        printf("%s wins!", fighterOne->data->name);
    if(result < 0)
        printf("%s wins!", fighterTwo->data->name);
}
void freePokemonNode(PokemonNode *node)
{
    if (!node)
        return;
    if(node->data)
    {
        if (node->data->name)
        {
            free(node->data->name);
            node->data->name = NULL;
        }
        free(node->data);
        node->data = NULL;
    }
    free(node);
    node = NULL;
}
void freePokemonTree(PokemonNode *root);
void freeOwnerNode(OwnerNode *owner);
void displayBFS(PokemonNode *root)
{
    if (!root)
        return;
    Queue *queue = createQueue();
    enQueue(queue, root);

    while (queue->front != NULL)
    {
        PokemonNode *current = deQueue(queue);

        printPokemonNode(current);

        if(current->left)
            enQueue(queue, current->left);
        if(current->right)
            enQueue(queue, current->right);
    }
    free(queue);
}
void postOrderTraversal(PokemonNode *root)
{
    if (!root)
        return;
    //traverse left tree
    postOrderTraversal(root->left);
    //traverse right tree
    postOrderTraversal(root->right);
    //print current node
    printPokemonNode(root);
}
void inOrderTraversal(PokemonNode *root)
{
    if (!root)
        return;
    inOrderTraversal(root->left);
    printPokemonNode(root);
    inOrderTraversal(root->right);
}
//Uses recursion to print the tree with a priority to the left(smaller) side at every intersection
void preOrderTraversal(PokemonNode *root)
{
    if (!root)
        return;
    printPokemonNode(root);
    preOrderTraversal(root->left);
    preOrderTraversal(root->right);
}
void collectAllNodes(PokemonNode *root, Queue *queue)
{
    if (!root)
        return;
    enQueue(queue, root);
    if (root->left)
        collectAllNodes(root->left, queue);
    if (root->right)
        collectAllNodes(root->right, queue);
}
void sortPokemonNodesAlphabetically(Queue *queue)
{
    if (!queue || !queue->front || !queue->front->next)
        return;

    int swapped = 0;do
    {
        QueueNode *current = queue->front;
        swapped = 0;while (current->next != NULL)
        {
            if(strcmp(current->data->data->name, current->next->data->data->name) > 0)
            {
                PokemonNode *temp = current->data;
                current->data = current->next->data;
                current->next->data = temp;
                swapped = 1;
            }
            current = current->next;
        }
    }while(swapped);
}
void displayAlphabetical(PokemonNode *pokedexRoot)
{
    if (!pokedexRoot)
        return;
    int count = 0;
    Queue *queue = createQueue();
    collectAllNodes(pokedexRoot, queue);
    sortPokemonNodesAlphabetically(queue);
    QueueNode *current = queue->front;
    while (current != NULL)
    {
        printPokemonNode(current->data);
        current = current->next;
    }
    freeQueue(queue);
}

void freePokemon(OwnerNode *owner)
{
    if (!owner)
        return;
    if(!owner->pokedexRoot)
    {
        printf("No Pokemon to release.\n");
        return;
    }
    printf("Enter Pokemon ID to release: ");
    int ID;
    scanf("%d", &ID);
    printf("\n");
    owner->pokedexRoot = removeNodeBST(owner->pokedexRoot, ID);
}
//
// --------------------------------------------------------------
// Display Menu
// --------------------------------------------------------------
void displayMenu(OwnerNode *owner)
{
    if (!owner->pokedexRoot)
    {
        printf("Pokedex is empty.\n");
        return;
    }

    printf("Display:\n");
    printf("1. BFS (Level-Order)\n");
    printf("2. Pre-Order\n");
    printf("3. In-Order\n");
    printf("4. Post-Order\n");
    printf("5. Alphabetical (by name)\n");

    int choice = readIntSafe("Your choice: ");

    switch (choice)
    {
    case 1:
        displayBFS(owner->pokedexRoot);
        break;
    case 2:
        preOrderTraversal(owner->pokedexRoot);
        break;
    case 3:
        inOrderTraversal(owner->pokedexRoot);
        break;
    case 4:
        postOrderTraversal(owner->pokedexRoot);
        break;
    case 5:
        displayAlphabetical(owner->pokedexRoot);
        break;
    default:
        printf("Invalid choice.\n");
    }
}
void addPokemon(OwnerNode *owner)
{
    int newID = readIntSafe("Enter ID to add: ");
    if (checkDuplicateID(owner->pokedexRoot, newID))
    {
        printf("Pokemon with ID %d is already in the Pokedex. No changes made.\n",newID);
        return;
    }
    PokemonNode *node = createPokemonNode(&pokedex[newID-1]);
    if (!node)
        return;
    if(owner->pokedexRoot == NULL)
    {
        owner->pokedexRoot = node;
        return;
    }
    insertPokemonNode(owner->pokedexRoot, node);
}
void enterExistingPokedexMenu()
{
    OwnerNode *cur = ownerHead;
    // list owners
    if (cur == NULL)
    {
        printf("No existings Pokedexes.\n");
        return;
    }
    printf("\nExisting Pokedexes:\n");
    int i = 1;
    do{
        printf("%d. %s\n", i++, cur->ownerName);
    }while(cur->next != ownerHead);
    i = readIntSafe("Choose a Pokedex by number: ");
    cur = ownerHead;
    for(int j = 1 ; j < i ;j++)
        cur = cur->next;
    printf("\nEntering %s's Pokedex...\n", cur->ownerName);

    int subChoice;
    do
    {
        printf("\n-- %s's Pokedex Menu --\n", cur->ownerName);
        printf("1. Add Pokemon\n");
        printf("2. Display Pokedex\n");
        printf("3. Release Pokemon (by ID)\n");
        printf("4. Pokemon Fight!\n");
        printf("5. Evolve Pokemon\n");
        printf("6. Back to Main\n");

        subChoice = readIntSafe("Your choice: ");

        switch (subChoice)
        {
        case 1:
            addPokemon(cur);
            break;
        case 2:
            displayMenu(cur);
            break;
        case 3:
            freePokemon(cur);
            break;
        case 4:
            pokemonFight(cur);
            break;
        // case 5:
        //     evolvePokemon(cur);
        //     break;
        case 6:
            printf("Back to Main Menu.\n");
            break;
        default:
            printf("Invalid choice.\n");
        }
    } while (subChoice != 6);
}
int checkDuplicateID(PokemonNode *pokemonRoot, int ID)
{
    if (pokemonRoot == NULL)
        return 0;
    if (pokemonRoot->data->id == ID)
        return 1;
    if(ID < pokemonRoot->data->id)
        return checkDuplicateID(pokemonRoot->left, ID);
    else
        return checkDuplicateID(pokemonRoot->right, ID);
}
int checkDuplicateNames(char *name)
{
    int duplicate = 0;
    if (ownerHead == NULL)
        return duplicate;
    OwnerNode *cur = ownerHead;

    do
    {
        if (strcmp(cur->ownerName, name) == 0)
        {
            duplicate++;
            return duplicate;
        }
        cur = cur->next;
    }while(cur != ownerHead);
    return duplicate;
}
PokemonNode *createPokemonNode(const PokemonData *data)
{
    PokemonNode *node = (PokemonNode *)malloc(sizeof(PokemonNode));
    if (!node)
        return NULL;
    node->data = (PokemonData *)malloc(sizeof(PokemonData));
    node->data->id = data->id;
    node->data->name = data->name;
    node->data->hp = data->hp;
    node->data->attack = data->attack;
    node->data->TYPE = data->TYPE;
    node->data->CAN_EVOLVE = data->CAN_EVOLVE;
    node->left = NULL;
    node->right = NULL;
    return node;
}
void openPokedexMenu()
{
    printf("Your name: ");
    char *name = getDynamicInput();
    if (checkDuplicateNames(name))
    {
        printf("Owner '%s' already exists. Not creating a new Pokedex.\n", name);
        return;
    }
    printf("\nChoose Starter: \n");
    printf("1. Bulbasaur\n");
    printf("2. Charmander\n");
    printf("3. Squirtle\n");
    int choice = readIntSafe("Your choice: ");
    PokemonNode *starter;
    switch (choice)
    {
        case 1:
            starter = createPokemonNode(&pokedex[0]);
            break;
        case 2:
            starter = createPokemonNode(&pokedex[3]);
            break;
        case 3:
            starter = createPokemonNode(&pokedex[6]);
            break;
        default:
            printf("Invalid choice, I love Squirtle so have him.\n");//you did not ask for validation, so your errors are getting squirtles
            starter = createPokemonNode(&pokedex[6]);
            break;
    }
    OwnerNode *new = createOwner(name, starter);
    if (ownerHead == NULL)
    {
        ownerHead = new;
        ownerHead->next = ownerHead;
        ownerHead->prev = ownerHead;
    }
    else
    {
        OwnerNode *cur = ownerHead;
        while (cur->next != ownerHead)
            cur = cur->next;
        new->next = ownerHead;
        new->prev = cur;
        cur->next = new;
    }
}

OwnerNode *createOwner(char *name, PokemonNode *starter)
{
    OwnerNode *new = (OwnerNode *)malloc(sizeof(OwnerNode));
    if (!new)
        return NULL;
    new->ownerName = name;
    new->pokedexRoot = starter;
    new->next = NULL;
    new->prev = NULL;
    return new;
}
// --------------------------------------------------------------
// Main Menu
// --------------------------------------------------------------
void mainMenu()
{
    int choice;
    do
    {
        printf("\n=== Main MenuTEST ===\n");
        printf("1. New Pokedex\n");
        printf("2. Existing Pokedex\n");
        printf("3. Delete a Pokedex\n");
        printf("4. Merge Pokedexes\n");
        printf("5. Sort Owners by Name\n");
        printf("6. Print Owners in a direction X times\n");
        printf("7. Exit\n");
        choice = readIntSafe("Your choice: ");

        switch (choice)
        {
        case 1:
            // clearBuffer();
            openPokedexMenu();
            break;
        case 2:
            // clearBuffer();
            enterExistingPokedexMenu();
             break;
        // case 3:
        //     deletePokedex();
        //     break;
        // case 4:
        //     mergePokedexMenu();
        //     break;
        // case 5:
        //     sortOwners();
        //     break;
        // case 6:
        //     printOwnersCircular();
        //     break;
        // case 7:
        //     printf("Goodbye!\n");
        //     break;
        default:
            printf("Invalid.\n");
        }
    } while (choice != 7);
}
int main()
{
    mainMenu();
    // freeAllOwners();
    return 0;
}
