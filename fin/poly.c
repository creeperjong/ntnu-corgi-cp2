#include "poly.h"

void set_poly( struct list_head *pPoly, int32_t coefficient, int32_t degree ){
    
    sPolyTerm* new = (sPolyTerm*)malloc(sizeof(sPolyTerm));

    new->coefficient = coefficient;
    new->degree = degree;

    list_add(&(new->list), pPoly);

    return;
}

void print_poly( struct list_head *pPoly ){

    if(list_empty(pPoly)){
        printf("Invalid\n");
        return;
    }

    struct list_head* ptr = NULL;
    list_for_each_prev(ptr, pPoly){
        sPolyTerm* cptr = list_entry(ptr, sPolyTerm, list);

        if(cptr->coefficient == 0) continue;
        
        if(cptr->coefficient > 0){
            if(ptr != pPoly->prev){
                if(cptr->coefficient == 1){
                    if(cptr->degree == 0) printf("+ %d", cptr->coefficient);
                    else printf("+ ");
                }
                else printf("+ %d", cptr->coefficient);
            }
            else{
                if(cptr->coefficient != 1) printf("%d", cptr->coefficient);
            }
        }
        else{
            if(cptr->coefficient == -1){
                if(cptr->degree == -1) printf("- %d", cptr->coefficient);
                else printf("- ");
            }
            else printf("- %d", abs(cptr->coefficient));
        }

        if(cptr->degree == 1) printf("x ");
        else if(cptr->degree == 0) printf(" ");
        else printf("x^%d ", cptr->degree);
    }
    printf("\n");

    return;

}

void clear_poly( struct list_head *pPoly ){

    if(list_empty(pPoly)) return;

    sPolyTerm* pre = NULL;
    sPolyTerm* now = list_entry(pPoly->next, sPolyTerm, list);

    while(&now->list != pPoly){
        pre = now;
        now = list_entry(now->list.next, sPolyTerm, list);
        free(pre);
    }

    return;
}

// void diff_poly( struct list_head *pResult, struct list_head *pPoly ){

//     if(list_empty(pPoly)) return;

//     struct list_head* ptr = NULL;
//     list_for_each_prev(ptr, pPoly){
//         sPolyTerm* cptr = list_entry(ptr, sPolyTerm, list);

//         clear_poly(pResult);
//         set_poly(pResult, cptr->coefficient * cptr->degree, cptr->degree - 1);

//     }

//     return;
// }