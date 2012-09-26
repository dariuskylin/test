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

#pragma pack(4)

namespace  gcpprc
{

#define  SOCKET_ERR           -1001
#define  CONNECT_ERR          -1002
#define  ERR_FIND_OBJ         -1003
#define  ERR_FIND_FUNC        -1004

#define  NUM_ARG              10
#define  PACK_SIZE            (4096*2)

struct  IdObj
{
	guint32     id_class;
	guint32     id_object;
};

struct  RemotePoint
{
	guint32     index;
	guint32     size;
};

struct  FileStat
{
	char         test[7];
	guint32      st_size;
	guint32      st_time;
};

void error_exit(char *message_error);

class PAny
{
    public:
        char     *p_data;
        guint32   size_data;

    public:

        PAny(): p_data(0), size_data(0)
        {
        }

        PAny(char* a_p, guint32 a_size_data)
        {
                p_data = a_p;
                size_data = a_size_data;               
        }

        PAny(const PAny & other)
          : p_data(other.p_data), size_data(other.size_data)
        {
        }

        template<typename ValueType>
        PAny (const ValueType & rhs)
        {
                if (rhs == 0)
                {
                        p_data = 0;
                        size_data = 0;
                }
                else
                {       
                        p_data = (char*)rhs;
                        size_data = sizeof(*rhs);
                }
        }

        PAny (char* rhs)
        {
                if (rhs == 0)
                {
                        p_data = 0;
                        size_data = 0;
                }
                else
                {       
                        p_data = (char*)rhs;
                        size_data = strlen(rhs);
			if (size_data) size_data++;
                }
        }

        template<typename ValueType>
        PAny & operator=(const ValueType & rhs)
        {
                if (rhs == 0)
                {
                        p_data = 0;
                        size_data = 0;
                }
                else
                {       
                        p_data = (char*)rhs;
                        size_data = sizeof(*rhs);
                }
                return *this;
        }

        PAny & operator=(PAny &rhs)
        {
                p_data = rhs.p_data;
                size_data = rhs.size_data;
                return *this;
        }

        PAny & operator=(char* rhs)
        {
                if (rhs == 0)
                {
                        p_data = 0;
                        size_data = 0;
                }
                else
                {       
                        p_data = (char*)rhs;
                        size_data = strlen(rhs);
			if (size_data) size_data++;
                }
                return *this;
        }

    public:
       
        void   set_pany(char* a_p, int a_size_data);
        char*  get_data();
        int    size_of_data();
};


class   SerAgent
{
    protected:
	struct sockaddr_in sa; 
	struct sockaddr_in sa_serv;
	struct sockaddr_in sa_cli;

	char     data_buf[PACK_SIZE];
	IdObj    id_obj;
	int      err;
	int      listen_sd;
	int      sd;
	int      client_len;

    public:
        gint32    init(char *ap_server_ip, gint32 a_port);	
	gint32  call_ser(char* ap_data);
	void    close_socket();

    protected:
	int     remote_call(guint32 a_func_id, PAny pa = (char*)0 , PAny pb = (char*)0, PAny pc = (char*)0, 
			    PAny pd = (char*)0, PAny pe = (char*)0, PAny pf = (char*)0, PAny pg = (char*)0,
			    PAny ph = (char*)0, PAny pi = (char*)0, PAny pj = (char*)0);
};

class  ModuleServer 
{
    protected:
	Glib::Mutex  mutex_map;

    protected:
	typedef  void  (ModuleServer::*P_FUNC)(char  *ap_data, char *ap_a,  char *ap_b,  char *ap_c,
					       char *ap_d,  char *ap_e,  char *ap_f, char *ap_g,
					       char *ap_h,  char *ap_i,  char *ap_j);

	P_FUNC  p_func;

	std::map<guint32, P_FUNC>                    map_func;   
	std::map<guint32, P_FUNC>::iterator          it_map_func;
 
	typedef  std::pair<guint32, P_FUNC>          Ps_Func;

    protected:
        void  reg_func(guint32  a_id_func,  P_FUNC a_pfunc);	
	void  find_function_error(char *ap_data);
	void  unpack_data(char *ap_data, char **pp_ch);
	void  fix_size_of_data(char *ap_data,  gint32 a_fix_size);

    public:
	void  call_switch(char *ap_data);		
};

class  CommunicateSer
{
    private:

	typedef  std::pair<guint64, ModuleServer*>     Ps;   

	struct sockaddr_in sa;
 
	int err;
	int listen_sd;
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
	void     find_object_error(char  *ap_data);

    public:
  	void     reg_server(IdObj*  ap_id_obj, ModuleServer* ap_server);
        void     init(){};
	void     listen_client(gint32 a_port);
	void     server(gint32 a_server_sd);
        void     erase_map(IdObj a_id_obj);
};

}; //namespace  gcpprc

