/*
 * =====================================================================================
 *
 *       Filename:  read.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/12/2012 01:47:45 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dongyuchi (dongyuchi), dongyuchi@gmail.com
 *        Company:  UESTC
 *
 * =====================================================================================
 */
#include<iostream>
#include<libxml/parser.h>
//#include<libxml/xmlmemory.h>
#include<string>

using namespace std;

const char* xmlConfig="create.xml";

int main(){
    xmlDocPtr doc;
    xmlNodePtr curNode;
    xmlChar* szKey;

    doc = xmlParseFile(xmlConfig);
    if(doc == NULL){
        cout<<"Document not parsed successfully!"<<endl;
        return -1;
    }
    //cout<<"Document parsed successfully!"<<endl;
    curNode = xmlDocGetRootElement(doc);
    if(curNode == NULL){
        cout<<"empty document.\n"<<endl;
        xmlFreeDoc(doc);
        return -1;
    }
    //cout<<"xmlDocGetRootElement OK.\n"<<endl;
    if(xmlStrcmp(curNode->name,BAD_CAST"root")){
        cerr<<"root node is not 'root'"<<endl;
        xmlFreeDoc(doc);
        return -1;
    } 
    curNode = curNode->xmlChildrenNode;
    xmlNodePtr propNodePtr = curNode;
    while(NULL != curNode){
        if((!xmlStrcmp(curNode->name,(const xmlChar*)"IP"))){
            szKey = xmlNodeGetContent(curNode);
            cout<<"IP:"<<szKey<<endl;
            //string s((char*)szKey);
            xmlFree(szKey);
            //cout<<s<<endl;
        }else if((!xmlStrcmp(curNode->name,(const xmlChar*)"Port"))){
            szKey = xmlNodeGetContent(curNode);
            cout<<"Port:"<<szKey<<endl;
            xmlFree(szKey);
        }else if((!xmlStrcmp(curNode->name,(const xmlChar*)"num"))){
            szKey = xmlNodeGetContent(curNode);
            cout<<"num:"<<szKey<<endl;
            xmlFree(szKey);
        }else if((!xmlStrcmp(curNode->name,(const xmlChar*)"node2"))){
            szKey = xmlNodeGetContent(curNode);
            cout<<"node2:"<<szKey<<endl;
            xmlFree(szKey);
            // get attributes
            szKey = xmlGetProp(curNode,(const xmlChar*)"attribute");
            cout<<"node2->attribute:"<<szKey<<endl;
            xmlFree(szKey);
        }
        curNode= curNode->next;
    }
    
    return 0;
}
