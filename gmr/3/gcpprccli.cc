#include "gcpprccli.h"

gint32  CalcAgent::init(char *ap_server_ip, gint32 a_port)
{
	id_obj.id_class = CLASS_CALC;
	id_obj.id_object = 0;

	return SerAgent::init(ap_server_ip, a_port);
}

gint32  CalcAgent::add(gint32 a, gint32 b, gint32 &c)
{
	return  remote_call(FUNC_ADD, &a, &b, &c);
}

gint32  CalcAgent::sub(gint32 a, gint32 b, gint32 &c)
{
	return  remote_call(FUNC_SUB, &a, &b, &c);	
}

gint32  CalcAgent::strlink(char *str_a, char *str_b, char *str_c)
{
	return  remote_call(FUNC_STRLINK, str_a, str_b, str_c);	
}

gint32    FileAgent::init(char *ap_server_ip, gint32 a_port)
{
	gint32   ret_val;

	id_obj.id_class = CLASS_FILE_REMOTE;
	id_obj.id_object = 0;

	ret_val = SerAgent::init(ap_server_ip, a_port);
	if (ret_val < 0) return ret_val;

	//std::cout << "before clone id_obj.id_object = " << id_obj.id_object << std::endl;
	ret_val = clone(id_obj.id_object);
	//std::cout << "after clone id_obj.id_object = " << id_obj.id_object << std::endl;
	return ret_val;
}

gint32  FileAgent::clone(guint32 &a_id_obj)
{
	return  remote_call(FUNC_CLONE, &a_id_obj);		
}

gint32  FileAgent::open(char* filename, char*  mode)
{
	gint32  ret_val;

	pos_file = 0;
	end_file = false;
	size_file = 0;

	ret_val = remote_call(FUNC_OPEN, filename,  mode, &stat_file);
	size_file = stat_file.st_size;

	std::cout << "stat_file.st_size = " << stat_file.st_size << std::endl;

	return  ret_val; 		
}

gint32   FileAgent::read(char *ap_buf, guint32 &a_size_read)
{
	gint32  ret_val = 0;
	size_read = 0;

	if (a_size_read >= size_file - pos_file) a_size_read = size_file - pos_file;

	ret_val = remote_call(FUNC_READ, &a_size_read, ap_buf);

	size_read = a_size_read;
	pos_file += size_read;
	if (pos_file >= size_file) end_file = true;

	return  ret_val; 			
}

gint32   FileAgent::write(char *ap_buf, guint32 &a_size_write)
{
	gint32  ret_val = 0;
	size_write = 0;

	ret_val = remote_call(FUNC_WRITE, &a_size_write, gcpprc::PAny(ap_buf, a_size_write));

	size_write = a_size_write;
	pos_file += size_write;

	return  ret_val; 			
}

gint32   FileAgent::seek(guint32  &a_pos)
{
	gint32  ret_val = 0;
	pos_file = a_pos;

	if (pos_file >= size_file) pos_file = size_file;

	ret_val = remote_call(FUNC_SEEK, &pos_file);
	if (pos_file >= size_file) end_file = true;

	return  ret_val; 			
}

gint32   FileAgent::close()
{
	return  remote_call(FUNC_CLOSE);		
}

gint32   FileAgent::erase()
{
	return  remote_call(FUNC_ERASE);		
}


int main(int argc, char *argv[])
{
    #ifdef _WIN32
	WSADATA            wsaData;
    #endif

    #ifdef _WIN32
	if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0)
	{
		printf("WSAStartup()fail:%ld\n",GetLastError());
		exit(-1);
	}
    #endif
	gint32           ret_val;

	CalcAgent        calc;
	FileAgent        file_remote;
	gint32           ret_int;
        std::ifstream    file_in;
        std::ofstream    file_out;

	char   chstr_ser_ip[] = "127.0.0.1";

	ret_val = file_remote.init(chstr_ser_ip, PORT);
	if (ret_val < 0) gcpprc::error_exit("file_remote init  error!");

	ret_val = calc.init(chstr_ser_ip, PORT);
	if (ret_val < 0) gcpprc::error_exit("calc init  error!");
		
	char    file_name[1024];
	char    file_mode[16];
	char    file_buf[4096];
	bool    local_file_good;

	strcpy(file_name, "remote_in01.dat");
	strcpy(file_mode, "r");

	
	ret_val = file_remote.open(file_name, file_mode);
	std::cout << "file_remote.open  ret_val = " << ret_val << std::endl;
	if (ret_val < 0) gcpprc::error_exit("file_remote.open  error!");
	std::cout << "remote file size = " << file_remote.get_size() << std::endl;

	file_out.open("local_out01.dat" , std::ios::binary);
	local_file_good = file_out.good();

	while (local_file_good)
	{
		guint32 file_size_read = 1024;
		file_buf[0] = '\0';
		ret_val = file_remote.read(file_buf, file_size_read);
		file_size_read = file_remote.get_size_read();
		//std::cout << "file_remote size_read = " << file_size_read << std::endl;
		file_out.write(file_buf, file_size_read);
		std::cout << "file_remote.read pos_file = " << file_remote.get_pos_file() << std::endl;
		if (file_remote.get_if_end() == true) break;
		Glib::usleep(1000*10);
	}

	ret_val = file_remote.close();
	if (ret_val < 0) gcpprc::error_exit("file_remote close  error!");
	

	strcpy(file_name, "remote_out01.dat");
	strcpy(file_mode, "w");

	ret_val = file_remote.open(file_name, file_mode);
	std::cout << "file_remote.open  ret_val = " << ret_val << std::endl;
	if (ret_val < 0) gcpprc::error_exit("file_remote.open  error!");

	file_in.open("local_in01.dat" , std::ios::binary);
	local_file_good = file_in.good();

	while (local_file_good)
	{
		guint32 file_size_read = 1024;
		guint32 file_size_write;
		file_in.read(file_buf, file_size_read);
		file_size_write = file_in.gcount();
		ret_val = file_remote.write(file_buf, file_size_write);
		//std::cout << "file_remote size_write = " << file_size_write << std::endl;
		std::cout << "file_remote.write pos_file = " << file_remote.get_pos_file() << std::endl;
		if (!file_in.good()) break;
		Glib::usleep(1000*10);
	}

	ret_val = file_remote.close();
	if (ret_val < 0) gcpprc::error_exit("file_remote close  error!");

	file_out.close();
	file_in.close();
	
	ret_val = calc.add(101, 765, ret_int);
	std::cout << "int_calc.add ret_val = " << ret_val << std::endl;
	if (ret_val < 0) gcpprc::error_exit("int_calc.add  error!");
	std::cout << "int add ret = " << ret_int << std::endl;

	ret_val = calc.sub(45, 9, ret_int);
	std::cout << "int_calc.sub ret_val = " << ret_val << std::endl;
	if (ret_val < 0) gcpprc::error_exit("int_calc.sub  error!");
	std::cout << "int sub ret = " << ret_int << std::endl;
	

	char str_a[]="WinXp/Linux ";
	char str_b[]="DeskTop 0124567789 :-)";
	char str_c[1024];
	
	str_c[0] = '\0';

	ret_val = calc.strlink(str_a, str_b, str_c);
	std::cout << "int_calc.strlink ret_val = " << ret_val << std::endl;

	std::cout << str_a << std::endl;
	std::cout << str_b << std::endl;
	std::cout << str_c << std::endl;

	file_out.close();

	calc.close_socket();

	ret_val = file_remote.close();
	if (ret_val < 0) gcpprc::error_exit("file_remote close  error!");

	ret_val = file_remote.erase();
	if (ret_val < 0) gcpprc::error_exit("file_remote erase  error!");

	file_remote.close_socket();
	
	return 0;
}
