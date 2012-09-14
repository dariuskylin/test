/*
 * =====================================================================================
 *
 *       Filename:  client.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/13/2012 06:26:49 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dongyuchi (dongyuchi), dongyuchi@gmail.com
 *        Company:  UESTC
 *
 * =====================================================================================
 */
#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<libxml/parser.h>
#include<libxml/xmlmemory.h>

using namespace std;

const char* configxml = "config.xml";
const unsigned int port =60021;

int main(){
    int fd;
    struct sockaddr_in servaddr;
    char succ[64];
    fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd<0){
        cerr<<"socket() error!"<<endl;
        return -1;
    }
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family =AF_INET;
    servaddr.sin_addr.s_addr=inet_addr("172.16.228.128");
    servaddr.sin_port = htons(port);
    
    xmlDocPtr doc;
    xmlChar *xmlbuf;
    int buffersize;
    //doc = xmlReadFile(configxml,"GB2312",XML_PARSE_RECOVER);
    doc = xmlParseFile(configxml);
    xmlDocDumpFormatMemory(doc,&xmlbuf,&buffersize,1);
    cout<<"content:"<<xmlbuf<<endl;
    cout<<"length:"<<buffersize<<endl;
    int ret = connect(fd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    if(ret<0){
        cout<<"connect() error!"<<endl;
        return -1;
    }
    ret = write(fd,xmlbuf,buffersize);
    if(ret<0){
        cout<<"write error!"<<endl;
        return -1;
    }
    memset(succ,0,64);
    ret = read(fd,succ,64);
    cout<<succ<<endl;
    return 0;
}
