/*
 * =====================================================================================
 *
 *       Filename:  test.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/10/2012 12:34:48 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dongyuchi (dongyuchi), dongyuchi@gmail.com
 *        Company:  UESTC
 *
 * =====================================================================================
 */
#include"sock.h"
#include<iostream>
#include<string>
using namespace std;
int main()
{
    SOCK sock("192.168.1.12",1024);
    string ret=sock.ToString();
    cout<<ret<<endl;
    return 0;
}
