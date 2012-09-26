#include <stdio.h>
#include <stdlib.h>
#include <glibmm.h>
#include <unistd.h>

#ifdef _WIN32
    #include <windows.h>
    #include <winsock2.h>
    #include <gdk/gdkwin32.h>
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netinet/tcp.h>
    #include <netdb.h>
    #include <arpa/inet.h>
#endif
 
#include <algorithm>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <deque>
#include <queue>
#include <map>
#include <glibmm/thread.h>

#pragma pack(4)

#define CHK_NULL(x) if ((x)==NULL) exit (-1)
#define CHK_ERR(err,s) if ((err) < 0) {printf("%s error\n", s); exit(-2); }

#define  PORT        8000

#define  CLASS_INT_CALC       10001
#define  CLASS_FLOAT_CALC     10002

#define  FUNC_ADD         20001
#define  FUNC_SUB         20002

#define  PACK_SIZE        4096

class  ModuleServer 
{
    protected:
	Glib::Mutex  mutex_map;

    public:
	virtual void call_switch(char *p_recv,  char *p_send) = 0;		
};

class  CommunicateSer
{
    private:

	typedef  std::pair<guint32, ModuleServer*>     Ps;   

	struct sockaddr_in sa;
 
	int err;
	int listen_sd;
	int sd;
	struct sockaddr_in sa_serv;
	struct sockaddr_in sa_cli;
	int client_len;

	std::map<guint32, ModuleServer*>               map_servers;   
	std::map<guint32, ModuleServer*>::iterator     it_map; 

	Glib::Mutex                                    mutex_map;

	std::queue<int>                                queue_sd;
	Glib::Thread                                  *p_server_thread;
	Glib::Mutex                                    mutex_sd;

    public:
  	void     reg_server(guint32 a_ser_id, ModuleServer* ap_server);
        void     init(){};
	void     listen_client();
	void     server();	
};

class  IntCalcSer :public  ModuleServer    
{
    public:
	void  call_switch(char *ap_recv,  char *ap_send);
	void  init();

    protected:

	struct  Arg_Data
	{
		gint32  a;
		gint32  b;
		gint32  c;
	};

    protected:
	void add(char *ap_recv,  char *ap_send);
	void sub(char *ap_recv,  char *ap_send);

    protected:
	typedef  void  (IntCalcSer::*P_FUNC)(char *ap_recv,  char *ap_send);

	P_FUNC  p_func;

	std::map<guint32, P_FUNC>                    map_func;   
	std::map<guint32, P_FUNC>::iterator          it_map_func;
 
	typedef  std::pair<guint32, P_FUNC>          Ps_Func;
};

class  FloatCalcSer :public  ModuleServer    
{
    public:
	void  call_switch(char *ap_recv,  char *ap_send);
	void  init();

    protected:

	struct  Arg_Data
	{
		double  a;
		double  b;
		double  c;
	};


    protected:
	void add(char *ap_recv,  char *ap_send);
	void sub(char *ap_recv,  char *ap_send);

    protected:
	typedef  void  (FloatCalcSer::*P_FUNC)(char *ap_recv,  char *ap_send);

	P_FUNC  p_func;

	std::map<guint32, P_FUNC>                    map_func;   
	std::map<guint32, P_FUNC>::iterator          it_map_func;
 
	typedef  std::pair<guint32, P_FUNC>          Ps_Func;
};

void CommunicateSer::reg_server(guint32 a_ser_id, ModuleServer* ap_server)
{
	map_servers.insert(Ps(a_ser_id, ap_server));   	
}

void CommunicateSer::listen_client()
{
	std::cout << "Begin TCP socket..." << std::endl;

	listen_sd = socket (AF_INET, SOCK_STREAM, 0);   
	CHK_ERR(listen_sd, "socket");
  
	memset (&sa_serv, '\0', sizeof(sa_serv));
	sa_serv.sin_family      = AF_INET;
	sa_serv.sin_addr.s_addr = INADDR_ANY;
	sa_serv.sin_port        = htons (PORT);          
  
	err = bind(listen_sd, (struct sockaddr*) &sa_serv, sizeof (sa_serv));                   
	CHK_ERR(err, "bind");
         
	err = listen (listen_sd, 10);                    
	CHK_ERR(err, "listen");

	std::cout << "listen at port " << PORT << std::endl;

	client_len = sizeof(sa_cli);

	while (true)
	{
    	  #ifdef _WIN32
		sd = accept (listen_sd, (struct sockaddr*)&sa_cli, &client_len);
    	  #else
		sd = accept (listen_sd, (struct sockaddr*)&sa_cli, (socklen_t*)&client_len);
    	  #endif
		CHK_ERR(sd, "accept");
		
		//close(sd);
		//close(listen_sd);

		mutex_sd.lock();
		queue_sd.push(sd);
		mutex_sd.unlock();
		
		p_server_thread = Glib::Thread::create(sigc::mem_fun(*this, &CommunicateSer::server), false /*true*/);
		//server();
	}
}

void CommunicateSer::server()
{
	int     server_sd = 0;

	char    recv_buf[PACK_SIZE];
	char    send_buf[PACK_SIZE];

	ModuleServer*      p_server;

	mutex_sd.lock();
	server_sd = queue_sd.front();
	queue_sd.pop();
	mutex_sd.unlock();

	while (true)
	{	
		gint32 ret_recv = recv(server_sd,  recv_buf, PACK_SIZE, 0);
		std::cout << "server read: " <<  ret_recv << std::endl;
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

		guint32    *p_obj_id;
		p_obj_id = (guint32*)recv_buf;

		guint32    id_obj;
	
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

void  IntCalcSer::init()
{
	p_func = &IntCalcSer::add;
	map_func.insert(Ps_Func(FUNC_ADD, p_func));

	p_func = &IntCalcSer::sub;
	map_func.insert(Ps_Func(FUNC_SUB, p_func));
}

void  IntCalcSer::call_switch(char *ap_recv,  char *ap_send)
{
	char       *p_buf;
	guint32    *p_obj_id;
	guint32    *p_fun_id;
	P_FUNC      p_func_tmp;
	
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

void  IntCalcSer::add(char *ap_recv,  char *ap_send)
{
	char       *p_buf;
	guint32    *p_obj_id;
	guint32    *p_fun_id;
	gint32     *p_ret_val;

	Arg_Data      add_data;
	
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

void  IntCalcSer::sub(char *ap_recv,  char *ap_send)
{
	char       *p_buf;
	guint32    *p_obj_id;
	guint32    *p_fun_id;
	gint32     *p_ret_val;

	Arg_Data      sub_data;
	
	p_buf = ap_recv;
	p_obj_id = (guint32*)p_buf;
	p_buf += sizeof(guint32);
	p_fun_id = (guint32*)p_buf;
	p_buf += sizeof(guint32);

	memmove((char*)&sub_data, p_buf, sizeof(sub_data));
	
	sub_data.c = sub_data.a - sub_data.b;

	p_buf = ap_send;
	p_ret_val = (gint32*)p_buf;
	p_buf += sizeof(gint32);

	memmove(p_buf, (char*)&sub_data, sizeof(sub_data));
	*p_ret_val = 0;
}

void  FloatCalcSer::init()
{
	p_func = &FloatCalcSer::add;
	map_func.insert(Ps_Func(FUNC_ADD, p_func));

	p_func = &FloatCalcSer::sub;
	map_func.insert(Ps_Func(FUNC_SUB, p_func));
}

void  FloatCalcSer::call_switch(char *ap_recv,  char *ap_send)
{
	char       *p_buf;
	guint32    *p_obj_id;
	guint32    *p_fun_id;
	P_FUNC      p_func_tmp;
	
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

void  FloatCalcSer::add(char *ap_recv,  char *ap_send)
{
	char       *p_buf;
	guint32    *p_obj_id;
	guint32    *p_fun_id;
	gint32     *p_ret_val;

	Arg_Data      add_data;
	
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

void  FloatCalcSer::sub(char *ap_recv,  char *ap_send)
{
	char       *p_buf;
	guint32    *p_obj_id;
	guint32    *p_fun_id;
	gint32     *p_ret_val;

	Arg_Data      sub_data;
	
	p_buf = ap_recv;
	p_obj_id = (guint32*)p_buf;
	p_buf += sizeof(guint32);
	p_fun_id = (guint32*)p_buf;
	p_buf += sizeof(guint32);

	memmove((char*)&sub_data, p_buf, sizeof(sub_data));
	
	sub_data.c = sub_data.a - sub_data.b;

	p_buf = ap_send;
	p_ret_val = (gint32*)p_buf;
	p_buf += sizeof(gint32);

	memmove(p_buf, (char*)&sub_data, sizeof(sub_data));
	*p_ret_val = 0;
}

class Application : public sigc::trackable
{
    public:
	Application();
	virtual ~Application();
	void run();
 
    private:
	Glib::RefPtr<Glib::MainLoop>  my_main_loop;
	CommunicateSer                my_comm;
  #ifdef _WIN32
	HWND               hWnd;
	WSADATA            wsaData;
  #endif

    private:
	bool on_timeout(int timer_number);
};
 

Application::Application() 
: my_main_loop (Glib::MainLoop::create())
{
    #ifdef _WIN32
	if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0)
	{
		printf("WSAStartup()fail:%ld\n",GetLastError());
		exit(-1);
	}
    #endif

	//sigc::slot<bool> my_slot = sigc::bind(sigc::mem_fun(*this, &Application::on_timeout), 1);
	//sigc::connection conn = Glib::signal_timeout().connect(my_slot, 1000);
}
 
Application::~Application()
{
}

bool Application::on_timeout(int timer_number)
{
	std::cout << "timeout" << std::endl;
	return true;
}

void Application::run()
{
	IntCalcSer                *p_int_calc = new IntCalcSer();
	FloatCalcSer              *p_float_calc = new FloatCalcSer();
	
	p_int_calc->init();
	p_float_calc->init();

	my_comm.reg_server(CLASS_INT_CALC, p_int_calc);
	my_comm.reg_server(CLASS_FLOAT_CALC, p_float_calc);

	Glib::Thread *p_listen_thread = Glib::Thread::create(sigc::mem_fun(my_comm, &CommunicateSer::listen_client), true);    
	//my_comm.listen_client();
	my_main_loop->run();
}


int main(int argc, char *argv[])
{
	Application   *p_app;

	Glib::thread_init();
  
	p_app = new Application;

	p_app->run();
 
	return 0;
}
 
