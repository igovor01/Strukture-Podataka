#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_LINE (1024)


//b) stablo država sortirano po nazivu, svaki čvor stabla sadrži vezanu listu gradova sortiranu po broju stanovnika, zatim po nazivu grada.

struct _List;
typedef struct _List* PositionList;
typedef struct _List
{
    int brStanovnika;
    char grad[MAX_LINE];
    PositionList next;
}List;

struct _Tree;
typedef struct _Tree* PositionTree;
typedef struct _Tree{
    char drzava[MAX_LINE];
    PositionList head;
    PositionTree left;
    PositionTree right;
}Tree;


PositionTree ReadFromFile(char *filename, PositionTree root);
int ReadCitiesFromFile(char* filename, PositionList head);
PositionList CreateNew(char* ime, int brStan);
int SortiraniUnosListe(PositionList new, PositionList head);
int InsertAfter(PositionList temp, PositionList new);
PositionList FindBefore(PositionList find, PositionList head);
PositionTree SortiraniUnosDrzavaUStablo(PositionTree current, PositionTree temp );
PositionList InitializeList();
int PrintList(PositionList head);
int PrintTree(PositionTree current);

int PronadiGradoveSViseStanovnika(char* drzava, int broj, PositionTree root);
PositionTree PronadiDrzavu(char* drzava, PositionTree current);

PositionTree ReadFromFile(char *filename, PositionTree root)
{
    char buffer[MAX_LINE] = { 0 };
    char fileGradovi[MAX_LINE] = { 0 };
    FILE *pDat;
    pDat=fopen(filename, "r");

    PositionTree tempTree = NULL;

    if(!pDat)
    {
        perror("Greska pri otvaranju datoteke\n");
		return NULL;
    }

    while(!feof(pDat))
    {  
        tempTree = NULL;
        tempTree = (Tree*)malloc(sizeof(Tree));
        tempTree->left= NULL;
        tempTree->right=NULL;
        tempTree->head = InitializeList();

        fgets(buffer, MAX_LINE, pDat);
 
        if(sscanf(buffer, " %s %s",  tempTree->drzava, fileGradovi) == 2)
        {
            ReadCitiesFromFile(fileGradovi, tempTree->head);
            
            root = SortiraniUnosDrzavaUStablo(root, tempTree); // dodala sam sve u podatke u jedan novi cvor stabla koji je jos nepovezan, onda poslala taj cvor i iz njega prebacila elemente u current cvor koji sam u ovoj funkciji bas dodala u listu
        }
    }

    fclose(pDat);

    return root;
}

int ReadCitiesFromFile(char* filename, PositionList head) 
{
    
    char buffer[MAX_LINE] = { 0 };
    char ime[MAX_LINE] = { 0 };
    int brojStanovnika = 0;
    FILE *pDat;
    pDat=fopen(filename, "r");

    PositionList new = NULL;

    if(!pDat)
    {
        perror("Greska pri otvaranju datoteke\n");
		return -1;
    }

    while(!feof(pDat))
    {  
        new= NULL;
        fgets(buffer, MAX_LINE, pDat);
        if(sscanf(buffer, " %s %d", ime, &brojStanovnika) == 2)
        {
            new = CreateNew(ime, brojStanovnika);
            SortiraniUnosListe(new, head);
            
        }
    }
    
    fclose(pDat);
    
}

PositionList CreateNew(char* ime, int brStan)
{
    PositionList new = NULL;
    new = (List*)malloc(sizeof(List));
    strcpy(new->grad,ime);
    new->brStanovnika = brStan;
    new->next = NULL;

    return new;
}

int SortiraniUnosListe(PositionList new, PositionList head)
{
    
    PositionList temp = head;
    
    while(temp->next!=NULL && temp->next->brStanovnika < new->brStanovnika)
    {
        temp = temp->next;
    }
    if(temp->next == NULL || temp->next->brStanovnika != new->brStanovnika)
    {
        InsertAfter(temp, new);
    }
    else if(temp->next->brStanovnika == new->brStanovnika)
    {
        if(strcmp(temp->next->grad, new->grad) < 0)
            InsertAfter(temp->next, new);
        else if(strcmp(temp->next->grad, new->grad) > 0)
            InsertAfter(temp, new);
        else
        {
            printf("Cannot have two same cities in a list");
            return 0;
        }
    }
    else
        free(temp);
    return EXIT_SUCCESS;
}

int InsertAfter(PositionList temp, PositionList new)
{
    new->next = temp->next;
    temp->next = new;
    return EXIT_SUCCESS;
}

PositionList FindBefore(PositionList find, PositionList head)
{
    PositionList temp = head;
    while(temp->next != NULL && temp->next != find)
        temp=temp->next;
    return temp;
}

PositionTree SortiraniUnosDrzavaUStablo(PositionTree current, PositionTree temp)
{
    if(current == NULL)
    {
        current = (Tree*)malloc(sizeof(Tree));
        strcpy(current->drzava, temp->drzava);
        current->head = temp->head;
        current->left = NULL;
        current->right = NULL; 
    }
    else if(strcmp(temp->drzava, current->drzava) < 0)
        current->left = SortiraniUnosDrzavaUStablo(current->left, temp);
    else if(strcmp(temp->drzava, current->drzava) > 0)
        current->right = SortiraniUnosDrzavaUStablo(current->right, temp);

    return current;
}

PositionList InitializeList()
{
    PositionList new = NULL;
    new = (List*)malloc(sizeof(List));
    new->brStanovnika = 0;
    strcpy(new->grad ,  "");
    new->next = NULL;

    return new;

}

int PrintTree(PositionTree current)
{
    if(current == NULL)
        return 0;
    PrintTree(current->left);
    printf("\n%s\n", current->drzava);
    PrintList(current->head);
    PrintTree(current->right);

    return 0;
}

int PrintList(PositionList head)
{
    PositionList temp = head->next; // temp ce za pocetak bit first
    while (temp != NULL)
    {
        printf("  %s --- %d\n", temp->grad, temp->brStanovnika );
        temp= temp->next;
    }
    return 0;
}

int PronadiGradoveSViseStanovnika(char* drzava, int broj, PositionTree root)
{
    
    PositionTree odabranaDrzava = NULL;
    odabranaDrzava = PronadiDrzavu(drzava, root);
    if(odabranaDrzava == NULL)
    {
        printf("Drzava %s nije pronadena.\n", drzava);
        return -1;
    }
    PositionList listOdabranaDrzava = odabranaDrzava->head;
    while(listOdabranaDrzava->next != NULL && listOdabranaDrzava->next->brStanovnika < broj)
        listOdabranaDrzava = listOdabranaDrzava->next;
    if(listOdabranaDrzava->next == NULL)
    {
        printf("Nisu pronadeni gradovi u %s koji imaju vise od %d stanovnika.\n", drzava, broj);
        return -2;
    }
    printf("\nGradovi u %s s vise od %d stanovnika:\n", drzava, broj);
    PrintList(listOdabranaDrzava);
    return 0;
    
}

PositionTree PronadiDrzavu(char* drzava, PositionTree current)
{
    PositionTree temp1 = NULL;
    PositionTree temp2 = NULL;
    if(current == NULL)
        return NULL;
    else if(strcmp(current->drzava, drzava) == 0)
        return current;
    temp1 = PronadiDrzavu(drzava, current->left);
    temp2 = PronadiDrzavu(drzava, current->right);
    if(temp1!=NULL)
        current = temp1;
    else if(temp2!=NULL)
        current = temp2;
    else 
        current = NULL;
    return current;
}

int main()
{
    PositionTree root = NULL;
    char drzava[MAX_LINE] = { 0 };
    int broj = 0;
    root = ReadFromFile("drzave.txt", root);
    PrintTree(root);

    printf("\nPretrazivanje gradova koji imaju vise stanovnika od unesenog broja!\n");
    printf("Odaberite drzavu: ");
    scanf("%s", drzava);
    printf("Unesite broj: ");
    scanf("%d", &broj);
    PronadiGradoveSViseStanovnika(drzava, broj, root);

    return 0;
}