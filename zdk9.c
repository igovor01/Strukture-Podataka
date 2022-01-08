/*
9. Zadan je niz brojeva 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 koji su spremljeni u čvorove binarnog stabla.

a) Napisati funkciju insert koja dodaje element u stablo tako da se pozivima te funkcije za
sve element zadanog niza brojeva stvori stablo kao na slici Slika 1. Funkcije vraća
pokazivač na korijen stabla.

b) Napisati funkciju replace koja će svaki element stabla zamijeniti sumom elemenata u
njegovom lijevom i desnom podstablu (tj. sumom svih potomaka prije zamjene
vrijednosti u tim potomcima). Npr. stablo sa slike Slika 1 transformirat će se u stablo na
slici Slika 2.

c) Prepraviti program na način da umjesto predefiniranog cjelobrojnog polja korištenjem
funkcije rand() generira slučajne brojeve u rasponu <10, 90>. Također, potrebno je
upisati u datoteku sve brojeve u inorder prolasku nakon korištenja funkcije iz a), zatim b)
dijela zadatka.
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#define MAX_LINE (1024)

struct _Tree;
typedef struct _Tree * Position;
typedef struct _Tree {
    int element;
    Position left;
    Position right;
}Tree;

struct _List;
typedef struct _List * ListPosition;
typedef struct _List{
    int number;
    ListPosition next;
}List;

Position insert(int number, Position current);
int PrintInOrder(Position current);
int replace(Position current);
ListPosition FindLastInList(ListPosition head);
int AddTreeToList(ListPosition head, Position current);
int AddElementToList(ListPosition head, int number);
int WriteInFile(ListPosition head, char* filename);
int DeleteAll(ListPosition head);
Position InitializeTree(Position current);

//pod a
Position insert(int number, Position current)
{
    if(NULL == current)
    {
        current = (Position)malloc(sizeof(Tree));
        current->element = number;
        current->left = NULL;
        current->right = NULL; 
    }
    else if( number < current->element )
    {
        current->right = insert( number, current->right );
    }
    else if( number >= current->element )
    {
        current->left = insert( number, current->left );
    }
    return current;
}

int PrintInOrder(Position current)
{
    if(current == NULL)
    return 0;

    PrintInOrder(current->left);
    printf("%d ", current->element);
    PrintInOrder(current->right);
}


// pod b
int replace(Position current)
{
    Position temp = current;
    if(current->left == NULL && current->right == NULL)
        current->element = 0;
    else if(current->left == NULL && current->right != NULL)
        current->element = current->right->element + replace(current->right);
    else if(current->left != NULL && current->right == NULL)
        current->element = current->left->element + replace(current->left);
    else
        current->element = current->left->element + current->right->element + replace(current->left) + replace(current->right);

    return current->element;
    
}

//pod c, pisanje u datoteku

ListPosition FindLastInList(ListPosition head)
{

    ListPosition temp = NULL;
    temp = head;
    while(temp->next != NULL){
        temp = temp->next;
    }   
    return temp;
}


int AddTreeToList(ListPosition head, Position current)
{
    if(current == NULL)
        return 0;

    AddTreeToList(head, current->left);

    AddElementToList(head, current->element);

    AddTreeToList(head, current->right);

    return 0;
}
int AddElementToList(ListPosition head, int number)
{
    
    ListPosition temp = NULL;
    ListPosition pos = NULL;
    temp = (ListPosition)malloc(sizeof(List));
    if(!temp)
    {
        printf("Allocation not succesful.");
        return 0;
    }
    temp->number = number;

    pos = FindLastInList(head);
    temp->next = pos->next; // ********* greska je bila tu
    pos->next = temp;

}

int WriteInFile(ListPosition head, char* filename)
{
    
    ListPosition temp = head;
    FILE *pDat = NULL;
    pDat = fopen(filename, "a");
    while(temp->next!= NULL){
        fprintf(pDat, "%d ", temp->next->number);
        temp = temp->next;
    }
    fprintf(pDat, "\n");

    fclose(pDat);
    return 0;
}


int DeleteAll(ListPosition head)
{
    ListPosition temp = NULL;
    while(head->next != NULL)
    {
        temp = head->next;
        head->next=temp->next;
        free(temp);
    }
}

Position InitializeTree(Position current)
{
    if(current != NULL)
    {
        InitializeTree(current->left);
        InitializeTree(current->right);
        free(current);
    }
    return NULL;
}



int main()
{
    srand((unsigned) time(NULL));

    int array[] = { 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 };
    int i = 0, choice = 0, num = 0;
    char filename[MAX_LINE] = { 0 };
    Position root = NULL;
    List head = {.number = 0, .next = NULL};

    while(1){
    printf(
    "\n1 - Insert default array ( 2, 5, 7, 8, 11, 1, 4, 2, 3, 7) into the binary tree\n"
    "2 - Replaced every element of the default array with sum of the element's original left&right children\n"
    "3 - Inorder Print in the file\n"
    "4 - Create new tree with random numbers <10, 90>\n"
    "0 - Exit\n"
    "Your choice: ");

    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        root = InitializeTree(root);
        for(i = 0; i < sizeof(array)/sizeof(int); i++)
        {
            root = insert(array[i], root);
        }
        printf("Print In Order : ");
        PrintInOrder(root);
        printf("\n");
        break;
    case 2:
        replace(root);
        printf("Print In Order : ");
        PrintInOrder(root);
        printf("\n");
        break;
    case 3:
        printf("Insert filename: ");
        scanf("%s", filename); 
        AddTreeToList(&head, root); 
        WriteInFile(&head, filename);
        DeleteAll(&head);
        break;
    case 4: 
        root = InitializeTree(root);
        printf("Number of nodes in the new binary tree: ");
        scanf("%d", &num);
        for(i = 0; i < num; i++)
            root = insert( rand() %(90-10+1) +10 , root );
        printf("Print In Order : ");
        PrintInOrder(root);
        printf("\n");
        break;
    case 0: 
        return 0;
    default:
        break;
    }
    
    }

}


