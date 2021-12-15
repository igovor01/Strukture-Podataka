/*
7. Napisati program koji pomoću vezanih listi (stabala) predstavlja strukturu direktorija.
Omogućiti unos novih direktorija i pod-direktorija, ispis sadržaja direktorija i
povratak u prethodni direktorij. Točnije program treba preko menija simulirati
korištenje DOS naredbi: 1- "md", 2 - "cd dir", 3 - "cd..", 4 - "dir" i 5 – izlaz.

!Jos samo izlaz, mozda probat drugacije
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_LINE (1024)


struct _Str;
typedef struct _Str* Position;
typedef struct _Str{
    char name[MAX_LINE]; //bitno! ovo mora bit char name[MAX_LINE] a ne char* name!!!!!!!!
    Position child;
    Position sibling;
}Str;

struct _StackElement;
typedef struct _StackElement* PositionStack;
typedef struct _StackElement {
    Position directory;
    PositionStack next;
}StackElement;



int Push(PositionStack headStack, Position directory);
PositionStack CreateStackElement(Position info);
int Pop(PositionStack headStack);
PositionStack FindLast(PositionStack headStack);
PositionStack FindBeforeLast(PositionStack headStack);
Position InsertDir(Position current, Position q);
Position CreateElement(char *name);
Position ChangeDir(Position current, char *naziv, PositionStack myStack);
int PrintDirectories(Position current);
int PrintStack(PositionStack headStack);
int FreeStack(PositionStack headStack);
int Menu(Position root, PositionStack myStack);



int Push(PositionStack headStack, Position directory){
    
    //dodavanje novog elementa na kraj liste(i uvijek cemo izbacivat i dodavat sa zadnjeg mjesta)
    PositionStack newElement = NULL;
    PositionStack last = FindLast(headStack);
    newElement = CreateStackElement(directory); //stvorimo element sa poslanim podacima
    if(!newElement){
        return EXIT_FAILURE;
    }

    newElement->next = last->next;
    last->next = newElement; //stvoreni element insertamo iza zadnjeg
    return EXIT_SUCCESS;
}

PositionStack CreateStackElement(Position info){
    PositionStack newElement = NULL;
    newElement=(PositionStack)malloc(sizeof(StackElement));
    if(!newElement){
        perror("Can't allocate memory!\n");
        return NULL;
    }
    newElement->directory = info;
    newElement->next = NULL;

    return newElement;

}

int Pop(PositionStack headStack){
    // izbacujemo element iz liste, prvo sam radila s prvog mjesta, ali zbog lakseg ispisa trenutnih direktorija, pop-at i push-at cu sa zadnjeg mjesta
    PositionStack toDelete= NULL;
    PositionStack before = FindBeforeLast(headStack);

    if(headStack->next->next== NULL){
        printf("Stack empty.\n");
        return 0;
    }
    //headStack = NULL;
    //headStack->next->name= "C:" (first->name = "C:")
    //headStack->next->next->name = ?? tj. u koju god mapu udemo. Ako je headStack->next->next == NULL, tj. ako nismo usli u nijednu
    //... mapu već smo samo u C:, onda ćemo reć da je Stack Empty i nastavit ce nam se ispisivat C:

    toDelete = before->next;
    before->next = toDelete->next; 
    free(toDelete); //pop-anje

    return EXIT_SUCCESS;
}
PositionStack FindLast(PositionStack headStack)
{
    PositionStack temp= headStack;
    while(temp->next!=NULL)
        temp=temp->next;
    return temp;
}

PositionStack FindBeforeLast(PositionStack headStack)
{
	PositionStack temp = headStack;
	while (temp->next->next != NULL)
		temp = temp->next;

	return temp;
}

Position InsertDir(Position current, Position q){

    //current je dijete npr roota
    if(current == NULL)// ako je dijete roota prazno onda vraćamo taj q koji ce se spremit na root.child
        return q;
    else if(strcmp(current->name, q->name) > 0){ //ako root nije prazan idemo sortirano unijeti, ako je recimo dijete koje postoji Marko, a unosimo Ana
    // onda ce ovaj if biti ispunjen, i mi cemo kazad da je Ana->sibling = Marko, i returnat cemo Ana, pa ce Ana postat direktno dijete roota
        q->sibling = current;
        return q;
    }
    else if(strcmp(current->name, q->name) < 0){  //ako imamo Ana Luka a zelimo upisat Marko=q
        current->sibling = InsertDir(current->sibling, q); // tada cemo morat insertat Marka(q->name) negdje iza Ane(tj. u Anin->sibling, tj current->sibling) pa idemo u rekurziju
        // sada je current = Ana, a current->sibling = Luka , opet cemo doc do ovoga pa cemo opet u rekurziju i tek cemo onda u 3. ulazu
        //.. u funkciju uc u prvi uvjet, current ce bit NULL pa cemo vratit da je Luka->sibling = q; onda Ana->sibling=Luka i to je to
    }
    else{
        printf("Directory already exists\n");
        free(q);
    }
    return current;
}

Position CreateElement(char *name){

    Position q = NULL;
    q=(Position)malloc(sizeof(Str));
    if(!q){
        perror("Can't allocate memory!\n");
        return NULL;
    }
    strcpy(q->name, name);
    q->sibling = NULL;
    q->child = NULL;
    return q;
}

Position ChangeDir(Position current, char *naziv, PositionStack myStack)
{
       
    Position childOfCurrent= current->child;

    if(childOfCurrent == NULL)
    {
        printf("Directory not found (Current directory is empty.)\n");
        return current;
    } 

    while(childOfCurrent != NULL)
    {
        if(strcmp(childOfCurrent->name, naziv)==0)
        {            
            Push(myStack, childOfCurrent);
            return childOfCurrent;
        }
        else{
            childOfCurrent = childOfCurrent->sibling;
        }
    }

    printf("Directory by that name not found.\n");
    return current;
    
}

int PrintDirectories(Position current)
{
    Position childOfCurrent= current->child;

    if(childOfCurrent == NULL)
    {
        printf("Current directory is empty\n");
        return 0;
    } 

    while(childOfCurrent != NULL)
    {
        printf("\t%s\n", childOfCurrent->name);
        childOfCurrent = childOfCurrent->sibling;
    }

    return 0;
}


int PrintStack(PositionStack headStack)
{
    PositionStack temp = headStack->next;
    printf("\n");

    while(temp!=NULL)
    {
        printf("%s\\", temp->directory->name);
        temp=temp->next;
    }
    printf(">");

    return 0;
}

int FreeStack(PositionStack headStack)
{
    PositionStack temp = NULL;
    while(headStack->next!=NULL)
    {
        temp = headStack->next;
        headStack->next = temp->next; 
        free(temp);
    }
    return 0;
}

int Menu(Position root, PositionStack myStack)
{
    Position new = NULL;
    Position current = root;
    char command[MAX_LINE] = { 0 };
    char name[MAX_LINE] = { 0 };
    
    Push(myStack, root);
    

    while(1)
    {
        PrintStack(myStack);
        printf("\nCommand: ");
        scanf(" %s", command);
        if(strcmp(command, "md")== 0)
        {
            printf("Directory name : ");
            scanf(" %s", name);
            new = CreateElement(name);
            current->child = InsertDir(current->child, new);
        }
        else if(strcmp(command, "cd")== 0)
        {
            printf("Directory name: ");
            scanf(" %s", name);
            current = ChangeDir(current, name, myStack);
        }
        else if(strcmp(command, "cd..")==0)
        {
            //funkcija za izlaz iz trenutnog direktorija
            if(Pop(myStack)==EXIT_SUCCESS)
                current = FindLast(myStack)->directory;
            else
                continue;
        }
        else if(strcmp(command, "dir")==0)
        {
            PrintDirectories(current);
            //funkcija za ispisati sve elemente trenutnog direktorija, child, i sve njegove siblingove
        }
        else if(strcmp(command, "exit")==0)
        {
            //trebala bi oslobodit cijelu memoriju? ili ne?
            //a i trebamo li uopce oslobadat svaki put memoriju koju zauzimaju...mape i slicno. idk
            FreeStack(myStack);
            return 0;
        }
        else
            printf("Command not recognized.\n");
        
    }
    return 0;
}

int main()
{
    Str root = { .name = "C:", .child = NULL, .sibling = NULL};
    StackElement myStack = { .directory = NULL, .next = NULL};

    Menu(&root, &myStack);
    
    
    return 0;
}


/*
    Primjer: 

    root
    |
    users
    |
    ana  -  luka  -  marko
            |
            mapa
            |
            papir - skare - kamen
*/