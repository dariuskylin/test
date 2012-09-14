/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/10/2012 07:49:52 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dongyuchi (dongyuchi), dongyuchi@gmail.com
 *        Company:  UESTC
 *
 * =====================================================================================
 */
#include<stdlib.h>
void f(void){
    int *x = malloc(10*sizeof(int));
    x[10] = 0;
}
int main(void){
    f();
    return 0;
}
