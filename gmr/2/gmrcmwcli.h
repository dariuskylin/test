#include <stdio.h>
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
#include <fstream>
#include <list>
#include <memory>
#include <deque>
#include <map>
#include <glibmm/thread.h>
#include "gmrcmwdef.h"

class  CommunicateCli
{
    protected:
	struct sockaddr_in sa;

	Glib::Mutex     mutex_cli;

	int err;
	int listen_sd;
	int sd;
	struct sockaddr_in sa_serv;
	struct sockaddr_in sa_cli;
	int    client_len;

    public:
        void    init();	
	gint32  call_ser(char* ap_send, char* ap_recv);
	void    close_socket();
};

class   SerAgent
{
    protected:
	char     recv_buf[PACK_SIZE];
	char     send_buf[PACK_SIZE];

	CommunicateCli   *p_client;
};

class  IntCalcAgent :public  SerAgent    
{
    public:
	gint32  add(gint32 a, gint32 b, gint32 &c);
	gint32  sub(gint32 a, gint32 b, gint32 &c);

    protected:
	struct  Arg_Data
	{
		gint32  a;
		gint32  b;
		gint32  c;
	};

    public:
	void    init(CommunicateCli  *ap_client);		
};

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

class Test
{
    protected:
	FileAgent       file_agent01;
	FileAgent       file_agent02;

	CommunicateCli  *p_comm_cli;

    public:
	void  init(CommunicateCli  *ap_comm_cli);
	void  thread01();
	void  thread02();
};
