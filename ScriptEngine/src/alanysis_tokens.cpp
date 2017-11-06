///////////////////////////////////////////////////////////////////////////////////////
//@module		:	�﷨������(ʵ��)
//@author		:	zhiyong.luo
//@create date	:	2006-12-07
//@last_modify	:	
///////////////////////////////////////////////////////////////////////////////////////

#include "alanysis_tokens.h"
#include "string_parse.h"
#include "script/script_rule.h"
#include "press_computer.h"
static tag_token g_dot_token(",");
static tag_block g_top_block;
#define THIS_PROGRAM  prog.
alanysis_tokens::~alanysis_tokens(){}

//�﷨����,�����﷨��
tag_program* alanysis_tokens::get_program(const string& script,string& error)
{
	/*
	int a=0;
	a=a++ + ++a;
	_tag_data_object obj,obj_t,obj_ss;
	obj.set_string_type();
	obj_t.set_int_type();
	obj_t.set_int_value(1000);
	obj.set_string_value("sfaskajfkalfla");
	INT n=2;
	{
		string strt=obj.get_string_value();
	}
	string strtt=obj.get_string_value();
	obj_ss=obj;

	obj=obj_ss;
	obj=obj_t;
	obj=obj_ss;
	*/
	ASSERT(m_ptrOuter!=NULL);
	string_parse para(m_ptrOuter,"test_script",script);
	tag_token_chain* pchain=para.get_token_chain();
	tag_program* ret=NULL,*pcurr=NULL;
	tag_token_chain* ptr_old=NULL;
	tag_user_function* pfunc=NULL;
	tag_block* pt=NULL,*pnull=NULL,*ptrt=NULL;
	string fun_name;
	INT level=INIT_LEVEL;
	_tag_key_word_type r_type;
	while(pchain){
		if(ret==NULL){
			ret=new tag_program();
		}
		if(is_user_define_function(pchain,r_type)){
			//���û��Զ��庯��
			pchain=pchain->next;
			fun_name=pchain->node.value;//������
			pchain=pchain->next;
			tag_user_function* pprep_fun=new tag_user_function();
			ret->set_function(fun_name,pprep_fun);
			pfunc=get_user_function(*ret,ret->m_block,pprep_fun
				,(r_type==string_type_def?type_result_string:type_result_number),pchain,error);
			if(pfunc==NULL){
				ret->remove_top(fun_name);
				if(ret)
					delete ret;
				ret=NULL;
				goto to_exit;
			}
			continue;
		}else{
			//һ����ʽ��
			if(!get_block(*ret,ret->m_block,pchain,++level,error)){
				if(ret)
					delete ret;
				ret=NULL;
				goto to_exit;
			}
		}
		if(!pchain)break;
		pchain=pchain->next;
	}
to_exit:
	return ret;
}
#define TEST_ERROR_PHRASE() \
	if(!(pchain=pchain->next)){\
		error="�﷨����!";\
		ret=0;\
		goto to_exit;\
	}
#define TEST_ERROR_PHRASE_EX() \
	if(!(pchain=pchain->next)){\
		error="�﷨����!";\
		goto to_exit;\
	}
vector<tag_result_value>* alanysis_tokens::parse_listvar_init(vector<tag_result_value>* parr,tag_token_chain *&pchain,string& error)
{
	BOOL bcreate=FALSE,bOK=FALSE;
	vector<tag_result_value>* pret=parr;
	tag_result_value rst;
	if(!(pchain->node.value=="{")){
		error="list������ʼ���﷨���������ǰ��\"{\"!";
		goto to_exit;
	}
	TEST_ERROR_PHRASE_EX();
	if(bcreate=(pret==NULL))pret=new vector<tag_result_value>();
	for(;;){
		if(pchain->node.value=="{"){
			vector<tag_result_value>* psub=parse_listvar_init(NULL,pchain,error);
			if(!psub)goto to_exit;
			rst.set_list(psub,TRUE);
			if(!(pchain->node.value=="}")){
				error="list������ʼ���﷨��������н���\"}\"!";
				goto to_exit;
			}
			//pchain=pchain->next;
		}else{
			if(pchain->node.type!=type_token_string){
				error="list������ʼ��ʧ��,����Ϊ�ַ���!";
				goto to_exit;
			}
			rst.set_string(pchain->node.value);
		}
		pret->push_back(rst);
		//rst.set_type(return_no_type);
		TEST_ERROR_PHRASE_EX();
		if(!(pchain->node.value==","))break;
		TEST_ERROR_PHRASE_EX();
	}
	if(!(pchain->node.value=="}")){
		error="list������ʼ���﷨��������н���\"}\"!";
		goto to_exit;
	}
	
	bOK=TRUE;
to_exit:
	if(!bOK && bcreate){
		delete pret;
		pret=NULL;
	}
	return pret;
}

INT alanysis_tokens::parse_var_define(tag_program& prog,tag_block& curr_b,tag_token_chain *&pchain,
									   _tag_key_word_type type,string& error)
{
	INT ret=0;
	string name;
	tag_computer_result cret;
	ASSERT(pchain->next);
	name=pchain->next->node.value;
	vector<tag_result_value>* parr=NULL;
	pchain=pchain->next;
	if(pchain)
		pchain=pchain->next;

	switch(type){
	case string_type_def:
		cret.set_string(NULL);
		ret=prog.set_local_var(cret);
		if(!curr_b.set_location_var(name,ret)){
			error="����["+name+"]�Ѿ�����,�����ظ�����!";
			ret=0;
			goto to_exit;
		}
		if(pchain->node.value=="="){
			pchain=pchain->prev;
			if(!get_press_seq(prog,curr_b,pchain,error,TRUE)){
				ret=0;
				goto to_exit;
			}
			/*
			TEST_ERROR_PHRASE();
			if(pchain->node.type!=type_token_string){
				error="string����["+name+"]��ʼ������!";
				ret=0;
				goto to_exit;
			}
			cret.set_string(&pchain->node.value);
			pchain=pchain->next;
			if(pchain)
				pchain=pchain->next;
			*/
		}
		
		/*
		pchain=pchain->next;
		if(pchain)
			pchain=pchain->next;
		*/
		break;
	case number_type_def:
		cret.set_number(0);
		ret=prog.set_local_var(cret);
		if(!curr_b.set_location_var(name,ret)){
			error="����["+name+"]�Ѿ�����,�����ظ�����!";
			ret=0;
			goto to_exit;
		}
		if(pchain->node.value=="="){
			pchain=pchain->prev;
			if(!get_press_seq(prog,curr_b,pchain,error)){
				ret=0;
				goto to_exit;
			}
		}
		/*
		pchain=pchain->next;
		if(pchain)
			pchain=pchain->next;
		*/
		break;
	case list_type_def:
		if(pchain->node.value=="="){
			TEST_ERROR_PHRASE();
			if(!(parr=parse_listvar_init(NULL,pchain,error))){
				ret=0;
				goto to_exit;
			}
			cret.set_list(parr,TRUE);
			pchain=pchain->next;
			if(pchain)
				pchain=pchain->next;
		}else cret.set_list(NULL);
		ret=prog.set_local_var(cret);
		if(!curr_b.set_location_var(name,ret)){
			error="����["+name+"]�Ѿ�����,�����ظ�����!";
			ret=0;
			goto to_exit;
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}
to_exit:
	if(!ret && parr)delete parr;
	return ret;
}
#define PRESS_ERROR_AND_RETURN(msg) \
	error=msg,berror=TRUE;\
	delete ptr,ptr=NULL;\
	goto to_exit

//�õ�һ���������ָ���֮ǰ�ı��ʽ
tag_press* alanysis_tokens::get_press(tag_program& prog,tag_block& curr_b,tag_token_chain *&pchain
										,const tag_token* token,string& error,BOOL& berror,BOOL b_return_when_lease_level){
	tag_press* ptr=NULL,*ptr_curr=NULL;
	INT level=INIT_LEVEL;
	BOOL bexit=FALSE,bfirst=TRUE,b_oper=FALSE;
	tag_operator * ptr_old_oper=NULL;
	berror=FALSE;
	while(pchain){
		if(ptr==NULL){
			ptr_curr=new tag_press();
			ptr=ptr_curr;
		}else{
			ptr_curr=ptr_curr->new_next();
		}
		if(token && (pchain->node==(*token)) && level<=INIT_LEVEL){
			//�˳�
			if(ptr_curr && ptr_curr->prev)
				ptr_curr->prev->next=NULL;
			else
				ptr=NULL;
			delete ptr_curr;
			break;
		}else{
			b_oper=(pchain->node.type==type_token_operator);
			switch(pchain->node.type){
			case type_token_operator:
				ptr_curr->node.type=type_operator;
				ptr_curr->node.p_oper=pchain->node.poper->get_operator(ptr_old_oper,bfirst);
				ptr_old_oper=ptr_curr->node.p_oper;
				if(!ptr_old_oper){
					PRESS_ERROR_AND_RETURN("û�еõ������Ĳ������������Ǳ��ʽ����");
				}
				if(is_begin_shift(&pchain->node)){
					//���ÿ��Ǻ��������<><>
					level++;
				}else if(is_end_shift(&pchain->node)){
					if(b_return_when_lease_level && level==INIT_LEVEL){
						if(ptr_curr && ptr_curr->prev)
							ptr_curr->prev->next=NULL;
						else
							ptr=NULL;
						delete ptr_curr;
						bexit=TRUE;
					}
					level--;
				}
				break;
			case type_token_split:
//				ASSERT(level==INIT_LEVEL);
				if(ptr_curr && ptr_curr->prev)
					ptr_curr->prev->next=NULL;
				else
					ptr=NULL;
				delete ptr_curr;
				bexit=TRUE;
				break;
			case type_token_pair:
				//��Է�
				bexit=is_block_split(&pchain->node);
				if(bexit){
					if(ptr_curr && ptr_curr->prev)
						ptr_curr->prev->next=NULL;
					else
						ptr=NULL;
					delete ptr_curr;
				}
				break;
			case type_token_key:
				//�ؼ���
				 break;
			case type_token_string:
				ptr_curr->node.str_value=new string(pchain->node.value);
				ptr_curr->node.type=type_string;
				break;
			case type_token_id:
				if(pchain->next && is_begin_shift(&pchain->next->node)){
					//�����Ǹ�����
					if(IsNumeric(pchain->node.value.c_str())){
						PRESS_ERROR_AND_RETURN("���ʽ����,�������Ʋ���Ϊ���֡�");
					}else{
						//�Ǻ���
						tag_func* pfun=new tag_func();
						pfun->name=pchain->node.value;
						pchain=pchain->next->next;
						pfun->param=get_param(prog,curr_b,pchain,error);
						ptr_curr->node.ptr_func=pfun;
						ptr_curr->node.type=type_function;
						pfun->fun_index=prog.get_function_index(pfun->name);
					}
				}else{
					if(IsNumeric(pchain->node.value.c_str())){
						char **pend=NULL;
						ptr_curr->node.dbl_value=strtod(pchain->node.value.c_str(),pend);
						ptr_curr->node.type=type_number;
					}else if(0==strcmp(pchain->node.value.c_str(),"null")){
						ptr_curr->node.type=type_null;
					}else{
						//�õ��������
						ptr_curr->node.var_index=curr_b.get_location_index(pchain->node.value);
						if(ptr_curr->node.var_index<=0){
							ptr_curr->node.str_value=new string(pchain->node.value);
						}
						ptr_curr->node.type=type_normal;
					}
				}
				break;
			}
			if(bexit)
				break;
		}
		bfirst=FALSE;
		if(!b_oper){
			ptr_old_oper=NULL;
		}
		pchain=pchain->next;
	}
	if(ptr){
#if (defined _WIN32) && (defined _DEBUG)
#ifdef LZY_COUNT
		setClipboadText(oper_parse);
#endif
#endif
		if(!press_computer::pre_computer_press(*ptr,error)){
			berror=TRUE;
			delete ptr;
			ptr=NULL;
		}
	}
to_exit:
	return ptr;
}
//������������
tag_param* alanysis_tokens::get_param(tag_program& prog,tag_block& curr_b,tag_token_chain *&pchain,string& error){
	tag_param* ret=NULL,*curr=NULL;
	BOOL berror;
	tag_press* ptr=get_press(prog,curr_b,pchain,&g_dot_token,error,berror,TRUE);
	if(berror)return NULL;
	while(ptr){
		if(NULL==ret){
			ret=new tag_param();
			curr=ret;
		}else{
			curr=curr->get_next();
		}
		curr->press=ptr;
		if(is_end_shift(&pchain->node)){
			break;
		}
		pchain=pchain->next;
		ptr=get_press(prog,curr_b,pchain,&g_dot_token,error,berror,TRUE);
		if(berror){
			delete ret;
			return NULL;
		}
	}
	return ret;
}
//�õ�һ�����ʽ�� ��{}�ָ��򣻷ָ�
BOOL alanysis_tokens::get_block(tag_program& prog,tag_block &curr_b/*��ǰ��*/
									  , tag_token_chain *&pchain,INT& level,string& error)
{
	BOOL ret_v=TRUE,berror
		,bsing_press=(pchain && pchain->prev && is_begin_block(&pchain->prev->node));
	tag_block_node* ret=NULL,*ptr_pn=NULL;
	_tag_key_word_type type,r_type=key_word_unknown_type;
	tag_token_type t_tp;
	tag_token_chain* ptr=pchain,*prev=NULL;
	INT old_level=level;
	while(ptr && level>=old_level){
		if(level==INIT_LEVEL)level--;
		BOOL bset_new=TRUE;
		if(is_begin_block(&ptr->node)){
			//��ʼһ���µı��ʽ��
			//����һ���µĽڵ�
			tag_block_node* pnode=curr_b.node.new_next();
			pnode->type=type_noname_block;//������
			pnode->p_block=curr_b.new_child();
			ptr=ptr->next;//��һ��token
			if(!get_block(prog,*(pnode->p_block),ptr,++level,error)){
				ret_v=FALSE;
				goto to_exit;
			}
		}else if(is_end_block(&ptr->node)){
			--level;
			ptr=ptr->next;
			continue;
		}
		t_tp=ptr->node.type;
		if(!ptr->prev && t_tp==type_token_operator)
			t_tp=type_token_id;
		switch(t_tp){
		case type_token_id:
		case type_token_operator:
			type=get_keyword_type(ptr->node.value);
			switch(type){
			case comm_id_type:
				if(!get_press_seq(prog,curr_b,ptr,error,bsing_press)){
					ret_v=FALSE;
					goto to_exit;
				}
				break;
			case condition_if:
				if(!get_sentence_if(prog,curr_b,ptr,error)){
					ret_v=FALSE;
					goto to_exit;
				}
				break;
			case while_loop:
				if(!get_sentence_while(prog,curr_b,ptr,error)){
					ret_v=FALSE;
					goto to_exit;
				}
				break;
			case for_loop:
				if(!get_sentence_for(prog,curr_b,ptr,error)){
					ret_v=FALSE;
					goto to_exit;
				}
				break;
			case for_each_loop:
				if(!get_sentence_for_each(prog,curr_b,ptr,error)){
					ret_v=FALSE;
					goto to_exit;
				}
				break;
			case return_oper:
				if(bset_new)
					ptr_pn=curr_b.node.new_next(TRUE);//�õ���ǰ�ڵ�
				ptr=ptr->next;
				ptr_pn->type=type_sentence_return;
				ptr_pn->p_return=new tag_sentence_return();
				if(((ptr_pn->p_return->ret_press=get_press(prog,curr_b,ptr,NULL,error,berror,TRUE)),berror)){
					delete ptr_pn->p_return;
					ptr_pn->p_return=NULL;
					ret_v=FALSE;
					goto to_exit;
				}
				break;
			case number_type_def:
			case string_type_def:
			case list_type_def:
				if(is_user_define_function(ptr,r_type)){
					if(ptr->prev)
						ptr=ptr->prev;
					pchain=ptr;
					goto to_exit;
				}else{
					if(!parse_var_define(prog,curr_b,ptr,type,error)){
						ret_v=FALSE;
						goto to_exit;
					}
					bset_new=FALSE;
				}
				break;
			case switch_case:
				break;
			case case_case:
				break;
			case break_loop:
				if(bset_new)
					ptr_pn=curr_b.node.new_next(TRUE);//�õ���ǰ�ڵ�
				//ptr=ptr->next;
				ptr_pn->type=type_sentence_break;
				ptr=ptr->next;
				break;
			case continue_loop:
				if(bset_new)
					ptr_pn=curr_b.node.new_next(TRUE);//�õ���ǰ�ڵ�
				//ptr=ptr->next;
				ptr_pn->type=type_sentence_continue;
				ptr=ptr->next;
				break;
			case condition_else:
				pchain=ptr;
				goto to_exit;
			default:
				break;
			}
			break;
		default:
			if(ptr)ptr=ptr->next;
		}
	}

	pchain=ptr;
to_exit:
	return ret_v;
}
BOOL alanysis_tokens::is_split_flag(tag_token *ptoken){
	ASSERT(ptoken!=NULL);
	return (ptoken->type==type_token_split);
}
BOOL alanysis_tokens::is_begin_shift(tag_token *ptoken)
{
	ASSERT(ptoken!=NULL);
	if(ptoken->type==type_token_operator){
		tag_operator_define* poper=ptoken->poper;
		if(poper){
			return (poper->p_normal?poper->p_normal->get_type()==oper_shift_begin:FALSE);
		}
	}
	return FALSE;
}
BOOL alanysis_tokens::is_begin_index(tag_token *ptoken)
{
	ASSERT(ptoken!=NULL);
	if(ptoken->type==type_token_operator){
		tag_operator_define* poper=ptoken->poper;
		if(poper){
			return (poper->p_normal?poper->p_normal->get_type()==oper_list_index_begin:FALSE);
		}
	}
	return FALSE;
}
BOOL alanysis_tokens::is_dot_flag(tag_token *ptoken)
{
	ASSERT(ptoken!=NULL);
	if(ptoken->type==type_token_operator){
		tag_operator_define* poper=ptoken->poper;
		if(poper){
			return (poper->p_normal?poper->p_normal->get_type()==oper_dot_f:FALSE);
		}
	}
	return FALSE;
}
BOOL alanysis_tokens::is_end_shift(tag_token *ptoken)
{
	ASSERT(ptoken!=NULL);
	if(ptoken->type==type_token_operator){
		tag_operator_define* poper=ptoken->poper;
		if(poper){
			return (poper->p_normal?poper->p_normal->get_type()==oper_shift_end:FALSE);
		}
	}
	return FALSE;
}
BOOL alanysis_tokens::is_end_index(tag_token *ptoken)
{
	ASSERT(ptoken!=NULL);
	if(ptoken->type==type_token_operator){
		tag_operator_define* poper=ptoken->poper;
		if(poper){
			return (poper->p_normal?poper->p_normal->get_type()==oper_list_index_end:FALSE);
		}
	}
	return FALSE;
}
BOOL alanysis_tokens::is_begin_block(tag_token *ptoken)
{
	ASSERT(ptoken!=NULL);
	if(ptoken->type==type_token_pair){
		tag_pair_string* pair=m_ptrOuter->get_pair(ptoken->index);
		if(pair){
			return pair->type==program_block_type && pair->begin==ptoken->value;
		}
	}
	return FALSE;
}
BOOL alanysis_tokens::is_end_block(tag_token *ptoken)
{
	ASSERT(ptoken!=NULL);
	if(ptoken->type==type_token_pair){
		tag_pair_string* pair=m_ptrOuter->get_pair(ptoken->index);
		if(pair){
			return pair->type==program_block_type && pair->end==ptoken->value;
		}
	}
	return FALSE;
}
BOOL alanysis_tokens::is_block_split(tag_token *ptoken)
{
	ASSERT(ptoken!=NULL);
	if(ptoken->type==type_token_pair){
		tag_pair_string* pair=m_ptrOuter->get_pair(ptoken->index);
		if(pair){
			return pair->type==program_block_type;
		}
	}
	return FALSE;
}

//�ж��Ƿ�Ϊ��������

BOOL alanysis_tokens::is_user_define_function(tag_token_chain *pchain,_tag_key_word_type& type)
{
	BOOL ret=FALSE;
	type=get_keyword_type(pchain->node.value);
	if(type==string_type_def || type==number_type_def || type==list_type_def){
		pchain=pchain->next;
		if(pchain && pchain->node.type==type_token_id){
			pchain=pchain->next;
			if(pchain && is_begin_shift(&pchain->node)){
				ret=TRUE;
			}
		}
	}
	return ret;
}

tag_user_function* alanysis_tokens::get_user_function(tag_program& prog,tag_block &curr_b,tag_user_function*& pfunc,tag_result_type r_type
													  , tag_token_chain *&pchain,string& error)
{
	BOOL bok=TRUE,b_has_param=FALSE;
	INT level=INIT_LEVEL;
	tag_user_function* pret=pfunc;
	tag_func_param_define_chain* pparam=NULL;
	//������������
	if(r_type==type_result_string)
		pret->ret_value.set_string(NULL);
	else if(r_type==type_result_list)
		pret->ret_value.set_list(NULL);
	else
		pret->ret_value.set_number(0);
	//������������
	ASSERT(is_begin_shift(&pchain->node));
	pchain=pchain->next;
	_tag_key_word_type tp;

	pret->p_node=curr_b.new_child();
	while(TRUE){
		if(is_end_shift(&pchain->node)){
			pchain=pchain->next;
			break;
		}
		b_has_param=TRUE;
		tp=get_keyword_type(pchain->node.value);
		if(pparam==NULL){
			pparam=&pret->m_param;
		}else{
			pparam=pparam->new_next();
		}
		if(tp==string_type_def){
			pparam->type=type_result_string;
		}else if(tp==number_type_def){
			pparam->type=type_result_number;
		}else{
			ASSERT(FALSE);
			bok=FALSE;
			break;
		}
		if(!(pparam->index=parse_var_define(prog,*(pret->p_node),pchain,tp,error))){
			bok=FALSE;
			goto to_exit;
		}
		pparam->param_name=pchain->prev->node.value;
		if(is_dot_flag(&pchain->node)){
			pchain=pchain->next;
		}
	}
	ASSERT(is_begin_block(&pchain->node));
	pchain=pchain->next;
	if(!get_block(prog,*(pret->p_node),pchain,++level,error)){
		bok=FALSE;
		goto to_exit;
	}
	if(is_split_flag(&pchain->node)){
		pchain=pchain->next;
	}
to_exit:
	//ASSERT(is_end_block(&pchain->node));
	if(!bok && pret){
		delete pret;
		pret=NULL;
	}
	return pret;
}
//������������������߷������ؼ��ֺ��˳�
BOOL alanysis_tokens::get_press_seq(tag_program& prog,tag_block &curr_b/*��ǰ��*/
											  , tag_token_chain *&pchain,string& error,BOOL bseq)
{
	if(!pchain)return TRUE;
	BOOL ret=TRUE,bnew=FALSE,berror;
	tag_press_seq* p_seq=NULL;
	INT level=INIT_LEVEL;
	//�õ���ǰ�ڵ�
	tag_block_node* ptr_pn=curr_b.node.new_next(TRUE);
	ptr_pn->type=type_sentence_press_seq;//���������������
	_tag_key_word_type type;
	do{
		level=INIT_LEVEL;
		//��ʼһ���µ�����
		type=get_keyword_type(pchain->node.value);
		if(comm_id_type!=type){
			//�ǹؼ���
			//����һ���ָ�����
			//pchain=pchain->prev;
			break;
		}else{
			//�ǹؼ��ֿ�ͷ
			if(is_end_block(&(pchain->node))){
				//ʶ�������
				break;
			}else if(is_begin_block(&(pchain->node))){
				//��ÿ�ʼһ���µĿ飨�����飩
				pchain=pchain->next;
				ptr_pn=curr_b.node.new_next(TRUE);
				ptr_pn->type=type_noname_block;
				ptr_pn->p_block=curr_b.new_child();
				//�õ��¿�
				if(!get_block(prog,*(ptr_pn->p_block),pchain,++level,error)){
					return FALSE;
				}
				bnew=TRUE;
			}else{
				//��һ���ַ���ȥ���ָ���
				if(bnew){
					ptr_pn=ptr_pn->new_next(TRUE);
					ptr_pn->type=type_sentence_press_seq;//���������������
					bnew=FALSE;
					p_seq=NULL;
				}
				if(!p_seq){
					p_seq=new tag_press_seq();
					ptr_pn->p_press=p_seq;
				}else{
					p_seq=p_seq->new_next();
				}
				if(((p_seq->press=get_press(prog,curr_b,pchain,NULL,error,berror,FALSE)),berror)){
					return FALSE;
				}
			}
		}
		if(pchain && bseq)
			pchain=pchain->next;
	}while(bseq && pchain);
	return ret;
}

BOOL alanysis_tokens::get_sentence_if(tag_program& prog,tag_block &curr_b, tag_token_chain *&pchain,string& error)
{
	//��if��䣬����һ���ڵ�
	BOOL bret=TRUE,bblock=FALSE,brec=FALSE,berror;
	pchain=pchain->next;//���Թؼ���
	tag_sentence_if* p_if=new tag_sentence_if();
	INT level=INIT_LEVEL;
	//�½�һ�����ʽ��
	tag_block_node* ptr_pn=curr_b.node.new_next(TRUE);
	ptr_pn->type=type_sentence_if;
	ptr_pn->p_if=p_if;
	//�õ��������ʽ
	p_if->p_block=curr_b.new_child();
	//���ʽ���������
	if(!is_begin_shift(&(pchain->node))){
		IF_ERROR_MSG_EXIT("if ���ȱ������������!");
	}
	pchain=pchain->next;//�Ե���ǰ�������
	if(!pchain){
		IF_ERROR_MSG_EXIT("if ���ȱ�ٱ��ʽ!");
	}
	if(((p_if->p_cond=get_press(prog,curr_b,pchain,NULL,error,berror,TRUE)),berror)){
		IF_ERROR_CLEAR();
		return FALSE;
	}
	//����Ǻ�����
	if(!is_end_shift(&(pchain->node))){
		IF_ERROR_MSG_EXIT("if �������Ų�ƥ�䣡");
	}
	//�ӹ���������
	pchain=pchain->next;
	if(is_begin_block(&(pchain->node))){//��һ������飬�ӹ�{
		pchain=pchain->next;
		level++;
	}
	if(is_end_shift(&(pchain->node))){
		IF_ERROR_MSG_EXIT("if �������Ų�ƥ�䣡");
	}
	if(!get_block(prog,*(p_if->p_block),pchain,level,error)){//�õ�if����Ϊ��ʱ�Ĳ�����
		IF_ERROR_CLEAR();
	}else if(pchain){
		if(is_split_flag(&(pchain->node)) ||is_end_shift(&(pchain->node))
			|| is_end_block(&(pchain->node))){
			pchain=pchain->next;
			brec=TRUE;
		}
		if(condition_else==get_keyword_type(pchain->node.value)){
			//��else��
			pchain=pchain->next;
			p_if->p_else_block=curr_b.new_child();
			level=INIT_LEVEL;
			if(is_begin_block(&pchain->node)){
				
				pchain=pchain->next;
				level++;
			}
			if(!get_block(prog,*(p_if->p_else_block),pchain,level,error)){
				IF_ERROR_CLEAR();
			}
		}else if(brec){
			if(pchain)
				pchain=pchain->prev;
		}
	}
	if(p_if->p_block->is_empty()){
		delete p_if->p_block;
		p_if->p_block=NULL;
	}
	if(p_if->p_else_block && p_if->p_else_block->is_empty()){
		delete p_if->p_else_block;
		p_if->p_else_block=NULL;
	}
	return bret;
}
#define FOR_ERROR_MSG_EXIT(msg) \
	if(p_for)delete p_for,ptr_pn->p_for=NULL;\
	berror=TRUE,error=msg;\
	return FALSE
#define FOR_ERROR_CLEAR() \
	if(p_for)delete p_for;\
		ptr_pn->p_for=NULL;\
		bret=FALSE
#define FOR_EACH_ERROR_MSG_EXIT(msg) \
	if(p_for_each)delete p_for_each,ptr_pn->p_for_each=NULL;\
	berror=TRUE,error=msg;\
	return FALSE
#define FOR_EACH_ERROR_CLEAR() \
	if(p_for_each)delete p_for_each;\
		ptr_pn->p_for_each=NULL;\
		bret=FALSE

BOOL alanysis_tokens::get_sentence_for_each(tag_program& prog,tag_block &curr_b, tag_token_chain *&pchain,string& error){
	BOOL bret=TRUE,bblock=FALSE,berror;
	pchain=pchain->next;//���Թؼ���
	INT level=INIT_LEVEL;
	tag_sentence_for_each* p_for_each=new tag_sentence_for_each();
	//�½�һ�����ʽ��
	tag_block_node* ptr_pn=curr_b.node.new_next(TRUE);
	ptr_pn->type=type_sentence_for_each;
	ptr_pn->p_for_each=p_for_each;

	//�õ����ʽ
	//���ʽ���������
	ASSERT(is_begin_shift(&(pchain->node)));
	pchain=pchain->next;//�Ե���ǰ�������
	ASSERT(pchain);
	if(0>=(p_for_each->var_index=curr_b.get_location_index(pchain->node.value))){
		FOR_EACH_ERROR_MSG_EXIT("for_each ����δ����\",\"");
	}
	pchain=pchain->next;//�Ե�
	//����Ϊ����
	if(!is_dot_flag(&(pchain->node))){
		FOR_EACH_ERROR_MSG_EXIT("for_each ���ȱ�������ķָ���\",\"");
	}
	pchain=pchain->next;//�Ե�
	if(0>=(p_for_each->list_index=curr_b.get_location_index(pchain->node.value))){
		FOR_EACH_ERROR_MSG_EXIT("for_each ����δ����\",\"");
	}
	pchain=pchain->next;//�Ե�
	//����Ǻ�����
	if(is_begin_index(&pchain->node)){
		pchain=pchain->next;//�Ե�
		//��������ʱ����
		BOOL berror;
		tag_token token_end;
		token_end.type=type_token_operator;
		token_end.poper=&g_oper[g_idx_list_index_end];
		p_for_each->p_index_press=get_press(prog,curr_b,pchain,&token_end,error,berror,TRUE);
	}
	if(!is_end_shift(&(pchain->node))){
		FOR_EACH_ERROR_MSG_EXIT("for_each ���ȱ�ٺ�����");
	}
	//�ӹ���������
	pchain=pchain->next;
	
	if(is_end_shift(&(pchain->node))){
		FOR_EACH_ERROR_MSG_EXIT("for_each ������Ų�ƥ��");
	}
	//�ӹ���������
	if(!is_split_flag(&(pchain->node))){
		//ȷ����ѭ����
		p_for_each->p_block=curr_b.new_child();
		if(is_begin_block(&(pchain->node))){
			pchain=pchain->next;//�Ե�
			level++;
		}
		if(!get_block(prog,*(p_for_each->p_block),pchain,level,error)){//�õ�������
			FOR_EACH_ERROR_CLEAR();
			bret=FALSE;
		}
		//ASSERT(bblock?is_end_block(&(pchain->node)):TRUE);
	}
	return bret;
}
BOOL alanysis_tokens::get_sentence_for(tag_program& prog,tag_block &curr_b, tag_token_chain *&pchain,string& error){
	BOOL bret=TRUE,bblock=FALSE,berror;
	pchain=pchain->next;//���Թؼ���
	INT level=INIT_LEVEL;
	tag_sentence_for* p_for=new tag_sentence_for();
	//�½�һ�����ʽ��
	tag_block_node* ptr_pn=curr_b.node.new_next(TRUE);
	ptr_pn->type=type_sentence_for;
	ptr_pn->p_for=p_for;
	//�õ��������ʽ
	//���ʽ���������
	ASSERT(is_begin_shift(&(pchain->node)));
	pchain=pchain->next;//�Ե���ǰ�������
	ASSERT(pchain);
	if(!is_split_flag(&(pchain->node)))
		if(((p_for->p_init=get_press(prog,curr_b,pchain,NULL,error,berror,TRUE)),berror)){
			FOR_ERROR_CLEAR();
			return FALSE;
		}
	//����Ϊ�ָ�����
	if(!is_split_flag(&(pchain->node))){
		FOR_ERROR_MSG_EXIT("for ���ȱ�������ķָ���\";\"");
	}
	pchain=pchain->next;//�Ե�
	if(!is_split_flag(&(pchain->node)))
		if(((p_for->p_cond=get_press(prog,curr_b,pchain,NULL,error,berror,TRUE)),berror)){
			FOR_ERROR_CLEAR();
			return FALSE;
		}
	if(!is_split_flag(&(pchain->node))){
		FOR_ERROR_MSG_EXIT("for ���ȱ�������ķָ���\";\"");
	}
	pchain=pchain->next;//�Ե�
	if(!is_end_shift(&(pchain->node)))
		if(((p_for->p_dosth=get_press(prog,curr_b,pchain,NULL,error,berror,TRUE)),berror)){
			FOR_ERROR_CLEAR();
			return FALSE;
		}
	//����Ǻ�����
	if(!is_end_shift(&(pchain->node))){
		FOR_ERROR_MSG_EXIT("for ���ȱ�ٺ�����");
	}
	//�ӹ���������
	pchain=pchain->next;
	if(is_end_shift(&(pchain->node))){
		FOR_ERROR_MSG_EXIT("for ������Ų�ƥ��");
	}
	//�ӹ���������
	if(!is_split_flag(&(pchain->node))){
		//ȷ����ѭ����
		p_for->p_block=curr_b.new_child();
		if(is_begin_block(&(pchain->node))){
			pchain=pchain->next;//�Ե�
			level++;
		}
		if(!get_block(prog,*(p_for->p_block),pchain,level,error)){//�õ�������
			FOR_ERROR_CLEAR();
			bret=FALSE;
		}
		//ASSERT(bblock?is_end_block(&(pchain->node)):TRUE);
	}
	return bret;
}
#define WHILE_ERROR_CLEAR() \
	if(p_while)delete p_while;\
		ptr_pn->p_while=NULL;\
		bret=FALSE
#define WHILE_ERROR_MSG_EXIT(msg) \
	if(p_while)delete p_while,ptr_pn->p_while=NULL;\
	berror=TRUE,error=msg;\
	return FALSE
BOOL alanysis_tokens::get_sentence_while(tag_program& prog,tag_block &curr_b, tag_token_chain *&pchain,string& error){
	BOOL bret=TRUE,bblock=FALSE,berror;
	pchain=pchain->next;//���Թؼ���
	INT level=INIT_LEVEL;
	tag_sentence_while* p_while=new tag_sentence_while();
	//�½�һ�����ʽ��
	tag_block_node* ptr_pn=curr_b.node.new_next(TRUE);
	ptr_pn->type=type_sentence_while;
	ptr_pn->p_while=p_while;
	//�õ��������ʽ
	//���ʽ���������
	if(!is_begin_shift(&(pchain->node))){
		WHILE_ERROR_MSG_EXIT("while ���ȱ��ǰ����!");
	}
	pchain=pchain->next;//�Ե���ǰ�������
	if(!pchain){
		WHILE_ERROR_MSG_EXIT("while ���ȱ�ٱ��ʽ!");
	}
	if(((p_while->p_cond=get_press(prog,curr_b,pchain,NULL,error,berror,TRUE)),berror)){
		WHILE_ERROR_CLEAR();
		return FALSE;
	}
	if(!is_end_shift(&(pchain->node))){
		WHILE_ERROR_MSG_EXIT("while ���ȱ�ٺ�����!");
	}
	//�ӹ���������
	pchain=pchain->next;
	if(is_end_shift(&(pchain->node))){
		WHILE_ERROR_MSG_EXIT("while �������Ų�ƥ��!");
	}
	if(!is_split_flag(&(pchain->node))){
		//ȷ����ѭ����
		p_while->p_block=curr_b.new_child();
		if(is_begin_block(&(pchain->node))){
			pchain=pchain->next;//�Ե�
			level++;
		}
		
		if(!get_block(prog,*(p_while->p_block),pchain,level,error)){//�õ�������
			WHILE_ERROR_CLEAR();
			bret=FALSE;
		}
		//ASSERT(bblock?is_end_block(&(pchain->node)):TRUE);
	}
	return bret;
}