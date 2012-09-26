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
#include <list>
#include <memory>
#include <deque>
#include <map>
#include <glibmm/thread.h>

#pragma pack(4)

#define CHK_NULL(x) if ((x)==NULL) exit (-1)
#define CHK_ERR(err,s) if ((err) < 0) {printf("%s error\n", s); exit(-2); }

#define  SERVER_IP        "127.0.0.1"
#define  PORT             8000

#define  CLASS_INT_CALC       10001
#define  CLASS_FLOAT_CALC     10002

#define  FUNC_ADD         20001
#define  FUNC_SUB         20002

#define  PACK_SIZE        4096

void error_exit(char *message_error)
{
	std::cout << message_error << std::endl;
	exit(-1); 
}

class  CommunicateCli
{
    protected:
	struct sockaddr_in sa;
 
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

    public:
	void    init(CommunicateCli  *ap_client);		
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
};

class  FloatCalcAgent :public  SerAgent    
{
    public:
	gint32  add(double a, double b, double &c);
	gint32  sub(double a, double b, double &c);

    protected:
	struct  Arg_Data
	{
		double  a;
		double  b;
		double  c;
	};
};

void  SerAgent::init(CommunicateCli  *ap_client)
{
	p_client = ap_client;
}

gint32  IntCalcAgent::add(gint32 a, gint32 b, gint32 &c)
{
	char       *p_buf;
	guint32    *p_obj_id;
	guint32    *p_fun_id;
	gint32      ret_val;
	gint32     *p_ret_val;

	Arg_Data    add_data;

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
	
	if (ret_val < 0) return  ret_val;
	
	p_buf = recv_buf;
	p_ret_val = (gint32*)p_buf;
	ret_val = *p_ret_val;
	p_buf += sizeof(gint32);

	memmove((char*)&add_data, p_buf, sizeof(add_data));

	c = add_data.c;

	return  ret_val;
}

gint32  IntCalcAgent::sub(gint32 a, gint32 b, gint32 &c)
{
	char       *p_buf;
	guint32    *p_obj_id;
	guint32    *p_fun_id;
	Arg_Data    sub_data;
	gint32      ret_val;
	gint32     *p_ret_val;
	
	p_buf = send_buf;
	p_obj_id = (guint32*)p_buf;
	*p_obj_id = CLASS_INT_CALC;
	p_buf += sizeof(guint32);
	p_fun_id = (guint32*)p_buf;
	*p_fun_id = FUNC_SUB;
	p_buf += sizeof(guint32);

	sub_data.a = a;
	sub_data.b = b;

	memmove(p_buf, (char*)&sub_data, sizeof(sub_data));

	ret_val = p_client->call_ser(send_buf, recv_buf);
	
	if (ret_val < 0) return  ret_val;

	p_buf = recv_buf;
	p_ret_val = (gint32*)p_buf;
	ret_val = *p_ret_val;
	p_buf += sizeof(gint32);

	memmove((char*)&sub_data, p_buf, sizeof(sub_data));

	c = sub_data.c;

	return  ret_val;
}

gint32  FloatCalcAgent::add(double a, double b, double &c)
{
	char       *p_buf;
	guint32    *p_obj_id;
	guint32    *p_fun_id;
	gint32      ret_val;
	gint32     *p_ret_val;
	Arg_Data    add_data;
	
	p_buf = send_buf;
	p_obj_id = (guint32*)p_buf;
	*p_obj_id = CLASS_FLOAT_CALC;
	p_buf += sizeof(guint32);
	p_fun_id = (guint32*)p_buf;
	*p_fun_id = FUNC_ADD;
	p_buf += sizeof(guint32);

	add_data.a = a;
	add_data.b = b;

	memmove(p_buf, (char*)&add_data, sizeof(add_data));
		
	ret_val = p_client->call_ser(send_buf, recv_buf);

	if (ret_val < 0) return  ret_val;

	p_buf = recv_buf;
	p_ret_val = (gint32*)p_buf;
	ret_val = *p_ret_val;
	p_buf += sizeof(gint32);

	memmove((char*)&add_data, p_buf, sizeof(add_data));

	c = add_data.c;

	return  ret_val;
}

gint32  FloatCalcAgent::sub(double a, double b, double &c)
{
	char       *p_buf;
	guint32    *p_obj_id;
	guint32    *p_fun_id;
	gint32      ret_val;
	gint32     *p_ret_val;
	Arg_Data    sub_data;
	
	p_buf = send_buf;
	p_obj_id = (guint32*)p_buf;
	*p_obj_id = CLASS_FLOAT_CALC;
	p_buf += sizeof(guint32);
	p_fun_id = (guint32*)p_buf;
	*p_fun_id = FUNC_SUB;
	p_buf += sizeof(guint32);

	sub_data.a = a;
	sub_data.b = b;

	memmove(p_buf, (char*)&sub_data, sizeof(sub_data));
		
	ret_val = p_client->call_ser(send_buf, recv_buf);
	
	if (ret_val < 0) return  ret_val;

	p_buf = recv_buf;
	p_ret_val = (gint32*)p_buf;
	ret_val = *p_ret_val;
	p_buf += sizeof(gint32);

	memmove((char*)&sub_data, p_buf, sizeof(sub_data));

	c = sub_data.c;

	return  ret_val;
}

void  CommunicateCli::init()
{
	gint32  ret_val = 0;

	sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sd < 0) error_exit("socket create error !");

	memset(&sa_serv, 0, sizeof(sa_serv));   
        sa_serv.sin_family = AF_INET;                  
        sa_serv.sin_addr.s_addr = inet_addr(SERVER_IP);   
        sa_serv.sin_port = htons(PORT);    
	ret_val = connect(sd, (struct sockaddr*)&sa_serv, sizeof(sa_serv));   
	if (ret_val < 0) error_exit("connect error !");
}

gint32  CommunicateCli::call_ser(char* ap_send, char* ap_recv)
{
	gint32  ret_val = 0;

	gint32 ret_send = send(sd, ap_send, PACK_SIZE, 0);
	//std::cout << "client send: " << ret_send << std::endl;

	gint32 ret_recv = recv(sd,  ap_recv, PACK_SIZE, 0);
	//std::cout << "client read: " <<  ret_recv << std::endl;
	if (ret_recv != PACK_SIZE) ret_val = -1;

	return ret_val;
}

void  CommunicateCli::close_socket()
{
	close(sd);
}

CommunicateCli  my_comm_cli;
IntCalcAgent    int_calc;
FloatCalcAgent  float_calc;

int main(int argc, char *argv[])
{
    #ifdef _WIN32
	HWND               hWnd;
	WSADATA            wsaData;
    #endif

    #ifdef _WIN32
	if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0)
	{
		printf("WSAStartup()fail:%ld\n",GetLastError());
		exit(-1);
	}
    #endif

	my_comm_cli.init();
	int_calc.init(&my_comm_cli);
	float_calc.init(&my_comm_cli);

	gint32  ret_int;
	double  ret_float;

	gint32  ret_val;

	ret_val = int_calc.add(1, 2, ret_int);
	if (ret_val != 0) error_exit("int_calc.add  error!");
	std::cout << "int add ret = " << ret_int << std::endl;

	ret_val = int_calc.sub(35, 9, ret_int);
	if (ret_val != 0) error_exit("int_calc.sub  error!");
	std::cout << "int sub ret = " << ret_int << std::endl;

	ret_val = float_calc.add(1.156, 1.21, ret_float);
	if (ret_val != 0) error_exit("float_calc.add  error!");
	std::cout << "float add ret = " << ret_float << std::endl;

	ret_val = float_calc.sub(6.34, 2.21, ret_float);
	if (ret_val != 0) error_exit("float_calc.sub  error!");
	std::cout << "float sub ret = " << ret_float << std::endl;

	my_comm_cli.close_socket();

	return 0;
}
