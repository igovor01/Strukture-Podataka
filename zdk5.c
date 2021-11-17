/*

RADI

5. Napisati program koji iz datoteke čita postfiks izraz i zatim korištenjem stoga računa
rezultat. Stog je potrebno realizirati preko vezane liste.

*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_LINE (1024)

struct _StackElement;
typedef struct _StackElement* Position;
typedef struct _StackElement {
    double number;
    Position next;
}StackElement;

Position CreateElement(double number);
int InsertAfter(Position position, Position newElement);
int Push(Position head, double number);
int DeleteAfter(Position position);
int Pop(double* resultDestination, Position head);
int PerformOperation(Position head, char operator);
int CalculatePostfixFromFile(double* resultDestination, char* fileName);
int DeleteAll(Position head);

Position CreateElement(double number){
    //stvaranje elementa koji ce sadrzat broj
    Position newElement = NULL;

    newElement = (Position)malloc(sizeof(StackElement));
    if(!newElement){
        perror("Can't allocate memory!\n");
        return NULL;
    }

    newElement->number=number;
    newElement->next=NULL;

    return newElement;
}

int InsertAfter(Position position, Position newElement){
    newElement->next=position->next;
    position->next = newElement;

    return EXIT_SUCCESS;
}

int Push(Position head, double number){
    
    //dodavanje novog elementa na pocetak liste(i uvijek cemo izbacivat i dodavat sa prvog mjesta)
    Position newElement = NULL;
    newElement = CreateElement(number); //stvorimo element sa poslanim podacima
    if(!newElement){
        return EXIT_FAILURE;
    }

    InsertAfter(head, newElement); //stvoreni element insertamo iza heada
    return EXIT_SUCCESS;
}

int DeleteAfter(Position position){
    //brisanje elementa, sluzit ce nam u pop-u, kad izbacujemo element iz liste
    Position temp= position->next;
    if(!temp){
        return EXIT_SUCCESS;
    }
    position->next =temp->next;
    free(temp);
    return EXIT_SUCCESS;
}

int Pop(double* resultDestination, Position head){
    // izbacujemo element iz liste, uvijek s prvog mjesta, i po pokazivacu vratimo njegov sadrzaj(vrijednost number) u funkciju u kojo pozovemo Pop
    Position first = head->next;

    if(!first){
        printf("Postfix not valid! Please check your file ");
        return -1;

    }
    *resultDestination = first->number; // zvjezdica - dereferenciranje
    DeleteAfter(head);

    return EXIT_SUCCESS;
}

int PerformOperation(Position head, char operator){
    //posalat cemo operator, i head liste koja bi sad vec trebala sadrzavat bar 2 broja ako je postfix valjan
    double operand1=0;
    double operand2=0;
    double result =0;
    int status1 = EXIT_SUCCESS;
    int status2 = EXIT_SUCCESS;

    status2 = Pop(&operand2, head); //izbacit cemo posljednji uneseni broj iz liste i spremit ga u varijablu operand2
    if(status2 != EXIT_SUCCESS){
        return -1;
    }

    status1 = Pop(&operand1, head); //izbacit cemo SADASNJI posljednji broj iz liste i spremit ga u varijablu operand1
    if(status1 != EXIT_SUCCESS){
        return -2;
    }

    //ovo ispod, izvodenje matematicke operacije
    switch (operator)
    {
    case '+':
        result = operand1 + operand2;
        break;
    case '-':
        result = operand1 - operand2;
        break;
    case '*':
        result = operand1 * operand2;
        break;
    case '/':
        if(operand2 == 0){
            perror("Can't divide with zero!\n");
            return -3;
        }
        result = operand1 / operand2;
        break;
    
    default:
        printf("This operation is not supported yet! Please pay and we will implement it.\n");
        return -4;
    }
    //ako je sve uspilo, napravili smo zadanu operaciju i spremili je u varijablu result onda returnamo Push(head, result)
    return Push(head, result); //returnat cemo ono sta returna Push, odnosno EXIT_FAILURE ili EXIT_SUCESS
    //a sami Push ce podatak iz varijable result spremiti u novi element koji ce dodat na pocetak nase liste/stoga
}

int CalculatePostfixFromFile(double* resultDestination, char* fileName){


   FILE* file = NULL;
   int fileLength = 0; //velicina datoteke ce se tu spremit
   char*  buffer = NULL;
   char*  currentbuffer = NULL;
   int numBytes = 0;
   double number = 0;
   int status = 0;
   char operation = 0;
   StackElement head = {.next = NULL, .number =0}; //tek ovdje deklariramo head element stoga

    file = fopen(fileName, "rb");
    if(!file){
        perror("Can't open file!\n");
        return -1;
    }

    fseek(file, 0, SEEK_END); //trazi kraj datoteke(i dodemo do njega), od 0 do SEEK_END koji signalizira kraj datoteke(?)
    fileLength = ftell(file); //ftell - govori koliko smo udaljeni od pocetka datoteke, a s fseek smo se pomakli do kraja datoteke....
    // .....tako da nam govori velicinu datoteke pa tako i velicinu buffera koji cemo trebat alocirat
    
    buffer = (char*) calloc(fileLength +1, sizeof(char)); // alocira i stavlja sve bajtove na nulu
    //calloc prima koliko cemo mjesta alocirat (fileLength +1 zbog '\0') i koliko to svako mjesto zauzima memorije-sizeof(char))

    if(!buffer)
    {
        perror("Can't allocate memory");
        return -2;
    }

    rewind(file); //posto smo s fseek dosli do kraja datoteke, ovime se vracamo na pocetak
    fread(buffer, sizeof(char), fileLength, file); //kad fgets, samo sta on moze primit samo 1024 znaka, a ovo moze cijelu datoteku
    // buffer - ono u sto spremamo, file - ono iz cega citamo ono sta ce se spremit u buffer, fileLength - velicina ili koliko toga ima za procitat...
    //... sizeof(char) - velicina jedinicnog podatka koji cemo procitat
    printf("|%s|\n", buffer);
    fclose(file);

    currentbuffer = buffer;
    while(strlen(currentbuffer)>0) //vrtimo dok ne dodemo do kraja (current)buffera
    {
        status = sscanf(currentbuffer, " %lf %n", &number, &numBytes);  //skeniramo jedan realni broj, a numBytes vraca koliko smo pritom prosli mjesta stringa
        //status ce vratit 1 samo ako je ucitan tocno jedan, tocno %lf podatak
        
        if(status == 1)
        {
            status = Push(&head, number); //status od prije 2, 3 linije je odradia svoj posao, a sad u istu tu varijablu spremimo ono sto ce vratit Push...
            //... a to ce bit EXIT_SUCCESS ili EXIT_FAILURE
            //Push-u saljemo adresu heada i ucitani broj, a u njemu cemo stvorit novi element s tim brojem u sebi i dodat ga tocno iza heada
            if(status != EXIT_SUCCESS){
                //ako je Push vratia EXIT_FAILURE, znaci da je nesto krenilo po zlu, svu cemo memoriju oslobodit i returnat -3
                free(buffer);
                DeleteAll(&head);
                return -3;
            }
            currentbuffer +=numBytes; //ako je Push vratia EXIT_SUCCESS onda se pomicemo dalje po currentBufferu, i ucitavamo iduci broj/operaciju
        }
        else //if ucitani element nije bia %lf onda smo vjerojatno dosli do znaka operacije
        {

            sscanf(currentbuffer, " %c %n", &operation, &numBytes); //ponovit cemo sscanf sa %c jer to ocekujemo
            status = PerformOperation(&head, operation); //odradujemo matematicku operaciju u funkciji PerformOperation...
            //...jer smo dosli bas do operacije, pa cemo ukratko izbacit(pop-at) 2 prethodna broja iz liste i nad njima izvrsit unesenu operaciju
            if(status != EXIT_SUCCESS){

                free(buffer);
                DeleteAll(&head);
                return -4;
            }
            currentbuffer +=numBytes; //pomicemo se dalje po stringu i ucitavamo iduci broj/operaciju
        }
        //ponavljamo ucitavanje brojeva i operacijskih znakova dok ne dodemo do kraja stringa

    }
    free(buffer); //oslobodimo buffer, ne treba nam vise

    status = Pop(resultDestination, &head); //u ovom trenutku mi smo sve iz datoteke unijeli u listu, ali i izbacili, pa unijeli itd itd...
    //... sve u svrhu obavljanja matematickih operacija, i sada je na stogu ostao jos samo konacni rezultat, pa cemo za kraj...
    //...jos njega pop-at i njegovu vrijednost vratit preko pokazivaca, u varijablu resultDestination

    if(status != EXIT_SUCCESS)
    {
        //ako u pop-u nesto krenulo krivo, sve brisemo i returnamo 
        DeleteAll(&head);
        return -5;
    }

    if(head.next)
    {
        //jos za kraj, ako je head.next!=NULL onda nesto ne valja, odnosno ako head.next nesto sadrzi onda znaci da ono...
        //...sta smo u zadnjem Pop-u popali nije konacan rezultat nego smo nesto falili
        //Nama nakon tog zadnjeg Pop-a head.next mora bit NULL, ako jest onda je sve u redu, i ako jest onda necemo uc...
        //u ovaj if jer ce if izgledat ovako if(NULL) sta naravno nije TRUE ili istina ili "ista razlicito od 0"
        printf("Postfix not valid! Please check your file.");
        DeleteAll(&head);
        return -6;
    }

    return EXIT_SUCCESS;
}

int DeleteAll(Position head){
    //oslobada cilu listu
    while(head->next){
        DeleteAfter(head);
    }
}



int main(){
    
    double result;
    char filename[MAX_LINE] = {0};
    printf("Unesite ime datoteke: ");
    scanf(" %s", filename);
    
    CalculatePostfixFromFile(&result, "dat.txt");
    printf("Rezultat = %f\n", result);

	return EXIT_SUCCESS;
}