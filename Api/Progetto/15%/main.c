#include<stdio.h>
#include <stdlib.h>
#include <string.h>

    typedef struct _node
    {
        char *key;
        struct _node *left;
        struct _node *right;
    } node;

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

    void word_checker(char *rif, char *word, int dim, node *filtrate)
    {
        char temp[dim];
        int check = 0;
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


    void stampa_albero(node* tree)
    {
        if(tree == NULL)
            return;

        stampa_albero(tree->left);
        printf("%s\n", tree->key);
        stampa_albero(tree->right);
    }

    node * binarytree_destroy(node * bt)
    {
        if( bt == NULL)
        {
            return NULL;
        }
        else if((bt->left == NULL) && (bt->right == NULL))
        {
            free(bt->key);
            free(bt);
            return NULL;
        }

        else
        {
            bt->left = binarytree_destroy(bt->left);
            bt->right = binarytree_destroy(bt->right);

            free(bt->key);
            free(bt);
            return bt;
        }
    }

    void deleteBinaryTree(node* bt)
    {
        // Caso base: albero vuoto
        if (bt == NULL) {
        return;
        }

        // elimina prima il sottoalbero sinistro e destro (Postorder)
        deleteBinaryTree(bt->left);
        deleteBinaryTree(bt->right);

        // elimina il nodo corrente dopo aver eliminato il suo sottoalbero sinistro e destro
        free(bt->key);
        free(bt);

        // imposta root come null prima di restituire
        bt = NULL;
    }


    int main()
    {
        unsigned int dim, err, tent, check2 = 0,check1=0;
        node * bt = NULL;
        node * filtrate = NULL;
        //leggo il numero di parole da leggere
        err=fscanf(stdin, "%d\n", &dim);
        printf("Dimensione delle parole: %d\n", dim);
        dim++;


        char c ;
        char *p = (char*) malloc(dim*sizeof(char));
        while(1)
        {

            fgets(p,dim,stdin);
            c = getchar_unlocked();
            //printf("%s\n",&(*p));

            if(p[0] == '+')
            {
                while(getchar_unlocked()!='\n')
                {
                    //printf("_%c",c);
                }
                //printf("\n");
                break;
            }

            else

                bt = insertRec(bt,p,dim);
                //bt = binarytree_destroy(bt);


        }

        printf("Stampa dell'albero: \n");
        stampa_albero(bt);
        char *par_let = malloc(dim*sizeof(char));

        while(check1==0) {
            //ripristino check2
            check2 = 0;


            //leggo la parola di riferimento
            //char *par = malloc(dim*sizeof(char));
            fgets(p, dim, stdin);
            c = getchar_unlocked();
            printf("Parola da indovinare: %s\n", &(*p));

            //leggo il numero di tentativi
            err = fscanf(stdin, "%d\n", &tent);
            printf("Numero tentativi: %d\n", tent);


            while (check2 != 3) {
                fscanf(stdin, "%s\n", &(*par_let));
                //printf("%s\n",&(*par_let));
                if (par_let[0] == '+') {

                    if (par_let[1] == 's')
                        stampa_albero(filtrate);
                    else {
                        if (par_let[1] == 'i') {
                            printf("%s\n", &(*par_let));
                            if (check2 == 1)
                                check2 = 0;
                            else
                                check2 = 1;
                        }
                    }
                    if (par_let[1] == 'n') {
                        check2 = 3;
                        printf("\nFine partita\n");
                    }

                } else {
                    if (check2 == 1) {
                        bt = insertRec(bt, par_let, dim);
                        //printf("parola da aggiungere\n");
                    } else {
                        if (ricerca_parola(bt, par_let) == NULL)
                            printf("not exists\n");
                        else {
                            if (strcmp(p, par_let) == 0) {
                                printf("ok\n");
                                check1 = 1;
                                check2 = 3;
                            } else {
                                word_checker(p, par_let, dim - 1, filtrate);
                                tent--;
                                if(tent == 0)
                                {
                                    printf("ko\n");
                                    check2 = 3;
                                }
                            }
                        }

                    }

                }
            }
        }
        deleteBinaryTree(bt);
        free(p);
        return 0;

    }