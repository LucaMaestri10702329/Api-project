void word_checker_3(char *rif, char *word,node * bt,unsigned int dim, int stampa, char *vincoli, int non_presente[], int presente_n[][64],int presente_pos_sbagliata[][320]) {
    char * temp = malloc(sizeof(char)*dim);
    char * temp_rif = malloc(sizeof(char)*dim);
    char * output = malloc(sizeof(char)*dim);

    unsigned int i;

    memmove(temp,word,dim);
    memmove(temp_rif, rif,dim);
    //memmove(output,word,dim);

    for( i=0;i<dim-1;i++) {
        if (rif[i] == word[i]) {
            temp[i] = '+';
            output[i] = '+';
            temp_rif[i] = '+';
        } else
            output[i] = '/';
    }
    for(i=0;i<dim-1;i++) {
        if(output[i]=='/') {
            unsigned int j;
            for(j=0;j<dim-1;j++) {
                if(temp[i]==temp_rif[j]) {
                    output[i] = '|';
                    temp_rif[j] = '*';
                    break;
                }
            }
        }
    }

    output[dim-1] = '\0';
    printf("%s\n", output);
    for(i=0;i<dim-1;i++) {
        if(output[i]=='+')
            vincoli[i] = word[i];
        else {
            if(output[i]=='|') {
                unsigned int k, pres_word = 0;
                for(k=0;k<dim-1;k++) {
                    if(word[i] == word[k] && (output[k]!='/'))
                        pres_word ++;
                }
                unsigned int m = 0;
                while(presente_n[0][m]!=-1 && presente_n[0][m]!=word[i])
                    m++;
                presente_n[0][m] = word[i];
                presente_n[1][m] = pres_word;
                m = 0;
                while(presente_pos_sbagliata[0][m]!=-1)
                    m++;
                presente_pos_sbagliata[0][m] = word[i];
                presente_pos_sbagliata[1][m] = i;
            }
            else {
                unsigned int k, pres_rif = 0;
                for(k=0;k<dim-1;k++) {
                    if(word[i] == rif[k])
                        pres_rif++;
                }
                if(pres_rif == 0) {
                    unsigned int u=0;
                    while(non_presente[u]!=-1)
                        u++;
                    non_presente[u] = word[i];
                }

                else {

                    unsigned int m = 0;
                    while(presente_n[0][m]!=-1 && presente_n[0][m]!=word[i]) {
                        m++;
                    }
                    presente_n[0][m] = word[i];
                    presente_n[2][m] = pres_rif;

                    m = 0;
                    while(presente_pos_sbagliata[0][m]!=-1)
                        m++;
                    presente_pos_sbagliata[0][m] = word[i];
                    presente_pos_sbagliata[1][m] = i;
                }
            }
        }
    }
    free(temp);
    free(temp_rif);
    free(output);
}

node * applica_vincoli_vettori(node * bt, node * temp, unsigned int dim, char *vincoli, int non_presente[],int presente_n[][64],int presente_pos_sbagliata[][320], int  conta_filtrate[]) {
    if(bt != NULL) {
        unsigned int i, vincolata = 0;
        for (i = 0; i < dim - 1; i++) {
            if(vincoli[i]!=0 && vincoli[i]!=bt->key[i]) {

                vincolata = 1;
                break;
            }
            unsigned int o = 0;
            while(non_presente[o]!=-1) {
                unsigned int p;
                for(p=0;p<dim-1;p++) {
                    if(bt->key[p]==non_presente[o]) {

                        vincolata = 1;
                        break;
                    }
                }
                o++;
            }
            if(vincolata == 1)
                break;

            unsigned int j=0;
            while(presente_n[0][j]!=-1) {
                unsigned int m = 0, occorrenze = 0;
                for(m=0;m<dim-1;m++) {
                    if(presente_n[0][j]==bt->key[m])
                        occorrenze ++;
                }
                if(presente_n[2][j]!=-1 && presente_n[2][j]!=occorrenze) {
                    vincolata = 1;
                    break;
                }

                if(presente_n[1][j]!=-1 && presente_n[1][j]>occorrenze) {

                    vincolata = 1;
                    break;
                }
                j++;
            }
            if(vincolata == 1)
                break;
            j = 0;
            while(presente_pos_sbagliata[0][j]!=-1) {
                if(presente_pos_sbagliata[0][j] == bt->key[presente_pos_sbagliata[1][j]]) {

                    vincolata = 1;
                    break;
                }
                j++;
            }
            if(vincolata == 1)
                break;
        }
        if(vincolata == 1) {
            conta_filtrate[0] = conta_filtrate[0] - 1;
        }
        else
            temp = BST_insert_iterative(temp,bt->key,dim);

    }
    if(bt->right!=NULL)
        temp = applica_vincoli_vettori(bt->right,temp,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
    if(bt->left!=NULL)
        temp = applica_vincoli_vettori(bt->left,temp,dim,vincoli,non_presente,presente_n,presente_pos_sbagliata,conta_filtrate);
    return temp;
}
