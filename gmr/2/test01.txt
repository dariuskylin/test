#include  "gmrcmwser.h"


guint32  FileSer::counter = 0;

void  ModuleServer::find_function_error(char *ap_send)
{
	char       *p_buf;
	gint32     *p_size_send;
	gint32     *p_ret_val;

	p_buf = ap_send;
	p_size_send = (gint32*)p_buf;
	p_buf += sizeof(gint32);
	p_ret_val = (gint32*)p_buf;
	p_buf += sizeof(gint32);

	*p_size_send = p_buf - ap_send;
	*p_ret_val = ERR_FIND_FUNC;

	std::cout << "error: can not find server function!" << std::endl; 
}

void  CommunicateSer::find_object_error(char  *ap_send)
{
	char       *p_buf;
	gint32     *p_size_send;
	gint32     *p_ret_val;

	p_buf = ap_send;
	p_size_send = (gint32*)p_buf;
	p_buf += sizeof(gint32);
	p_ret_val = (gint32*)p_buf;
	p_buf += sizeof(gint32);

	*p_size_send = p_buf - ap_send;
	*p_ret_val = ERR_FIND_OBJ;

	std::cout << "error: can not find server object!" << std::endl; 
}

void  CommunicateSer::reg_server(IdObj*  ap_id_obj, ModuleServer* ap_server)
{
	//guint64  id_gu64;
	guint64  *p_id_gu64;

	p_id_gu64 = (guint64*)ap_id_obj;
	//id_gu64 = *p_id_gu64;

	map_servers.insert(Ps(*p_id_gu64, ap_server));
}

void  CommunicateSer::listen_client()
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
		gint32   ret_recv = 0;
		gint32   ret_tmp = 0;
		gint32  *p_size_read = (gint32*)recv_buf;
		char    *p_recv = recv_buf;
			       
		ret_tmp = recv(server_sd,  (char*)p_size_read, 4, 0);
		
		*p_size_read -= 4;

		p_recv += 4;

		ret_recv += ret_tmp;

		while (true)
		{
			ret_tmp = recv(server_sd,  p_recv, *p_size_read, 0);
			if (ret_tmp <= 0) break;
			p_recv += ret_tmp;
			ret_recv += ret_tmp;
			*p_size_read -= ret_tmp;
			if (*p_size_read == 0) break;
		}

		std::cout << "server read: " <<  ret_recv << std::endl;

		if (ret_tmp <= 0)
		{
                    #ifdef _WIN32
			closesocket(server_sd);
		    #else	
			close(server_sd);
		    #endif
			std::cout << "close server socket" << std::endl;
			break;
		}

		guint64     *p_id_gu64;
		p_id_gu64 = (guint64*)(recv_buf + 4);

		bool   find_object = false;

		mutex_map.lock();	
		it_map = map_servers.find(*p_id_gu64);
		if (it_map == map_servers.end())
		{
			find_object_error(send_buf);
		}
		else
		{
			p_server = it_map->second;
			find_object = true;
		}
		mutex_map.unlock();	
		
		//std::cout << it_map->first << std::endl;

		if (find_object) p_server->call_switch(recv_buf, send_buf);	

		gint32     *p_size_send;

		p_size_send = (gint32*)send_buf;

		gint32 ret_send = send(server_sd, send_buf, *p_size_send, 0);
		
		std::cout << "server send: " << ret_send << std::endl;			
	}
}

void  CommunicateSer::erase_map(IdObj a_id_obj)
{
        std::cout << "erase map" << std::endl;
 
	guint64  *p_id_gu64;
	p_id_gu64 = (guint64*)&a_id_obj;
        map_servers.erase(*p_id_gu64);
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
	guint64    *p_id_gu64;
	guint32    *p_fun_id;
	P_FUNC      p_func_tmp;
	
	p_buf = ap_recv + 4;
	p_id_gu64 = (guint64*)p_buf;
	p_buf += sizeof(guint64);
	p_fun_id = (guint32*)p_buf;
	p_buf += sizeof(guint32);

	bool      find_func = false;

	mutex_map.lock();	
	it_map_func = map_func.find(*p_fun_id);
	if (it_map_func == map_func.end()) 
	{
		find_function_error(ap_send);
	}
	else
	{
		p_func_tmp = it_map_func->second;
		find_func = true;
	}
	mutex_map.unlock();
	
        if (find_func) (this->*p_func_tmp)(ap_recv, ap_send);
}

void  IntCalcSer::add(char *ap_recv,  char *ap_send)
{
	Arg_Data      add_data;

	char       *p_buf;
	guint64    *p_id_gu64;
	guint32    *p_fun_id;
	P_FUNC      p_func_tmp;
	gint32     *p_ret_val;

	p_buf = ap_recv + 4;
	p_id_gu64 = (guint64*)p_buf;
	p_buf += sizeof(guint64);
	p_fun_id = (guint32*)p_buf;
	p_buf += sizeof(guint32);

	memmove((char*)&add_data, p_buf, sizeof(add_data));
	
	add_data.c = add_data.a + add_data.b;

	gint32     *p_size_send;

	p_buf = ap_send;
	p_size_send = (gint32*)p_buf;
	p_buf += sizeof(gint32);
	p_ret_val = (gint32*)p_buf;
	p_buf += sizeof(gint32);

	*p_size_send = p_buf - ap_send + sizeof(add_data);

	memmove(p_buf, (char*)&add_data, sizeof(add_data));

	*p_ret_val = 0;
}

void  IntCalcSer::sub(char *ap_recv,  char *ap_send)
{
	Arg_Data      sub_data;

	char       *p_buf;
	guint64    *p_id_gu64;
	guint32    *p_fun_id;
	P_FUNC      p_func_tmp;
	gint32     *p_ret_val;

	p_buf = ap_recv + 4;
	p_id_gu64 = (guint64*)p_buf;
	p_buf += sizeof(guint64);
	p_fun_id = (guint32*)p_buf;
	p_buf += sizeof(guint32);

	memmove((char*)&sub_data, p_buf, sizeof(sub_data));
	
	sub_data.c = sub_data.a - sub_data.b;

	gint32     *p_size_send;

	p_buf = ap_send;
	p_size_send = (gint32*)p_buf;
	p_buf += sizeof(gint32);
	p_ret_val = (gint32*)p_buf;
	p_buf += sizeof(gint32);

	*p_size_send = p_buf - ap_send + sizeof(sub_data);

	memmove(p_buf, (char*)&sub_data, sizeof(sub_data));

	*p_ret_val = 0;
}


void  FileSer::init(CommunicateSer  *ap_my_comm, IdObj  a_id_obj)
{
        m_id_obj = a_id_obj;

	p_my_comm = ap_my_comm;

	p_func = &FileSer::clone;
	map_func.insert(Ps_Func(FUNC_CLONE, p_func));

	p_func = &FileSer::open;
	map_func.insert(Ps_Func(FUNC_OPEN, p_func));

	p_func = &FileSer::read;
	map_func.insert(Ps_Func(FUNC_READ, p_func));

	p_func = &FileSer::seek;
	map_func.insert(Ps_Func(FUNC_SEEK, p_func));

	p_func = &FileSer::close;
	map_func.insert(Ps_Func(FUNC_CLOSE, p_func));

	p_func = &FileSer::erase;
	map_func.insert(Ps_Func(FUNC_ERASE, p_func));

}

void  FileSer::call_switch(char *ap_recv,  char *ap_send)
{
	std::cout << "File Server Module" << std::endl;

	char       *p_buf;
	guint64    *p_id_gu64;
	guint32    *p_fun_id;
	P_FUNC      p_func_tmp;
	
	p_buf = ap_recv + 4;
	p_id_gu64 = (guint64*)p_buf;
	p_buf += sizeof(guint64);
	p_fun_id = (guint32*)p_buf;
	p_buf += sizeof(guint32);

	bool      find_func = false;

	mutex_map.lock();	
	it_map_func = map_func.find(*p_fun_id);
	if (it_map_func == map_func.end()) 
	{
		find_function_error(ap_send);
	}
	else
	{
		p_func_tmp = it_map_func->second;
		find_func = true;
	}
	mutex_map.unlock();
	
        if (find_func) (this->*p_func_tmp)(ap_recv, ap_send);
}

void  FileSer::clone(char *ap_recv,  char *ap_send)
{
	std::cout << "File Server Module clone" << std::endl;

	Glib::Mutex::Lock lock(mutex_clone);

	counter++; 

	char       *p_buf;
	guint64    *p_id_gu64;
	guint32    *p_fun_id;
	P_FUNC      p_func_tmp;
	gint32     *p_ret_val;

	p_buf = ap_recv + 4;
	p_id_gu64 = (guint64*)p_buf;
	p_buf += sizeof(guint64);
	p_fun_id = (guint32*)p_buf;
	p_buf += sizeof(guint32);
	
	arg_out_clone.id_obj = counter; 

	FileSer    *p_file_ser = new FileSer;

	IdObj   *p_id_obj = &m_id_obj;
	
	p_id_obj->id_class = CLASS_FILE_SER; 
	p_id_obj->id_object = arg_out_clone.id_obj; 

	p_file_ser->init(p_my_comm, m_id_obj);

	p_my_comm->reg_server(p_id_obj, p_file_ser);

	gint32     *p_size_send;

	p_buf = ap_send;
	p_size_send = (gint32*)p_buf;
	p_buf += sizeof(gint32);
	p_ret_val = (gint32*)p_buf;
	p_buf += sizeof(gint32);

	*p_size_send = p_buf - ap_send + sizeof(arg_out_clone);

	memmove(p_buf, (char*)&arg_out_clone, sizeof(arg_out_clone));
	*p_ret_val = 0;
}

void  FileSer::open(char *ap_recv,  char *ap_send)
{
	std::cout << "File Server Module open" << std::endl;
	
	char       *p_name_file;
	char       *p_buf;
	guint64    *p_id_gu64;
	guint32    *p_fun_id;
	P_FUNC      p_func_tmp;
	gint32     *p_ret_val;

	struct stat  stat_file;

	p_buf = ap_recv + 4;
	p_id_gu64 = (guint64*)p_buf;
	p_buf += sizeof(guint64);
	p_fun_id = (guint32*)p_buf;
	p_buf += sizeof(guint32);

	memmove((char*)&arg_in_open, p_buf, sizeof(arg_in_open));

	p_name_file = ap_recv + arg_in_open.rp_name_file.index;
	
	*(p_name_file + arg_in_open.rp_name_file.size) = '\0';

	std::cout << "open file name = " << p_name_file << std::endl;

	char  *p_chtmp = p_name_file + arg_in_open.rp_name_file.size;

	while (true)
	{
		if (*p_chtmp == '/' || *p_chtmp == '\\')
		{
			p_chtmp++;
			break; 
		}
		p_chtmp--;
		if (p_chtmp == p_name_file) break;
	}

        my_file.open(p_chtmp, std::ios::binary);

	stat(p_chtmp, &stat_file);

	arg_out_open.size_file = stat_file.st_size; 

	gint32     *p_size_send;

	p_buf = ap_send;
	p_size_send = (gint32*)p_buf;
	p_buf += sizeof(gint32);
	p_ret_val = (gint32*)p_buf;
	p_buf += sizeof(gint32);

	*p_size_send = p_buf - ap_send + sizeof(arg_out_open);

	memmove(p_buf, (char*)&arg_out_open, sizeof(arg_out_open));

	*p_ret_val = 0;
	if (my_file.good() == false) *p_ret_val = ERR_FILE_OPEN;
}

void  FileSer::read(char *ap_recv,  char *ap_send)
{
	std::cout << "File Server Module read" << std::endl;

	char       *p_buf;
	guint64    *p_id_gu64;
	guint32    *p_fun_id;
	P_FUNC      p_func_tmp;
	gint32     *p_ret_val;
	gint32     *p_size_send;
	
        

	p_buf = ap_recv + 4;
	p_id_gu64 = (guint64*)p_buf;
	p_buf += sizeof(guint64);
	p_fun_id = (guint32*)p_buf;
	p_buf += sizeof(guint32);

	memmove((char*)&arg_in_read, p_buf, sizeof(arg_in_read));

	p_buf = ap_send;
	p_size_send = (gint32*)p_buf;
	p_buf += sizeof(gint32);
	p_ret_val = (gint32*)p_buf;
	p_buf += sizeof(gint32);
	
	arg_out_read.rp_str.index = p_buf - ap_send + sizeof(arg_out_read);
	arg_out_read.rp_str.size = arg_in_read.size_read;
	*p_size_send = arg_out_read.rp_str.index + arg_out_read.rp_str.size;

	memmove(p_buf, (char*)&arg_out_read, sizeof(arg_out_read));

	p_buf = ap_send + arg_out_read.rp_str.index;

	my_file.read(p_buf, arg_in_read.size_read);

	//std::cout << arg_out_read.buf << std::endl;

	*p_ret_val = 0;
}

void  FileSer::seek(char *ap_recv,  char *ap_send)
{
	std::cout << "File Server Module seek" << std::endl;

	char       *p_buf;
	guint64    *p_id_gu64;
	guint32    *p_fun_id;
	P_FUNC      p_func_tmp;
	gint32     *p_ret_val;
	
	p_buf = ap_recv + 4;
	p_id_gu64 = (guint64*)p_buf;
	p_buf += sizeof(guint64);
	p_fun_id = (guint32*)p_buf;
	p_buf += sizeof(guint32);

	memmove((char*)&arg_in_seek, p_buf, sizeof(arg_in_seek));

	my_file.seekg(arg_in_seek.pos, std::ios::beg);

	//arg_out_read.ret_err = 0; 

	gint32     *p_size_send;

	p_buf = ap_send;
	p_size_send = (gint32*)p_buf;
	p_buf += sizeof(gint32);
	p_ret_val = (gint32*)p_buf;
	p_buf += sizeof(gint32);
	*p_size_send = p_buf - ap_send;

	*p_ret_val = 0;
}

void  FileSer::close(char *ap_recv,  char *ap_send)
{
	std::cout << "File Server Module close" << std::endl;

	char       *p_buf;
	guint64    *p_id_gu64;
	guint32    *p_fun_id;
	P_FUNC      p_func_tmp;
	gint32     *p_ret_val;
	
	p_buf = ap_recv + 4;
	p_id_gu64 = (guint64*)p_buf;
	p_buf += sizeof(guint64);
	p_fun_id = (guint32*)p_buf;
	p_buf += sizeof(guint32);

	my_file.close();

	//arg_out_read.ret_err = 0; 

	gint32     *p_size_send;

	p_buf = ap_send;
	p_size_send = (gint32*)p_buf;
	p_buf += sizeof(gint32);
	p_ret_val = (gint32*)p_buf;
	p_buf += sizeof(gint32);
	*p_size_send = p_buf - ap_send;

	*p_ret_val = 0;
}

void  FileSer::erase(char *ap_recv,  char *ap_send)
{
	std::cout << "File Server Module erase" << std::endl;

	char       *p_buf;
	guint64    *p_id_gu64;
	guint32    *p_fun_id;
	P_FUNC      p_func_tmp;
	gint32     *p_ret_val;
	
	p_buf = ap_recv + 4;
	p_id_gu64 = (guint64*)p_buf;
	p_buf += sizeof(guint64);
	p_fun_id = (guint32*)p_buf;
	p_buf += sizeof(guint32);


	gint32     *p_size_send;

	p_buf = ap_send;
	p_size_send = (gint32*)p_buf;
	p_buf += sizeof(gint32);
	p_ret_val = (gint32*)p_buf;
	p_buf += sizeof(gint32);
	*p_size_send = p_buf - ap_send;

	*p_ret_val = 0;

        p_my_comm->erase_map(m_id_obj);

	delete this;
}

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
	IdObj         *p_id_obj = new IdObj;
	
	IntCalcSer    *p_int_calc = new IntCalcSer;

	p_id_obj->id_class = CLASS_INT_CALC; 
	p_id_obj->id_object = 0; 
	
	p_int_calc->init();

	my_comm.reg_server(p_id_obj, p_int_calc);
	
	FileSer       *p_file_ser = new FileSer;

	
	p_id_obj->id_class = CLASS_FILE_SER; 
	p_id_obj->id_object = 0; 

	p_file_ser->init(&my_comm, *p_id_obj);

	my_comm.reg_server(p_id_obj, p_file_ser);
	
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
 
