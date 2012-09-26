                                         C++ 简单远程调用中间件设计 (二)

     我写过一些 socket 程序， 随着程序的逐步复杂，直接用 socket 编程显得有些麻烦。根据软件应该模块化的思想，应该把软件中的 socket 通信部分相对独立出来做成一个“中间件”。我用 C++ 写了一个最简单的“远程调用中间件” 原理演示程序，共享出来和大家讨论（本人水平有限，如有错误，欢迎批评）。这个程序演示了一组简单的远程对象调用，应用程序通过本地代理对象访问远程计算机上 C++ 对象，如同访问真正的本地对象一样方便。这个远程对象调用程序的原理并不难，服务端有一个通信类处理 socket 通信，提供各种服务的服务端对象把自己的指针传给通信类的对象，保存在一个 std::map 中，通信类的对象根据客户端的请求调用服务端的不同功能的 C++ 对象，然后把调用结果返回客户端。客户端也有一个通信类处理 socket 通信，客户端的代理对象代表服务端对象接受调用，然后通过通信类把类标识，函数标识，函数参数传给服务端，调用服务端对象，然后把调用结果返回客户端的调用者。附件中是程序源码，真正的本人原创 :-) 。


   这只是一个简单的演示程序，我也不能保证它没有错误。您可以把这个程序中的源码用在您自己的程序中，只要您自己负责程序的质量，并且注明引用部分的原作者。

    这个程序是“C++ 简单远程调用中间件设计 (一)”的升级版本，这次实现了根据客户端的请求动态的创建和撤销服务端对象。原来的“C++ 简单远程调用中间件设计 (一)”程序中一个服务端类只有一个实例，客户端同一代理类的不同实例都调用这一个服务端对象，服务端对象的服务函数必须是可重入的，这样服务端对象就不能保存多次调用中的中间信息。现在的“C++ 简单远程调用中间件设计 (二)”程序可以为每一个客户端代理类实例动态创件一个对应的服务端类实例，这样就可以保存中间信息。在我的这个程序中用这个功能实现了一个文件只读访问类。 

客户端 FileAgent 类:

class  FileAgent :public  SerAgent , FileCSArgBase    
{
    protected:
	guint32      pos_file;
	bool         end_file;
	guint32      size_file;
	guint32      size_read;

    public:
	gint32  init(CommunicateCli  *ap_client);		
	gint32  open(char* filename);
	gint32  read(char *ap_buf, guint32  a_size_read);
	gint32  seek(guint32  a_pos);
	gint32  close();
	gint32  erase();
	bool    get_if_end(){return  end_file;}
	guint32 get_size_read(){return  size_read;}
};


服务端 FileSer 类:

class  FileSer :public  ModuleServer , FileCSArgBase   
{
    protected:
	std::string        name_module;
	static  guint32    counter;
	IdObj              m_id_obj;
	CommunicateSer    *p_my_comm;
	Glib::Mutex        mutex_clone;

        std::ifstream      my_file;

    protected:
	typedef  void  (FileSer::*P_FUNC)(char *ap_recv,  char *ap_send);

	P_FUNC  p_func;

	std::map<guint32, P_FUNC>                    map_func;   
	std::map<guint32, P_FUNC>::iterator          it_map_func;
 
	typedef  std::pair<guint32, P_FUNC>          Ps_Func;


    public:
	void  call_switch(char *ap_recv,  char *ap_send);
	void  init(CommunicateSer  *ap_my_comm, IdObj  a_id_obj);

    protected:
	//void  init();

	void  clone(char *ap_recv,  char *ap_send);
	void  open(char *ap_recv,  char *ap_send);
	void  read(char *ap_recv,  char *ap_send);
	void  seek(char *ap_recv,  char *ap_send);
	void  close(char *ap_recv,  char *ap_send);
	void  erase(char *ap_recv,  char *ap_send);
};

当客户端代理类的实例初始化时它调用服务端对象的 clone 方法，clone 方法创建一个服务端对象的新实例，并把新实例的指针保存在一个 std::map 中，用新实例的对象标识做 std::map 中的 key ，把这个对象标识返回给客户端，然后客户端就可以用这个对象标识调用服务端对象了，这时服务端对象和客户端代理是一对一服务，所以服务端对象可以保存打开的文件对象，文件读取指针等中间信息。当客户端代理不再需要服务时它调用服务端对象的 erase 方法， erase 方法先清除服务端对象在 std::map 中的信息，然后从内存中清除这个对象。我在这个程序中演示了远程文件读取和移动读取指针。如果服务端对象不需要保存中间信息，那么只要有一个实例就行了，客户端的代理用同一个标识调用它。我在演示程序中演示了这种情况，调用一个远程对象做整数的加减法。 


     我是在我的 win2k/mingw 环境下编译测试这个程序的，程序需要用到 gtkmm 库 http://www.gtkmm.org ，在这个程序中我没有处理网络字节顺序问题， 实用的程序应该处理这个问题。

mingw  下编译命令是：
 
服务端程序：g++ -o  gmrcmwser  gmrcmwser.cc `pkg-config.exe gtkmm-2.4 --cflags --libs` -lgthread-2.0 -lws2_32

客户端程序：g++ -o  gmrcmwcli  gmrcmwcli.cc `pkg-config.exe gtkmm-2.4 --cflags --libs` -lgthread-2.0 -lws2_32

这个程序源码可以移植到 Linux , Linux 下编译命令应该是：

服务端程序：g++ -o  gmrcmwser  gmrcmwser.cc `pkg-config gtkmm-2.4 --cflags --libs` -lgthread-2.0

客户端程序：g++ -o  gmrcmwcli  gmrcmwcli.cc `pkg-config gtkmm-2.4 --cflags --libs` -lgthread-2.0


                                                                
                                                     zhujiang

                                                   2007年3月24日
                                                                 