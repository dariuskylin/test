#include  "gcpprclib.h"

namespace  gcpprc
{

void error_exit(char *message_error)
{
	std::cout << message_error << std::endl;
	exit(-1); 
}

void PAny::set_pany(char* a_p, int a_size_data)
{
        p_data = a_p;
        size_data = a_size_data;
}

char* PAny::get_data()
{
        return  (char*)p_data;
}

int   PAny::size_of_data()
{
        return  size_data;
}


gint32  SerAgent::init(char *ap_server_ip, gint32 a_port)
{
	gint32  ret_val = 0;

	sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sd < 0)  return  SOCKET_ERR;

	memset(&sa_serv, 0, sizeof(sa_serv));   
        sa_serv.sin_family = AF_INET;                  
        sa_serv.sin_addr.s_addr = inet_addr(ap_server_ip);   
        sa_serv.sin_port = htons(a_port);    
	ret_val = connect(sd, (struct sockaddr*)&sa_serv, sizeof(sa_serv));   
	if (ret_val < 0) return  CONNECT_ERR;

	return 0;
}

gint32  SerAgent::call_ser(char* ap_data)
{
	gint32      ret_val = 0;

	guint32     *p_size_send;

	p_size_send = (guint32*)ap_data;

	gint32 ret_send = send(sd, ap_data, *p_size_send, 0);
	//std::cout << "client send: " << ret_send << std::endl;

	gint32   ret_recv = 0;
	gint32   ret_tmp = 0;
	guint32  *p_size_read = (guint32*)ap_data;
	char     *p_recv = ap_data;
			       
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

	//std::cout << "client read: " <<  ret_recv << std::endl;

	return ret_val;
}

void  SerAgent::close_socket()
{
    #ifdef _WIN32
	closesocket(sd);
    #else	
	close(sd);
    #endif
}

gint32   SerAgent::remote_call(guint32 a_func_id, PAny pa, PAny pb, PAny pc, PAny pd, PAny pe, PAny pf, PAny pg, PAny ph, PAny pi, PAny pj)
{
        PAny        panys[NUM_ARG];

	IdObj      *p_id_obj;

        char       *p_ch;
	char       *p_buf;
	guint64     id_gu64;
	guint64    *p_id_gu64;

	guint32     *p_fun_id;
	gint32       ret_val;
	gint32      *p_ret_val;
        guint32      size_data;
	guint32     *p_size_send;
	guint32     *p_size_data;
	guint32     *p_num_data;
	char        *p_data;

        panys[0] = pa;
        panys[1] = pb;
        panys[2] = pc;
        panys[3] = pd;
        panys[4] = pe;
        panys[5] = pf;
        panys[6] = pg;
        panys[7] = ph;
        panys[8] = pi;
        panys[9] = pj;

	p_buf = data_buf;
	p_size_send = (guint32*)p_buf;
	p_buf += sizeof(guint32);

	p_id_obj = (IdObj*)p_buf;
	*p_id_obj = id_obj;
	p_buf += sizeof(guint64);

	p_fun_id = (guint32*)p_buf;
	*p_fun_id = a_func_id;
	p_buf += sizeof(guint32);

	p_ret_val = (gint32*)p_buf;
	p_buf += sizeof(guint32);

	p_num_data = (guint32*)p_buf;
	p_buf += sizeof(guint32);

	p_size_data = (guint32*)p_buf;
	p_buf += sizeof(guint32);

	*p_num_data = 0;

        for(gint32 i=0; i < NUM_ARG; i++)
        {
                p_ch = panys[i].get_data();
                if (p_ch == 0) break;
		*p_num_data = *p_num_data + 1;
		*p_size_data = panys[i].size_of_data();
		p_size_data++;
        }      

	p_data = (char*)p_size_data;

	p_size_data -= *p_num_data;

        for(gint32 i=0; i<*p_num_data; i++)
        {
                p_ch = panys[i].get_data();
                //std::cout << "size of data : " << panys[i].size_of_data()  << std::endl;
                if (p_ch == 0) break;
                size_data = panys[i].size_of_data();
		*p_size_data = size_data;
		p_size_data++;
                memmove(p_data, p_ch, size_data);
                p_data += size_data;
        }      


	*p_size_send = p_data - data_buf;

	ret_val = call_ser(data_buf);

	if (ret_val < 0) return ret_val;

	p_buf = data_buf;
	p_size_send = (guint32*)p_buf;
	p_buf += sizeof(guint32);

	p_id_obj = (IdObj*)p_buf;
	p_buf += sizeof(guint64);

	p_fun_id = (guint32*)p_buf;
	p_buf += sizeof(guint32);

	p_ret_val = (gint32*)p_buf;
	p_buf += sizeof(gint32);

	if (*p_ret_val < 0) return *p_ret_val;

	p_num_data = (guint32*)p_buf;
	p_buf += sizeof(guint32);
	p_size_data = (guint32*)p_buf;

	p_data = p_buf + sizeof(guint32)*(*p_num_data); 
	//std::cout << "num_data = " << *p_num_data << std::endl;

	//std::cout << "p_data = " << p_data << std::endl;

	for (int i=0; i < *p_num_data; i++)
	{
		p_ch = panys[i].get_data();
		memmove(p_ch, p_data, *p_size_data);
		p_data += *p_size_data;
		p_size_data++;
	}

	ret_val = *p_ret_val;

	return  ret_val;
}

void  ModuleServer::reg_func(guint32  a_id_func,  P_FUNC ap_func)
{
	map_func.insert(Ps_Func(a_id_func, ap_func));	
}

void  ModuleServer::unpack_data(char *ap_data, char **pp_ch)
{
	char       *p_buf;
	guint64    *p_id_gu64;
	guint32    *p_fun_id;
	P_FUNC      p_func_tmp;
	gint32     *p_ret_val;
	guint32     *p_size_data;
	guint32     *p_num_data;
	char       *p_data;
	char       *p_args[NUM_ARG];       

	p_buf = ap_data + 4;
	p_id_gu64 = (guint64*)p_buf;
	p_buf += sizeof(guint64);
	p_fun_id = (guint32*)p_buf;
	p_buf += sizeof(guint32);

	p_ret_val = (gint32*)p_buf;
	p_buf += sizeof(gint32);

	p_num_data = (guint32*)p_buf;
	p_buf += sizeof(guint32);

	p_size_data = (guint32*)p_buf;

	p_buf += sizeof(guint32)*(*p_num_data);

	p_data = p_buf;

	gint32   idx = 0;
	
	for (int i=0; i < *p_num_data; i++)
	{
		*pp_ch = p_data + idx;
		idx += *p_size_data;
		//std::cout << *p_size_data << std::endl;
		p_size_data++;
		//std::cout << *pp_ch << std::endl;
		pp_ch++;
	}
}

void  ModuleServer::call_switch(char *ap_data)
{
	char       *p_buf;
	guint64    *p_id_gu64;
	guint32    *p_fun_id;
	P_FUNC      p_func_tmp;
	gint32     *p_ret_val;
	char       *p_args[NUM_ARG];       

	p_buf = ap_data + 4;
	p_id_gu64 = (guint64*)p_buf;
	p_buf += sizeof(guint64);
	p_fun_id = (guint32*)p_buf;
	p_buf += sizeof(guint32);

	p_ret_val = (gint32*)p_buf;
	p_buf += sizeof(guint32);

	*p_ret_val = 0;

	bool      find_func = false;

	mutex_map.lock();	
	it_map_func = map_func.find(*p_fun_id);
	if (it_map_func == map_func.end()) 
	{
		find_function_error(ap_data);
	}
	else
	{
		p_func_tmp = it_map_func->second;
		find_func = true;
	}
	mutex_map.unlock();

	unpack_data(ap_data, p_args);
	
        if (find_func) (this->*p_func_tmp)(ap_data, p_args[0], p_args[1], p_args[2], p_args[3], p_args[4], p_args[5], p_args[6], p_args[7], p_args[8], p_args[9]);
}


void  ModuleServer::find_function_error(char *ap_data)
{
	char       *p_buf;
	guint32    *p_size_send;
	guint64    *p_id_gu64;
	guint32    *p_fun_id;
	gint32     *p_ret_val;

	p_buf = ap_data;
	p_buf += sizeof(*p_size_send);	
	p_buf += sizeof(*p_id_gu64);
	p_buf += sizeof(*p_fun_id);
	p_ret_val = (gint32*)p_buf;
	p_buf += sizeof(*p_ret_val);

	*p_size_send = p_buf - ap_data;
	*p_ret_val = ERR_FIND_FUNC;

	std::cout << "error: can not find server function!" << std::endl; 
}

void  ModuleServer::fix_size_of_data(char *ap_data, gint32 a_fix_size)
{
	char       *p_buf;
	guint32     *p_size_send;
	guint64    *p_id_gu64;
	guint32    *p_fun_id;
	P_FUNC      p_func_tmp;
	gint32     *p_ret_val;
	guint32     *p_size_data;
	guint32     *p_num_data;

	p_buf = ap_data;
	p_buf += sizeof(*p_size_send);	
	p_buf += sizeof(*p_id_gu64);
	p_buf += sizeof(*p_fun_id);
	p_buf += sizeof(*p_ret_val);
	p_num_data = (guint32*)p_buf;
	p_buf += sizeof(*p_num_data);

	p_size_data = (guint32*)p_buf;

	p_buf += sizeof(*p_size_data)*(*p_num_data - 1);

	guint32  *p_size_of_last_data = (guint32*)p_buf;
	
        if (a_fix_size > 0) 
	{
		*p_size_of_last_data = a_fix_size;
	}
	else
	{
		*p_size_of_last_data = 0;
	}

	p_size_send = (guint32*)ap_data;	

	*p_size_send = *p_size_send + a_fix_size;
}

void  CommunicateSer::find_object_error(char  *ap_data)
{
	char       *p_buf;
	guint32    *p_size_send;
	guint64    *p_id_gu64;
	guint32    *p_fun_id;
	gint32     *p_ret_val;

	p_buf = ap_data;
	p_buf += sizeof(*p_size_send);	
	p_buf += sizeof(*p_id_gu64);
	p_buf += sizeof(*p_fun_id);
	p_ret_val = (gint32*)p_buf;

	*p_size_send = p_buf - ap_data;
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

void  CommunicateSer::listen_client(gint32 a_port)
{
	std::cout << "Begin TCP socket..." << std::endl;

	listen_sd = socket (AF_INET, SOCK_STREAM, 0);   
	if (listen_sd < 0) error_exit("socket error");
  
	memset (&sa_serv, '\0', sizeof(sa_serv));
	sa_serv.sin_family      = AF_INET;
	sa_serv.sin_addr.s_addr = INADDR_ANY;
	sa_serv.sin_port        = htons (a_port);          
  
	err = bind(listen_sd, (struct sockaddr*) &sa_serv, sizeof (sa_serv));                   
	if (err < 0) error_exit("bind error");
         
	err = listen (listen_sd, 100);                    
        if (err < 0) error_exit("listen error");

	std::cout << "listen at port " << a_port << std::endl;

	client_len = sizeof(sa_cli);

	int sd;

	while (true)
	{
    	  #ifdef _WIN32
		sd = accept (listen_sd, (struct sockaddr*)&sa_cli, &client_len);
    	  #else
		sd = accept (listen_sd, (struct sockaddr*)&sa_cli, (socklen_t*)&client_len);
    	  #endif
		if (sd < 0)
		{
			std::cout << "accept error" << std::endl;
		}
		else
		{
			p_server_thread = Glib::Thread::create(sigc::bind(sigc::mem_fun(*this, &CommunicateSer::server), sd), false /*true*/);
		}
	}
}

void CommunicateSer::server(gint32 a_server_sd)
{
	char    data_buf[PACK_SIZE];

	ModuleServer*      p_server;

	while (true)
	{	
		gint32   ret_recv = 0;
		gint32   ret_tmp = 0;
		gint32   size_read;
		gint32   *p_size_read = (gint32*)data_buf;
		char     *p_recv = data_buf;
			       
		ret_tmp = recv(a_server_sd,  (char*)p_size_read, 4, 0);

		size_read = *p_size_read;
		
		size_read -= 4;

		p_recv += 4;

		ret_recv += ret_tmp;

		while (true)
		{
			ret_tmp = recv(a_server_sd,  p_recv, size_read, 0);
			if (ret_tmp <= 0) break;
			p_recv += ret_tmp;
			ret_recv += ret_tmp;
			size_read -= ret_tmp;
			if (size_read == 0) break;
		}

		//std::cout << "server read: " <<  ret_recv << std::endl;

		if (ret_tmp <= 0)
		{
                    #ifdef _WIN32
			closesocket(a_server_sd);
		    #else	
			close(a_server_sd);
		    #endif
			std::cout << "close server socket" << std::endl;
			break;
		}

		guint64     *p_id_gu64;
		p_id_gu64 = (guint64*)(data_buf + 4);

		bool   find_object = false;

		mutex_map.lock();	
		it_map = map_servers.find(*p_id_gu64);
		if (it_map == map_servers.end())
		{
			find_object_error(data_buf);
		}
		else
		{
			p_server = it_map->second;
			find_object = true;
		}
		mutex_map.unlock();	
		
		//std::cout << it_map->first << std::endl;

		if (find_object) p_server->call_switch(data_buf);	

		guint32     *p_size_send;

		p_size_send = (guint32*)data_buf;

		gint32 ret_send = send(a_server_sd, data_buf, *p_size_send, 0);
		
		//std::cout << "server send: " << ret_send << std::endl;			
	}
}

void  CommunicateSer::erase_map(IdObj a_id_obj)
{
        std::cout << "erase map" << std::endl;
 
	guint64  *p_id_gu64;
	p_id_gu64 = (guint64*)&a_id_obj;
        map_servers.erase(*p_id_gu64);
}

}; //namespace  gcpprc
