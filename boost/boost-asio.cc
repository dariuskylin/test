/*
 * =====================================================================================
 *
 *       Filename:  boost-asio.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/25/2012 08:31:14 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dongyuchi (dongyuchi), dongyuchi@gmail.com
 *        Company:  UESTC
 *
 * =====================================================================================
 */
#include <boost/asio.hpp> 
#include <iostream> 

void handler(const boost::system::error_code &ec) 
{ 
      std::cout << "5 s." << std::endl; 
} 

int main() 
{ 
      boost::asio::io_service io_service; 
        boost::asio::deadline_timer timer(io_service, boost::posix_time::seconds(5)); 
          timer.async_wait(handler); 
            io_service.run(); 
} 
