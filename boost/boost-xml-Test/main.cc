/*
 * =====================================================================================
 *
 *       Filename:  main.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/28/2012 08:43:16 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dongyuchi (dongyuchi), dongyuchi@gmail.com
 *        Company:  UESTC
 *
 * =====================================================================================
 */
#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/xml_parser.hpp>
#include<boost/foreach.hpp>
#include<string>
#include<set>
#include<vector>
#include<exception>
#include<iostream>

using namespace std;
using boost::property_tree::ptree;

static const string str_node_name[]={"id","name","accout"};
struct xmlStageDetail{
    string id;
    string name;
    string accout;
};

int load(const char* filename,vector<xmlStageDetail>* m_xml_detail)
{
    xmlStageDetail xml_stage_detail;
    ptree pt;
    ptree root;
    try{
        read_xml(filename,pt);
        root = pt.get_child("root");
    }   
    catch(std::exception& e){
        return -1;
    }
    
    for(ptree::iterator itr=root.begin();itr!=root.end();itr++){
        ptree strage = itr->second;
        for(int i=0;i<3;i++){
            ptree pcur;
            string str_value;
            try{
                pcur =  strage.get_child(str_node_name[i]);
                str_value = pcur.data();
            }
            catch(std::exception& e){
                return -1; 
            }
            switch(i){
                case 0:
                   xml_stage_detail.id = str_value;
                   break;
                case 1:
                   xml_stage_detail.name = str_value;
                   break;
                case 2:
                   xml_stage_detail.accout = str_value;
                   break; 
            }
        }
        m_xml_detail->push_back(xml_stage_detail);
    }
    return 0;
}

int main(){
    vector<xmlStageDetail> m_xml_detail;
    try{
        load("config.xml",&m_xml_detail);
        std::cout<<"Success\n"<<endl; 
    }
    catch(std::exception &e){
        std::cout<<"Error:"<<e.what()<<endl;
        return -1; 
    }
    for(vector<xmlStageDetail>::iterator it=m_xml_detail.begin();it!=m_xml_detail.end();it++)
    {
        cout<<it->id<<endl;
        cout<<it->name<<endl;
        cout<<it->accout<<endl;
    }
    return 0;
}
