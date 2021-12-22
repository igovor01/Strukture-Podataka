/*
8. Napisati program koji omogućava rad s binarnim stablom pretraživanja. Treba
omogućiti unošenje novog elementa u stablo, ispis elemenata (inorder, preorder, postorder i
level order), brisanje i pronalaženje nekog elementa.
*/

//popravit cu + level order

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct _Tree;
typedef struct _Tree * Position;
typedef struct _Tree {
    int El;
    Position L;
    Position R;
}Tree;

Position FindMin(Position pos);
//Position FindMax(Position pos);
Position CreateElement(int num);
Position AddNumberToTheTree(int num, Position pos);
Position DeleteElement(int num, Position pos);
Position FindElement(int num, Position pos);
int printInOrder(Position current);
int printPreOrder(Position current);
int printPostOrder(Position current);
int printLevelOrder(Position current);
int Menu(Position root);


Position FindMin(Position pos)
{
    if(NULL == pos)
        return NULL;
    else if(NULL == pos->L)
        return pos;
    else
        return FindMin(pos->L);
}
/*
Position FindMax(Position pos)
{
    if(NULL != pos)
        while(NULL != pos->R)
            pos=pos->R;
    return pos;
}
*/

Position CreateElement(int num)
{
    Position new;
    new = (Tree *) malloc(sizeof(Tree));
    if(!new)
    {
        printf("Failed to allocate memory");
        return NULL;
    }

    new->El= num;
    new->L = NULL;
    new->R = NULL;

    return new;
}

Position AddNumberToTheTree(int num, Position pos)
{
    if(NULL == pos)
        pos = CreateElement(num);
    else if(num < pos->El)
        pos->L = AddNumberToTheTree(num, pos->L);
    else if(num > pos->El)
        pos->R = AddNumberToTheTree(num, pos->R);
    else
        free(pos);

    return pos;
}

Position DeleteElement(int num, Position pos)
{
    Position temp = NULL;
    if(pos == NULL)
        printf("Element not found.");
    else if(num < pos->El)
        pos->L = DeleteElement(num, pos->L);
    else if(num > pos->El)
        pos->R = DeleteElement(num, pos->R);
    else if(pos->L != NULL && pos->R != NULL){
        temp= FindMin(pos->R);
        pos->El = temp->El;
        pos->R = DeleteElement(pos->El, pos->R);
    }
    else{
        temp= pos;
        if( NULL == pos->L )
            pos= pos->R;
        else
            pos = pos->L;
        free(temp);
    }
    return pos;
}

Position FindElement(int num, Position pos){
    if (pos == NULL)
        return NULL;
    else if(num < pos->El)
        return FindElement(num, pos->L);
    else if(num > pos->El)
        return FindElement(num, pos->R);
    else
        return pos; 
}

int printInOrder(Position current)
{
	if (current == NULL) 
		return 0; 

	printInOrder(current->L);
	printf("%d ", current->El);
	printInOrder(current->R);

	return 0;
}

int printPreOrder(Position current)
{
	if (current == NULL) 
		return 0; 
    
	printf("%d ", current->El);
	printInOrder(current->L);
	printInOrder(current->R);

	return 0;
}

int printPostOrder(Position current)
{
	if (current == NULL) 
		return 0; 
    
    printInOrder(current->L);
	printInOrder(current->R);
	printf("%d ", current->El);

	return 0;
}

/*int printLevelOrder(Position current)
{
    //nije napisano
	if (current == NULL) 
		return 0; 
    
	printf("%d ", current->El);

	return 0;
}*/

int Menu(Position root)
{
    int number, n, i;
    int odabir, broj;
    Position pozicija = NULL;
    printf("1 - Unos niza brojeva u stablo\n"
    "2 - Unos jednog broja u stablo\n"
    "3 - Brisanje elementa iz stabla\n"
    "4 - Trazenje elementa u stablu\n"
    "5 - Print stablo in-order\n"
    "6 - Print stablo pre-order\n"
    "7 - Print stablo post-order\n"
    "8 - Print stablo level-order\n"
    "0 - Izlaz iz izbornika\n"
    );
    printf("Vas odabir: ");
    scanf("%d", &odabir);
    printf("\n");

    switch (odabir)
    {
    case 1:
        printf("Broj elemenata koje zelite unijeti u stablo: ");
		scanf("%d", &n);
		for (i = 0; i < n; i++)
		{
			printf("%d. broj: ", i + 1);
			scanf("%d",&number);
            root = AddNumberToTheTree(number, root);

		}
        break;
    case 2:
        printf("Unesite element: ");
		scanf("%d",&number);
        root = AddNumberToTheTree(number, root);
        break;
    case 3:
        printf("Element koji zelite obrisati :");
        scanf("%d", &broj);
        root = DeleteElement(broj, root);
        break;
    case 4:
        printf("Element koji zelite pronaci:");
        scanf("%d", &broj);
        pozicija = FindElement(broj, root);
        break;
    case 5:  
        printf("Ispis stabla, in-order: ");
        printInOrder(root);
        break;
    case 6:  
        printf("Ispis stabla, pre-order: ");
        printPreOrder(root);
        break;
    case 7:  
        printf("Ispis stabla, post-order: ");
        printPostOrder(root);
        break;
    case 8:  
        printf("Ispis stabla, level-order: ");
        //printLevelOrder(root);
        break;
    case 0:
        return 0;
    default:
        break;
    }
}

int main()
{
    Tree root = {.El = 0, .L = NULL, .R = NULL};
    //Position root = NULL;
    int flag=1;
    while(flag!=0){
        flag= Menu(&root);
    }

}