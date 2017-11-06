///////////////////////////////////////////////////////////////////////////////////////
//@module		:	ȫ�����ݶ���ģ��
//@author		:	zhiyong.luo
//@create date	:	2006-11-27
//@last_modify	:	
///////////////////////////////////////////////////////////////////////////////////////
#ifndef _LZY_INTERFACE_ENGINE_H_
#define _LZY_INTERFACE_ENGINE_H_
#include "global_include.h"
#if defined(SCRIPTENGINE_EXPORTS) || defined(SNIFFER_CORE_EXPORTS)
	#include "lockmgrl.h"
	#include "hash_map.h"
#else
	#include "../../LComm/LLockMgr.h"
	#include "../../LComm/LMap.h"
#endif
//#define LZY_COUNT //�Ƿ�֧��ͳ�ƺ�
#define _LZY_SCRIPT_ENGINE
//#define LIB_SUPPORT_VB //�Ƿ�֧��VB����
#ifndef LIB_SUPPORT_VB
#define LIB_SUPPORT_C_EX //c++�ӿڹ淶,�����c�ӿ���Ҫ���������
#endif
//#define LZY_INVOID_ERROR //�Ƿ���Դ���


#define LZY_USE_HASH

#define OPER_UNDEFINED_ID	0 //��������ЧID
#define OPER_ID_SHIFT 1 //С�������ID
#define INIT_LEVEL	0
#define SIG_OPER_NONE	OPER_UNDEFINED_ID   //��Ч����

#define INDEX_ERROR -1

#define NO_LEVEL -1000

#define MAX_LEVEL	0
#define LEVEL_SFT_BG MAX_LEVEL//ǰ������ (
#define LEVEL_SFT_IDX LEVEL_SFT_BG+1//ǰ������ [
#define LEVEL_SELF_ADD_RIGHT LEVEL_SFT_IDX+1			//	++//�ҽ��
#define LEVEL_SELF_SUB_RIGHT LEVEL_SELF_ADD_RIGHT	//	--
#define LEVEL_SELF_ADD_LEFT LEVEL_SELF_ADD_RIGHT+1	//	++//����
#define LEVEL_SELF_SUB_LEFT LEVEL_SELF_ADD_LEFT		//	--
#define LEVEL_NOT	LEVEL_SELF_SUB_LEFT+1	//	^
#define LEVEL_BOOL_NOT LEVEL_NOT+1		//	!
#define LEVEL_SUB_LEFT LEVEL_BOOL_NOT   //	-(����)
#define LEVEL_ADD_LEFT LEVEL_SUB_LEFT	//  +(����)

#define LEVEL_MUL	LEVEL_SUB_LEFT+1	//	*
#define LEVEL_DIV	LEVEL_MUL			//	/
#define LEVEL_MOD	LEVEL_MUL			//	%
#define LEVEL_ADD	LEVEL_MUL+1			//	+
#define LEVEL_SUB	LEVEL_ADD			//	-
#define LEVEL_SHR_LEFT LEVEL_SUB+1		//	<<
#define LEVEL_SHR_RIGHT LEVEL_SHR_LEFT	//	>>
#define LEVEL_L		LEVEL_SHR_RIGHT+1	//	<
#define LEVEL_A		LEVEL_L				//	>
#define LEVEL_LE    LEVEL_L				//	<=
#define LEVEL_AE    LEVEL_L				//	>=
#define LEVEL_E     LEVEL_L+1			//	==
#define LEVEL_NE    LEVEL_E				//	!=
#define LEVEL_AND   LEVEL_NE+1			//	&
#define LEVEL_XOR   LEVEL_AND+1			//	^
#define LEVEL_OR    LEVEL_XOR+1			//	|
#define LEVEL_BOOL_AND   LEVEL_OR+1		//	&&
#define LEVEL_BOOL_OR   LEVEL_BOOL_AND+1//	||
#define LEVEL_PTTV	LEVEL_BOOL_OR+1		//	=
#define LEVEL_ADD_PTTV LEVEL_PTTV+1		//	+=
#define LEVEL_SUB_PTTV LEVEL_ADD_PTTV	//	-=
#define LEVEL_DIV_PTTV LEVEL_ADD_PTTV	//	/=
#define LEVEL_MOD_PTTV LEVEL_ADD_PTTV	//	%=
#define LEVEL_MUL_PTTV LEVEL_ADD_PTTV	//	*=
#define LEVEL_SHRL_PTTV LEVEL_ADD_PTTV	//	<<=
#define LEVEL_SHRR_PTTV LEVEL_ADD_PTTV	//	>>=
#define LEVEL_OR_PTTV LEVEL_ADD_PTTV	//	|=
#define LEVEL_AND_PTTV LEVEL_ADD_PTTV	//	&=
#define LEVEL_XOR_PTTV LEVEL_ADD_PTTV	//	^=
#define LEVEL_DOT_FLAG  LEVEL_XOR_PTTV+1 //	,

#define LEVEL_SFT_ED_IDX LEVEL_DOT_FLAG+1 //]
#define LEVEL_SFT_ED LEVEL_SFT_ED_IDX+1//�������� )

#define MIN_LEVEL	LEVEL_SFT_ED

template<class T>
T* conv_type(VOID* ptr){
	struct _tag_conv_type{
		union {
			VOID*		p_src;
			typename T*	p_dst;
		};
	}tp;
	tp.p_src=ptr;
	return tp.p_dst;
};
//////////////////////////////���ݽṹ���壨��ʼ��////////////////////////////

enum tag_comp_result{
	comp_undefine,
	comp_equal,
	comp_great,
	comp_less
};
enum tag_param_type{
	type_by_value,//ֵ����
	type_by_ref//���ô���
};
typedef tag_param_type ret_value_type;
enum tag_result_type{
	return_no_type,
	type_result_string,
	type_result_number,
	type_result_list,
	type_result_null
};
typedef tag_result_type _tag_function_return_type;

enum _tag_token_type{
	no_type,
	operator_word,
	pair_begin_word,
	pair_end_word,
	key_word,
	normal_word
};

enum _tag_operator_type{
	oper_type_undefine,
	oper_shift_begin, //(
	oper_shift_end,   //)
	oper_list_index_begin,//[
	oper_list_index_end,//]
	oper_add_left,  // +//����
	oper_sub_left,  // -//����
	oper_add,  // +
	oper_sub,  // -
	oper_shrl, // <<
	oper_shrr, // >>
	oper_mul,  // *
	oper_mod,  // %
	oper_div,  // /
	oper_ae,   // >=
	oper_a,		// >
	oper_e,		// ==
	oper_ne,	// !=
	oper_le,	// <=
	oper_l,		// <
	oper_pttv,	// =
	oper_dot_f, // ,
	oper_and,	// &
	oper_bool_and,// &&
	oper_or,//	|
	oper_bool_or,	// ||
	oper_bool_not, //!
	oper_not, // ~
	oper_xor,// ^
	oper_self_add_left,//++
	oper_self_sub_left,//--
	oper_self_add_right,//++
	oper_self_sub_right,//--
	oper_add_pttv,//+=
	oper_sub_pttv,//-=
	oper_mul_pttv,//*=
	oper_div_pttv,// /=
	oper_or_pttv,// |=
	oper_and_pttv,// &=
	oper_xor_pttv,// ^=
	oper_shrl_pttv,// <<=
	oper_shrr_pttv,// >>=
	oper_mod_pttv// %=
};
#ifdef LZY_COUNT
static string oper_parse="";
static string g_oper_parse[]={
	"oper_type_undefine",
	"oper_shift_begin", //(
	"oper_shift_end",   //)
	"oper_list_index_begin",//[
	"oper_list_index_end",//]
	"oper_add_left",  // +//����
	"oper_sub_left",  // -//����
	"oper_add",  // +
	"oper_sub",  // -
	"oper_shrl", // <<
	"oper_shrr", // >>
	"oper_mul",  // *
	"oper_mod",  // %
	"oper_div",  // /
	"oper_ae",   // >=
	"oper_a",		// >
	"oper_e",		// ==
	"oper_ne",	// !=
	"oper_le",	// <=
	"oper_l",		// <
	"oper_pttv",	// =
	"oper_dot_f", // ",
	"oper_and",	// &
	"oper_bool_and",// &&
	"oper_or",//	|
	"oper_bool_or",	// ||
	"oper_bool_not", //!
	"oper_not", // ~
	"oper_xor",// ^
	"oper_self_add_left",//++
	"oper_self_sub_left",//--
	"oper_self_add_right",//++
	"oper_self_sub_right",//--
	"oper_add_pttv",//+=
	"oper_sub_pttv",//-=
	"oper_mul_pttv",//*=
	"oper_div_pttv",// /=
	"oper_or_pttv",// |=
	"oper_and_pttv",// &=
	"oper_xor_pttv",// ^=
	"oper_shrl_pttv",// <<=
	"oper_shrr_pttv",// >>=
	"oper_mod_pttv"
};
#endif

enum _tag_pair_type{
	string_block_type
	,change_level_type
	,program_block_type
};
enum _tag_key_word_type{
	key_word_unknown_type,
	condition_if,
	condition_else,
	while_loop,
	for_loop,
	return_oper,
	string_type_def,
	number_type_def,
	switch_case,
	case_case,
	break_loop,
	continue_loop,
	comm_id_type,
	list_type_def,/*����list���ͱ���*/
	for_each_loop,
	null_type_def
};
//ģ�庯�����ڼ��������С
template<class T> 
INLINE INT chain_size(T* ptr){
	INT ret=0;
	for(typename T* p=ptr;p;p=p->next,ret++);
	return ret;
}
class tag_pair_string{
public:
	tag_pair_string(_tag_pair_type t,const string& v_b,const string& v_e)
		:type(t),begin(v_b),end(v_e){}
	tag_pair_string(const tag_pair_string& src){
		type=src.type;
		begin=src.begin;
		end=src.end;
	}
	_tag_pair_type type;
	string begin;
	string end;
};
//����������
#define OPER_NORMAL	1//һ��˫Ŀ������
#define OPER_SINGLE_LEFT 2//��Ŀ����������
#define OPER_SINGLE_RIGHT 4//��Ŀ�������ҽ��
class tag_operator{
private:
	INT level;//���������ȼ�
	_tag_operator_type type;//����������
	INT property;//����������
	INT bcomputer;//�Ƿ�Ϊ���������
public:
	tag_operator():level(NO_LEVEL),type(oper_type_undefine),property(OPER_NORMAL),bcomputer(TRUE){}
	tag_operator(_tag_operator_type t,INT l,INT prop=OPER_NORMAL,INT bcp=TRUE):level(l)
		,type(t),property(prop),bcomputer(bcp){}
	INLINE BOOL is_validate() const{
		return level!=NO_LEVEL && type!=oper_type_undefine;
	}
	INLINE _tag_operator_type get_type() const{
		return type;
	}
	INLINE BOOL operator ==(const tag_operator& dst) const{
		return level==dst.level;
	}
	INLINE BOOL operator >(const tag_operator& dst) const{
		return level<dst.level;
	}
	INLINE BOOL operator <(const tag_operator& dst) const{
		return level>dst.level;
	}
	INLINE BOOL operator >=(const tag_operator& dst) const{
		return !(operator <(dst));
	}
	INLINE BOOL operator <=(const tag_operator& dst) const{
		return !(operator >(dst));
	}
	INLINE BOOL is_normal_oper() const{
		return (property==OPER_NORMAL);
	}
	INLINE BOOL is_single_oper() const{
		return is_left_single_oper() || is_right_single_oper();
	}
	INLINE BOOL is_left_single_oper() const{
		return (property==OPER_SINGLE_LEFT);
	}
	INLINE BOOL is_right_single_oper() const{
		return (property==OPER_SINGLE_RIGHT);
	}
	INLINE BOOL is_computer_oper() const{
		return bcomputer;
	}
};

#define OPER_SINGLE_ALL		(OPER_SINGLE_LEFT|OPER_SINGLE_RIGHT)//��Ŀ������˫��ɽ��
#define OPER_ALL	(OPER_NORMAL | OPER_SINGLE_ALL) //ȫ�ܲ�����<��������>
class tag_operator_define;
typedef map<string,tag_operator_define*>::value_type v_m_type;
class tag_operator_define{
	friend class alanysis_tokens;
	friend class string_parse;
public:
	tag_operator_define(const tag_operator_define& src){
		m_bis_compute=src.m_bis_compute;
		property=src.property;
		oper=src.oper;
		index=src.index;
		p_normal=src.p_normal;
		p_single_left=src.p_single_left;
		p_single_right=src.p_single_right;
	}
	tag_operator_define(const string& op,map<string,tag_operator_define*>& map
		,tag_operator* pn,INT idx,INT prop=OPER_NORMAL,BOOL bcp=TRUE
		,tag_operator* psl=NULL,tag_operator*psr=NULL):m_bis_compute(bcp)
		,property(prop),oper(op),p_normal(pn),p_single_left(psl)
		,p_single_right(psr),index(idx){
		map.insert(v_m_type(op,(tag_operator_define*)this));
	}
	
	INLINE INT get_index(){
		return index;
	}
	INLINE tag_operator* get_operator(const tag_operator* prev,BOOL bfirst=FALSE){
		tag_operator* ret=NULL;
		if(!m_bis_compute){
			//�����ǰΪ�Ǽ�����ţ�ֱ��ѡ��ͨ�÷�
			ret=p_normal;
		}else if(prev){
			//ǰһ���ǲ�����
			if(prev->is_computer_oper()){
				//�Ǽ��������
				if(prev->is_right_single_oper()){
					//����ȡΪ��׺
					if(property & OPER_SINGLE_RIGHT){
						ret=p_single_right;
					}else if(property & OPER_NORMAL){
						ret=p_normal;
					}
				}else{
					if(property & OPER_SINGLE_LEFT){
						ret=p_single_left;
					}
				}
			}else{
				if(prev->get_type()==oper_shift_begin || 
					prev->get_type()==oper_dot_f){
					//ֻ����ǰ׺
					if(property & OPER_SINGLE_LEFT){
						//����ȡΪǰ׺
						ret=p_single_left;
					}
				}else{
					if(property & OPER_SINGLE_RIGHT){
						//����ȡΪ��׺
						ret=p_single_right;
					}else if(property & OPER_NORMAL){
						//һ�������
						ret=p_normal;
					}
				}
			}//ǰ׺��������һ������
		}else if(bfirst){
			if(property & OPER_SINGLE_LEFT){
				ret=p_single_left;
			}
		}else{
			//���ǵ�һ��˵��ǰһ��tokenΪ�Ƿ���
			if(property & OPER_SINGLE_RIGHT){
				ret=p_single_right;
			}else if(property & OPER_NORMAL){
				ret=p_normal;
			}
		}
#ifdef LZY_COUNT
		oper_parse+=oper+"[";
		if(ret){
			oper_parse+=g_oper_parse[(INT)ret->get_type()];
		}else{
			oper_parse+="error";
		}
		oper_parse+="]";
#endif
		return ret;
	}
	
private:
	BOOL m_bis_compute;//�Ƿ�Ϊ���������
	INT property;//����������
	string oper;//��������
	INT index;//ȫ������
	tag_operator* p_normal;//һ����������
	tag_operator* p_single_left;//��Ŀ�������������
	tag_operator* p_single_right;//��Ŀ�������ҽ�����
};
enum tag_node_calc_type{
	calc_type_value,
	calc_type_operator,
	calc_type_none
};
class tag_calc_operator_flag{
public:
	tag_calc_operator_flag():oper(NULL),type(calc_type_none){}
	tag_calc_operator_flag(tag_operator* p,tag_node_calc_type t):oper(p),type(t){}
	tag_calc_operator_flag(const tag_calc_operator_flag& src){
		oper=src.oper;
		type=src.type;
	}
	~tag_calc_operator_flag(){};
	tag_operator* oper;
	tag_node_calc_type type;
};
static tag_operator g_oper_max(oper_shift_begin,LEVEL_SFT_BG,OPER_NORMAL,0);//�����Ȳ�����
static tag_operator g_oper_min(oper_shift_end,LEVEL_SFT_ED,OPER_NORMAL,0);//����Ȳ�����
class tag_key_word{
public:
	tag_key_word(_tag_key_word_type tp,const string& v):type(tp),key(v){}
	tag_key_word(const tag_key_word& src){
		type=src.type;
		key=src.key;
	}
	_tag_key_word_type type;
	string key;
};
//�ڵ�ö������
enum _node_type{
	type_node_notype,
	type_function,//����
	type_string,//�ַ���
	type_normal,//һ�����
	type_number,//��������
	type_operator,//������
	type_null
};
//���ʽ����
class tag_func;
static void delete_function(tag_func* pfun);
class tag_node{
public:
	tag_node():type(type_node_notype),dbl_value(0){}
	~tag_node(){
		clear();
	}
	INLINE void clear(){
		switch(type){
		case type_function:
			if(ptr_func)
				delete_function(ptr_func);
			break;
		case type_string:
			if(str_value)
				delete str_value;
			break;
		case type_normal:
			if(var_index<=0 && str_value)
				delete str_value;
			break;
		}
	}
	INLINE void clone_and_clear_node(tag_node& dst) {
		clear();
		memcpy(this,&dst,sizeof(tag_node));
		dst.dbl_value=0;
	}
	_node_type type;
	union{
		string* str_value;
		tag_func* ptr_func;
		tag_operator* p_oper;
		DOUBLE	dbl_value;
	};
	INT			var_index;
};
class tag_press{
public:
	tag_press():next(NULL),prev(NULL){}
	~tag_press(){
		if(next)
			delete next;
	}
	tag_press* next;
	tag_press* prev;
	tag_node	node;
	INLINE tag_press* new_next(){
		tag_press* ptr=new tag_press();
		append_press(ptr);
		return ptr;
	}
	INLINE tag_press* get_tailer(){
		tag_press* ptr=this;
		for(;ptr&& ptr->next;ptr=ptr->next);
		return ptr;
	}
	INLINE void append_press(tag_press* ptr){
		if(ptr){
			tag_press* ptrv=get_tailer();
			if(ptrv){
				ptrv->next=ptr;
				ptr->prev=ptrv;
			}
		}
	}
#if (defined PRINT_PRESS)
	string to_string() const;
#endif
};

//����������
class tag_param{
public:
	tag_param():press(NULL),next(NULL){};
	~tag_param(){
		if(press){
			delete press;
		}
		if(next)
			delete next;
	}
	tag_press* press;
	tag_param* next;
	INLINE tag_param* get_next(){
		if(!next)
			next=new tag_param();
		return next;
	}
};
//��������
class tag_func{
public:
	tag_func():ret_type(return_no_type),name(""),param(NULL),fun_index(0){}
	~tag_func(){
		if(param){
			delete param;
		}
	}
	tag_result_type ret_type;
	string name;
	INT fun_index;
	tag_param* param;
};

static void delete_function(tag_func* pfun){
	if(pfun)
		delete pfun;
}
//if��䶨��
class tag_block;

class tag_sentence_if{
public:
	tag_sentence_if():p_cond(NULL),p_else_block(NULL),p_block(NULL){}
	~tag_sentence_if();
	tag_press* p_cond;//�������ʽ
	tag_block* p_block;//
	tag_block* p_else_block;//
};
class tag_sentence_for_each{
public:
	tag_sentence_for_each():p_block(NULL),var_index(0),index(0),list_index(0),p_index_press(NULL){}
	~tag_sentence_for_each();
	int index;//�ڲ�������
	int var_index;//���ʽ
	int list_index;
	tag_press* p_index_press;//�������ʽ
	tag_block* p_block;
};
class tag_sentence_for{
public:
	tag_sentence_for():p_init(NULL),p_cond(NULL),p_dosth(NULL),p_block(NULL){}
	~tag_sentence_for();
	tag_press* p_init;//��ʼ������
	tag_press* p_cond;//��������
	tag_press* p_dosth;//����������������
	//ѭ�����ֳ����
	tag_block* p_block;
};
class tag_sentence_while{
public:
	tag_sentence_while():p_cond(NULL),p_block(NULL){}
	~tag_sentence_while();
	tag_press* p_cond;//��������
	//ѭ�����ֳ����
	tag_block* p_block;
};
enum tag_sentence_type{
	type_noname_block,//������
	type_sentence_if,//if���
	type_sentence_for,//for ���
	type_sentence_else,//else���
	type_sentence_while,
	type_sentence_do_while,
	type_sentence_return,//���ز�����
	type_sentence_break,
	type_sentence_continue,
	type_sentence_press_seq,//һ����ʽ
	type_sentence_for_each//for each
};

enum tag_token_type{
	type_token_undefine
	,type_token_split
	,type_token_operator
	,type_token_shift//ת���
	,type_token_pair//��Է�
	,type_token_key//�ؼ���
	,type_token_string//�ַ���
	,type_token_id//һ���ʾ��
};
//Դ�ַ�������
class tag_word{
public:
	tag_word():word(""),next(NULL){}
	~tag_word(){
		if(next)
			delete next;
	}
	string word;
	tag_word* next;
	INLINE is_empty(){
		return word.size()==0;
	}
	INLINE tag_word* get_next(){
		if(!next)
			next=new tag_word();
		return next;
	}
	INLINE void clear(){
		word="";
		if(next){
			delete next;
			next=NULL;
		}
	}
	INLINE void append(CHAR ch){
		word+=ch;
	}
	INLINE void append(const string& str){
		word+=str;
	}
};

class tag_token{
public:
#ifdef _DEF_LINE_
	tag_token(string v):value(v),type(type_token_undefine),index(0),nlines(0),ncols(0){};
	tag_token():value(""),type(type_token_undefine),index(0),nlines(0),ncols(0){};
#else
	tag_token(string v):value(v),type(type_token_undefine),index(0){};
	tag_token():value(""),type(type_token_undefine),index(0){};
#endif
	~tag_token(){};
	tag_token_type type;
	string value;
#ifdef _DEF_LINE_
	INT nlines;
	INT ncols;
#endif
	union{
		INT index;
		tag_operator_define* poper;
	};
	INLINE is_empty(){
		return value=="" && type==type_token_undefine && index==0;
	}
	INLINE string to_string(){
		string ret="";
		ret+=value;
		return ret;
	}
	INLINE void clear(){
		value="";
		type=type_token_undefine;
		index=0;
	}
	INLINE void append(CHAR ch){
		value+=ch;
	}
	INLINE void append(const string& str){
		value+=str;
	}
	INLINE void append(const CHAR* pch,INT nlen,BOOL b_string=FALSE){
	#define STRING_BUFFER_LEN 1024
		int i=0;
		CHAR ch=*(pch+nlen),ch_t;
		char buf[STRING_BUFFER_LEN];
		char* ptr=(char*)(pch),*p,*ptmp;
		*(ptr+nlen)='\0';
		if(b_string){
			//ת����滻
			p=((nlen>STRING_BUFFER_LEN)?new char[nlen]:buf);
			for(ptmp=p;i<nlen && 0!=(ch_t=*ptr++);i++){
				if(ch_t=='\\'){
					switch(*ptr){
					case 'n':
						ch_t='\n';
						break;
					case 'r':
						ch_t='\r';
						break;
					case 't':
						ch_t='\t';
						break;
					case '"':
						ch_t='"';
						break;
					case 'e':
						ch_t=0x1B;
						break;
					case '\\':
						ch_t='\\';
						break;
					default:
						//�Ƿ�Ϊ����
						break;
					}
					ptr++,i++;
				}
				*ptmp++=ch_t;
			}
			*ptmp='\0';
			value+=p;
			if(nlen>STRING_BUFFER_LEN)delete p;
		}else value+=ptr;
		*(((char*)(pch))+nlen)=ch;
	}
	INLINE BOOL operator ==(const tag_token& token){
		return value==token.value;
	}
};
class tag_token_chain{
public:
	tag_token node;
	tag_token_chain* prev;//������û�б�Ҫ����
	tag_token_chain* next;
	tag_token_chain():prev(NULL),next(NULL){};
	~tag_token_chain(){
#ifndef LZY_USE_HASH
		if(prev)prev->next=NULL;
		if(next)delete next;
#endif
	}
	INLINE is_empty(){
		return node.is_empty();
	}
	INLINE tag_token_chain* get_next(){
		if(!next){
			next=new tag_token_chain();
			next->prev=this;
		}
		return next;
	}
	INLINE void clear(){
#ifndef LZY_USE_HASH
		node.clear();
		if(next){
			delete next;
			next=NULL;
		}
#endif
	}
	
	INLINE void print_debug(){
		printf("%s",node.to_string().c_str());
		if(next)
			next->print_debug();
	}
};
//ֵ����
class tag_result_value{
	friend class press_computer;
	friend class alanysis_tokens;
private:
	void set_type(tag_result_type v){
		type=v;
	}
protected:
	tag_result_type type;
	ret_value_type ref_type;
	tag_result_value* pref_obj;
	union{
		string* str_value;
		DOUBLE dbl_value;
		vector<tag_result_value>* list_value;
	};
public:
	tag_result_value(const tag_result_value& src){type=return_no_type,*this=src;}
	tag_result_value():dbl_value(0),type(return_no_type),pref_obj(NULL),ref_type(type_by_value){}
	~tag_result_value(){
		clear_mem();
	}
public:
	INLINE void refresh_refer_object(){
		if(pref_obj!=NULL && is_refer_type()){
			pref_obj->copy_values(*this);
			if(pref_obj->is_refer_type())pref_obj->refresh_refer_object();
		}
	}
	INLINE void clear_value(){
		switch(type){
		case type_result_list:
			if(list_value)delete list_value;
			break;
		case type_result_string:
			if(str_value)delete str_value;
			break;
		default:
			break;
		}
		dbl_value=0;
	}
	INLINE void copy_values(const tag_result_value& src){
		if(type!=src.type){
			clear_value();
			type=src.type;
		}
		switch(src.type){
		case type_result_list:
			if(src.list_value){
				if(list_value)*list_value=*src.list_value;
				else list_value=new vector<tag_result_value>(*src.list_value);
			}
			break;
		case type_result_string:
			if(src.str_value){
				if(str_value)*str_value=*src.str_value;
				else str_value=new string(src.str_value->c_str());
			}
			break;
		case type_result_number:
			dbl_value=src.dbl_value;
			break;
		default:
			break;
		}
	}
	INLINE void set_refer_list(vector<tag_result_value>* parr){
		if(is_refer_type()){
			ASSERT(pref_obj!=NULL);
			pref_obj->set_list(parr);
		}
	}
	INLINE void set_refer_string(string* str){
		if(is_refer_type()){
			//����������
			ASSERT(pref_obj!=NULL);
			pref_obj->set_string(str);
		}
	}
	INLINE tag_result_type get_rel_type(){return (is_refer_type()?pref_obj->get_type():get_type());}
	INLINE void set_refer_number(DOUBLE val){
		if(is_refer_type()){
			//����������
			ASSERT(pref_obj!=NULL);
			pref_obj->set_number(val);
		}
	}
	INLINE string* get_refer_string(){
		if(is_refer_type()){
			ASSERT(pref_obj!=NULL);
			return pref_obj->get_string();
		}
		return NULL;
	}
	INLINE void* get_refer_list(){
		if(is_refer_type()){
			ASSERT(pref_obj!=NULL);
			return pref_obj->get_list();
		}
		return NULL;
	}
	INLINE DOUBLE get_refer_number(){
		if(is_refer_type()){
			ASSERT(pref_obj!=NULL);
			return pref_obj->get_number();
		}
		return 0;
	}
	INLINE void get_copy_value(){
		if(pref_obj!=NULL && is_refer_type())
			copy_values(*pref_obj);
	}
	INLINE void set_refer_object(tag_result_value* pobj){
		ref_type=pobj?type_by_ref:type_by_value;
		if(pref_obj=pobj)copy_values(*pobj);
	}
public:
	//�Ƿ�Ϊ��������
	INLINE BOOL is_refer_type(){
		return (ref_type==type_by_ref);
	}
	INLINE BOOL is_null_type()const{
		return (type==type_result_null);
	}
	INLINE tag_result_type get_type() const{
		return type;
	}
	INLINE BOOL is_list_type()const{
		return (type==type_result_list);
	}
	INLINE BOOL is_number_type() const{
		return (type==type_result_number);
	}
	INLINE BOOL is_string_type() const{
		return (type==type_result_string);
	}
	INLINE void clear_mem(){
		clear_value();
		type=return_no_type;
		dbl_value=0;
		pref_obj=NULL;
		ref_type=type_by_value;
	}
	INLINE void set_list(const vector<tag_result_value>* val,BOOL byref=FALSE){
		if(byref || type!=type_result_list){
			clear_value();
			set_type(type_result_list);
		}
		if(val){
			if(list_value)*list_value=*val;
			else list_value=(vector<tag_result_value>*)(byref?val:new vector<tag_result_value>(*val));
		}else if(list_value){
			((vector<tag_result_value>*)list_value)->clear();
		}
	}
	INLINE void set_number(DOUBLE val){
		if(type!=type_result_number){
			clear_value();
			set_type(type_result_number);
		}
		dbl_value=(DOUBLE)val;
	}
	INLINE void set_string(string* val){
		if(type!=type_result_string){
			clear_value();
			set_type(type_result_string);
		}
		if(val){
			if(str_value)*str_value=*val;
			else{
				str_value=new string(*val);
#if 0
				if((((LONG)str_value) & 0xFFFF)==0xD9C8){
					int iii=0;
				}
#endif
			}
		}else if(str_value) *str_value="";
	}
	INLINE void set_string(const string& val){
		if(type!=type_result_string){
			clear_value();
			set_type(type_result_string);
		}
		if(str_value)*str_value=val;
		else str_value=new string(val);
	}
	INLINE string* get_string() const{
		return is_string_type()?str_value:NULL;
	}
	INLINE DOUBLE get_number() const{
		return (is_string_type()?0:dbl_value);
	}
	INLINE void* get_list()const{
		return is_list_type()?list_value:NULL;
	}
	INLINE BOOL operator ==(const tag_result_value& dst){
		return (type==dst.type 
			&& ((is_string_type() || is_list_type())?(*str_value==*dst.str_value):dbl_value==dst.dbl_value));
	}
	INLINE tag_result_value& operator =(const tag_result_value& dst){
		if(dst.is_string_type()){
			set_string(dst.str_value);
		}else if(dst.is_list_type()){
			set_list(dst.list_value);
		}else{
			set_number(dst.dbl_value);
		}
		return (*this);
	}
};
enum tag_exit_type{
	type_not_exit,
	type_return_exit,
	type_break_exit,
	type_continue_exit
};
//����/����/��������
class tag_computer_result:public tag_result_value{
	friend class tag_computer_result;
public:
	tag_computer_result():tag_result_value(){}
	tag_computer_result(const tag_computer_result& src){*this=src;}
	~tag_computer_result(){
		pref_obj=NULL;
		ref_type=type_by_value;
	}
public:
	
	INLINE BOOL calculate(tag_computer_result* pdst
		,const tag_operator& oper,string& error){
		BOOL ret=TRUE;
		if(pdst){
			ret=calculate(*pdst,oper,error);
			set_refer_object(NULL);
		}
		return ret;
	}
	INLINE BOOL operator ==(const tag_computer_result& dst){
		return (ref_type==dst.ref_type && is_refer_type()?pref_obj==dst.pref_obj:
				(*(tag_result_value*)this)==(tag_result_value)dst);
	}
	INLINE tag_computer_result& operator =(const tag_computer_result& dst){
		tag_result_value* pv=(tag_result_value*)this
			,*pdst=(tag_result_value*)&dst;
		*pv=*pdst;
		pref_obj=dst.pref_obj;
		ref_type=dst.ref_type;
		return (*this);
	}
	
	//��Ŀ����
	INLINE BOOL single_calculate(const tag_operator& v_oper,string& error){
		BOOL ret=TRUE;
		get_copy_value();//��������
		DOUBLE dlbv=get_number();
		if(!is_number_type()){
			error=string("����Ļ�û��ָ�����������͡�");
			return FALSE;
		}
		switch(v_oper.get_type()){
		case oper_add_left:
			break;
		case oper_sub_left:
			set_number(-get_number());
			set_refer_object(NULL);//ȡ������
			break;
		case oper_self_add_left:
			if(!is_refer_type()){
				error=string("++���������������ڱ���");
				return FALSE;
			}
			ASSERT(pref_obj!=NULL);
			set_refer_number(get_refer_number()+1);
			//set_refer_object(NULL);//ȡ������
			set_number(dlbv+1);
			break;
		case oper_self_add_right:
			if(!is_refer_type()){
				error=string("++���������������ڱ���");
				return FALSE;
			}
			ASSERT(pref_obj!=NULL);
			set_refer_number(get_refer_number()+1);
			set_refer_object(NULL);//ȡ������
			break;
		case oper_self_sub_left:
			if(!is_refer_type()){
				error=string("--���������������ڱ���");
				return FALSE;
			}
			ASSERT(pref_obj!=NULL);
			set_refer_number(get_refer_number()-1);
			//set_refer_object(NULL);//ȡ������
			set_number(dlbv-1);
			break;
		case oper_self_sub_right:
			if(!is_refer_type()){
				error=string("--���������������ڱ���");
				return FALSE;
			}
			ASSERT(pref_obj!=NULL);
			set_refer_number(get_refer_number()-1);
			set_refer_object(NULL);//ȡ������
			break;
		case oper_not:
			{
				INT val=(INT)dlbv;
				set_number(~val);
				set_refer_object(NULL);//ȡ������
			}
			break;
		case oper_bool_not:
			{
				INT val=(INT)dlbv;
				set_number(!val);
				set_refer_object(NULL);//ȡ������
			}
			break;
		default:
			break;
		}
		//set_refer_object(NULL);
		return ret;
	}
	INLINE void append_value(DOUBLE dbl){
		char ptr[64];
		long val=(long)dbl;
		if((DOUBLE)val==dbl)sprintf(ptr,"%d\0",val);
		else sprintf(ptr,"%.15f\0",dbl);
		string strt=(str_value?((*str_value)+ptr):ptr);
		set_string(&strt);
	}
	INLINE BOOL calculate(tag_computer_result& dst
		,const tag_operator& oper,string& error){
		BOOL ret=TRUE;
		get_copy_value();//��������
		DOUBLE dlbv=get_number();
		//vector<string>* parr,*ptr_arr;
		string* ptr,*ptr_curr,strt;
		_tag_operator_type tp_d;
		tag_result_type src_t;
		DOUBLE number,val=0; 
		INT int_val=0;
		src_t=get_type();
		if(src_t==return_no_type){
			error=string("����Ļ�û��ָ�����������͡�");
			return FALSE;
		}
		switch((src_t==type_result_list?src_t:dst.get_type())){
		case type_result_number:
			number=dst.get_number();
			val=get_number();
			int_val=(INT)val;
			tp_d=oper.get_type();
			if(is_string_type() && (tp_d!=oper_add && tp_d!=oper_add_pttv)){
				error=string("����Ļ�û��ָ�����������͡�");
				return FALSE;
			}
			switch(tp_d){
			case oper_add:
				if(src_t==type_result_number)
					set_number(val+number);
				else//���ַ���
					append_value(number);
				break;
			case oper_add_pttv:
				if(NULL==pref_obj){
					error=string("+= ���������������ڱ���");
					return FALSE;
				}
				if(src_t==type_result_number){
					set_number(val+number);
					set_refer_number(get_number());
				}else{//���ַ���
					append_value(number);
					set_refer_string(get_string());
				}
				
				break;
			case oper_sub:
				set_number(val-number);
				break;
			case oper_sub_pttv:
				if(NULL==pref_obj){
					error=string("-= ���������������ڱ���");
					return FALSE;
				}
				set_number(val-number);
				set_refer_number(get_number());
				break;
			case oper_mul:
				set_number(val*number);
				break;
			case oper_mul_pttv:
				if(NULL==pref_obj){
					error=string("*= ���������������ڱ���");
					return FALSE;
				}
				set_number(val*number);
				set_refer_number(get_number());
				break;
			case oper_div:
				set_number(val/number);
				break;
			case oper_div_pttv:
				if(NULL==pref_obj){
					error=string("/= ���������������ڱ���");
					return FALSE;
				}
				set_number(val/number);
				set_refer_number(get_number());
				break;
			case oper_ae:
				set_number(val>=number);
				break;
			case oper_a:
				set_number(val>number);
				break;
			case oper_le:
				set_number(val<=number);
				break;
			case oper_l:
				set_number(val<number);
				break;
			case oper_e:
				set_number(val==number);
				break;
			case oper_ne:
				set_number(val!=number);
				break;
			case oper_pttv:
				if(NULL==pref_obj){
					error=string("= ���������������ڱ���");
					return FALSE;
				}
				set_number(number);
				set_refer_number(get_number());
				break;
			case oper_and:
				set_number(int_val &=(INT)number);
				break;
			case oper_and_pttv:
				if(NULL==pref_obj){
					error=string("&= ���������������ڱ���");
					return FALSE;
				}
				set_number(int_val &=(INT)number);
				set_refer_number(get_number());
				break;
			case oper_bool_and:
				set_number(int_val && (INT)number);
				break;
			case oper_or:
				set_number(int_val |=(INT)number);
				break;
			case oper_or_pttv:
				if(NULL==pref_obj){
					error=string("|= ���������������ڱ���");
					return FALSE;
				}
				set_number(int_val |=(INT)number);
				set_refer_number(get_number());
				break;
			case oper_bool_or:
				set_number(int_val || (INT)number);
				break;
			case oper_xor:
				set_number(int_val^=((INT)number));
				break;
			case oper_xor_pttv:
				if(NULL==pref_obj){
					error=string("^= ���������������ڱ���");
					return FALSE;
				}
				set_number(int_val^=((INT)number));
				set_refer_number(get_number());
				break;
			case oper_shrl:
				set_number(int_val >>=(INT)number);
				break;
			case oper_shrl_pttv:
				if(NULL==pref_obj){
					error=string("<<= ���������������ڱ���");
					return FALSE;
				}
				set_number(int_val <<=(INT)number);
				set_refer_number(get_number());
				break;
			case oper_shrr:
				set_number(int_val >>=(INT)number);;
				break;
			case oper_shrr_pttv:
				if(NULL==pref_obj){
					error=string(">>= ���������������ڱ���");
					return FALSE;
				}
				set_number(int_val >>=(INT)number);
				set_refer_number(get_number());
				break;
			case oper_mod:		
				set_number(int_val %=(INT)number);
				break;
			case oper_mod_pttv:
				if(NULL==pref_obj){
					error=string("%= ���������������ڱ���");
					return FALSE;
				}
				set_number(int_val %=(INT)number);
				set_refer_number(get_number());
				break;
			}
			break;
		case type_result_list:
			//if(is_list_type()){
				switch(oper.get_type()){
				case oper_pttv:
					if(!pref_obj){
						error=string("= ���������������ڱ���");
						return FALSE;
					}
					*pref_obj=dst;
					get_copy_value();//��������
					break;
				case oper_e:
					if(dst.is_null_type()){
						set_number(pref_obj->get_list()==NULL);
					}else{
						set_number(pref_obj==dst.get_list());
					}
					break;
				case oper_ne:
					if(dst.is_null_type()){
						set_number(pref_obj->get_list()!=NULL);
					}else{
						set_number(pref_obj!=dst.get_list());
					}
					break;
				default:
					error=string("�������Ͳ�ƥ��,��ָ�����������ϵ�����û�ж��塣");
					ret=FALSE;
					break;
				}
				/*
			}else{
				error=string("�������Ͳ�ƥ�䡣");
				ret=FALSE;
			}*/
			break;
		case type_result_null:
			if(is_list_type()){
				switch(oper.get_type()){
				case oper_e:
					break;
				default:
					error=string("�������Ͳ�ƥ��,��ָ�����������ϵ�����û�ж��塣");
					ret=FALSE;
					break;
				}
			}else{
				error=string("�������Ͳ�ƥ�䡣");
				ret=FALSE;
			}
			break;
		case type_result_string:
			if(is_string_type()){
				ptr=dst.get_string();
				ptr_curr=get_string();
				switch(oper.get_type()){
				case oper_add:
					strt=(*ptr_curr+(ptr?*ptr:""));
					set_string(ptr_curr?&strt:ptr);
					break;
				case oper_add_pttv:
					if(NULL==pref_obj){
						error=string("+= ���������������ڱ���");
						return FALSE;
					}
					ASSERT(pref_obj!=NULL);
					strt=(*ptr_curr+(ptr?*ptr:""));
					set_string(ptr_curr?&strt:ptr);
					set_refer_string(get_string());
					break;
				case oper_ae:
					set_number(ptr_curr?(ptr?(*ptr_curr)>=(*ptr):TRUE):(ptr?FALSE:TRUE));
					break;
				case oper_a:
					set_number(ptr_curr?(ptr?(*ptr_curr)>(*ptr):TRUE):FALSE);
					break;
				case oper_l:
					set_number(ptr_curr?(ptr?(*ptr_curr)<(*ptr):FALSE):FALSE);
					break;
				case oper_le:
					set_number(ptr_curr?(ptr?(*ptr_curr)<=(*ptr):FALSE):TRUE);
					break;
				case oper_e:
					set_number(ptr_curr?(ptr?(*ptr_curr)==(*ptr):TRUE):!ptr);
					break;
				case oper_ne:
					set_number(ptr_curr?(ptr?(*ptr_curr)!=(*ptr):FALSE):ptr!=NULL);
					break;
				case oper_pttv:
					if(!pref_obj){
						error=string("= ���������������ڱ���");
						return FALSE;
					}
					*pref_obj=dst;
					get_copy_value();//��������
					break;
				default:
					error=string("�������Ͳ�ƥ��,��ָ�����������ϵ�����û�ж��塣");
					ret=FALSE;
					break;
				}
			}else{
				error=string("�������Ͳ�ƥ�䡣");
				ret=FALSE;
			}
			break;
		
		}
		return ret;
	}
};

//��������ʵ�ζ���
class tag_func_param_value_chain{
public:
	tag_func_param_value_chain():next(NULL){}
	~tag_func_param_value_chain(){
		if(next)
			delete next;
	}
	tag_func_param_value_chain* next;
	tag_computer_result param;
};
//������ʽ��������
class tag_func_param_define_chain{
public:
	tag_func_param_define_chain():next(NULL),param_name(""),type(return_no_type)
		,p_type(type_by_value),param_value(NULL),index(-1){}
	~tag_func_param_define_chain(){
		if(param_value && p_type==type_by_value){
			delete param_value;
		}
		if(next)
			delete next;
	}
	INLINE BOOL is_empty(){
		return (param_name=="");
	}	
	INLINE tag_func_param_define_chain* new_next(BOOL breuse=TRUE){
		tag_func_param_define_chain* pret=get_tailer();
		if(breuse && pret->is_empty()){
			return pret;
		}else{
			pret->next=new tag_func_param_define_chain();
			return pret->next;
		}
	}
	INLINE tag_func_param_define_chain* get_tailer(){
		tag_func_param_define_chain* ptr=this;
		for(;ptr && ptr->next;ptr=ptr->next);
		return ptr;
	}
	tag_func_param_define_chain* next;
	string param_name;//��������
	INT	index;//����ȫ�����
	tag_result_type type;//������������
	tag_param_type p_type;//������������(ֵ����/���ô���)
	tag_computer_result* param_value;//����ֵ
};
class tag_press_seq{
public:
	tag_press_seq():next(NULL),press(NULL){}
	~tag_press_seq(){
		if(press){
			delete press;
		}
		if(next)
			delete next;
	}
	INLINE tag_press_seq* new_next(){
		tag_press_seq* ptr=new tag_press_seq();
		append(ptr);
		return ptr;
	}
	INLINE tag_press_seq* get_tailer(){
		tag_press_seq* ptr=this;
		for(;ptr && ptr->next;ptr=ptr->next);
		return ptr;
	}
	INLINE void append(tag_press_seq* ptr){
		if(ptr){
			tag_press_seq* ptrv=get_tailer();
			ASSERT(ptrv!=NULL);
			ptrv->next=ptr;
		}
	}
	tag_press_seq* next;
	tag_press* press;
};
class tag_sentence_return{
public:
	tag_sentence_return():ret_press(NULL){}
	~tag_sentence_return(){
		if(ret_press)
			delete ret_press;
	}
	tag_press* ret_press;
};
//����ڵ����
class tag_block_node{
public:
	tag_block_node* next;//��һ���ڵ�
public:
	INLINE tag_block_node* new_next(BOOL breuse=TRUE) const{
		tag_block_node* ptr=get_tailer();
		return ((breuse && ptr->is_empty())?ptr:(ptr->next=new tag_block_node()));
	}
	INLINE tag_block_node* get_tailer() const{
		tag_block_node* ptr=(tag_block_node*)this;
		for(;ptr && ptr->next;ptr=ptr->next);
		return ptr;
	}
	INLINE BOOL is_empty() const{
		return p_press==NULL;
	}
	tag_block_node():next(NULL),p_press(NULL),type(type_noname_block){}
	~tag_block_node();
	tag_sentence_type type;//���������
	union{
		tag_sentence_return* p_return;
		tag_block*	p_block;
		tag_sentence_if* p_if;
		tag_press_seq*		p_press;
		tag_sentence_for* p_for;
		tag_sentence_while* p_while;
		tag_sentence_for_each* p_for_each;
	};
};
//�������
typedef map<string,INT>::value_type local_var_type;
class tag_block{
public:
	INLINE INT get_location_index(const string& name){
		map<string,INT>::iterator it;
		if((it=m_local.find(name))==m_local.end()){
			if(parent){
				return parent->get_location_index(name);
			}else{
				return 0;
			}
		}else{
			return (INT)it->second;
		}
	}
	INLINE BOOL set_location_var(const string& name,INT idx){
		map<string,INT>::iterator it;
		if((it=m_local.find(name))==m_local.end()){
			m_local.insert(local_var_type(name,idx));
			return TRUE;
		}else{
			//�����Ѿ�����
			return FALSE;
		}
	}
	tag_block():parent(NULL){}
	~tag_block(){
		
	}
	tag_block_node node;//����ڵ����
	map<string,INT> m_local;//��ǰ��ľֲ������������󼯺ϡ�
	tag_block* parent;//��Ӧ���ϼ��������
public:
	INLINE BOOL is_empty() const{
		return node.is_empty();
	}
	INLINE tag_block* new_child() const{
		tag_block* ptr=new tag_block();
		ptr->parent=(tag_block*)this;
		return ptr;
	}
	
};
//�û��Զ��庯������
class tag_user_function{
public:
	tag_user_function():p_node(NULL){}
	~tag_user_function();
	tag_func_param_define_chain m_param;//
	tag_block* p_node;
	tag_computer_result ret_value;
	//��������
};

typedef std::map<string,INT>::value_type  val_Type;
typedef std::map<string,INT>::iterator  function_itorator;
//������
class tag_program{
private:
	INLINE void clear_function_arr(){
		INT key;
		BOOL bFind=FALSE;
		tag_user_function* ptrItem;
		POSITION pos = arr_function.GetStartPosition();
		while (pos != NULL){
			arr_function.GetNextAssoc(pos, key, ptrItem);
			if(ptrItem){
				delete ptrItem;
			}
		}
	}
public:
	tag_program():var_index(1),fun_index(1){}
	~tag_program(){
		clear_function_arr();
	}
	//�õ�����
	INLINE tag_user_function* get_function(INT index){
		tag_user_function* ret=NULL;
		if(index>0 && index<fun_index){
			tag_user_function** pret=arr_function.GetValue(index);
			if(pret)ret=(*pret);
		}
		return ret;
	}
	//�õ��������
	INLINE INT get_function_index(const string& name){
		INT ret=0;
		function_itorator it;
		if((it=m_idx_func.find(name))!=m_idx_func.end()){
			ret=it->second;
		}
		return ret;
	}
	INLINE BOOL remove_top(const string &name){
		if(fun_index>1){
			arr_function.RemoveKey(--fun_index);
			m_idx_func.erase(name);
			return TRUE;
		}
		return FALSE;
	}
	INLINE INT set_function(const string &name,const tag_user_function* pval){
		if(m_idx_func.find(name)==m_idx_func.end()){//��֧�ֶ�̬
			INT ret=fun_index++;
			arr_function.SetValue(ret,(tag_user_function**)&pval);
			m_idx_func.insert(val_Type(name,ret));
			return ret;
		}else
			return 0;
	}
	//�õ�����
	INLINE tag_computer_result* get_local_var(INT index){
		tag_computer_result* ret=NULL;
		if(index>0 && index<var_index){
			ret=var_location.GetValue(index);
		}
		return ret;
	}
	INLINE BOOL set_value_param(INT idx,tag_computer_result* pval){
		tag_computer_result* pret=get_local_var(idx);
		if(pret){
			*pret=*pval;
			if(pret)pret->refresh_refer_object();
			return TRUE;
		}else{
			return FALSE;
		}
	}
	//�������
	INLINE INT set_local_var(const tag_computer_result& val){
		INT ret=var_index++;
		var_location.SetValue(ret,(tag_computer_result*)&val);
		return ret;
	}
	INT var_index;
	INT fun_index;
	map<string,INT>	 m_idx_func;//��������
	CLMapBase<INT,tag_computer_result>	 var_location;//��������
	CLMapBase<INT,tag_user_function*>	 arr_function;//��������
	tag_block	m_block;
};

interface IParse_Function_Macro;
//��̬����ʱ������
class tag_global_context{
public:
	tag_global_context():m_ptr_outer(NULL){}
	IParse_Function_Macro*			m_ptr_outer;
	//map<string,tag_user_function*>   *m_user_func;//�û��Զ��庯������
};
#ifndef LIB_SUPPORT_C_EX
#include "api_face/script_include.h"
#else
	typedef void* (*palloc_mem)(size_t sz);
	typedef void (*pfree_mem)(void* p);
	typedef void* (*padd_list_item)(void* hdr,const char* pitem);
	typedef void* (*padd_list)(void* hdr,void* psublist);
	typedef int (*pget_list_size)(void* hdr);
	typedef struct _tag_memory_mgrl{
		palloc_mem new_mem;
		pfree_mem delete_mem;
		padd_list_item add_list_item;
		padd_list add_list;
		pget_list_size get_list_size;
	}tag_memory_mgrl;
#endif
//////////////////////////////���ݽṹ���壨�꣩////////////////////////////
interface IParse_Alpha
{
public:
	virtual _tag_key_word_type get_keyword_type(const string& key) ABSTRACT;
	virtual BOOL is_valid_alpha(CHAR ch) ABSTRACT;/*�ַ��Ƿ��Ǹ��Ϸ��ַ�*/
	/*���ݵ�ǰָ���ж��Ƿ�Ϊ��Է�*/
	virtual BOOL is_pair_alpha(const CHAR* pch,INT& nmove,tag_pair_string*& ppair,BOOL& bisend,INT& id) ABSTRACT;
	/*�ж��Ƿ�Ϊ���õĿհ׷ָ��ַ�*/
	virtual BOOL is_skip_alpha(const CHAR* pch,INT& nmove) ABSTRACT;
	virtual BOOL is_remark_flag(const CHAR* pch,INT& nmove) ABSTRACT;
	/*�Ƿ�Ϊ������*/
	virtual BOOL is_operator_alpha(const CHAR* it,INT& nmove,tag_operator_define*& poper) ABSTRACT;
	/*�Ƿ�Ϊת���*/
	virtual BOOL is_shift_alpha(const CHAR* pch,INT& nmove) ABSTRACT;
	/*�Ƿ�ΪԤ����ָ���*/
	virtual BOOL is_split_alpha(const CHAR* pch,INT& nmove) ABSTRACT;
	/*�Ƿ�Ϊ�����ָ�����*/
	virtual BOOL is_other_alpha(const CHAR* pch,INT& nmove,string& pstring) ABSTRACT;
	//virtual tag_operator* get_operator(INT index) ABSTRACT;
	virtual tag_pair_string* get_pair(INT index) ABSTRACT;
};
#ifdef LIB_SUPPORT_C_EX
typedef int (*prun_script_direction)(char* script,double& ret_dbl
						  ,char*& ret_str,IParse_Function_Macro* pcall);
typedef int (*prun_script_handle)(INT handle,double& ret_dbl,char*& ret_str
						  ,IParse_Function_Macro* pcall);
typedef int (*pget_script_handle)(const char* script,char*& error);
typedef int (*pfree_script_handle)(INT handle);
#endif
interface IParse_Function_Macro
{
public:
	virtual BOOL call_function(const string& name,const tag_func_param_value_chain* param
		,tag_result_type type,tag_computer_result& ret,string& error,LPVOID pparam) ABSTRACT;
	virtual BOOL get_macro_define(const string& name,tag_computer_result& ret
		,string& error,LPVOID pparam) ABSTRACT;
};

class tag_runtime_context{
public:
	tag_runtime_context():m_ptr_current(NULL),m_ptr_global(NULL){}
	INLINE tag_user_function* get_user_function(INT idx){
		return (ptr_prog?ptr_prog->get_function(idx):NULL);
	}
	//�õ�����ʶ��ķ���<���ص�ʼ��Ϊ����>
	INLINE BOOL get_recognise_flag(INT index,tag_computer_result& ret){
		BOOL pret=FALSE;
		tag_computer_result* rett=NULL;
		if(ptr_prog){
			rett=ptr_prog->get_local_var(index);
			if(rett){
				ret.set_refer_object(rett);
				pret=TRUE;
			}
		}
		return pret;
	}
	INLINE BOOL call_function(const string& name,const tag_func_param_value_chain* param
		,tag_result_type type,tag_computer_result& ret,string& error,LPVOID pparam){
		ASSERT(m_ptr_global!=NULL);
		if(m_ptr_global->m_ptr_outer!=NULL){
			return m_ptr_global->m_ptr_outer->call_function(name,param,type,ret,error,pparam);
		}
		return FALSE;
	}
	INLINE BOOL get_macro_define(const string& name,tag_computer_result& ret
		,string& error,LPVOID pparam){
		ASSERT(m_ptr_global!=NULL && m_ptr_global->m_ptr_outer!=NULL);
		return m_ptr_global->m_ptr_outer->get_macro_define(name,ret,error,pparam);
	}
	tag_block*						m_ptr_current;
	tag_global_context*				m_ptr_global;
	tag_program*					ptr_prog;//��������
};

INLINE BOOL IsNumeric(const CHAR* src)
{
	if(!src)return FALSE;
	BOOL bPre=1;
	INT	dotCount=0;
	CHAR val;
	while(val=*src++){
		if(bPre && val==' ')continue;//�ӹ�ǰ���ո�
		if(bPre){
			bPre=0;
			if(val=='+' || val=='-')continue;
		}
		if((val<'0' || val>'9') 
			&& !(val=='.' && !dotCount++))
			return FALSE;
	}
	return TRUE;
}
#ifndef LIB_SUPPORT_C_EX
INLINE void copy_result_out_2_in(const tag_result_out& out,tag_computer_result& in){
	if(out.type==type_result_string){
		string v_t="";
		if(out.str_value){
			v_t=out.str_value;
		}
		in.set_string(&v_t);
	}else if(out.type==type_result_list){
		in.set_list((vector<tag_result_value>*)out.list_value,TRUE);
	}else{
		in.set_number(out.dbl_value);
	}
}
INLINE void copy_result_in_2_out(const tag_computer_result& in,tag_result_out& out){
	out.type=in.get_type();
	string* strt;
	void* parr;
	if(out.type==type_result_string && (strt=in.get_string())){
		out.str_value=new char[strt->size()+1];
		strcpy(out.str_value,strt->c_str());
	}else if(out.type==type_result_list && (parr=in.get_list())){
		out.list_value=new vector<tag_result_value>(*((vector<tag_result_value>*)parr));
	}else{
		out.dbl_value=in.get_number();
	}
	out.m_buse_malloc=0;
}
INLINE void copy_result_param_chain_in_2_out(const tag_func_param_value_chain& in
											 ,tag_param_chain_out& out){
	tag_func_param_value_chain* ptr=(tag_func_param_value_chain*)&in;
	tag_param_chain_out* ptr_o=&out;
	while(ptr){
		copy_result_in_2_out(ptr->param,ptr_o->param);
		ptr=ptr->next;
		if(ptr){
			ptr_o->next=new tag_param_chain_out();
			ptr_o=(tag_param_chain_out*)ptr_o->next;
			ptr_o->next=NULL;
		}
	}
}
INLINE void clear_result_out(tag_result_out* pout,BOOL bclrMem=FALSE){
	if(pout){
		if(pout->type==type_result_string 
			&& pout->str_value){
#ifdef LZY_INVOID_ERROR
			try{
#endif
				delete pout->str_value;
				pout->str_value=NULL;
#ifdef LZY_INVOID_ERROR
			}catch(...){
				try{
					free(pout->str_value);
				}catch(...){}
			}
#endif
		}else if(pout->type==type_result_list 
			&& pout->list_value){
#ifdef LZY_INVOID_ERROR
			try{
#endif
				if(bclrMem)delete (vector<tag_result_value>*)pout->list_value;
				pout->list_value=NULL;
#ifdef LZY_INVOID_ERROR
			}catch(...){
				try{
					free(pout->str_value);
				}catch(...){}
			}
#endif
		}
	}
}
INLINE void clear_param_chain_out(tag_param_chain_out* pparam){
	if(pparam){
		if(pparam->next){
			clear_param_chain_out((tag_param_chain_out*)pparam->next);
		}
		clear_result_out((tag_result_out*)&pparam->param,TRUE);
		delete pparam;
	}
}
#if (defined _WIN32) && (defined _DEBUG)
static void setClipboadText(string& str){
	DWORD nLen=str.length()+1;
	HGLOBAL hMem=GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE,nLen);
	LPVOID ptr=GlobalLock(hMem);
	strcpy((char*)ptr,(char*)str.c_str());
	GlobalUnlock(hMem);
	OpenClipboard(NULL);
	EmptyClipboard();
	SetClipboardData(CF_TEXT,hMem);
	CloseClipboard();
}
static void setClipboadText(tag_press* ptr_p){
	string str="";
	while(ptr_p){
		str+=ptr_p->to_string();
		ptr_p=ptr_p->next;
	}
	setClipboadText(str);
}
#endif
#endif
//#include "../script_engine/src/base/inter_define.h"
#endif
