#include "../lisp.h"
#include "specific.h"


lisp* lisp_atom(const atomtype a)
{
    //Initialize when it is an atom
    lisp* l = (lisp*)malloc(sizeof(lisp));  
    l->type = ATOM;
    l->atom = a;
    l->car = NIL;
    l->cdr = NIL;
    return l;
}

lisp* lisp_cons(const lisp* l1, const lisp* l2)
{
    //type turns NOT ATOM, l1 means car, l2 means cdr
    lisp* l = (lisp*)malloc(sizeof(lisp));
    l->type = NATOM;
    l->car = (lisp*)l1;
    l->cdr = (lisp*)l2;
    return l;
}

lisp* lisp_car(const lisp* l)
{
    //if NOT ATOM return car
    if (l->type == ATOM){
        return NIL;
    }
    return l->car;
}

lisp* lisp_cdr(const lisp* l)
{
    //if NOT ATOM return cdr
    if (l->type == ATOM){
        return NIL;
    }
    return l->cdr;
}

atomtype lisp_getval(const lisp* l)
{
    //get val when ATOM
    if (l->type == NATOM){
        return 0;
    }
    return l->atom;
}

lisp* lisp_copy(const lisp* l)
{
    //NULL?
    if(l == NIL){
        return NIL;
    }
    //ATOM
    if (l->type == ATOM){
        return lisp_atom(l->atom);
    }
    //NOT ATOM
    return lisp_cons(lisp_copy(l->car), lisp_copy(l->cdr));
}

int lisp_length(const lisp* l)
{
    if (l == NIL){
        return 0;
    }
    if (l->type == ATOM){
        return 0;
    }
    return 1 + lisp_length(l->cdr);
}

void lisp_tostring(const lisp* l, char* str)
{
    //NULL return ()
    if(l == NIL){
        if(sprintf(str, BRACKETS) > 0){
            sprintf(str, BRACKETS);
            return;
        }
    }
    //ATOM return atom
    if (l->type == ATOM){
        if(sprintf(str, "%d", l->atom) > 0){
            sprintf(str, "%d", l->atom);
            return;
        }
    }
    lisp* head = (lisp*)l;
    if(sprintf(str, LEFTBRACKET) > 0){
        sprintf(str, LEFTBRACKET);
    }
    while(head != NIL){
        //car first
        if(head->car != NIL) {
            char* temp = (char*)malloc(TEMPSIZE);
            lisp_tostring(head->car, temp); 
            strcat(str, temp); 
            free(temp); 
        }
        //keep on cdr
        head = head->cdr; 
        if (head != NIL){
            strcat(str, SPACE); //add space behind str
        }  
    }
    strcat(str, RIGHTBRACKET);   //add ) behind str
}

void lisp_free(lisp** l)
{
    //NULL
    if(*l == NIL){
        return;
    }
    //ATOM
    if ((*l)->type == ATOM) {
        free(*l);
        *l = NIL;
        return;
    }
    lisp_free(&(*l)->car);
    lisp_free(&(*l)->cdr);
    free(*l);
    *l = NIL;
}

bool lisp_isatomic(const lisp* l){
    if(l == NIL){
        return false;
    }
    if(l->type == ATOM){
        return true;
    }
    return false;
}

lisp* lisp_fromstring(const char* str)
{
    //ATOM
    if (str[0] != LEFTBRACKET[0] && str[0] != SPACE[0]){
        atomtype a = 0;
        int flag = 0;
        int index = 0;
        //if negative, set flag to 1
        if(str[0] == NEGSIMPLE){
            flag = NEGATIVE;
            index++;
        }
        //get numbers
        while(str[index] != END && (str[index] >= ZERO && str[index] <= NINE)){
            a = a * 10 + (str[index] - ZERO);
            index++;
        }
        if(flag == NEGATIVE){
            a = -a; //negative
        }
        return lisp_atom(a);
    }
    //NATOM
    int i = 1;
    int length = strlen(str);
    lisp* car = NIL;
    lisp* cdr = NIL;
    while (i < length){
        //seperate car 
        if (str[i] == LEFTBRACKET[0]  &&  car == NIL){
            int index = i+1;
            int bracket = 1;
            //match ( and )
            while(bracket != 0){
                if(str[index] == LEFTBRACKET[0]){
                    bracket++;
                }
                if(str[index] == RIGHTBRACKET[0]){
                    bracket--;
                }
                index++;
            }
            index--;
            char* temp = (char*)malloc(index-i+2);
            strncpy(temp, str+i, index-i+1); //read the value form ()
            temp[index-i+1] = END;
            car = lisp_fromstring(temp); //lisp temp
            free(temp);
            i = index+1; //to one space after )
        }
        //cdr
        else if (str[i] == SPACE[0] || str[i] == LEFTBRACKET[0]){
            char* temp;
            if(str[i] == LEFTBRACKET[0]){
                temp = (char*)malloc(length-i+2);
                temp[0] = SPACE[0];
                strncpy(temp+1, str+i, length-i+1);
            }
            else{
                temp = (char*)malloc(length-i+1);
                strncpy(temp, str+i, length-i+1);
            }
            cdr = lisp_fromstring(temp);  // lisp temp
            free(temp);
            return lisp_cons(car, cdr); //cons again
        }
        else if (str[i] == RIGHTBRACKET[0]){
            i++;
        }     
        else{
            car = lisp_fromstring(str+i);
            i++; 
            //skip all the numbers
            while(str[i] >= ZERO && str[i] <= NINE){
                i++;
            }
        } 
    }
    
    if(car == NIL && cdr == NIL){
        return NIL;
    }
    return lisp_cons(car, cdr); //return a new cons
}

lisp* lisp_list(const int n, ...)
{
    va_list args;
    va_start(args, n);
    lisp* head = NIL;
    lisp* p;
    //add at the end
    for(int i = 0; i < n;i++){
        lisp* l = va_arg(args, lisp*);
        if(i == 0){
            head = lisp_cons(l, NIL);
            p = head;
        }
        else{
            p->cdr = lisp_cons(l, NIL);
            p = p->cdr;
        }
    }
    va_end(args);
    return head;
}

void lisp_reduce(void (*func)(lisp* l, atomtype* n), lisp* l, atomtype* acc)
{
    if(l == NIL){
        return;
    }
    if(l->type == ATOM){
        func(l, acc);   
        return;
    }
    if(l->type == NATOM && l->cdr == NIL){
        func(l->car, acc);
        return;
    }
    lisp_reduce(func, l->car, acc);
    lisp_reduce(func, l->cdr, acc);
}
void test(void){
    char str[LISTSTRLEN];
    lisp* l1 = cons(atom(2), NIL);
    assert(lisp_length(l1)==1);
    lisp_tostring(l1, str);
    assert(strcmp(str, "(2)")==0);

    lisp* l2 = cons(atom(1), l1);
    lisp_tostring(l1, str);
    lisp_tostring(l2, str);
    assert(strcmp(str, "(1 2)")==0);

    lisp* l3 = cons(atom(3), cons(atom(4), cons(atom(5), NIL)));
    lisp_tostring(l3, str);
    assert(strcmp(str, "(3 4 5)")==0);

    lisp* l4 = cons(l2, l3);
    lisp_tostring(l4, str);
    assert(strcmp(str, "((1 2) 3 4 5)")==0);

    lisp_free(&l4);

    lisp* b1 = fromstring("(1(2(3(4 5))))");
    lisp_tostring(b1, str);
    assert(strcmp(str, "(1(2(3(4 5))))"));
    lisp_free(&b1);

    lisp* g1 = lisp_list(3, atom(3), atom(2), atom(1));
    lisp* g2 = lisp_list(3, atom(1), atom(2), atom(3));
    lisp* g3 = lisp_list(2, g1, g2);
    lisp_tostring(g3, str);
    assert(strcmp(str, "((3 2 1) (1 2 3)"));
    lisp_free(&g3);

    lisp* h1 = lisp_fromstring("(1 2 99 78)");
    lisp_tostring(h1, str);
    assert(strcmp(str, "(1 2 99 78)")==0);
    lisp_free(&h1);
}
