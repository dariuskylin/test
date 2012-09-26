/*
 * =====================================================================================
 *
 *       Filename:  boost-timer.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/25/2012 08:08:03 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dongyuchi (dongyuchi), dongyuchi@gmail.com
 *        Company:  UESTC
 *
 * =====================================================================================
 */
#include<iostream>
#include<boost/timer.hpp>
using namespace boost;
using std::endl;
using std::cout;
int main(){
    timer t;
    cout<<"max timespan:"<<t.elapsed_max()/3600<<"h"<<endl;
    cout<<"min timespan:"<<t.elapsed_min()<<"s"<<endl;
    cout<<"now time elapsed:"<<t.elapsed()<<"s"<<endl;

    return 0;
}
