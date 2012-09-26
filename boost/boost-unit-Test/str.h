/*
 * =====================================================================================
 *
 *       Filename:  str.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/16/2012 11:06:01 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dongyuchi (dongyuchi), dongyuchi@gmail.com
 *        Company:  UESTC
 *
 * =====================================================================================
 */
#ifndef _MYSTRING_H
#define _MYSTRING_H
#include<string.h>
class mystring{
    char* buffer;
    int length;
    public:
        void setbuffer(char* s){
            buffer = s;
            length = strlen(s);
        }
        char& operator[](const int index){
            return buffer[index];
        }
        int size(){
            return length;
        }
};

#endif

