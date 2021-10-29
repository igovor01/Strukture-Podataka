/*

RADI

2.
Definirati strukturu osoba (ime, prezime, godina rođenja) i napisati program koji:
A. dinamički dodaje novi element na početak liste,
B. ispisuje listu,
C. dinamički dodaje novi element na kraj liste,
D. pronalazi element u listi (po prezimenu),
E. briše određeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_SIZE (50)

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
int DeleteAfter(Position toDelete, Position head);

int main(int argc, char** argv){
    Person Head = { .next = NULL, .name = {0}, .surname = {0}, .birthYear = 0};
    
 /*   
    // isprobala sam ovime, i cini se da sve radi
    Position wantToDelete = NULL;

    PrependList(&Head, "ivana", "ivanic", 2001);
    PrependList(&Head, "mate", "matic", 2000);
    AppendList(&Head, "klara", "klaric", 1999);
    AppendList(&Head, "luka", "lukic", 2002);

    printf("Lista: ");
    PrintList(Head.next);

    wantToDelete = FindBySurname(Head.next, "klaric");
    DeleteAfter(wantToDelete, &Head);

    printf("\nLista nakon brisanja elementa: \n"); 
    PrintList(Head.next);
    
*/
    return EXIT_SUCCESS;
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
        printf("Name %s, surname %s, birthyear: %d\n", temp->name, temp->surname, temp->birthYear);
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

int DeleteAfter(Position toDelete, Position head){
    
    Position before = NULL;

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
