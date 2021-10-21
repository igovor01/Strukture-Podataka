/*
1. Napisati program koji prvo pročita koliko redaka ima datoteka, tj. koliko ima studenata
zapisanih u datoteci. Nakon toga potrebno je dinamički alocirati prostor za niz struktura
studenata (ime, prezime, bodovi) i učitati iz datoteke sve zapise. Na ekran ispisati ime,
prezime, apsolutni i relativni broj bodova.
Napomena: Svaki redak datoteke sadrži ime i prezime studenta, te broj bodova na kolokviju.
relatvan_br_bodova = br_bodova/max_br_bodova*100

*/
#include<stdio.h>
#include<stdlib.h>
#define MAX_ZNAKOVA 100

typedef struct studenti
{
    char ime[MAX_ZNAKOVA];
    char prezime[MAX_ZNAKOVA];
    double br_bodova;

} student;

student* AlocirajMemorijuZaStudente (int brRedaka, char * datoteka);
int IzbrojiRetke(char * datoteka);
void IspisStudenata(student* NizStudenata, int brRedaka);


int IzbrojiRetke(char* datoteka)
{
    int brRedaka=0;
    char buffer[1000] = { 0 };
    FILE *pDat=NULL;

    pDat=fopen(datoteka, "r");
    if (!pDat) 
	{  
		printf("Greska\n");
		return -1;
	}
    while(!feof(pDat))
    {   
        fgets(buffer, 1000, pDat);
        brRedaka++;
    }
    
    fclose(pDat);
    return brRedaka;
}

student* AlocirajMemorijuZaStudente (int brRedaka, char * datoteka)
{
    int i;
    FILE *pDat=NULL;
    student *NizStudenata=NULL;

    pDat=fopen(datoteka, "r");
    NizStudenata=(student*)malloc(brRedaka*sizeof(student));

    if (!pDat)
	{
		printf("Greska\n");
		free(NizStudenata);
		return NULL;
	}
    for(i=0 ; i<brRedaka ; i++)
    {
        fscanf(pDat, " %s %s %lf", NizStudenata[i].ime, NizStudenata[i].prezime, &NizStudenata[i].br_bodova);
        
    }

    fclose(pDat);


    return NizStudenata;
}

void IspisStudenata(student* NizStudenata, int brRedaka)
{
    int i;
    double relativan_br_bodova, max_br_bodova=0.0;
    for(i=0 ; i<brRedaka ; i++)
        if(NizStudenata[i].br_bodova > max_br_bodova)
                max_br_bodova = NizStudenata[i].br_bodova;

    printf("IME\tPREZIME\tBR BODOVA\tRELATIVAN BR BODOVA");  
    for(i=0 ; i<brRedaka ; i++)
    {
        relativan_br_bodova = NizStudenata[i].br_bodova / max_br_bodova * 100;
        printf("%s\t%s\t%lf\t%lf\n", NizStudenata[i].ime, NizStudenata[i].prezime, NizStudenata[i].br_bodova, relativan_br_bodova);
    }


}


int main()
{
    int brRedaka=0;
    student *NizStudenata=NULL;
    brRedaka= IzbrojiRetke("datoteka.txt");
    NizStudenata= AlocirajMemorijuZaStudente(brRedaka, "datoteka.txt");
    IspisStudenata(NizStudenata, brRedaka);
   
    return 0;
}