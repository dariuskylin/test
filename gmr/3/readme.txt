    今天 “C++ 简单远程调用” 第三版完成了，现在我叫他 “gcpprc” ，这次主要改进了远程调用过程中参数的打包和解包过程。这次我把 gcpprc 和演示程序的源码分成两组源文件，这样 gcpprc 就可以单独编译成一个共享库。我决定 gcpprc 在 lgpl2 协议下发行，对 lgpl 通常的理解是，如果您写的软件始终以动态连接的形式连接到这个函数库，您可以不公开源码，静态连接和对库本身的修改需要公开源码。这样您就可以用 gcpprc 开发商业程序了，我个人认为 gcpprc 可以把一部分网络程序的编程难度降低一半以上，如果您需要对 gcpprc 的技术支持和定制开发，我就要考虑收费了。  这次发布的 gcpprc 的演示程序 gcpprccli/gcpprcser 在 gpl2 协议下发行，关于对 gpl/lgpl 的权威解释请看 http://www.gnu.org/licenses/ 。



    注意：正如 gpl 和 lgpl 相关条款中所说的，这个软件默认没有担保。我不能保证 gcpprc 和 gcpprccli/gcpprcser 中没有错误，如果您需要某种程度的担保或技术支持和定制开发等服务请和我联系，我会考虑，不过这些通常要收费。我的 email 是 zhujiang@vmail.btvnet.net ，还有 zhujiangmail@hotmail.com 。



    我在以前的博客文章中说过 gcpprc 的原理不难。但 gcpprc 比较简单实用，这也是我的设计目标。这个库封装了远程对象调用的大部分操作，包括远程对象的创建和撤销，远程对象和方法编号注册，参数的打包和解包，打包后的远程对象和方法编号和参数数据的网络传输等， gcpprc 提供了一组比较容易使用的 C++ 类，我们只需要比较简单的编程就能开发出网络应用程序。gcpprc 服务端有一个通信类处理 socket 通信，提供各种服务的服务端对象把自己的指针传给通信类的对象，保存在一个 std::map 中，客户端的代理对象代表服务端对象接受调用，然后把服务端对象类标识，函数标识，函数参数打包传给服务端，服务端把收到的数据解包，调用服务端对象，然后把调用结果数据打包传回客户端，客户端把返回的数据解包，最后客户端代理对象把调用结果返回客户端程序。

    gcpprc 可以为每一个客户端代理类实例动态创建一个对应的服务端类实例，这样就可以保存中间信息。在演示程序中我用这个功能实现了一个文件读写访问类。当客户端代理类的实例初始化时它调用服务端对象的 clone 方法，clone 方法创建一个服务端对象的新实例，并把新实例的指针保存在 std::map 中，用新实例的对象标识做 std::map 中的 key ，并且把这个对象标识返回给客户端，然后客户端就可以用这个对象标识调用服务端对象了，这时服务端对象和客户端代理是一对一服务，所以服务端对象可以保存打开的文件对象，文件读取指针等中间信息。当客户端代理不再需要服务时它调用服务端对象的 erase 方法， erase 方法先清除服务端对象在 std::map 中的信息，然后从内存中清除这个对象。如果服务端对象不需要保存中间信息，那么只要有一个实例就行了，客户端的代理用同一个标识调用它。我在演示程序中演示了这种情况，调用一个远程对象做整数的加减法和连接字符串。


    gcpprc 系统需求：x86-32 兼容处理器，Linux 或 win2k/winxp 平台。目前我没有处理网络字节顺序问题，但用于 x86-32 机器之间的远程调用编程是没有问题的。运行于 32 位模式的 x86-64 处理器也没问题，我的机器用的就是 AMD64 处理器, 但操作系统是 32 位的。 Linux 和 win2k/winxp 平台之间的 gcpprc 程序也可以互相远程调用。 BSD 平台没有测试，应该问题不大。

        
    gcpprc 程序开发环境： Linux 或 win32/mingw 开发环境，需要 gtkmm 开发库。

    Linux 下编译命令：

         gcpprc 共享库： g++ -shared -o  libgcpprc.so gcpprclib.cc  `pkg-config gtkmm-2.4 --cflags --libs` -lgthread-2.0 

         gcpprc 演示程序：

             客户端 gcpprccli:  g++ -Wall -g -o  gcpprccli  gcpprccli.cc `pkg-config  gtkmm-2.4 --cflags --libs` -lgthread-2.0  -L./  -lgcpprc
             服务器 gcpprcser:  g++ -Wall -g -o  gcpprcser  gcpprcser.cc `pkg-config  gtkmm-2.4 --cflags --libs` -lgthread-2.0  -L./  -lgcpprc
        
         您可能需要： export LD_LIBRARY_PATH="./"  。

    win32/mingw 开发环境下编译命令：
         gcpprc 共享库： g++ -shared -o gcpprc.dll gcpprclib.cc -Wl,--out-implib,libgcpprc.a `pkg-config.exe gtkmm-2.4 --cflags --libs` -lgthread-2.0 -lws2_32

         gcpprc 演示程序：

             客户端 gcpprccli:  g++ -Wall -g -o  gcpprccli  gcpprccli.cc `pkg-config.exe gtkmm-2.4 --cflags --libs` -lgthread-2.0 -lws2_32 -L./ -lgcpprc
             服务器 gcpprcser:  g++ -Wall -g -o  gcpprcser  gcpprcser.cc `pkg-config.exe gtkmm-2.4 --cflags --libs` -lgthread-2.0 -lws2_32 -L./ -lgcpprc

 
    gcpprc 程序开发说明：

             客户端程序开发主要是编写客户端代理，客户端代理对象代表服务端对象接受调用。客户端代理类的基本类是 gcpprc::SerAgent ，SerAgent 实现并封装了很多客户端代理的操作，提供了比较简单的编程接口。

             客户端代理类需要有一个初始化函数 init,  init 函数首先设置对象标识，对象标识包括类标识和类的实例标识两个部分，客户端和服务端的类标识是相同的，统一定义在一个头文件中，如果服务端类不需要保存中间信息，那么实例标识永远是 0 , 客户端代理类的不同实例都调用同一个服务端类实例，init 函数设置对象标识后调用 SerAgent::init 函数连接到服务器，如果服务端类需要保存中间信息，那么还要调用 clone 函数，clone 会产生一个服务端类的新实例，并把新实例的标识返回客户端，然后客户端就可以用新对象标识调用服务端实例了。

             
             客户端代理类还需要实现一组代理函数，代表服务端类的对应函数接受客户端程序的调用。这些代理函数调用 SerAgent::remote_call 函数和服务端通信，SerAgent::remote_call 的第一个参数是函数标识，客户端和服务端的函数标识和类标识统一定义在一个头文件中，统一的函数标识和类标识是客户端和服务端为实现远程调用所做的约定。remote_call 的其他参数是代理函数要传给服务端函数的参数数据的地址，remote_call 会把数据打包传给服务端，完成远程调用后再把服务端返回的数据解包返回给客户端代理函数，SerAgent::remote_call 是 gcpprc 最重要的部分。


             服务端程序开发主要是编写各种服务端服务模块类，服务端程序启动时要创建一个 gcpprc::CommunicateSer 的实例， CommunicateSer 是 gcpprc 中处理服务端网络通信的类，服务端服务模块类的实例需要调用 CommunicateSer::reg_server 函数，把自己的指针注册到 CommunicateSer 中的一个 std::map<guint64, ModuleServer*> 中，然后 CommunicateSer 开始监听客户端的连接请求，CommunicateSer 会为每一个客户端连接创建一个新线程，然后在新线程中根据客户端传来的对象标识调用不同的服务端类实例的 call_switch 函数, call_switch 函数把客户端传来的参数数据解包, 然后根据函数标识调用各个具体的服务函数，把调用结果返回 CommunicateSer , 最后 CommunicateSer 把调用结果返回客户端。为了能用函数标识调用服务端类实例的函数，服务端类要实现一个初始化函数 init , 服务端类实例调用 init 函数把各个服务函数的指针保存到自己的 std::map<guint32, P_FUNC> 中 , std::map 的 key 就是函数标识.       



                                                            朱江
                                                           2007.05.04