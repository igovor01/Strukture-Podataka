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


Position insert(int number, Position root);
int PrintInOrder(Position current);
int replace(Position current);
int WriteInFile(ListPosition current, char* filename);
ListPosition FindLastInList(ListPosition head);
int AddElementToList(ListPosition head, int number);
int AddToList(ListPosition head, int number);
int PrintList(ListPosition head);
int DeleteAll(ListPosition head);

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



ListPosition FindLastInList(ListPosition head){

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

int PrintList(ListPosition head)
{
    while(head->next!= NULL)
    {
        printf("%d ", head->next->number);
        head=head->next;
    }
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
// pod c, rand

int RandomNumbers(Position root)
{
    int i=0;
    int a;
    srand((unsigned) time(NULL));
    a=rand()%(90-10+1) + 10;
    root = insert(a, root);
    printf("Rand is: %d\n", a);
    return 0;
}

int main()
{
    int array[] = { 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 };
    int i = 0, choice = 0, num = 0;
    char filename[1000];
    int *newArray;
    Position root = NULL;
    List head = {.number = 0, .next = NULL};
/*
    for(i = 0; i < sizeof(array)/sizeof(int); i++)
    {
        root = insert(array[i], root);
    }
    //pod a
    PrintInOrder(root);
    AddTreeToList(&head, root);
    printf("\n");
    WriteInFile(&head, "dat.txt");

    DeleteAll(&head);

    //pod b
    replace(root);
    PrintInOrder(root);
    AddTreeToList(&head, root);
    WriteInFile(&head, "dat.txt");

    //pod c
    for(i = 0; i < 10; i++)
    {
        //printf("%d", root->left);
        root = root = insert(rand() %(90-10+1) +10, root);
    }
    PrintInOrder(root);
*/

    while(1){
    printf(
    "1 - Insert default array ( 2, 5, 7, 8, 11, 1, 4, 2, 3, 7) into the binary tree\n"
    "2 - Replaced every element of the default array with sum of the element's original left&right children\n"
    "3 - Inorder Print in the file\n"
    "4 - Create new tree with random numbers <10, 90>\n"
    "0 - Exit\n"
    "Your choice: \n");

    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        for(i = 0; i < sizeof(array)/sizeof(int); i++)
        {
            root = insert(array[i], root);
        }
        PrintInOrder(root);
        AddTreeToList(&head, root);
        printf("\n");
        break;
    case 2:
        replace(root);
        PrintInOrder(root);
        AddTreeToList(&head, root);
        break;
    case 3:
        printf("Insert filename: ");
        scanf("%s", filename);
        WriteInFile(&head, filename);
        DeleteAll(&head);
        break;
    case 4: 
        printf("Number of nodes in the new binary tree: ");
        scanf("%d", &num);
        for(i = 0; i < num; i++)
            root = root = insert(rand() %(90-10+1) +10, root);
        PrintInOrder(root);
    case 0: 
        return 0;
    default:
        return 0;
    }
}
}


