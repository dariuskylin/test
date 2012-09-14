/*
 * =====================================================================================
 *
 *       Filename:  sock.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/10/2012 12:29:03 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dongyuchi (dongyuchi), dongyuchi@gmail.com
 *        Company:  UESTC
 *
 * =====================================================================================
 */
#ifndef _SOCK_H_
#define _SOCK_H_
#include<string>
using std::string;
class SOCK{
    public:
        SOCK(string ip,unsigned short port);
        string ToString();
    private:
        string m_Ip;
        unsigned short port;
};
#endif
