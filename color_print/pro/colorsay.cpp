/*Copyright 2012 NDSL. All Rights Reserved.
 * =====================================================================================
 *       Filename:  colorsay.cpp
 *    Description:  
 *        Version:  1.0
 *        Created:  11/24/2012 11:17:08 AM
 *         Author:  dongyuchi (dongyuchi), dongyuchi@gmail.com
 *        Company:  UESTC.NDSL
 * =====================================================================================
 */
#include"colorsay.h"
#include<iostream>

void Redcout(const char* string) {
    std::cout<<"\033[0;31m"<<string<<"\033[0m"<<std::endl;
}
