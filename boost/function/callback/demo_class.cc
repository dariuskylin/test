/*
 * =====================================================================================
 *
 *       Filename:  demo_class.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/25/2012 08:07:19 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  dongyuchi (dongyuchi), dongyuchi@gmail.com
 *        Company:  UESTC
 *
 * =====================================================================================
 */
#include<boost/function.hpp>
#include<iostream>

class demo_class {
    private:
        typedef boost::function< void ( int ) > func_t;
        func_t func;
        int n;
    public:
        demo_class( int i ) : n(i) { }

        template< typename CallBack >
        void accept ( CallBack f) {
            func = f;

        }
        void run() {
            func(n);
        }
};
void call_back_func( int i ) {
    std::cout<<"call_back_func"<<std::endl;
    std::cout<< i * 2<<std::endl;
}
int main() {
    demo_class dc(10);
    dc.accept(call_back_func);
    dc.run();
    return 0;
}
