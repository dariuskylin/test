/*
 * =====================================================================================
 *
 *       Filename:  sock.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/10/2012 12:28:47 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dongyuchi (dongyuchi), dongyuchi@gmail.com
 *        Company:  UESTC
 *
 * =====================================================================================
 */
#include"sock.h"
#include<sstream>
using namespace std;
SOCK::SOCK(string ip,unsigned short port):m_Ip(ip),port(port){
}
string SOCK::ToString(){
    stringstream ss;
    ss<<m_Ip<<"     ";
    ss<<port;
    return ss.str();
}
