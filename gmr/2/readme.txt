                                         C++ ��Զ�̵����м����� (��)

     ��д��һЩ socket ���� ���ų�����𲽸��ӣ�ֱ���� socket ����Ե���Щ�鷳���������Ӧ��ģ�黯��˼�룬Ӧ�ð�����е� socket ͨ�Ų�����Զ�����������һ�����м���������� C++ д��һ����򵥵ġ�Զ�̵����м���� ԭ����ʾ���򣬹�������ʹ�����ۣ�����ˮƽ���ޣ����д��󣬻�ӭ�����������������ʾ��һ��򵥵�Զ�̶�����ã�Ӧ�ó���ͨ�����ش���������Զ�̼������ C++ ������ͬ���������ı��ض���һ�����㡣���Զ�̶�����ó����ԭ�����ѣ��������һ��ͨ���ദ�� socket ͨ�ţ��ṩ���ַ���ķ���˶�����Լ���ָ�봫��ͨ����Ķ��󣬱�����һ�� std::map �У�ͨ����Ķ�����ݿͻ��˵�������÷���˵Ĳ�ͬ���ܵ� C++ ����Ȼ��ѵ��ý�����ؿͻ��ˡ��ͻ���Ҳ��һ��ͨ���ദ�� socket ͨ�ţ��ͻ��˵Ĵ������������˶�����ܵ��ã�Ȼ��ͨ��ͨ��������ʶ��������ʶ������������������ˣ����÷���˶���Ȼ��ѵ��ý�����ؿͻ��˵ĵ����ߡ��������ǳ���Դ�룬�����ı���ԭ�� :-) ��


   ��ֻ��һ���򵥵���ʾ������Ҳ���ܱ�֤��û�д��������԰���������е�Դ���������Լ��ĳ����У�ֻҪ���Լ�������������������ע�����ò��ֵ�ԭ���ߡ�

    ��������ǡ�C++ ��Զ�̵����м����� (һ)���������汾�����ʵ���˸��ݿͻ��˵�����̬�Ĵ����ͳ�������˶���ԭ���ġ�C++ ��Զ�̵����м����� (һ)��������һ���������ֻ��һ��ʵ�����ͻ���ͬһ������Ĳ�ͬʵ����������һ������˶��󣬷���˶���ķ����������ǿ�����ģ���������˶���Ͳ��ܱ����ε����е��м���Ϣ�����ڵġ�C++ ��Զ�̵����м����� (��)���������Ϊÿһ���ͻ��˴�����ʵ����̬����һ����Ӧ�ķ������ʵ���������Ϳ��Ա����м���Ϣ�����ҵ�������������������ʵ����һ���ļ�ֻ�������ࡣ 

�ͻ��� FileAgent ��:

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


����� FileSer ��:

class  FileSer :public  ModuleServer , FileCSArgBase   
{
    protected:
	std::string        name_module;
	static  guint32    counter;
	IdObj              m_id_obj;
	CommunicateSer    *p_my_comm;
	Glib::Mutex        mutex_clone;

        std::ifstream      my_file;

    protected:
	typedef  void  (FileSer::*P_FUNC)(char *ap_recv,  char *ap_send);

	P_FUNC  p_func;

	std::map<guint32, P_FUNC>                    map_func;   
	std::map<guint32, P_FUNC>::iterator          it_map_func;
 
	typedef  std::pair<guint32, P_FUNC>          Ps_Func;


    public:
	void  call_switch(char *ap_recv,  char *ap_send);
	void  init(CommunicateSer  *ap_my_comm, IdObj  a_id_obj);

    protected:
	//void  init();

	void  clone(char *ap_recv,  char *ap_send);
	void  open(char *ap_recv,  char *ap_send);
	void  read(char *ap_recv,  char *ap_send);
	void  seek(char *ap_recv,  char *ap_send);
	void  close(char *ap_recv,  char *ap_send);
	void  erase(char *ap_recv,  char *ap_send);
};

���ͻ��˴������ʵ����ʼ��ʱ�����÷���˶���� clone ������clone ��������һ������˶������ʵ����������ʵ����ָ�뱣����һ�� std::map �У�����ʵ���Ķ����ʶ�� std::map �е� key ������������ʶ���ظ��ͻ��ˣ�Ȼ��ͻ��˾Ϳ�������������ʶ���÷���˶����ˣ���ʱ����˶���Ϳͻ��˴�����һ��һ�������Է���˶�����Ա���򿪵��ļ������ļ���ȡָ����м���Ϣ�����ͻ��˴�������Ҫ����ʱ�����÷���˶���� erase ������ erase �������������˶����� std::map �е���Ϣ��Ȼ����ڴ����������������������������ʾ��Զ���ļ���ȡ���ƶ���ȡָ�롣�������˶�����Ҫ�����м���Ϣ����ôֻҪ��һ��ʵ�������ˣ��ͻ��˵Ĵ�����ͬһ����ʶ��������������ʾ��������ʾ���������������һ��Զ�̶����������ļӼ����� 


     �������ҵ� win2k/mingw �����±�������������ģ�������Ҫ�õ� gtkmm �� http://www.gtkmm.org ���������������û�д��������ֽ�˳�����⣬ ʵ�õĳ���Ӧ�ô���������⡣

mingw  �±��������ǣ�
 
����˳���g++ -o  gmrcmwser  gmrcmwser.cc `pkg-config.exe gtkmm-2.4 --cflags --libs` -lgthread-2.0 -lws2_32

�ͻ��˳���g++ -o  gmrcmwcli  gmrcmwcli.cc `pkg-config.exe gtkmm-2.4 --cflags --libs` -lgthread-2.0 -lws2_32

�������Դ�������ֲ�� Linux , Linux �±�������Ӧ���ǣ�

����˳���g++ -o  gmrcmwser  gmrcmwser.cc `pkg-config gtkmm-2.4 --cflags --libs` -lgthread-2.0

�ͻ��˳���g++ -o  gmrcmwcli  gmrcmwcli.cc `pkg-config gtkmm-2.4 --cflags --libs` -lgthread-2.0


                                                                
                                                     zhujiang

                                                   2007��3��24��
                                                                 