/*
11.
Prepraviti zadatak 10 na način da se formira hash tablica država. Tablica ima 11 mjesta, a
funkcija za preslikavanje ključ računa da se zbraja ASCII vrijednost prvih pet slova države zatim
računa ostatak cjelobrojnog dijeljenja te vrijednosti s veličinom tablice. Države s istim ključem se
pohranjuju u vezanu listu sortiranu po nazivu države. Svaki čvor vezane liste sadrži stablo
gradova sortirano po broju stanovnika, zatim po nazivu grada.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define HASH_SIZE (11)
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




int ReadFromFile(char *filename, PositionList head, List HashTabl[]);
PositionTree ReadCitiesFromFile(char* filename);
int SortiraniUnosDrzavaUListu(PositionList new, PositionList head);
int InsertAfter(PositionList temp, PositionList new);
PositionTree SortiraniUnosStabla(char *ime, int brojStanovnika, PositionTree current);
int PrintList(PositionList head);
int PrintTreeInOrder(PositionTree current);

int PronadiGradoveSViseStanovnika(char* drzava, int broj, List HashTabl[]);
int TraziMax(PositionTree current);
PositionList PronadiDrzavu(char* drzava, PositionList head);
int PronadiPrintajGradove(int broj, PositionTree current);

int HashTablica(List HashTabl[],PositionList new);
int HashBroj(char* drzava);

int DeleteList(PositionList head);
int DeleteTree(PositionTree current);

int ReadFromFile(char *filename, PositionList head, List HashTabl[])
{
    char buffer[MAX_LINE] = { 0 };
    char fileGradovi[MAX_LINE] = { 0 };
    int number;
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
            
            HashTablica(HashTabl, temp);
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

int PronadiGradoveSViseStanovnika(char* drzava, int broj, List HashTabl[])
{
    int number = HashBroj(drzava);
    PositionList odabranaDrzava = PronadiDrzavu(drzava, &HashTabl[number]);
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
    PositionList temp = head;
    while(temp->next!=NULL && strcmp(temp->next->drzava,drzava) != 0)
        temp=temp->next;
    return temp->next;
}

int PronadiPrintajGradove(int broj, PositionTree current)
{
    if(current == NULL)
        return 0;
    PronadiPrintajGradove(broj, current->left);
    if(current->brStanovnika >= broj)
        printf("  %s --- %d\n", current->grad, current->brStanovnika);
    PronadiPrintajGradove(broj, current->right);

    return 0;
}

int HashTablica(List HashTabl[],PositionList new)
{
	int num = HashBroj(new->drzava);

    SortiraniUnosDrzavaUListu(new, &HashTabl[num]);

	return 0;
}

int HashBroj(char* drzava)
{
	int num = 0;
	num = drzava[0] + drzava[1] + drzava[2] + drzava[3] + drzava[4];

	return num % 11;
}

PositionList InitializeList()
{
    PositionList new = NULL;
    new = (List*)malloc(sizeof(List));
    strcpy(new->drzava ,  "");
    new->next = NULL;
    new->root = NULL;

    return new;

} 

int DeleteList(PositionList head)
{
    PositionList delete = NULL;

    while(head->next!=NULL)
    {
        delete = head->next;
        DeleteTree(delete->root);
        head->next = delete->next;
        free(delete);
    }

    return 0;
}

int DeleteTree(PositionTree current)
{
    if(current == NULL)
        return 0;
    DeleteTree(current->left);
    DeleteTree(current->right);
    free(current);

    return 0;
}

int main()
{
    List head = { .drzava = 0 ,  .root = NULL, .next = NULL };
    char drzava[MAX_LINE] = { 0 };
    int broj = 0;

    List HashTabl[HASH_SIZE];

    for (int i = 0; i < HASH_SIZE; i ++)
	{
        HashTabl[i] = *InitializeList();
	}

    ReadFromFile("drzave.txt", &head, HashTabl);
    
    for (int i = 0; i < HASH_SIZE; i++)
	{
		if (HashTabl[i].next != NULL)
		{
			PrintList(&HashTabl[i]);
		}
	}

    printf("\nPretrazivanje gradova koji imaju vise stanovnika od unesenog broja!\n");
    printf("Odaberite drzavu: ");
    scanf("%s", drzava);
    printf("Unesite broj: ");
    scanf("%d", &broj);
    PronadiGradoveSViseStanovnika(drzava, broj, HashTabl);
	
	for (int i = 0; i < HASH_SIZE; i++)
	{
        	DeleteList(&HashTabl[i]);
	}
    return 0;
}



