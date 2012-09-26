#include  "gcpprcser.h"

void  CalcSer::init()
{
	number = 0;

	reg_func(FUNC_ADD, (P_FUNC)(&CalcSer::add));
	reg_func(FUNC_SUB, (P_FUNC)(&CalcSer::sub));
	reg_func(FUNC_STRLINK, (P_FUNC)(&CalcSer::strlink));
}


void  CalcSer::add(char  *ap_data, char *ap_a,  char *ap_b,  char *ap_c,
		   char *ap_d,  char *ap_e,  char *ap_f, char *ap_g,
		   char *ap_h,  char *ap_i,  char *ap_j)
{
	do_add((gint32*)ap_a, (gint32*)ap_b, (gint32*)ap_c);
}

void  CalcSer::do_add(gint32 *ap_a,  gint32 *ap_b,  gint32 *ap_c)
{
	std::cout << "number = " << number++ << std::endl;
	*ap_c = *ap_a + *ap_b;
}

void  CalcSer::sub(char  *ap_data, char *ap_a,  char *ap_b,  char *ap_c,
		   char *ap_d,  char *ap_e,  char *ap_f, char *ap_g,
		   char *ap_h,  char *ap_i,  char *ap_j)
{
	do_sub((gint32*)ap_a, (gint32*)ap_b, (gint32*)ap_c);
}

void  CalcSer::do_sub(gint32 *ap_a,  gint32 *ap_b,  gint32 *ap_c)
{
	std::cout << "number = " << number++ << std::endl;
	*ap_c = *ap_a - *ap_b;
}

void  CalcSer::strlink(char  *ap_data, char *ap_a,  char *ap_b,  char *ap_c,
		       char *ap_d,  char *ap_e,  char *ap_f, char *ap_g,
		       char *ap_h,  char *ap_i,  char *ap_j)
{
	do_strlink(ap_a, ap_b, ap_c);

	fix_size_of_data(ap_data, strlen(ap_c)+1);
}

void  CalcSer::do_strlink(char *str_a, char *str_b, char *str_c)
{
	std::cout << str_a << std::endl;
	std::cout << str_b << std::endl;
	strcpy(str_c, str_a);
	strcat(str_c, str_b);
}

void  FileSer::init(gcpprc::CommunicateSer  *ap_comm, gcpprc::IdObj  a_id_obj)
{
        m_id_obj = a_id_obj;
	p_comm = ap_comm;

	reg_func(FUNC_CLONE, (P_FUNC)(&FileSer::clone));
	reg_func(FUNC_OPEN,  (P_FUNC)(&FileSer::open));
	reg_func(FUNC_READ, (P_FUNC)(&FileSer::read));
	reg_func(FUNC_SEEK,  (P_FUNC)(&FileSer::seek));
	reg_func(FUNC_CLOSE, (P_FUNC)(&FileSer::close));
	reg_func(FUNC_ERASE,  (P_FUNC)(&FileSer::erase));
	reg_func(FUNC_WRITE,  (P_FUNC)(&FileSer::write));
}

void  FileSer::clone(char  *ap_data, char *ap_a,  char *ap_b,  char *ap_c,
		     char *ap_d,  char *ap_e,  char *ap_f, char *ap_g,
		     char *ap_h,  char *ap_i,  char *ap_j)
{
	do_clone((guint32*)ap_a);
}

void  FileSer::open(char  *ap_data, char *ap_a,  char *ap_b,  char *ap_c,
		    char *ap_d,  char *ap_e,  char *ap_f, char *ap_g,
		    char *ap_h,  char *ap_i,  char *ap_j)
{
	do_open(ap_a, ap_b, (gcpprc::FileStat*)ap_c);
}

void  FileSer::read(char  *ap_data, char *ap_a,  char *ap_b,  char *ap_c,
		    char *ap_d,  char *ap_e,  char *ap_f, char *ap_g,
		    char *ap_h,  char *ap_i,  char *ap_j)
{
	guint32  *p_size_read = (guint32*)ap_a;
	do_read(ap_b, p_size_read);
	fix_size_of_data(ap_data, *p_size_read);
}

void  FileSer::write(char  *ap_data, char *ap_a,  char *ap_b,  char *ap_c,
		    char *ap_d,  char *ap_e,  char *ap_f, char *ap_g,
		    char *ap_h,  char *ap_i,  char *ap_j)
{
	guint32  *p_size_write = (guint32*)ap_a;
	gint32   fix_size = 0 - *p_size_write;
	do_write(ap_b, p_size_write);
	fix_size_of_data(ap_data, fix_size);
}

void  FileSer::seek(char  *ap_data, char *ap_a,  char *ap_b,  char *ap_c,
		    char *ap_d,  char *ap_e,  char *ap_f, char *ap_g,
		    char *ap_h,  char *ap_i,  char *ap_j)
{
	do_seek((guint32*)ap_a);
}

void  FileSer::close(char  *ap_data, char *ap_a,  char *ap_b,  char *ap_c,
		     char *ap_d,  char *ap_e,  char *ap_f, char *ap_g,
		     char *ap_h,  char *ap_i,  char *ap_j)
{
	do_close();
}

void  FileSer::erase(char  *ap_data, char *ap_a,  char *ap_b,  char *ap_c,
		     char *ap_d,  char *ap_e,  char *ap_f, char *ap_g,
		     char *ap_h,  char *ap_i,  char *ap_j)
{
	do_erase();
}

void  FileSer::do_clone(guint32 *ap_id)
{
	std::cout << "File Server Module clone" << std::endl;

	Glib::Mutex::Lock lock(mutex_clone);
	counter++; 
	*ap_id = counter;

	FileSer    *p_file_ser = new FileSer;

	gcpprc::IdObj   *p_id_obj = &m_id_obj;
	
	p_id_obj->id_class = CLASS_FILE_REMOTE; 
	p_id_obj->id_object = *ap_id; 

	p_file_ser->init(p_comm, m_id_obj);

	p_comm->reg_server(p_id_obj, p_file_ser);
}

void  FileSer::do_open(char  *file_name, char  *mode,  gcpprc::FileStat *ap_stat_file)
{
	std::cout << "open file name = " << file_name << std::endl;
	std::string  str_mode = mode;

	struct stat  my_fstat;

	char  *p_ch = file_name + strlen(file_name);

	while (true)
	{
		if (*p_ch == '/' || *p_ch == '\\')
		{
			p_ch++;
			break; 
		}
		p_ch--;
		if (p_ch == file_name) break;
	}

	std::cout << "open file :  " << p_ch << std::endl;

	if (str_mode == "r") m_file.open(p_ch, std::ios::in | std::ios::binary);
	if (str_mode == "w") m_file.open(p_ch, std::ios::out | std::ios::binary);
	if (str_mode == "rw") m_file.open(p_ch, std::ios::in | std::ios::out | std::ios::binary);

	stat(p_ch, &my_fstat);
	ap_stat_file->st_size = my_fstat.st_size;
	ap_stat_file->st_time = my_fstat.st_mtime;
	std::cout << "size_file = " << ap_stat_file->st_size << std::endl;
	//std::cout << "size of stat = " << sizeof(struct stat) << std::endl; 
}

void  FileSer::do_read(char  *ap_buf, guint32 *ap_size_read)
{
	m_file.read(ap_buf, *ap_size_read);
}

void  FileSer::do_write(char  *ap_buf, guint32 *ap_size_write)
{
	//std::cout << "size_write = " << *ap_size_write << std::endl;
	m_file.write(ap_buf, *ap_size_write);
}

void  FileSer::do_seek(guint32  *ap_pos)
{
	m_file.seekg(*ap_pos, std::ios::beg);
}

void  FileSer::do_close()
{
	m_file.close();
	m_file.clear();
}

void  FileSer::do_erase()
{
        p_comm->erase_map(m_id_obj);

	delete this;
}

Application::Application() 
: rp_main_loop (Glib::MainLoop::create())
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

bool Application::on_timeout(gint32 timer_number)
{
	std::cout << "timeout" << std::endl;
	return true;
}

void Application::run()
{
	Glib::Thread     *p_listen_thread;

	gcpprc::IdObj    *p_id_obj = new gcpprc::IdObj;
	
	CalcSer    *p_calc = new CalcSer;
	p_id_obj->id_class = CLASS_CALC; 
	p_id_obj->id_object = 0; 	
	p_calc->init();
	main_comm.reg_server(p_id_obj, p_calc);

	FileSer    *p_file_ser = new FileSer;
	p_id_obj->id_class = CLASS_FILE_REMOTE; 
	p_id_obj->id_object = 0; 	
	p_file_ser->init(&main_comm, *p_id_obj);
	main_comm.reg_server(p_id_obj, p_file_ser);
	
	p_listen_thread = Glib::Thread::create(sigc::bind(sigc::mem_fun(main_comm, &gcpprc::CommunicateSer::listen_client), PORT), true);    
	rp_main_loop->run();
}

guint32  FileSer::counter = 0;

int main(int argc, char *argv[])
{
	Application   *p_app;

	Glib::thread_init();
  
	p_app = new Application;

	p_app->run();
 
	return 0;
}
 
