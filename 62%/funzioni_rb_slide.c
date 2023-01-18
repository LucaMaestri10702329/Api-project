void leftRotate(struct node * tree, struct node*  x){
    struct node * y = x->r;
    x->r = y->l;
    if(y->l != NULL){
        y->l->p = x;
    }
    y->p = x->p;
    if(x->p == NULL){
        tree = y;
    }
    else if(x == x->p->l){
        x->p->l = y;
    } else {
        x->p->r = y;
    }
    y->l = x;
    x->p = y;
}
void rightRotate(struct node * tree, struct node*  x){
    struct node * y = x->l;
    x->l = y->r;
    if(y->r != NULL){
        y->r->p = x;
    }
    y->p = x->p;
    if(x->p == NULL){
        tree = y;
    }
    else if(x == x->p->r){
        x->p->r = y;
    } else {
        x->p->l = y;
    }
    y->r = x;
    x->p = y;
}
void riparaRBInserisci(struct node * tree, struct node * z){
    while(z->p->c == 1){
        if(z->p == z->p->p->l){
            struct node * y = z->p->p->r;
            if(y->c == 1){
                z->p->c = 0;
                y->c = 0;
                z->p->p->c = 1;
                z = z->p->p;
            }else {
                if(z == z->p->r){
                    z = z->p;
                    leftRotate(tree,z);
                }
                z->p->c = 0;
                z->p->p->c = 1;
                rightRotate(tree,z);

            }
        }else{
            struct node * y = z->p->p->l;
            if(y->c == 1){
                z->p->c = 0;
                y->c = 0;
                z->p->p->c = 1;
                z = z->p->p;
            }else {
                if(z == z->p->l){
                    z = z->p;
                    rightRotate(tree,z);
                }
                z->p->c = 0;
                z->p->p->c = 1;
                leftRotate(tree,z);
            }
        }
    }
    tree->c = 0;
}