/*
 * =====================================================================================
 *
 *       Filename:  server.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/13/2012 07:06:53 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dongyuchi (dongyuchi), dongyuchi@gmail.com
 *        Company:  UESTC
 *
 * =====================================================================================
 */
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<libxml/parser.h>
#include<libxml/xmlmemory.h>
#include<errno.h>

using namespace std;

const unsigned int port = 60021;

int parseXML(xmlDocPtr tmpdoc){
    if(tmpdoc == NULL){
        cout<<"null pointer"<<endl;
        return -1;
    }
    xmlNodePtr curNode;
    xmlChar* szKey;
    curNode = xmlDocGetRootElement(tmpdoc);
    if(curNode == NULL){
        xmlFreeDoc(tmpdoc);
        return -1;
    }
    cout<<curNode->name<<endl;
    if(xmlStrcmp(curNode->name,BAD_CAST"root")){
        cerr<<"wrong type,root node is not 'root'"<<endl;
        xmlFreeDoc(tmpdoc);
        return -1;
    }
    curNode = curNode->xmlChildrenNode;
    while(curNode!=NULL){
        if(!xmlStrcmp(curNode->name,BAD_CAST"name")){
            szKey = xmlNodeGetContent(curNode);
            cout<<"name:"<<szKey<<endl;
            xmlFree(szKey);
        }else if(!xmlStrcmp(curNode->name,BAD_CAST"age")){
            szKey = xmlNodeGetContent(curNode);
            cout<<"age:"<<szKey<<endl;
            xmlFree(szKey);
        }
        curNode = curNode->next;
    }
    return 1; 
}
int main(){
    struct sockaddr_in servaddr,clientaddr;
    int sock_fd,client_fd;
    socklen_t len;
    char buff[65535];
    xmlDocPtr doc;

    bzero(&servaddr,sizeof(servaddr));
    bzero(&clientaddr,sizeof(clientaddr));
    servaddr.sin_family= AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    sock_fd = socket(AF_INET,SOCK_STREAM,0);
    if(sock_fd<0){
        cerr<<"socket() error!"<<endl;
        return -1;
    }
    int ret = bind(sock_fd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    if(ret < 0){
        cerr<<"bind() error"<<endl;
        return -1;
    }
    listen(sock_fd,20);
    cout<<"listening..."<<endl;
    while(1){
        client_fd = accept(sock_fd,(struct sockaddr*)&clientaddr,&len);
        if(client_fd<0){
            cerr<<"accept error"<<endl;
            perror("nimei");
            return -1;
        }
        ret = read(client_fd,buff,sizeof(buff));
        if(ret<0){
            cerr<<"read error"<<endl;
            return -1;
        }
        cout<<"xml:"<<buff<<endl;
        doc = xmlParseMemory((char *)buff,strlen(buff)+1);
        if(doc == NULL){
            cerr<<"xmlParseMemery error"<<endl;
            return -1;
        }
        cout<<"begin resolving the XML...."<<endl;
        ret = parseXML(doc); 
        if(ret>0){
            write(client_fd,"success",7);
        }
        else{
            write(client_fd,"failed",6);
        }
        close(client_fd);
    }

    return 0;
}
