/*
 * =====================================================================================
 *
 *       Filename:  test-boost-unit.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/30/2012 08:32:35 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dongyuchi (dongyuchi), dongyuchi@gmail.com
 *        Company:  UESTC
 *
 * =====================================================================================
 */
#define BOOST_TEST_MODULE stringbuild
#include<boost/test/included/unit_test.hpp>
#include"sock.h"

BOOST_AUTO_TEST_SUITE(stringbuild1)  //name

BOOST_AUTO_TEST_CASE(test1){
    SOCK sock("192.168.1.12",1024);
    string s = sock.ToString();
    BOOST_REQUIRE_EQUAL('2',s[0]);
}
BOOST_AUTO_TEST_CASE(test2){
    SOCK sock("192.168.1.12",1024);
    string s = sock.ToString();
    BOOST_CHECK(s.size() == 0);
}
BOOST_AUTO_TEST_SUITE_END()
