#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_LINE (1024)

//a) vezana lista država sortirana po nazivu, svaki čvor liste sadrži stablo gradova sortirano po broju stanovnika, zatim po nazivu grada.
struct _Tree;
typedef struct _Tree* PositionTree;
typedef struct _Tree{
    int brStanovnika;
    char grad[MAX_LINE];
    PositionTree left;
    PositionTree right;
}Tree;
 //prvo mora ic ova deklaracija stabla pa liste jer u deklaraciji liste koristimo PositionTree
struct _List;
typedef struct _List* PositionList;
typedef struct _List
{
    char drzava[MAX_LINE];
    PositionTree root;
    PositionList next;
}List;




int ReadFromFile(char *filename, PositionList head);
PositionTree ReadCitiesFromFile(char* filename);
int SortiraniUnosDrzavaUListu(PositionList new, PositionList head);
int InsertAfter(PositionList temp, PositionList new);
PositionTree SortiraniUnosStabla(char *ime, int brojStanovnika, PositionTree current);
int PrintList(PositionList head);
int PrintTreeInOrder(PositionTree current);

int PronadiGradoveSViseStanovnika(char* drzava, int broj, PositionList head);
int TraziMax(PositionTree current);
PositionList PronadiDrzavu(char* drzava, PositionList head);
int PronadiPrintajGradove(int broj, PositionTree current);



int ReadFromFile(char *filename, PositionList head)
{
    char buffer[MAX_LINE] = { 0 };
    char fileGradovi[MAX_LINE] = { 0 };
    FILE *pDat;
    pDat=fopen(filename, "r");

    PositionList temp = NULL;

    if(!pDat)
    {
        perror("Greska pri otvaranju datoteke\n");
		return -1;
    }

    while(!feof(pDat))
    {  
        
        PositionList temp = NULL;
        temp = (List*)malloc(sizeof(List));
        temp->next = NULL;

        fgets(buffer, MAX_LINE, pDat);

        if(sscanf(buffer, " %s %s",  temp->drzava, fileGradovi) == 2)
        {
            temp->root = NULL;
            temp->root = ReadCitiesFromFile(fileGradovi);
            
            SortiraniUnosDrzavaUListu(temp, head);
        }
    }

    fclose(pDat);

    return 0;
}

PositionTree ReadCitiesFromFile(char* filename)
{
    char buffer[MAX_LINE] = { 0 };
    char ime[MAX_LINE] = { 0 };
    int brojStanovnika = 0;
    FILE *pDat;
    pDat=fopen(filename, "r");

    PositionTree root = NULL;

    if(!pDat)
    {
        perror("Greska pri otvaranju datoteke\n");
		return NULL;
    }

    while(!feof(pDat))
    {  
        fgets(buffer, MAX_LINE, pDat);
        if(sscanf(buffer, " %s %d", ime, &brojStanovnika) == 2)
        {
            root = SortiraniUnosStabla(ime, brojStanovnika, root);
        }
    }
    fclose(pDat);
    return root;
    
}

int SortiraniUnosDrzavaUListu(PositionList new, PositionList head)
{
    PositionList temp = head;
    
    while (temp->next != NULL && strcmp(temp->next->drzava, new->drzava) < 0)
    {
        temp=temp->next;
    }
    if(temp->next == NULL || strcmp(temp->next->drzava, new->drzava) != 0)
    {   
        InsertAfter(temp, new); 
    }
    else if(strcmp(temp->next->drzava, new->drzava) == 0)
    {
        //printf("Two of the same countries can not be added");
        return -1;
    }
    return EXIT_SUCCESS;
}

int InsertAfter(PositionList temp, PositionList new)
{
    new->next = temp->next;
    temp->next = new;
}

PositionTree SortiraniUnosStabla(char *ime, int brojStanovnika, PositionTree current)
{
    if(current == NULL)
    {
        current = (Tree*)malloc(sizeof(Tree));
        strcpy(current->grad, ime);
        current->brStanovnika = brojStanovnika;
        current->left = NULL;
        current->right = NULL; 
    }
    else if(brojStanovnika < current->brStanovnika)
        current->left = SortiraniUnosStabla(ime, brojStanovnika, current->left);
    else if(brojStanovnika > current->brStanovnika)
        current->right = SortiraniUnosStabla(ime, brojStanovnika, current->right);
    else if(brojStanovnika == current->brStanovnika)
    {
        if(strcmp(ime, current->grad) < 0)
            current->left = SortiraniUnosStabla(ime, brojStanovnika, current->left);
        else if(strcmp(ime, current->grad) > 0)
            current->right = SortiraniUnosStabla(ime, brojStanovnika, current->right);
        else
        {
            //printf("Two of the same cities can not be added");
            return current;
        }   
    }
    else
        free(current);

    return current;
}

int PrintList(PositionList head)
{
    PositionList temp = head->next;
    while(temp!=NULL)
    {
        
        printf("\n%s\n", temp->drzava);
        PrintTreeInOrder(temp->root);
        temp = temp->next;
    }
    return 0;

}

int PrintTreeInOrder(PositionTree current)
{
    if(current == NULL)
        return 0;
    
    PrintTreeInOrder(current->left);
    printf("  %s --- %d\n", current->grad, current->brStanovnika);
    PrintTreeInOrder(current->right);

    return 0;
}

int PronadiGradoveSViseStanovnika(char* drzava, int broj, PositionList head)
{
    //printf("\nGlavna funk - pocetak\n");
    PositionList odabranaDrzava = PronadiDrzavu(drzava, head);
    int max = 0;
    if(odabranaDrzava == NULL)
    {
        printf("Drzava %s nije pronadena.\n", drzava);
        return -1;
    }
    max = TraziMax(odabranaDrzava->root);
    if (broj > max)
    {
        printf("Nisu pronadeni gradovi u %s koji imaju vise od %d stanovnika.\n", drzava, broj);
        return -2;
    }
    
    printf("\nGradovi u %s s vise od %d stanovnika:\n", odabranaDrzava->drzava, broj);
    PronadiPrintajGradove(broj, odabranaDrzava->root);

    //printf("\nGlavna funk - kraj\n");
    return 0;
}

int TraziMax(PositionTree current)
{
    int maxi = 0;
    if(current == NULL)
        return 0;
    if(current->right == NULL)
        return current->brStanovnika;
    maxi = TraziMax(current->right);

    return maxi;
}

PositionList PronadiDrzavu(char* drzava, PositionList head)
{
    //printf("\nPronadi drzavu - pocetak\n");
    PositionList temp = head;
    while(temp->next!=NULL && strcmp(temp->next->drzava,drzava) != 0)
        temp=temp->next;
    //printf("\nPronadi drzavu - kraj\n");
    return temp->next;
}

int PronadiPrintajGradove(int broj, PositionTree current)
{
    //printf("\nPronadi grad - pocetak\n");
    if(current == NULL)
        return 0;
    PronadiPrintajGradove(broj, current->left);
    if(current->brStanovnika >= broj)
        printf("  %s --- %d\n", current->grad, current->brStanovnika);
    PronadiPrintajGradove(broj, current->right);
    //printf("\nPronadi grad - pocetak\n");

    return 0;
}


int main()
{
    List head = { .drzava = 0 ,  .root = NULL, .next = NULL };
    char drzava[MAX_LINE] = { 0 };
    int broj = 0;
    ReadFromFile("drzave.txt", &head);
    PrintList(&head);

    printf("\nPretrazivanje gradova koji imaju vise stanovnika od unesenog broja!\n");
    printf("Odaberite drzavu: ");
    scanf("%s", drzava);
    printf("Unesite broj: ");
    scanf("%d", &broj);
    PronadiGradoveSViseStanovnika(drzava, broj, &head);
    return 0;
}



