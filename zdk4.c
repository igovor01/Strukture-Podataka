/*

RADI

4. Napisati program za zbrajanje i množenje polinoma. Koeficijenti i eksponenti se
čitaju iz datoteke.
Napomena: Eksponenti u datoteci nisu nužno sortirani.

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LINE 1024

struct _polinom;
typedef struct _polinom* Position;
typedef struct _polinom{
    int koef;
    int eksp;
    Position next;
}Polinom;

int ReadFromFile(char *dat, Position head1, Position head2);
int SpremanjePolinomaUListu(Position head, char *buffer); //funkcija koje ce iz nesortiranog stringa pravilno unijeti koeficijente i eksponente polinoma u listu
Position CreateElement(int koef, int eksp);
int SortedInput(Position head, Position newElement); //sortiramo uzlazno po eksponetima
int InsertAfter(Position pos, Position newElement);
int DeleteAfter(Position deleteAfterThis);
int PrintList(char *kojiPolinom, Position head);
int ZbrajanjePolinoma(Position head1, Position head2, Position headRez);
int MnozenjePolinoma(Position head1, Position head2, Position headRez);
int Oslobodi(Position head);

int main()
{
    Polinom head1 = { .koef = 0, .eksp = 0, .next = NULL};
    Polinom head2 = { .koef = 0, .eksp = 0, .next = NULL};
    Polinom headZbrojeni = { .koef = 0, .eksp = 0, .next = NULL};
    Polinom headPomnozeni = { .koef = 0, .eksp = 0, .next = NULL};
    char dat[MAX_LINE]= { 0 };
    printf("Unesite ime datoteke: ");
    scanf(" %s", dat);
    
    if(ReadFromFile(dat, &head1, &head2)== EXIT_SUCCESS){
        PrintList("stvaranje prvog polinoma", &head1);
        PrintList("stvaranje drugog polinoma", &head2);

        ZbrajanjePolinoma(&head1, &head2, &headZbrojeni);

        MnozenjePolinoma(&head1, &head2, &headPomnozeni);

        PrintList("zbrojanja polinoma", &headZbrojeni);
        PrintList("umnozak polinoma", &headPomnozeni);

    // oslobadanje memorije, provjereno radi
        Oslobodi(&head1);
        Oslobodi(&head2);
        Oslobodi(&headZbrojeni);
        Oslobodi(&headPomnozeni);
    }

    

    return 0;
}

int ReadFromFile(char *dat, Position head1, Position head2)
{
    FILE *pDat = NULL;
    char buffer[MAX_LINE] = { 0 };
    int status = 0; // ovo ima funkciju da zaustavi funkciju ako nesto krene lose u SpremanjePolinomaUListu

    pDat = fopen(dat, "r");
    if(!pDat)
    {
        perror("Greska pri otvaranju datoteke\n");
		return -1;
    }
    fgets(buffer, MAX_LINE, pDat); //unosimo prvu liniju
    status = SpremanjePolinomaUListu(head1, buffer);
    if (status != EXIT_SUCCESS){
		return EXIT_FAILURE;
    }

    fgets(buffer, MAX_LINE, pDat); //unosimo drugu liniju
    status = SpremanjePolinomaUListu(head2, buffer);
    if (status != EXIT_SUCCESS){
		return EXIT_FAILURE;
    }

    fclose(pDat);
    return EXIT_SUCCESS;
}

int SpremanjePolinomaUListu(Position head, char *buffer)
{
    char* currentBuffer = buffer;
    int koef = 0;
    int eksp = 0;
    int brMjesta = 0;
    int pomak=0;
    int status = 0;
    Position newElement = NULL;

    while(strlen(currentBuffer) > 1) // ovo je BITNO, dok je uvjet bia >0, nije mi radilo, sa >1 RADIII
    {
        status = sscanf(currentBuffer, " %d %d %n", &koef, &eksp, &brMjesta);
        
        if(status != 2)
        {
            //ovaj if ce bit istina ako imamo smo sa sscanfom procitali npr 2 podatka koji nisu valjanih tipova
            printf("Datoteka nije valjana.");
            return EXIT_FAILURE;
        }
        //ako nije greska u datoteci tj ako je proslo prethodni if, onda mozemo ic dalje stvarat element
        newElement = CreateElement(koef, eksp);
        if (!newElement)
		{
			return EXIT_FAILURE;
		}
        SortedInput(head, newElement);
        
        //posto je currentBuffer tipa char*, onda kad napisemo currentBuffer to oznacava i adresu cijelog niza znakova, ali i adresu prvog clana(currentBuffer+0)
        //zato, kada napisemo ovo ispod, currentBuffer= currentBuffer + brMjesta, ako smo u prethodnom citanju i sscanf-iranju ucitali iz buffera npr 1 2 iz...
        //... buffera koji sadrzi 1 2 8 9 15 12, onda ce brMjesta bit 4 a ova linija ispod ce izgledat ovako currentBuffer= currentBuffer + 4; odnosno ...
        // currentBuffer ce se smanjit i ovime ce sada pocinjat od svog prijasnjeg 4.elementa, i tako cemo ostvarit da hvatamo tocno dva po dva broja
        currentBuffer +=brMjesta;

    }
    return EXIT_SUCCESS;
}

Position CreateElement(int koef, int eksp)
{
    Position newElement = NULL;
    newElement= (Position)malloc(sizeof(Polinom));
    if(!newElement){
        perror("Can't allocate memory!\n");
        return NULL;
    }
    newElement->eksp=eksp;
    newElement->koef=koef;
    newElement->next=NULL;

    return newElement;
}

int SortedInput(Position head, Position newElement)
{
    Position temp = head;
    while(temp->next!=NULL && temp->next->eksp < newElement->eksp)
    {
        temp=temp->next;
    }
    

    // nakon izvrsavanja ovog while-a, postoje 3 mogucnosti. ili smo dosli do kraja i nismo nasli nista sto bi zaustavilo while drugim uvjeom...
    //ili smo nasli nesto sto ispunjava uvjet bas na nacin da je temp->next->eksp > newElement->eksp....
    //ILI smo nasli nesto sto ispunjava uvjet na nacin da je temp->next->eksp == newElement->eksp
    if(temp->next == NULL || temp->next->eksp != newElement->eksp)
    {
        /*
        ako imamo vec 5 clanova polinoma u listi i njihovi(vec sortirani) eksponenti su ovako poredani: 3 5 9 10 15
        ...a nas newElement->eksp iznosi 20
        onda nas je iz while petlje izbacio uvjet (temp->next!=NULL), pošto smo došli do kraja, a nije nađen nijedan već postojeći...
        ...koeficijent koji bi bio veci od ovog newElement->eksp(jer je prosao svaki uvjet da je svaki postojeći manji od newElement->eksp )...
        ..pa sad znamo da je taj novi ocito najveci od svih dosad videnih koeficijenata u listi pa ga mozemo ubacit iza tempa, ...
        ...a temp je zadnji element u ovoj situaciji

        ili se to dogodilo ILI
        ili smo imali 5 polinoma u listi u ekponentima 3 5 9 10 15 
        a nas newElement->eksp iznosi 7
        iz while petlje izbacio nas je drugi uvjet (temp->next->eksp < newElement->eksp) jer smo stvarno nasli postojeci eksponent koji nije manji
        od ovog novog newElement->eksp , naisli smo na ekspoenent 9, i izasli iz petlje.
        tada je temp na adresi petice(pa je zato temp->next jednaka adresa devetke)
        zato i ovdje idemo na insert after temp, temp je sada na petici a mi cemo ubaciti sedmicu iza njega, a time i ispred devetke

        */
       InsertAfter(temp, newElement);
    }
    else //if(temp->next->eksp == newElement->eksp)
    {
        //ulazak u ovaj if znaci da smo u vec postojecim eksponentima naisli na isti ovaj newElement->eksp 
        if(temp->next->koef + newElement->koef == 0)
        {
            //ako u listi imamo strukturu kojoj je koef=5, a eksp=7, to je 5 * x^7
            //ako nam newElement ima koef= -5, eksp=7, to je -5*x^7
            //zbrojeno to samo daje 0 i u nasm polinomu vise nemamo x sa eksponentnom 7
            DeleteAfter(temp);
        }
        else
        {
            temp->next->koef = temp->next->koef + newElement->koef;
            // inace samo u postojecu strukuturu s ovim eksponentom dodamo zbroj koeficijenata starog i novog
        }
    }

    return EXIT_SUCCESS;
}


int InsertAfter(Position pos, Position newElement)
{
    newElement->next=pos->next;
    pos->next=newElement;
    return EXIT_SUCCESS;
}

int DeleteAfter(Position deleteAfterThis)
{
    Position toDelete = NULL;

    toDelete = deleteAfterThis->next;
    deleteAfterThis->next= toDelete->next;
    free(toDelete);

    return EXIT_SUCCESS;
}

int PrintList(char *kojiPolinom, Position head)
{
    Position temp= head;
    printf("\nPrintamo rezultate operacije: %s\n", kojiPolinom);
    printf("KOEF x^EKSP\n");
    while(temp->next!=NULL){
        printf("%d x^%d\n", temp->next->koef, temp->next->eksp);
        temp=temp->next;
    }
    return EXIT_SUCCESS;
}

int ZbrajanjePolinoma(Position head1, Position head2, Position headRez)
{
    Position i = head1->next;
    Position j = head2->next;
    Position rez= headRez;

    Position temp= NULL;
    Position newElement = NULL;

    // zbrajanje sa idejom, dodaj cilu prvu listu sortiano u novu listu, i onda isto to i za drugu listu samo sta ce se sad ako se ponove eksponenti prve 
    //...liste u drugoj listii(tj.kad budemo dodavali 2.listu u novu listu ta nova vec iz prvog while-a sadrzi cijelu 1.listu, tako da je mozdda jasnije rec ako se u
    // novoj listi naide na ekspoenente iz 2.liste( koju tek dodajemo u novu) ), to ce se sve lipo zbrojit u SortedInputu
    while(i!=NULL)
    {
        newElement=CreateElement(i->koef, i->eksp); // tu je bila greska, poslan i->koef i j->eksp
        if (!newElement)
		{
			return EXIT_FAILURE;
		}
        SortedInput(rez, newElement);
        i=i->next;
    }
    while(j!=NULL)
    {
        newElement=CreateElement(j->koef, j->eksp);
        if (!newElement)
		{
			return EXIT_FAILURE;
		}
        SortedInput(rez, newElement);
        j=j->next;
    }

    return EXIT_SUCCESS;
}


int MnozenjePolinoma(Position head1, Position head2, Position headRez)
{
    Position i= head1->next;
    Position j= head2->next;
    Position newElement = NULL;
    
    //mnozimo svaki sa svakim i umnoske ubacujemo u novu listu, sortiano


    while(i!=NULL)
    {
        j=head2->next; //BITNO!!!! jer inace se ne radi, moramo se vratit na pocetak jota (kao sto bi bilo da su ovo dvije ugnjezdene for petlje, on bi se incijalizirao svaki put kad bi usli u unutrasnju petlju, tako mora i ovdje)
        while (j!=NULL)
        {
            newElement = CreateElement(i->koef * j->koef, i->eksp + j->eksp);
            if(!newElement)
            {
                return - EXIT_FAILURE;
            }
            SortedInput(headRez, newElement);
            
            j=j->next;
        }

        i=i->next;
    }
    return EXIT_SUCCESS;
}
int Oslobodi(Position head)
{
	Position temp = head;
    //stanemo na head element i ne micemo se, brisemo iza njega funkcijom DeleteAfter. Ta funkcija ovaj iduci izbrise i oslobodi prostor koji je zauzimao...
    //... i onda na njegovo mjesto stavi sljedbenika ovog izbrisanog. i tako se elementi prebacuju jedan po jedan na prvo mjesto(mjesto iza heada) gdje ih mi izbrisemo...
    //... sve dok ne dodemo do zadnjeg koji u svom ->next-u ima NULL, tada funkcija DeleteAfter bas taj NULL stavi iza head-a(nakon sta izbrise obog formalno zadnjeg)...
    //... i tada se ispuni uvjet da je temp->next== NULL tj. head->next ==NULL i tu je kraj

	while (temp->next != NULL)
	{
		DeleteAfter(temp);
	}
	
	return EXIT_SUCCESS;
}
