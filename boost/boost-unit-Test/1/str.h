/*
 * =====================================================================================
 *
 *       Filename:  str.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/17/2012 06:50:27 PM
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
class mystring{
    private:
        char* buffer;
        int length;
    public:
        void setbuffer(const char* s){
            buffer = const_cast< char * >(s);
            length = strlen(s);
        }
        char& operator[]
            ( const int index ){
                return buffer[ index ];
            }
        int size() {
            return length;
        }
};

#endif
