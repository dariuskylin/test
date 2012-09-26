#include "gmrcmwcli.h"

void error_exit(char *message_error)
{
	std::cout << message_error << std::endl;
	exit(-1); 
}

void  IntCalcAgent::init(CommunicateCli  *ap_client)
{
	p_client = ap_client;
}

gint32  IntCalcAgent::add(gint32 a, gint32 b, gint32 &c)
{
	Arg_Data    add_data;

	IdObj      *p_id_obj;

	char       *p_buf;
	guint64     id_gu64;
	guint64    *p_id_gu64;

	guint32    *p_fun_id;
	gint32      ret_val;
	gint32     *p_ret_val;

	gint32     *p_size_send;

	p_buf = send_buf;
	p_size_send = (gint32*)p_buf;
	p_buf += sizeof(gint32);

	p_id_obj = (IdObj*)p_buf;
	p_id_obj->id_class = CLASS_INT_CALC;
	p_id_obj->id_object = 0;

	p_buf += sizeof(guint64);
	p_fun_id = (guint32*)p_buf;
	*p_fun_id = FUNC_ADD;
	p_buf += sizeof(guint32);

	add_data.a = a;
	add_data.b = b;

	*p_size_send = p_buf - send_buf + sizeof(add_data);

	memmove(p_buf, (char*)&add_data, sizeof(add_data));
		
	ret_val = p_client->call_ser(send_buf, recv_buf);
	
	if (ret_val < 0) return  ret_val;

	p_buf = recv_buf + 4;
	p_ret_val = (gint32*)p_buf;
	ret_val = *p_ret_val;
	p_buf += sizeof(gint32);

	memmove((char*)&add_data, p_buf, sizeof(add_data));

	c = add_data.c;

	return  ret_val;
}

gint32  IntCalcAgent::sub(gint32 a, gint32 b, gint32 &c)
{
	Arg_Data    sub_data;

	IdObj      *p_id_obj;

	char       *p_buf;
	guint64     id_gu64;
	guint64    *p_id_gu64;

	guint32    *p_fun_id;
	gint32      ret_val;
	gint32     *p_ret_val;

	gint32     *p_size_send;

	p_buf = send_buf;
	p_size_send = (gint32*)p_buf;
	p_buf += sizeof(gint32);

	p_id_obj = (IdObj*)p_buf;
	p_id_obj->id_class = CLASS_INT_CALC;
	p_id_obj->id_object = 0;

	p_buf += sizeof(guint64);
	p_fun_id = (guint32*)p_buf;
	*p_fun_id = FUNC_SUB;
	p_buf += sizeof(guint32);

	sub_data.a = a;
	sub_data.b = b;

	*p_size_send = p_buf - send_buf + sizeof(sub_data);

	memmove(p_buf, (char*)&sub_data, sizeof(sub_data));
		
	ret_val = p_client->call_ser(send_buf, recv_buf);
	
	if (ret_val < 0) return  ret_val;

	p_buf = recv_buf + 4;
	p_ret_val = (gint32*)p_buf;
	ret_val = *p_ret_val;
	p_buf += sizeof(gint32);

	memmove((char*)&sub_data, p_buf, sizeof(sub_data));

	c = sub_data.c;

	return  ret_val;
}

gint32  FileAgent::init(CommunicateCli  *ap_client)
{
	p_client = ap_client;

	IdObj      *p_id_obj;

	char       *p_buf;
	guint64     id_gu64;
	guint64    *p_id_gu64;

	guint32    *p_obj_id;
	guint32    *p_fun_id;
	gint32      ret_val;
	gint32     *p_ret_val;
	
	gint32     *p_size_send;

	p_buf = send_buf;
	p_size_send = (gint32*)p_buf;
	p_buf += sizeof(gint32);

	p_id_obj = (IdObj*)p_buf;
	p_id_obj->id_class = CLASS_FILE_SER;
	p_id_obj->id_object = 0;

	p_buf += sizeof(guint64);
	p_fun_id = (guint32*)p_buf;
	*p_fun_id = FUNC_CLONE;
	p_buf += sizeof(guint32);

	//memmove(p_buf, (char*)&add_data, sizeof(add_data));
	
	*p_size_send = p_buf - send_buf;
	
	ret_val = p_client->call_ser(send_buf, recv_buf);
	
	if (ret_val < 0) return  ret_val;
	
	p_buf = recv_buf + 4;
	p_ret_val = (gint32*)p_buf;
	ret_val = *p_ret_val;
	p_buf += sizeof(gint32);

	memmove((char*)&arg_out_clone, p_buf, sizeof(arg_out_clone));

	std::cout << "arg_out_clone.id_obj = " << arg_out_clone.id_obj << std::endl;

	return  ret_val;
}

gint32  FileAgent::open(char* filename)
{
	pos_file = 0;
	end_file = false;
	size_file = 0;

	IdObj      *p_id_obj;

	char       *p_buf;
	guint64     id_gu64;
	guint64    *p_id_gu64;

	guint32    *p_fun_id;
	gint32      ret_val;
	gint32     *p_ret_val;

	gint32     *p_size_send;

	p_buf = send_buf;
	p_size_send = (gint32*)p_buf;
	p_buf += sizeof(gint32);

	p_id_obj = (IdObj*)p_buf;
	p_id_obj->id_class = CLASS_FILE_SER;
	p_id_obj->id_object = arg_out_clone.id_obj;
	p_buf += sizeof(guint64);

	p_fun_id = (guint32*)p_buf;
	*p_fun_id = FUNC_OPEN;
	p_buf += sizeof(guint32);

	arg_in_open.rp_name_file.index = p_buf - send_buf + sizeof(arg_in_open);
        arg_in_open.rp_name_file.size = strlen(filename);
	memmove(p_buf, (char*)&arg_in_open, sizeof(arg_in_open));
	p_buf += sizeof(arg_in_open);

	strncpy(p_buf, filename, arg_in_open.rp_name_file.size);

	*p_size_send = p_buf - send_buf + arg_in_open.rp_name_file.size;
		
	ret_val = p_client->call_ser(send_buf, recv_buf);
	
	if (ret_val < 0) return  ret_val;
	
	p_buf = recv_buf + 4;
	p_ret_val = (gint32*)p_buf;
	ret_val = *p_ret_val;
	p_buf += sizeof(gint32);

	memmove((char*)&arg_out_open, p_buf, sizeof(arg_out_open));

	size_file = arg_out_open.size_file;

	std::cout << "open ret_val = " << ret_val << std::endl;
	std::cout << "file size = " << arg_out_open.size_file << std::endl;

	return  ret_val;
}

gint32  FileAgent::read(char* ap_buf, guint32 a_size_read)
{
	size_read = 0;

	if (a_size_read >= size_file - pos_file) a_size_read = size_file - pos_file;

	IdObj      *p_id_obj;

	char       *p_buf;
	guint64     id_gu64;
	guint64    *p_id_gu64;

	guint32    *p_fun_id;
	gint32      ret_val;
	gint32     *p_ret_val;
	gint32     *p_size_send;

	p_buf = send_buf;
	p_size_send = (gint32*)p_buf;
	p_buf += sizeof(gint32);
	p_id_obj = (IdObj*)p_buf;
	p_id_obj->id_class = CLASS_FILE_SER;
	p_id_obj->id_object = arg_out_clone.id_obj;

	arg_in_read.size_read = a_size_read;

	p_buf += sizeof(guint64);
	p_fun_id = (guint32*)p_buf;
	*p_fun_id = FUNC_READ;
	p_buf += sizeof(guint32);

	*p_size_send = p_buf - send_buf + sizeof(arg_in_read);

	memmove(p_buf, (char*)&arg_in_read, sizeof(arg_in_read));
		
	ret_val = p_client->call_ser(send_buf, recv_buf);
	
	if (ret_val < 0) return  ret_val;
	
	p_buf = recv_buf + 4;
	p_ret_val = (gint32*)p_buf;
	ret_val = *p_ret_val;
	p_buf += sizeof(gint32);

	memmove((char*)&arg_out_read, p_buf, sizeof(arg_out_read));

	memmove(ap_buf, recv_buf + arg_out_read.rp_str.index, a_size_read);

	if (ret_val >= 0)
	{
		pos_file += a_size_read;
		size_read = a_size_read;
		if (pos_file >= size_file) end_file = true;
	}

	return  ret_val;
}

gint32  FileAgent::seek(guint32  a_pos)
{
	IdObj      *p_id_obj;

	char       *p_buf;
	guint64     id_gu64;
	guint64    *p_id_gu64;

	guint32    *p_fun_id;
	gint32      ret_val;
	gint32     *p_ret_val;

	gint32     *p_size_send;

	p_buf = send_buf;
	p_size_send = (gint32*)p_buf;
	p_buf += sizeof(gint32);

	p_id_obj = (IdObj*)p_buf;
	p_id_obj->id_class = CLASS_FILE_SER;
	p_id_obj->id_object = arg_out_clone.id_obj;

	arg_in_seek.pos = a_pos;

	p_buf += sizeof(guint64);
	p_fun_id = (guint32*)p_buf;
	*p_fun_id = FUNC_SEEK;
	p_buf += sizeof(guint32);

	*p_size_send = p_buf - send_buf + sizeof(arg_in_seek);

	memmove(p_buf, (char*)&arg_in_seek, sizeof(arg_in_seek));
		
	ret_val = p_client->call_ser(send_buf, recv_buf);
	
	if (ret_val < 0) return  ret_val;
	
	p_buf = recv_buf + 4;
	p_ret_val = (gint32*)p_buf;
	ret_val = *p_ret_val;
	p_buf += sizeof(gint32);

	if (ret_val >= 0) pos_file = a_pos; 

	return  ret_val;
}

gint32  FileAgent::close()
{
	IdObj      *p_id_obj;

	char       *p_buf;
	guint64     id_gu64;
	guint64    *p_id_gu64;

	guint32    *p_fun_id;
	gint32      ret_val;
	gint32     *p_ret_val;

	gint32     *p_size_send;

	p_buf = send_buf;
	p_size_send = (gint32*)p_buf;
	p_buf += sizeof(gint32);

	p_id_obj = (IdObj*)p_buf;
	p_id_obj->id_class = CLASS_FILE_SER;
	p_id_obj->id_object = arg_out_clone.id_obj;

	p_buf += sizeof(guint64);
	p_fun_id = (guint32*)p_buf;
	*p_fun_id = FUNC_CLOSE;
	p_buf += sizeof(guint32);
		
	ret_val = p_client->call_ser(send_buf, recv_buf);
	
	if (ret_val < 0) return  ret_val;
	
	p_buf = recv_buf + 4;
	p_ret_val = (gint32*)p_buf;
	ret_val = *p_ret_val;
	p_buf += sizeof(gint32);

	return  ret_val;
}

gint32  FileAgent::erase()
{
	IdObj      *p_id_obj;

	char       *p_buf;
	guint64     id_gu64;
	guint64    *p_id_gu64;

	guint32    *p_fun_id;
	gint32      ret_val;
	gint32     *p_ret_val;

	gint32     *p_size_send;

	p_buf = send_buf;
	p_size_send = (gint32*)p_buf;
	p_buf += sizeof(gint32);

	p_id_obj = (IdObj*)p_buf;
	p_id_obj->id_class = CLASS_FILE_SER;
	p_id_obj->id_object = arg_out_clone.id_obj;

	p_buf += sizeof(guint64);
	p_fun_id = (guint32*)p_buf;
	*p_fun_id = FUNC_ERASE;
	p_buf += sizeof(guint32);
		
	ret_val = p_client->call_ser(send_buf, recv_buf);
	
	if (ret_val < 0) return  ret_val;
	
	p_buf = recv_buf + 4;
	p_ret_val = (gint32*)p_buf;
	ret_val = *p_ret_val;
	p_buf += sizeof(gint32);

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
	Glib::Mutex::Lock lock(mutex_cli);

	gint32      ret_val = 0;

	gint32     *p_size_send;

	p_size_send = (gint32*)ap_send;

	gint32 ret_send = send(sd, ap_send, *p_size_send, 0);
	std::cout << "client send: " << ret_send << std::endl;

	gint32   ret_recv = 0;
	gint32   ret_tmp = 0;
	gint32  *p_size_read = (gint32*)ap_recv;
	char    *p_recv = ap_recv;
			       
	ret_tmp = recv(sd,  (char*)p_size_read, 4, 0);

	if (ret_tmp < 0) return -1;
		
	*p_size_read -= 4;

	p_recv += 4;

	ret_recv += ret_tmp;

	while (true)
	{
		ret_tmp = recv(sd,  p_recv, *p_size_read, 0);
		if (ret_tmp < 0) return -1;
		ret_recv += ret_tmp;
		p_recv += ret_tmp;
		*p_size_read -= ret_tmp;
		if (*p_size_read == 0) break;
	}

	//gint32 ret_recv = recv(sd,  ap_recv, PACK_SIZE, 0);
	std::cout << "client read: " <<  ret_recv << std::endl;
	ret_val = 1;
	return ret_val;
}

void  CommunicateCli::close_socket()
{
    #ifdef _WIN32
	closesocket(sd);
    #else	
	close(sd);
    #endif
}

void  Test::init(CommunicateCli  *ap_comm_cli)
{
	p_comm_cli = ap_comm_cli;
}

void  Test::thread01()
{
	char            my_buf[4096];
	gint32          size_read;
	gint32          ret_val;
        std::ofstream   my_out_file;

	ret_val = file_agent01.init(p_comm_cli);
	if (ret_val != 0) error_exit("remote file server init error!");
	std::cout << " ret_val = " << ret_val << std::endl;
	
	my_out_file.open("my_out01.txt" , std::ios::binary);
	ret_val = file_agent01.open("test01.txt");
	std::cout << "file_agent01.open  ret_val = " << ret_val << std::endl;
	if (ret_val != 0) error_exit("file_agent01.open  error!");

	while (true)
	{
		ret_val = file_agent01.read(my_buf, 1024);
		size_read = file_agent01.get_size_read();
		std::cout << "file_agent01 size_read = " << size_read << std::endl;
		my_out_file.write(my_buf, size_read);
		if (file_agent01.get_if_end() == true) break;
		Glib::usleep(1000*10);
	}

	my_out_file.close();

	ret_val = file_agent01.close();
	ret_val = file_agent01.erase();
}

void  Test::thread02()
{
	char            my_buf[4096];
	gint32          size_read;
	gint32          ret_val;
        std::ofstream   my_out_file;

	ret_val = file_agent02.init(p_comm_cli);
	if (ret_val != 0) error_exit("remote file server init error!");
	std::cout << " ret_val = " << ret_val << std::endl;
	
	my_out_file.open("my_out02.txt" , std::ios::binary);
	ret_val = file_agent02.open("test02.txt");
	std::cout << "file_agent02.open  ret_val = " << ret_val << std::endl;
	if (ret_val != 0) error_exit("file_agent02.open  error!");

	while (true)
	{
		ret_val = file_agent02.read(my_buf, 1024);
		size_read = file_agent02.get_size_read();
		std::cout << "file_agent02 size_read = " << size_read << std::endl;
		my_out_file.write(my_buf, size_read);
		if (file_agent02.get_if_end() == true) break;
		Glib::usleep(1000*10);
	}

	my_out_file.close();

	ret_val = file_agent02.close();
	ret_val = file_agent02.erase();
}

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

	Glib::thread_init();

	char            my_buf[4096];
	gint32          size_read;
	gint32          ret_val;
	FileAgent       file_agent;

	IntCalcAgent    int_calc;

	CommunicateCli  *p_comm_cli = new CommunicateCli;

	p_comm_cli->init();

	int_calc.init(p_comm_cli);

	gint32  ret_int;

	ret_val = int_calc.add(1, 2, ret_int);
	std::cout << "int_calc.add ret_val = " << ret_val << std::endl;
	if (ret_val != 0) error_exit("int_calc.add  error!");
	std::cout << "int add ret = " << ret_int << std::endl;

	ret_val = int_calc.sub(35, 9, ret_int);
	if (ret_val != 0) error_exit("int_calc.sub  error!");
	std::cout << "int sub ret = " << ret_int << std::endl;

	ret_val = file_agent.init(p_comm_cli);
	if (ret_val != 0) error_exit("remote file server init error!");
	std::cout << " ret_val = " << ret_val << std::endl;
	ret_val = file_agent.open("jgijg/test02.txt");
	ret_val = file_agent.seek(33);
	ret_val = file_agent.read(my_buf, 24);
	my_buf[24] = '\0';
	std::cout << "read remote file: " << my_buf << std::endl;
	ret_val = file_agent.close();
	ret_val = file_agent.erase();

	Test   *p_test = new Test;
	
	p_test->init(p_comm_cli);

	Glib::Thread  *p_thread01 = Glib::Thread::create(sigc::mem_fun(*p_test, &Test::thread01), true);
	Glib::Thread  *p_thread02 = Glib::Thread::create(sigc::mem_fun(*p_test, &Test::thread02), true);

	p_thread01->join();
	p_thread02->join();

	p_comm_cli->close_socket();

	return 0;
}
