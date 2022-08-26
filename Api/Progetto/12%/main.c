#include<stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _node
{
    char *key;
    struct _node *left;
    struct _node *right;
} node;


void word_checker(char *rif, char *word, int dim, node *filtrate)
{
    char temp[dim];
    int check = 0;
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
            {
                printf("+");
                //filtrate = ricerca_parola_pos_corretta(rif[i],i,filtrate);
            }

            else {
                int j;
                 check = 0;
                for(j=0;j<dim;j++)
                {
                    if(word[i] == temp[j])
                    {
                        printf("|");
                        //filtrate = ricerca_parola_pos_corretta(rif[i],filtrate);
                        temp[j] = '*';
                        check = 1;
                        break ;
                    }
                }
                if(check == 0)
                {
                    printf("/");
                    int k = 0, pres = 0;
                    /*for(k=0;k<dim;k++)
                    {
                        if(word[i] == rif[k])
                          pres = 1; break;
                    }
                     if(pres == 1)
                        filtrate = ricerca_parola_pos_non_corretta2
                    else
                        filtrate = ricerca_parola_non_contenuta
                     */
                }

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

    if((strcmp(tree->key, input)>=0))
        tree->left=insertRec(tree->left, input,dim);
    else
        tree->right=insertRec(tree->right, input,dim);

    return tree;
}
/*
void parola_da_eliminare(node *tree, char c, int pos, int dim)
{
    if(tree == NULL)
        return;

    parola_da_eliminare(tree->left);
    if(tree->key[pos] != c)
    {
        printf("Elimina nodo\n");
    }
    parola_da_eliminare(tree->right);
}
 */
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

node* binarytree_min (node *bt)
{
    if( bt == NULL)
        return NULL;
    else if(bt->left == NULL)
        return bt;
    else
    {
        node* min = binarytree_min(bt->left);
        return min;
    }
}

node* binarytree_delete(node* bt, char* info)
{
    if(bt == NULL)
        return NULL;
    else if(strcmp(bt->key,info)>0)
    {
        bt->left = binarytree_delete(bt->left,info);
        return bt;
    }
    else if(strcmp(info,bt->key)>0)
    {
        bt->right = binarytree_delete(bt->right, info);
        return bt;
    }
    else
    {
        node * min_right;
        if((bt->right == NULL) && (bt->left == NULL))
        {
           free(bt);
            return NULL;
        }
        if(bt->right == NULL)
        {
            node * alias;
            alias = bt->left;
            free(bt);
            return alias;
        }
        if(bt->left == NULL)
        {
            node * alias;
            alias = bt->right;
            free(bt);
            return alias;
        }
        min_right = binarytree_min(bt->right);

        bt->key = min_right->key;
        bt->right = binarytree_delete(bt->right, min_right->key);
        return bt;
    }
}

int main()
{
    unsigned int dim, err, tent, check2 = 0;
    node * bt = NULL;
    node * filtrate = NULL;
    //leggo il numero di parole da leggere
    err=fscanf(stdin, "%d\n", &dim);
    printf("Dimensione delle parole: %d\n", dim);

    //leggo le parole ammissibili e le aggiungo all'albero finché non trovo un comando
    while(1)
    {
        char *p = malloc(dim*sizeof(char));
        err = fscanf(stdin,"%s\n",&(*p));
        printf("%s\n",&(*p));

        if(p[0] == '+')
            break;
        else
            bt = insertRec(bt,p,dim);
            filtrate = insertRec(filtrate, p , dim);
        free(p);
    }

    //stampo l'albero delle parole
    printf("Stampa dell'albero: \n");
    stampa_albero(bt);

    //leggo la parola di riferimento
    char *par = malloc(dim*sizeof(char));
    err = fscanf(stdin,"%s\n",&(*par));
    printf("%s\n",&(*par));

    //leggo il numero di tentativi
    err=fscanf(stdin, "%d\n", &tent);
    printf("Numero tentativi: %d\n", tent);

    //leggo il comando successivo:
    //parola: faccio il confronto ed osservo i vincoli
    //+inserisci_inizio: da ora in poi ogni parola letta va aggiunta al numero di parole ammissibili
    //+inserisci_fine: finisco di aggiungere parole
    //+stampa_filtrate: stampa l'albero con le parole filtrate secondo i vincoli
    int i = 10;
    while(i>0)
    {
    char *par_let = malloc(dim*sizeof(char));
    //while(check2 != 3)
    //{
        i--;

        err=fscanf(stdin, "%s\n", &(*par_let));
        //printf("%s\n",&(*par_let));
        if(par_let[0] == '+')
        {

            if(par_let[1] == 's')
                stampa_albero(filtrate);
            else
            {
                if(par_let[1] == 'i')
                {
                    printf("%s\n",&(*par_let));
                    if(check2 == 1)
                        check2 = 0;
                    else
                        check2 = 1;
                }
            }
            if(par_let[1] == 'n')
                check2 = 3;
        }
        else
        {
            if(check2 == 1) {
                bt = insertRec(bt, par_let, dim);
                //printf("parola da aggiungere\n");
            }
            else
            {
                if(ricerca_parola(bt,par_let) == NULL)
                    printf("not exists\n");
                else
                {
                    word_checker(par,par_let,dim,filtrate);
                    tent --;

                }

            }

        }
        free(par_let);
        //if(tent == 0)
          //  printf("ko\n");
  }
  // }

    free(bt);
    free(par);
    free(filtrate);
    return 0;


}