#include<stdio.h>
#include <stdlib.h>
#include <string.h>

struct Snode {
    char *key;;
    struct SNode *right;
    struct Snode *left;
};


typedef struct Snode TNode;
typedef TNode *TBinaryTree;


int conta_presenze(char c, char *rif, int dim) {

    for(int i=0;i<dim;i++)
    {
        printf("Caratteri a confronto: %c %c\n",c,rif);
        if(rif[i] == c)
        {
            rif[i] = "*";
            printf("Rif dopo la modifica %s\n", rif);
            return 1;
        }
    }
    return 0;
}
TBinaryTree binarytree_insert(TBinaryTree bt, char *info,int dim)
{
    TNode *new=(TNode*) malloc(sizeof(TNode));

    new->key = malloc(dim * sizeof(char));
    strcpy(new->key, info);

    new->left=NULL;
    new->right=NULL;

    if(bt==NULL)
    {
        return new;
    }

    if((strcmp(bt->key, info)>=0))
        bt->left=binarytree_insert(bt->left, info,dim);
    else
        bt->right=binarytree_insert(bt->right, info,dim);

    return bt;
}

void binarytree_visit(TBinaryTree bt)
{
    if(bt!=NULL)
    {
        binarytree_visit (bt->left);
        printf("%s\n",bt->key);
        binarytree_visit(bt->right);
    }
}


int word_checker(char *rif, char *word,int dim)
{
    if (strcmp(rif,word)==0)
    {
        printf("ok");
    }

    else
    {
        char *par_mod = malloc(dim*sizeof(char));
        strcpy(par_mod,rif);

       for(int i=0;i<dim;i++)
       {

           if(rif[i]==word[i])
               printf("+");
           else
           {
              if(conta_presenze(rif[i],par_mod,dim)==0)
                  printf("/");
              else
                  printf("|");

           }
       }
       printf("\n");
    }
}

int main()
{
    unsigned int dim, err, tent, add=0;
    TBinaryTree bt = NULL;

    //leggo il numero di parole da leggere
    err=fscanf(stdin, "%d", &dim);
    printf("Numero parole da leggere: %d\n", dim);

    //leggo le parole ammissibili e le aggiungo all'albero finché non trovo un comando
    while(1)
    {
        char *p = malloc(dim*sizeof(char));
        err = fscanf(stdin,"%s",&(*p));
        printf("%s\n",&(*p));

        if(p[0] == '+')
            break;
        else
            bt = binarytree_insert(bt,p,dim);
        free(p);
    }

    //stampo l'albero delle parole
    printf("Stampa dell'albero: \n");
    binarytree_visit(bt);

    //while(add != 4)
    //{
        //leggo la parola di riferimento
        char *par = malloc(dim*sizeof(char));
        err = fscanf(stdin,"%s",&(*par));
        printf("%s\n",&(*par));

        //leggo il numero di tentativi
        err=fscanf(stdin, "%d", &tent);
        printf("Numero tentativi: %d\n", tent);

        //leggo il comando successivo:
        //parola: faccio il confronto ed osservo i vincoli
        //+inserisci_inizio: da ora in poi ogni parola letta va aggiunta al numero di parole ammissibili
        //+inserisci_fine: finisco di aggiungere parole
        //+stampa_filtrate: stampa l'albero con le parole filtrate secondo i vincoli

        while(add != 3)
        {
            char *par_let = malloc(dim*sizeof(char));
            err=fscanf(stdin, "%s", &(*par_let));
            //printf("%s\n",&(*par_let));
            if(par_let[0] == '+')
            {
                printf("%s\n",&(*par_let));
                if(par_let[1] == 's')
                    binarytree_visit(bt);
                else
                {
                    if(par_let[1] == 'i')
                        if(add == 1)
                            add = 0;
                        else
                            add = 1;

                }
                if(par_let[1] == 'n')
                    add = 3;
            }
            else
            {
                if(add == 1)
                    bt = binarytree_insert(bt,par_let,dim);
                else
                {

                        word_checker(par,par_let,dim);
                }
            }
            free(par_let);

        }
    //}




    return 0;


}