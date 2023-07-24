#pragma once

#define NIL NULL
//0 means atom, 1 means not atom
#define ATOM 0
#define NATOM 1
#define NEGATIVE 1
#define TEMPSIZE 900
#define BRACKETS "()"
#define LEFTBRACKET "("
#define RIGHTBRACKET ")"
#define SPACE " "
#define NEGSIMPLE '-'
#define END '\0'
#define ZERO '0'
#define NINE '9'

#define atom(X)       lisp_atom(X)
#define cons(X, Y)    lisp_cons(X, Y)
#define car(L)        lisp_car(L)
#define cdr(L)        lisp_cdr(L)
#define copy(L)       lisp_copy(L)
#define fromstring(L) lisp_fromstring(L)
#define LISTSTRLEN 1000

struct lisp
{
    int type; //0表示原子节点，1表示表节点
    atomtype atom; // 原子节点
    lisp* car; 
    lisp* cdr;
};
