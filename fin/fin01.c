#include <stdio.h>
#include <stdint.h>

#include "poly.h"

int main()
{
    // f1(x) = x+1
    LIST_HEAD( poly01 );
    set_poly( &poly01, 1, 1 );
    set_poly( &poly01, 1, 0 );
    print_poly( &poly01 );
    
    // f1(x) = x^3+3x^2+3X^1+1
    LIST_HEAD( poly02 );
    set_poly( &poly02, 1, 3 );
    set_poly( &poly02, 3, 2 );
    set_poly( &poly02, 3, 1 );
    set_poly( &poly02, 1, 0 );
    print_poly( &poly02 );
    
    LIST_HEAD( poly03 );
    
    // add_poly( &poly03, &poly01, &poly02 );
    // print_poly( &poly03 );
    // del_poly( &poly03, &poly01, &poly02 );
    // print_poly( &poly03 );
    // mul_poly( &poly03, &poly01, &poly02 );
    // print_poly( &poly03 );
    // pow_poly( &poly03, &poly01, 2 );
    // print_poly( &poly03 );
    
    // printf( "%d\n", eval_poly( &poly01, 0 ) );  // 1
    
    // diff_poly( &poly03, &poly01 );
    // print_poly( &poly03 );  // f3(x) = 1
    
    // printf( "%f\n", int_poly( &poly01, 0, 2 ) );  // 4
    
    clear_poly( &poly01 );
    clear_poly( &poly02 );
    clear_poly( &poly03 );
    
    return 0;
}
