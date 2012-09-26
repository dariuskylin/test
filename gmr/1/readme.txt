                                         C++ 简单远程调用中间件设计 (一)

     我写过一些 socket 程序， 随着程序的逐步复杂，直接用 socket 编程显得有些麻烦。根据软件应该模块化的思想，应该把软件中的 socket 通信部分相对独立出来做成一个“中间件”。我用 C++ 写了一个最简单的“远程调用中间件” 原理演示程序，共享出来和大家讨论（本人水平有限，如有错误，欢迎批评）。这个程序演示了一组简单的远程对象调用，应用程序通过本地代理对象访问远程计算机上 C++ 对象，如同访问真正的本地对象一样方便。这个远程对象调用程序的原理并不难，服务端有一个通信类处理 socket 通信，提供各种服务的服务端对象把自己的指针传给通信类的对象，保存在一个 std::map 中，通信类的对象根据客户端的请求调用服务端的不同功能的 C++ 对象，然后把调用结果返回客户端。客户端也有一个通信类处理 socket 通信，客户端的代理对象代表服务端对象接受调用，然后通过通信类把类标志，函数标志，函数参数传给服务端，调用服务端对象，然后把调用结果返回客户端的调用者。附件中是程序源码，真正的本人原创 :-) 。

     我是在我的 win2k/mingw 环境下编译测试这个程序的，程序需要用到 gtkmm 库 http://www.gtkmm.org  ，  在这个程序中我没有处理网络字节顺序问题， 实用的程序应该处理这个问题。

mingw  下编译命令是：
 
服务端程序： g++ -o  gmrcmwser  gmrcmwser.cc `pkg-config.exe gtkmm-2.4 --cflags --libs` -lgthread-2.0 -lws2_32

客户端程序：g++ -o  gmrcmwcli  gmrcmwcli.cc `pkg-config.exe gtkmm-2.4 --cflags --libs` -lws2_32

这个程序源码应该可以移植到 Linux , Linux 下编译命令应该是：

服务端程序： g++ -o  gmrcmwser  gmrcmwser.cc `pkg-config gtkmm-2.4 --cflags --libs` -lgthread-2.0

客户端程序：g++ -o  gmrcmwcli  gmrcmwcli.cc `pkg-config gtkmm-2.4 --cflags --libs`


   这只是一个简单的演示程序，我也不能保证它没有错误。您可以把这个程序中的源码用在您自己的程序中，只要您自己负责程序的质量，并且注明引用部分的原作者。



    要调用远端的 C++ 对象，本地程序需要按约定格式向远端发送：远端对象的类标识，同一个类的不同对象的对象标识（不过在我的这个小程序中远端同一个类只有一个对象，所以我省略了对象标识），成员函数标识，函数参数，然后接收远端返回的调用结果数据。

    远端对象的类标识是一个 32位无符号整数，在我的程序中有两个远端对象类，一个可以完成整数的加减法，另一个可以完成浮点数的加减法 (调用远程对象做加减法是有些夸张，但是这样可以演示远程调用如何工做) 。它们的类标识是：
#define  CLASS_INT_CALC       10001
#define  CLASS_FLOAT_CALC     10002

每个类中有两个成员函数完成整数的加减法，它们的成员函数标识是：
#define  FUNC_ADD         20001
#define  FUNC_SUB         20002

本地代理对象代表远端对象接受调用，它们的是：

class SerAgent
{
    protected:
    char recv_buf[PACK_SIZE];
    char send_buf[PACK_SIZE];

    CommunicateCli *p_client;

    public:
    void init(CommunicateCli *ap_client);
};

class IntCalcAgent :public SerAgent
{
    public:
    gint32 add(gint32 a, gint32 b, gint32 &c);
    gint32 sub(gint32 a, gint32 b, gint32 &c);

    protected:
    struct Arg_Data
    {
        gint32 a;
        gint32 b;
        gint32 c;
    };
};

class FloatCalcAgent :public SerAgent
{
    public:
    gint32 add(double a, double b, double &c);
    gint32 sub(double a, double b, double &c);

    protected:
    struct Arg_Data
    {
        double a;
        double b;
        double c;
    };
};

代理对象和远端通信调用远端对象。


gint32 IntCalcAgent::add(gint32 a, gint32 b, gint32 &c)
{
    char *p_buf;
    guint32 *p_obj_id;
    guint32 *p_fun_id;
    gint32 ret_val;
    gint32 *p_ret_val;

    Arg_Data add_data;

    p_buf = send_buf;
    p_obj_id = (guint32*)p_buf;
    *p_obj_id = CLASS_INT_CALC;
    p_buf += sizeof(guint32);
    p_fun_id = (guint32*)p_buf;
    *p_fun_id = FUNC_ADD;
    p_buf += sizeof(guint32);

    add_data.a = a;
    add_data.b = b;

    memmove(p_buf, (char*)&add_data, sizeof(add_data));
        
    ret_val = p_client->call_ser(send_buf, recv_buf);
    
    if (ret_val < 0) return ret_val;
    
    p_buf = recv_buf;
    p_ret_val = (gint32*)p_buf;
    ret_val = *p_ret_val;
    p_buf += sizeof(gint32);

    memmove((char*)&add_data, p_buf, sizeof(add_data));

    c = add_data.c;

    return ret_val;
}


gint32 CommunicateCli::call_ser(char* ap_send, char* ap_recv)
{
    gint32 ret_val = 0;

    gint32 ret_send = send(sd, ap_send, PACK_SIZE, 0);
    //std::cout << "client send: " << ret_send << std::endl;


    gint32 ret_recv = recv(sd, ap_recv, PACK_SIZE, 0);
    //std::cout << "client read: " << ret_recv << std::endl;

    if (ret_recv != PACK_SIZE) ret_val = -1;

    return ret_val;
}

远端（服务端）程序，把各个服务端对象的指针保存在 std::map<guint32, ModuleServer*> 中，
std::map 中的 key 就是各个类标识，服务端根据客户端传来的类标识调用不同服务对象。

#define PACK_SIZE 4096

class ModuleServer
{
    protected:
    Glib::Mutex mutex_map;

    public:
    virtual void call_switch(char *p_recv, char *p_send) = 0;        
};

class CommunicateSer
{
    private:

    typedef std::pair<guint32, ModuleServer*> Ps;

    struct sockaddr_in sa;
 
    int err;
    int listen_sd;
    int sd;
    struct sockaddr_in sa_serv;
    struct sockaddr_in sa_cli;
    int client_len;

    std::map<guint32, ModuleServer*> map_servers;
    std::map<guint32, ModuleServer*>::iterator it_map;

    Glib::Mutex mutex_map;

    std::queue<int> queue_sd;
    Glib::Thread *p_server_thread;
    Glib::Mutex mutex_sd;

    public:
      void reg_server(guint32 a_ser_id, ModuleServer* ap_server);
        void init(){};
    void listen_client();
    void server();    
};

class IntCalcSer :public ModuleServer
{
    public:
    void call_switch(char *ap_recv, char *ap_send);
    void init();

    protected:

    struct Arg_Data
    {
        gint32 a;
        gint32 b;
        gint32 c;
    };

    protected:
    void add(char *ap_recv, char *ap_send);
    void sub(char *ap_recv, char *ap_send);

    protected:
    typedef void (IntCalcSer::*P_FUNC)(char *ap_recv, char *ap_send);

    P_FUNC p_func;

    std::map<guint32, P_FUNC> map_func;
    std::map<guint32, P_FUNC>::iterator it_map_func;
 
    typedef std::pair<guint32, P_FUNC> Ps_Func;
};

class FloatCalcSer :public ModuleServer
{
    public:
    void call_switch(char *ap_recv, char *ap_send);
    void init();

    protected:

    struct Arg_Data
    {
        double a;
        double b;
        double c;
    };


    protected:
    void add(char *ap_recv, char *ap_send);
    void sub(char *ap_recv, char *ap_send);

    protected:
    typedef void (FloatCalcSer::*P_FUNC)(char *ap_recv, char *ap_send);

    P_FUNC p_func;

    std::map<guint32, P_FUNC> map_func;
    std::map<guint32, P_FUNC>::iterator it_map_func;
 
    typedef std::pair<guint32, P_FUNC> Ps_Func;
};

void CommunicateSer::reg_server(guint32 a_ser_id, ModuleServer* ap_server)
{
    map_servers.insert(Ps(a_ser_id, ap_server));     
}


void CommunicateSer::server()
{
    int server_sd = 0;

    char recv_buf[PACK_SIZE];
    char send_buf[PACK_SIZE];

    ModuleServer* p_server;

    mutex_sd.lock();
    server_sd = queue_sd.front();
    queue_sd.pop();
    mutex_sd.unlock();

    while (true)
    {    
        gint32 ret_recv = recv(server_sd, recv_buf, PACK_SIZE, 0);
        std::cout << "server read: " << ret_recv << std::endl;
        if (ret_recv != PACK_SIZE)
        {
                    #ifdef _WIN32
            closesocket(server_sd);
         #else    
            close(server_sd);
         #endif
            std::cout << "close server socket" << std::endl;
            break;
        }

        guint32 *p_obj_id;
        p_obj_id = (guint32*)recv_buf;

        guint32 id_obj;
    
        id_obj = *p_obj_id;
        
        std::cout << "id_obj = " << id_obj << std::endl;

        mutex_map.lock();    
        it_map = map_servers.find(id_obj);
        p_server = it_map->second;
        mutex_map.unlock();    
        
        std::cout << it_map->first << std::endl;

        p_server->call_switch(recv_buf, send_buf);    
        
        gint32 ret_send = send(server_sd, send_buf, PACK_SIZE, 0);

        std::cout << "server send: " << ret_send << std::endl;            
    }
}

服务端对象中有一个成员函数指针的 std::map 它的 key 是成员函数标识，服务端对象根据客户端传来成员函数标识用成员函数指针调用不同成员函数，然后把调用结果传回客户端。

void IntCalcSer::call_switch(char *ap_recv, char *ap_send)
{
    char *p_buf;
    guint32 *p_obj_id;
    guint32 *p_fun_id;
    P_FUNC p_func_tmp;
    
    p_buf = ap_recv;
    p_obj_id = (guint32*)p_buf;
    p_buf += sizeof(guint32);
    p_fun_id = (guint32*)p_buf;
    p_buf += sizeof(guint32);

    mutex_map.lock();    
    it_map_func = map_func.find(*p_fun_id);
    p_func_tmp = it_map_func->second;
    mutex_map.unlock();    

    (this->*p_func_tmp)(ap_recv, ap_send);
}

void IntCalcSer::add(char *ap_recv, char *ap_send)
{
    char *p_buf;
    guint32 *p_obj_id;
    guint32 *p_fun_id;
    gint32 *p_ret_val;

    Arg_Data add_data;
    
    p_buf = ap_recv;
    p_obj_id = (guint32*)p_buf;
    p_buf += sizeof(guint32);
    p_fun_id = (guint32*)p_buf;
    p_buf += sizeof(guint32);

    memmove((char*)&add_data, p_buf, sizeof(add_data));
    
    add_data.c = add_data.a + add_data.b;

    p_buf = ap_send;
    p_ret_val = (gint32*)p_buf;
    p_buf += sizeof(gint32);

    memmove(p_buf, (char*)&add_data, sizeof(add_data));
    *p_ret_val = 0;
}

最后客户端代理对象收到远端调用结果，返回给本地调用者。


                                                                
                                                     zhujiang

                                                   2007年2月23日
                                                                 