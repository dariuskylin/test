/*Copyright 2012 NDSL. All Rights Reserved.
 * =====================================================================================
 *       Filename:  test.cpp
 *    Description:  
 *        Version:  1.0
 *        Created:  11/24/2012 10:45:43 AM
 *         Author:  dongyuchi (dongyuchi), dongyuchi@gmail.com
 *        Company:  UESTC.NDSL
 * =====================================================================================
 */
#include<iostream>

int main() {
    std::cout<<"\033[0;31mHello World(Red).  \033[0m \n"<<std::endl;
    std::cout<<"\033[0;32m Hello World(Green).  \033[0m \n"<<std::endl;
    std::cout<<"\033[0;33mHello World(Yellow).  \033[0m \n"<<std::endl;
    std::cout<<"\033[0;34m Hello World(Blue).  \033[0m \n"<<std::endl;
    std::cout<<"\033[0;35mHello World(Purple).  \033[0m \n"<<std::endl;
    std::cout<<"\033[0;36m Hello World(Dark Green).  \033[0m \n"<<std::endl;
    std::cout<<"\033[0;37mHello World(Write).  \033[0m \n"<<std::endl;
    return 0;
}
