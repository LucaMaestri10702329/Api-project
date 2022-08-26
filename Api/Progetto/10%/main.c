#include<stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _node
{
    char *key;
    struct _node *left;
    struct _node *right;
} node;

void word_checker(char *rif, char *word,int dim)
{
    char temp[dim];

    if (strcmp(rif,word)==0)
    {
        printf("ok\n");
    }
    else
    {
        int i;
        for(i = 0; i<dim;i++)
        {
            temp[i] = rif[i];
        }
        for( i=0;i<dim;i++)
        {
            if(rif[i]==word[i])
                printf("+");
            else {
                int j;
                int check = 0;
                for(j=0;j<dim;j++)
                {
                    if(word[i] == temp[j])
                    {
                        printf("|");
                        temp[j] = '*';
                        check = 1;
                        break ;
                    }
                }
                if(check == 0)
                    printf("/");
            }
        }
        printf("\n");
    }
}

node* insertRec(node *tree, char *input,int dim)
{
    node *new=(node*) malloc(sizeof(node));
    new->key = malloc(dim * sizeof(char));
    strcpy(new->key, input);

    new->left=NULL;
    new->right=NULL;

    if(tree==NULL)
    {
        return new;
    }

    if((strcmp(tree->key, input)>=0)) tree->left=insertRec(tree->left, input,dim);

    else tree->right=insertRec(tree->right, input,dim);

    return tree;
}


void stampa_albero(node* tree)
{
    if(tree == NULL)
        return;

    stampa_albero(tree->left);
    printf("%s\n", tree->key);
    stampa_albero(tree->right);
}

node* ricerca_parola (node *bt, char * info)
{
    if((bt == NULL) || (strcmp(info,bt->key) == 0) )
        return bt;
    else
    {
        if(strcmp(info,bt->key)>0)
            return ricerca_parola(bt->right, info);
        else
            return ricerca_parola(bt->left,info);
    }
}


int main()
{
    unsigned int dim, err, tent, check2 = 0;
    node * bt = NULL;
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
            bt = insertRec(bt,p,dim);
        free(p);
    }

    //stampo l'albero delle parole
    printf("Stampa dell'albero: \n");
    stampa_albero(bt);

    //while(check1 != 1)
    //{
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

 // while(check2 != 3)
   // {
        char *par_let = malloc(dim*sizeof(char));
        fscanf(stdin, "%s", &(*par_let));
        //printf("%s\n",&(*par_let));
        if(par_let[0] == '+')
        {
            printf("%s\n",&(*par_let));
            if(par_let[1] == 's')
                stampa_albero(bt);
            else
            {
                if(par_let[1] == 'i')
                    if(check2 == 1)
                        check2 = 0;
                    else
                        check2 = 1;

            }
            if(par_let[1] == 'n')
                check2 = 3;
        }
        else
        {
            if(check2 == 1)
                bt = insertRec(bt,par_let,dim);
            else
            {
                if(ricerca_parola(bt,par_let) == NULL)
                    printf("not exists");
                else
                {
                    word_checker(par,par_let,dim);
                }
                tent --;
            }
        }
        free(par_let);
   //}
    //}

    return 0;


}