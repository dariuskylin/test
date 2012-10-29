/*
 * =====================================================================================
 *
 *       Filename:  result_of-test1.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/25/2012 12:48:02 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dongyuchi (dongyuchi), dongyuchi@gmail.com
 *        Company:  UESTC
 *
 * =====================================================================================
 */
#include<boost/utility/result_of.hpp>
#include<iostream>
#include<typeinfo>

double hello (double d) {
    return d*2;
}

int main() {
    typedef double (*Func)(double d);
    Func func = hello;
    boost::result_of< Func(double) >::type x = func(5.0);
    std::cout<<typeid(x).name()<<std::endl; 
    return 0;
}
