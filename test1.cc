/*
 * =====================================================================================
 *
 *       Filename:  test1.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/30/2012 08:14:28 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dongyuchi (dongyuchi), dongyuchi@gmail.com
 *        Company:  UESTC
 *
 * =====================================================================================
 */
#include<iostream>
#include<string>
using namespace std;
int sum = 0;
void getSum(int i);
int main(){
    for(int i = 1; i<100;i++){
        try{
            getSum(i);
        }
        catch(string s){
            cout<<s<<endl;
        }
    }
    cout<<"sum="<<sum<<endl;
    return 0;
}
void getSum(int i){
    if( i== 10){
        throw (string("invalid input:10"));
    }
    sum+=i;
}
