/*

RADI

3. Prethodnom zadatku dodati funkcije:
A. dinamički dodaje novi element iza određenog elementa,
B. dinamički dodaje novi element ispred određenog elementa,
C. sortira listu po prezimenima osoba,
D. upisuje listu u datoteku,
E. čita listu iz datoteke.

*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_SIZE (50)
#define MAX_LINE (1024)

struct _Person;
typedef struct _Person* Position;

typedef struct _Person{
    char name[MAX_SIZE];
    char surname[MAX_SIZE];
    int birthYear;
    Position next;

}Person;

int PrependList(Position head, char* name, char* surname, int birthYear); // ubacuje novu strukturu direktno iza head-a
Position CreatePerson(char* name, char* surname, int birthYear); // stvaramo i vraćamo osobu(aka onaj pravokutnik), sa svim njenim komponentama, ali je ne vezemo nigdje jos
int InsertAfter(Position position, Position newPerson); // inserta iza pozicije(koja sadrzi strukturu), a ono sto se inserta je newPerson struktura 
int AppendList(Position head, char* name, char* surname, int birthYear); // ubacuje novu strukturu skroz na kraj vezane liste
Position FindLast(Position head); //trazimo zadnju strukturu u vezanoj listu, koja za next ima NULL
int PrintList(Position first); // printamo cijelu listu
Position FindBySurname(Position first, char* surname);
Position FindBefore(Position wantedPerson, Position head);
int DeleteAfter(char *surname, Position head);

int AddListAfter(Position head, char* afterThisSurname, char *name, char *surname, int birthYear);
int AddListBefore(Position head, char* beforeThisSurname, char *name, char *surname, int birthYear);
int WriteIntoFile(Position head, char *datoteka);
int ReadFromFile(Position head, char *datoteka);
int SortedInput(Position head, Position newPerson);
int Menu(Position head);


int main(int argc, char** argv){
    Person Head = { .next = NULL, .name = {0}, .surname = {0}, .birthYear = 0};
   
    while(Menu(&Head)){}

    return 0;
}
int Menu(Position head){

    int odabir=0;
    int broj, i;
    char dat[MAX_LINE] = { 0 };
    char ime[MAX_LINE] = { 0 };
    char prezime[MAX_LINE] = { 0 }; 
    char prezime2[MAX_LINE] = { 0 };
    int godina;
    Position newPerson = NULL;

    printf("\nIzaberite jednu od opcija: \n");
    printf("1 - Ucitavanje studenata iz datoteke\n");
    printf("2 - Dodavanje studenata na pocetak liste\n");
    printf("3 - Dodavanje studenata na kraj liste\n"); 
    printf("4 - Dodavanje studenta nakon odredenog studenta       (po prezimenu)\n");
    printf("5 - Dodavanje studenta prije odredenog studenta       (po prezimenu)\n");
    printf("6 - Brisanje studenta po prezimenu\n");
    printf("7 - Upisivanje liste u datoteku\n");
    printf("8 - Printanje liste\n");
    printf("9 - Sortirani unos\n");
    printf("0 - Kraj odabiranja\n");
    printf("\nVas odabir: ");
    scanf("%d", &odabir);

    switch (odabir)
    {
    case 1:
        printf("-------- Ucitavanje studenata iz datoteke. --------\nUnesite datoteku oblika ime.txt: ");
        scanf(" %s", dat);
        PrintList(head->next);
        ReadFromFile(head, dat);
        PrintList(head->next);
        break;
    case 2:
        printf("-------- Dodavanje studenata na pocetak liste. --------\nUnesite studenta:\n ");
        printf("Ime: ");
        scanf(" %s", ime);
        printf("Prezime: ");
        scanf(" %s", prezime);
        printf("Godina rodenja: ");
        scanf(" %d", &godina);
        PrependList(head, ime, prezime, godina);
        break;
    case 3:
        printf("-------- Dodavanje studenata na kraj liste. --------\nUnesite studenta:\n ");
        printf("Ime: ");
        scanf(" %s", ime);
        printf("Prezime: ");
        scanf(" %s", prezime);
        printf("Godina rodenja: ");
        scanf(" %d", &godina);
        AppendList(head, ime, prezime, godina);
        break;
        
    case 4:
        printf("-------- Dodavanje studenta nakon odredenog studenta (po prezimenu) --------\nUnesite studenta:\n");
        printf("Ime: ");
        scanf(" %s", ime);
        printf("Prezime: ");
        scanf(" %s", prezime);
        printf("Godina rodenja: ");
        scanf(" %d", &godina);
        printf("Unesite prezime studenta nakon kojeg zelite dodati novounesenog studenta: ");
        scanf(" %s", prezime2);
        AddListAfter(head, prezime2, ime, prezime, godina);
        break;
    case 5:
        printf("-------- Dodavanje studenta prije odredenog studenta (po prezimenu) --------\nUnesite studenta:\n");
        printf("Ime: ");
        scanf(" %s", ime);
        printf("Prezime: ");
        scanf(" %s", prezime);
        printf("Godina rodenja: ");
        scanf(" %d", &godina);
        printf("Unesite prezime studenta ispred kojeg zelite dodati novounesenog studenta: ");
        scanf(" %s", prezime2);
        AddListBefore(head, prezime2, ime, prezime, godina);
        break;
    case 6:
        printf("-------- Brisanje studenta po prezimenu. -------- \nUnesite prezime studenta kojeg zelite izbrisati iz liste: ");
        scanf(" %s", prezime);
        DeleteAfter(prezime, head);
        break;
    case 7:
        printf("-------- Upisivanje liste u datoteku. --------\nUnesite datoteku oblika ime.txt: ");
        scanf(" %s", dat);
        WriteIntoFile(head, dat);
        break;
    case 8:
        printf("-------- Ispis liste: --------\n");
        PrintList(head->next);
        break;
    case 9:
        printf("-------- Sortirani ulaz, unos studenata, koji ce se smjestiti u listu uzlazno po prezimenima: --------\n");
        printf("Unesite studenta\n");
        printf("Ime: ");
        scanf(" %s", ime);
        printf("Prezime: ");
        scanf(" %s", prezime);
        printf("Godina rodenja: ");
        scanf(" %d", &godina);
        newPerson = CreatePerson(ime, prezime, godina);
        SortedInput(head, newPerson);
        printf("Rezultat sortiranog ulaza: ");
        PrintList(head->next);
        break;
    case 0:
        return 0;
    default:
        printf("Unijeli ste nedozvoljeni broj! ");
        break;
    }
    return 1;
}

int PrependList(Position head, char* name, char* surname, int birthYear){

    Position newPerson = NULL;

    newPerson= CreatePerson(name, surname, birthYear);
    if(!newPerson){
        return -1;
    }
    InsertAfter(head, newPerson);
    return EXIT_SUCCESS;
}

Position CreatePerson(char* name, char* surname, int birthYear){
    
    Position newPerson = NULL;

    newPerson = (Position)malloc(sizeof(Person));
    if(!newPerson){
        perror("Can't allocate memory!\n");
        return NULL;
    }
    
    strcpy(newPerson->name, name);
    strcpy(newPerson->surname, surname);
    newPerson->birthYear = birthYear;
    newPerson->next = NULL;

    return newPerson;
}

int InsertAfter(Position position, Position newPerson){
    
    //position je adresa strukture koja se vec nalazi u listi, newPerson je struktura koju zelimo dodat iza te position strukuture
    //bitno je da prvo u next komponentu newPersona-a stavimo position->next, pa tek onda u position->next stavimo newPerson i tako ih povezemo...
    //... jer zelimo da postoji veza izmedu position  i   newPerson  i  newPerson->next (used to be position->next)
    //... da smo zamjenili ove dvi linije, postavili bismo newPerson direktno iza positiona ali bi izgubili sve elemente liste...
    //... koje su se nalazile iza positiona jer im vise nemamo nigdje spremljenu adresu
    newPerson->next = position->next;
    position-> next = newPerson;

    return EXIT_SUCCESS;
}

int AppendList(Position head, char* name, char* surname, int birthYear){

    // incijaliziranje na NULL, dobra programerska praksa + postavljamo da se nalaze na adresi NULL(? mozda)
    Position newPerson = NULL;
    Position last = NULL;

    //stvaramo osobu po parametrima poslanima AppendList funkciji
    newPerson = CreatePerson(name, surname, birthYear);
    if(!newPerson){
        return -1;
    }
    
    //naci cemo zadnjeg i ubaciti listu iza njega
    last = FindLast(head);
    InsertAfter(last, newPerson);

    return EXIT_SUCCESS;
}

Position FindLast(Position head){
    
    Position temp = head;

    while(temp->next){
        temp = temp->next;
    }

    // while(temp->next) ODNOSNO while(temp->next!=NULL)
    // zadnji element liste u ->next-u ima "NULL" pa kad naidemo na temp ciji temp->next==NULL, izlazimo iz while petlje i returnamo bas taj temp...
    // ...jer je to ocito zadnji
    // dokle god ne nademo zadnji, temp->next!=NULL ispunjeno, radi se to da ce iduci temp bit jednak temp->next-u prethodnog

    return temp;
}

int PrintList(Position first){

    Position temp = first;

    //moze i sa for petljom, citljivije
    while(temp){
        printf("Name:  %s, surname:  %s, birthyear:  %d\n", temp->name, temp->surname, temp->birthYear);
        temp=temp->next;
    }

    // bitno je da idemo od prvog, ne od heada(head je prazan sto se podataka osim next-a tice, prvi tj. first je prvi iza heada)
    // printamo svaki i idemo dalje dok ne bude temp==NULL

    return EXIT_SUCCESS;
}

Position FindBySurname(Position first, char* surname)
{
    // i u ovoj i prethodnim funkcijama radimo ovo: iako je first poslan necemo s njim radit nego inicijaliziramo temp=first
    Position temp = first;

    // " If two strings are same then strcmp() returns 0 , otherwise, it returns a non-zero value. 
    // ...This function compares strings character by character using ASCII value of the characters"

    while (temp){
        if(strcmp(temp->surname, surname) == 0){
            return temp;
        }
        temp = temp->next;
    }

    // zelimo nac bas ovaj poslani argument char* surname, pa prolazimo po cijeloj listi i usporedujemo prezimena na listi sa tim poslanim...
    // ...prezimenom , ako ga nademo, returnamo adresu strukture(odnosno temp koji je tipa Position koji je tipa Person *) na kojoj je prezime nadeno
    // ...ako se prezime ne nade vracamo NULL

    return NULL;
}

Position FindBefore(Position wantedPerson, Position head){
    //trazimo prethodnika kako bi izbrisali neki element

    Position temp = head;

    while (temp->next != NULL && temp->next!=wantedPerson)
    {
        temp=temp->next;
    }

    // ako je sljedeci nakon temp-a(temp->next) onaj kojeg zelimo obrisat, onda bas taj temp trebamo i njega returnamo

    if(temp->next == NULL)
        return NULL; 
    else
        return temp;  

}

int DeleteAfter(char *surname, Position head){
    
    Position before = NULL;
    Position toDelete = NULL;

    toDelete = FindBySurname(head->next, surname);
    before = FindBefore(toDelete, head);
    if(!before){
        printf("Greska u brisanju!\n");

        return -1;
    }

    toDelete = before->next;
    before->next = toDelete->next;
    free(toDelete);

    return EXIT_SUCCESS;
}

int AddListAfter(Position head, char* afterThisSurname, char *name, char *surname, int birthYear){

    Position newPerson= NULL;
    Position afterThisOne = NULL;

    afterThisOne = FindBySurname(head->next, afterThisSurname);

    newPerson = CreatePerson(name, surname, birthYear);
    if(!newPerson)
    {
        return -1;
    }

    InsertAfter(afterThisOne, newPerson);

    return EXIT_SUCCESS;
}


int AddListBefore(Position head, char* beforeThisSurname, char *name, char *surname, int birthYear){

    Position newPerson = NULL;
    Position beforeThisOne =NULL;
    Position previous = NULL;

    newPerson= CreatePerson(name, surname, birthYear);
    if(!newPerson)
    {
        return -1;
    }

    beforeThisOne = FindBySurname(head->next, beforeThisSurname); 
    previous = FindBefore(beforeThisOne, head);

    InsertAfter(previous, newPerson);

    return EXIT_SUCCESS;
    
}

int SortedInput(Position head, Position newPerson){

    Position temp = head;

    while(temp->next != NULL && strcmp(temp->next->surname, newPerson->surname) <= 0){
        temp=temp->next;
    }
    InsertAfter(temp, newPerson);
    return EXIT_SUCCESS;
}


int WriteIntoFile(Position head, char* datoteka){

    FILE *pDat = NULL;
    Position temp = head;

    pDat= fopen(datoteka, "w");
    if (!pDat) 
	{  
		printf("Greska pri otvaranju datoteke\n");
		return -1;
	}

    while(temp->next != NULL)
    {
        fprintf(pDat, "%s %s %d\n", temp->next->name , temp->next->surname, temp->next->birthYear );
        temp= temp->next;
    }
    fclose(pDat);
    return EXIT_SUCCESS;

}
// radi
int ReadFromFile(Position head, char *datoteka){

    FILE *pDat = NULL;
    char buffer[MAX_LINE] = { 0 };
    char *name, *surname;
    int birthYear;
    Position newPerson = NULL;

    pDat= fopen(datoteka, "r");
    if (!pDat) 
	{  
		printf("Greska pri otvaranju datoteke\n");
		return -1;
	}

    while(!feof(pDat))
    {  
        newPerson =(Position)malloc(sizeof(Person));
        fgets(buffer, MAX_LINE, pDat);
        if(sscanf(buffer, " %s %s %d", newPerson->name, newPerson->surname, &newPerson->birthYear) == 3){
            SortedInput(head, newPerson);
        }
    }


    
    return EXIT_SUCCESS;
    
}
