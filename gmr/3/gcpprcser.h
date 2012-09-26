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
#include "cliserdef.h"
#include "gcpprclib.h"

class  CalcSer :public  gcpprc::ModuleServer    
{
    public:
	void  init();

    protected:
	gint32  number;

	void add(char  *ap_data, char *ap_a,  char *ap_b,  char *ap_c,
		 char *ap_d,  char *ap_e,  char *ap_f, char *ap_g,
		 char *ap_h,  char *ap_i,  char *ap_j);

	void sub(char  *ap_data, char *ap_a,  char *ap_b,  char *ap_c,
		 char *ap_d,  char *ap_e,  char *ap_f, char *ap_g,
		 char *ap_h,  char *ap_i,  char *ap_j);

	void strlink(char  *ap_data, char *ap_a,  char *ap_b,  char *ap_c,
		     char *ap_d,  char *ap_e,  char *ap_f, char *ap_g,
		     char *ap_h,  char *ap_i,  char *ap_j);

	void do_add(gint32 *ap_a,  gint32 *ap_b,  gint32 *ap_c);
	void do_sub(gint32 *ap_a,  gint32 *ap_b,  gint32 *ap_c);
	void do_strlink(char *str_a, char *str_b, char *str_c);
};

class  FileSer :public  gcpprc::ModuleServer    
{
    protected:
	static  guint32             counter;
	Glib::Mutex                 mutex_clone;
	gcpprc::IdObj               m_id_obj;
	gcpprc::CommunicateSer     *p_comm;
        std::fstream                m_file;

    public:
	void  init(gcpprc::CommunicateSer  *ap_comm, gcpprc::IdObj  a_id_ob);

    protected:
	void  clone(char  *ap_data, char *ap_a,  char *ap_b,  char *ap_c,
		    char *ap_d,  char *ap_e,  char *ap_f, char *ap_g,
		    char *ap_h,  char *ap_i,  char *ap_j);

	void  open(char  *ap_data, char *ap_a,  char *ap_b,  char *ap_c,
		   char *ap_d,  char *ap_e,  char *ap_f, char *ap_g,
		   char *ap_h,  char *ap_i,  char *ap_j);

	void  read(char  *ap_data, char *ap_a,  char *ap_b,  char *ap_c,
		   char *ap_d,  char *ap_e,  char *ap_f, char *ap_g,
		   char *ap_h,  char *ap_i,  char *ap_j);

	void  write(char  *ap_data, char *ap_a,  char *ap_b,  char *ap_c,
		   char *ap_d,  char *ap_e,  char *ap_f, char *ap_g,
		   char *ap_h,  char *ap_i,  char *ap_j);

	void  seek(char  *ap_data, char *ap_a,  char *ap_b,  char *ap_c,
		   char *ap_d,  char *ap_e,  char *ap_f, char *ap_g,
		   char *ap_h,  char *ap_i,  char *ap_j);

	void  close(char  *ap_data, char *ap_a,  char *ap_b,  char *ap_c,
		    char *ap_d,  char *ap_e,  char *ap_f, char *ap_g,
		    char *ap_h,  char *ap_i,  char *ap_j);

	void  erase(char  *ap_data, char *ap_a,  char *ap_b,  char *ap_c,
		    char *ap_d,  char *ap_e,  char *ap_f, char *ap_g,
		    char *ap_h,  char *ap_i,  char *ap_j);

	void  do_clone(guint32 *ap_id_obj);
	void  do_open(char  *file_name, char  *mode, gcpprc::FileStat  *ap_stat_file);
	void  do_read(char  *ap_buf, guint32 *ap_size_read);
	void  do_write(char  *ap_buf, guint32 *ap_size_write);
	void  do_seek(guint32  *ap_pos);
	void  do_close();
	void  do_erase();
};

class Application : public sigc::trackable
{
    public:
	Application();
	virtual ~Application();
	void run();
 
    private:
	Glib::RefPtr<Glib::MainLoop>     rp_main_loop;
	gcpprc::CommunicateSer           main_comm;
  #ifdef _WIN32
	WSADATA            wsaData;
  #endif

    private:
	bool on_timeout(gint32 timer_number);
};
