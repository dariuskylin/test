/*
 * =====================================================================================
 *
 *       Filename:  test.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/10/2012 02:54:26 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dongyuchi (dongyuchi), dongyuchi@gmail.com
 *        Company:  UESTC
 *
 * =====================================================================================
 */
#include<iostream>
using namespace std;
int main()
{
    int m=0;
    if(m++)
    {
        cout<<"+ yes"<<endl;
    }
    else
    {
        cout<<"+ no"<<endl;
    }
    cout<<"m="<<m<<endl;
    return 0;
}
