#include <stdio.h>
#include <stdlib.h>
#include <glibmm.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

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
#include <fstream>
#include <list>
#include <memory>
#include <deque>
#include <queue>
#include <map>
#include <glibmm/thread.h>
#include "gmrcmwdef.h"


class  ModuleServer 
{
    protected:
	Glib::Mutex  mutex_map;

    protected:
	void  find_function_error(char *ap_send);
    public:
	virtual void call_switch(char *ap_recv,  char *ap_send) = 0;		
};

class  CommunicateSer
{
    private:

	typedef  std::pair<guint64, ModuleServer*>     Ps;   

	struct sockaddr_in sa;
 
	int err;
	int listen_sd;
	int sd;
	struct sockaddr_in sa_serv;
	struct sockaddr_in sa_cli;
	int client_len;

	std::map<guint64, ModuleServer*>               map_servers;   
	std::map<guint64, ModuleServer*>::iterator     it_map; 

	Glib::Mutex                                    mutex_map;

	std::queue<int>                                queue_sd;
	Glib::Thread                                  *p_server_thread;
	Glib::Mutex                                    mutex_sd;

    private:
	void     find_object_error(char  *ap_send);

    public:
  	void     reg_server(IdObj*  ap_id_obj, ModuleServer* ap_server);
        void     init(){};
	void     listen_client();
	void     server();
        void     erase_map(IdObj a_id_obj);
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
