/*
 * =====================================================================================
 *
 *       Filename:  build.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/12/2012 12:35:20 AM
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
#include<libxml/xpath.h>
#include<libxml/tree.h>

using namespace std;

int main(){
    xmlDocPtr doc = xmlNewDoc(BAD_CAST"1.0");
    xmlNodePtr root_node = xmlNewNode(NULL,BAD_CAST"root");
    //set root node
    xmlDocSetRootElement(doc,root_node);
    //create nodes under root node
    xmlNewTextChild(root_node,NULL,BAD_CAST"IP",BAD_CAST"192.168.1.116");
    xmlNewTextChild(root_node,NULL,BAD_CAST"Port",BAD_CAST"65535");
    xmlNewTextChild(root_node,NULL,BAD_CAST"num",BAD_CAST"1000");
    //create a new node,set its content and attributes,then add it to root node
    xmlNodePtr node = xmlNewNode(NULL,BAD_CAST"node2");
    xmlNodePtr content = xmlNewText(BAD_CAST"NODE CONTENT");
    //add node to root node
    xmlAddChild(root_node,node);
    xmlAddChild(node,content);
    xmlNewProp(node,BAD_CAST"attribute",BAD_CAST"yes");
    //create a son node and a grandson one
    node = xmlNewNode(NULL,BAD_CAST"son");
    xmlAddChild(root_node,node);
    xmlNodePtr grandson = xmlNewNode(NULL,BAD_CAST"grandson");
    xmlAddChild(node,grandson);
    xmlAddChild(grandson,xmlNewText(BAD_CAST"This is a grandson node")); 
    //save xml doc
    int ret = xmlSaveFile("create.xml",doc);
    if(ret != -1){
        cout<<"A xml doc is created;Write in "<<ret<<" bytes"<<endl;
    }
    xmlFreeDoc(doc);
    return 0;
}
