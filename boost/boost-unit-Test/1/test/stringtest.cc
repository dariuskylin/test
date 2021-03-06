/*
 * =====================================================================================
 *
 *       Filename:  stringtest.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/17/2012 06:57:59 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dongyuchi (dongyuchi), dongyuchi@gmail.com
 *        Company:  UESTC
 *
 * =====================================================================================
 */
#define BOOST_TEST_MODULE stringtest
#include<boost/test/included/unit_test.hpp>
#include"../str.h"

//name of the test suite is stringtest
BOOST_AUTO_TEST_SUITE (stringtest)

BOOST_AUTO_TEST_CASE(test1){
    mystring s;
    BOOST_CHECK( s.size() == 0 );
}

BOOST_AUTO_TEST_CASE(test2){
    mystring s;
    s.setbuffer( "hello world" );
    BOOST_REQUIRE_EQUAL('h', s[0] );
}

BOOST_AUTO_TEST_SUITE_END()
