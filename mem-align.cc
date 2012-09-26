/*
 * =====================================================================================
 *
 *       Filename:  mem-align.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/25/2012 11:55:33 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dongyuchi (dongyuchi), dongyuchi@gmail.com
 *        Company:  UESTC
 *
 * =====================================================================================
 */
#include<iostream>
#include<sys/types.h>
using namespace std;
int main(){
    typedef struct T{
        char c;
        int64_t d;
        int e;
        short f;
        char g;
        short h;
    };
    cout<<"size:"<<sizeof(struct T)<<endl;
    return 0;
}
