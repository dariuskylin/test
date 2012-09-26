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
#include <map>
#include <glibmm/thread.h>
#include "cliserdef.h"
#include "gcpprclib.h"

class  CalcAgent :public  gcpprc::SerAgent    
{
    public:
	gint32  add(gint32 a, gint32 b, gint32 &c);
	gint32  sub(gint32 a, gint32 b, gint32 &c);
	gint32  strlink(char *str_a, char *str_b, char *str_c);

    public:
	gint32  init(char *ap_server_ip, gint32 a_port);		
};

class  FileAgent :public  gcpprc::SerAgent    
{
    protected:
	guint32                pos_file;
	bool                   end_file;
	guint32                size_file;
	guint32                size_read;
	guint32                size_write;

	gcpprc::FileStat       stat_file;

    public:
	gint32   clone(guint32 &a_id_obj);
	gint32   open(char* filename, char* mode);
	guint32  get_size(){return size_file;};
	gint32   read(char *ap_buf, guint32 &a_size_read);
	gint32   write(char *ap_buf, guint32 &a_size_write);
	gint32   seek(guint32  &a_pos);
	gint32   close();
	gint32   erase();
	bool     get_if_end(){return  end_file;}
	guint32  get_size_read(){return  size_read;}
	guint32  get_pos_file(){return  pos_file;}

    public:
	gint32   init(char *ap_server_ip, gint32 a_port);		
};

