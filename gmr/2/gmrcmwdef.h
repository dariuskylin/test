#pragma pack(4)

#define CHK_NULL(x) if ((x)==NULL) exit (-1)
#define CHK_ERR(err,s) if ((err) < 0) {printf("%s error\n", s); exit(-2); }

#define  SERVER_IP        "127.0.0.1"

#define  PORT              8000

#define  ERR_FIND_OBJ         -1000
#define  ERR_FIND_FUNC        -1001
#define  ERR_FILE_OPEN        -1002

#define  CLASS_INT_CALC       10001
#define  CLASS_FLOAT_CALC     10002
#define  CLASS_FILE_SER       10003

#define  FUNC_ADD             20001
#define  FUNC_SUB             20002

#define  FUNC_CLONE           20003
#define  FUNC_OPEN            20004
#define  FUNC_READ            20005
#define  FUNC_SEEK            20006
#define  FUNC_CLOSE           20007
#define  FUNC_ERASE           20008

#define  PACK_SIZE            4096


struct  IdObj
{
	guint32     id_class;
	guint32     id_object;
};

struct  RemotePoint
{
	guint32     index;
	guint32     size;
};

class  FileCSArgBase
{
    protected:

	struct  Arg_Out_Clone
	{
		guint32  id_obj;
	};

	Arg_Out_Clone      arg_out_clone;

	struct  Arg_In_Open
	{
		RemotePoint   rp_name_file;
	};

	Arg_In_Open       arg_in_open;

	struct  Arg_Out_Open
	{
		guint32   size_file;
	};

	Arg_Out_Open      arg_out_open;

	struct  Arg_In_Read
	{
		guint32   size_read;
	};

	Arg_In_Read       arg_in_read;

	struct  Arg_Out_Read
	{
		RemotePoint     rp_str;
	};

	Arg_Out_Read      arg_out_read;

	struct  Arg_In_Seek
	{
		guint32   pos;
	};

	Arg_In_Seek      arg_in_seek;
};
