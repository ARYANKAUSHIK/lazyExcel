///////////////////////////////////////////////////////////////////////////////////////
//@module		:	���ʽ������(ʵ��)
//@author		:	zhiyong.luo
//@create date	:	2006-12-04
//@last_modify	:	
///////////////////////////////////////////////////////////////////////////////////////

#include "press_computer.h"

template<class T>
LStack<T>::LStack():m_pmem(0),m_pcurr(0),m_esp(0){}
template<class T>
LStack<T>::~LStack()
{
	if(m_pmem)
		FreeMemList(m_pmem);
}

template<class T>
pmem_block_list LStack<T>::ExtentListSize()
{
	LONG size=sizeof(mem_block_list)+sizeof(info_item_list<T>)*BLOCK_SIZE;
	CHAR* ptr=(CHAR*)malloc(sizeof(CHAR)*size);
	memset(ptr,0x00,size);
#ifndef _DEBUG
	return new(ptr) mem_block_list();
#else
	return (mem_block_list*)ptr;
#endif
}
template<class T>
void LStack<T>::FreeMemList(pmem_block_list pList)
{
	if(pList){
		if(pList->pNext)
			delete ((CHAR*)pList->pNext);
		free(pList);
	}
}
template<class T>
BOOL LStack<T>::get_value(T& val,BOOL bpopup)
{
	BOOL ret=FALSE;
	LONG oldp=m_esp;
	pmem_block_list ptr;
	pmem_block_list pOld=m_pcurr;
	if(m_pmem){
		m_esp--;
		if(IS_HEAD(m_esp)){
			if(m_pcurr==m_pmem){
				m_esp=0;
				goto _to_exit;
			}else{
				for(ptr=m_pmem;ptr && ptr->pNext!=m_pcurr;ptr=ptr->pNext);
				if(ptr){
					if(bpopup){
						FreeMemList(m_pcurr);
						ptr->pNext=0;
					}
					m_pcurr=ptr;
					m_esp=BLOCK_SIZE-1;
				}else
					goto _to_exit;
			}
		}
		val=((info_item_list<T>*)DATA_PTR(m_pcurr,m_esp))->item;
		ret=TRUE;
	}
_to_exit:
	if(!bpopup){
		m_esp=oldp;
		m_pcurr=pOld;
	}
	return ret;
}
#ifdef _DEBUG
template<class T>
void LStack<T>::print_stack(pfun pprint){
	if(pprint){
		pmem_block_list ptr;
		T val;
		for(ptr=m_pmem;ptr && ptr;ptr=ptr->pNext){
			if(ptr==m_pcurr){
				for(INT pt=0;pt<m_esp;pt++){
					val=((info_item_list<T>*)DATA_PTR(ptr,pt))->item;
					pprint(&val);
				}
				//��ǰ��
			}else{
				//�ǵ�ǰ��
				for(INT pt=0;pt<BLOCK_SIZE;pt++){
					val=((info_item_list<T>*)DATA_PTR(ptr,pt))->item;
					pprint(&val);
				}
			}
		}
		printf("\n");
	}
}
#endif
template<class T>
BOOL LStack<T>::value(T& val)
{
	return get_value(val,0);
}
template<class T>
BOOL LStack<T>::pop(T& val)
{
	return get_value(val,1);
}
template<class T>
T* LStack<T>::replace_top(T val){
	T* pret=NULL;
	info_item_list<T>* ptr=get_top();
	if(ptr){
		pret=&(ptr->item);
		ptr->item=val;
	}
	return pret;
}
template<class T>
info_item_list<T>* LStack<T>::get_top()
{
	LONG top=m_esp-1;
	pmem_block_list ptr;
	if(m_pmem){
		if(IS_HEAD(top)){
			if(m_pcurr==m_pmem){
				top=0;
				goto _to_exit;
			}else{
				for(ptr=m_pmem;ptr && ptr->pNext!=m_pcurr;ptr=ptr->pNext);
				if(ptr){
					top=BLOCK_SIZE-1;
				}else
					goto _to_exit;
			}
		}else{
			ptr=m_pcurr;
		}
		return ((info_item_list<T>*)DATA_PTR(ptr,top));
	}
_to_exit:
	return NULL;
}
template<class T>
VOID LStack<T>::push(T val)
{
	if(!m_pmem){
		m_pmem=ExtentListSize();
		m_pcurr=m_pmem;
		m_esp=0;
	}
	if(IS_TAIL(m_esp)){
		//������β
		m_pcurr->pNext=ExtentListSize();
		m_pcurr=m_pcurr->pNext;
		m_esp=0;
	}
	((info_item_list<T>*)DATA_PTR(m_pcurr,m_esp))->item=val;
	m_esp++;
}
#define ERROR_RETURN() \
	ret_v=FALSE;\
	goto to_exit
#define ERROR_COMPUTE(msg) \
	error=string(msg);\
	ERROR_RETURN()
#define SET_STRING_VAR(var) \
	pret=var_pool.GetValue(++var_index,TRUE);\
	pret->set_string(var);\
	v_stack.push(pret)
#define SET_NUMBER_VAR(var) \
	pret=var_pool.GetValue(++var_index,TRUE);\
	pret->set_number(var);\
	v_stack.push(pret)
#define SAVE_CURR_OPER() \
	f_stack.push(poper);\
	pflag=poper

#ifdef LZY_COUNT
	INT press_computer::computer_count=0;//�������
	INT press_computer::get_var_count=0;//ȡ��������
	INT press_computer::run_block_count=0;//���г�������
	INT press_computer::run_func_count=0;//�������д���
#endif
//ɾ��һ���ڵ�
#define REMOVE_PRESS(p) \
	if(p->prev)p->prev->next=p->next;\
	if(p->next)p->next->prev=p->prev;\
	p->next=NULL,p->prev=NULL;\
	delete p
#define GET_VALUE(p,r_v) \
	(p->node.type==type_string?(r_v.set_string(p->node.str_value),TRUE):\
		p->node.type==type_number?(r_v.set_number(p->node.dbl_value),TRUE):FALSE)
#define SET_VALUE(p,r_v) \
	p->node.clear();\
	(r_v.is_string_type()?(p->node.str_value=(r_v.get_string()\
	?new string(r_v.get_string()->c_str()):NULL),p->node.type=type_string):\
		(p->node.dbl_value=r_v.get_number(),p->node.type=type_number))

#define PUSH_FLAG() \
	SAVE_CURR_OPER();\
	f_p_stack.push(pflag_press_curr);\
	pflag_press=pflag_press_curr

#define POP_FLAG() \
	(f_stack.pop(pflag)?(f_p_stack.pop(pflag_press)?TRUE:(pflag_press=NULL,TRUE)):FALSE)
#define GET_FLAG() \
	(f_stack.value(pflag)?(f_p_stack.value(pflag_press)?TRUE:(pflag_press=NULL,TRUE)):FALSE)

//���ʽԤ�������ڴ�������ʱ����Ҫ�����㣩�磺var=(1024/512+8)/2*var1;--->var=var1*5;
BOOL press_computer::pre_computer_press(tag_press &press,string& error)
{
	BOOL ret_v=TRUE,blast=FALSE,brevove=FALSE;
	register LStack<tag_operator*>	f_stack;//������ջ
	register LStack<tag_press*>		f_p_stack;//������ͬ����ջ
	register LStack<tag_press*>		v_stack;//���ʽ�ڵ�ջ
	tag_computer_result lft_v,rt_v;//��ʱ�����
	tag_press*	pvalue=NULL,*pprev=NULL,*pflag_press=NULL,*pflag_press_curr=NULL;
	register tag_operator* pflag=NULL,*poper;
	//ѹ���ջһ����
#ifdef LZY_INVOID_ERROR
	try{
#endif
	f_stack.push(&g_oper_max);
	for(const tag_press* ptr=&press;ptr || !blast;(ptr?ptr=ptr->next:ptr)){
to_cont:
		blast=(ptr==NULL);
		switch((blast?type_operator:ptr->node.type)){
		case type_operator:
			{
				pflag_press_curr=(tag_press*)ptr;
				pflag_press=pflag_press_curr;
				poper=(blast?(&g_oper_min):ptr->node.p_oper);
				ASSERT(poper!=NULL);
				if(NULL==pflag){
					PUSH_FLAG();
					break;
				}
				
				//�ǲ�����
				if(*poper>*pflag || 
					(*poper==*pflag && poper->is_single_oper())){
					//���ǵ�Ŀ���������ջ
					//if(poper->get_type()==oper_list_index_end)break;
					PUSH_FLAG();
				}else{
					while(*poper<=*pflag){
						//��ǰ���������ȼ���ͣ��������ݶ�ջ��
						if(!pflag->is_computer_oper()){
							if(!poper->is_computer_oper()){
								if(poper->get_type()==oper_shift_begin){
									PUSH_FLAG();
								}else{
									if(poper->get_type()==oper_dot_f){
										//�Ƕ��Ų�����
										if(!v_stack.pop(pprev)){
											pprev=NULL;
										}//���������������ԣ�
									}else{
										//�Ǽ������
										if(pflag==(&g_oper_max) && poper!=(&g_oper_min)){
											ERROR_COMPUTE("���ʽ����,���Ų�ƥ��!");
										}
										brevove=(pflag_press && pflag_press->next && pflag_press->next->next 
											&& pflag_press->next->next->node.p_oper==poper);
										POP_FLAG();
										if(brevove){
											pvalue=pflag_press->next;
											ptr=ptr->next;
											REMOVE_PRESS(pflag_press_curr);
											if(pflag_press==&press){
												if(!pprev){
													ERROR_COMPUTE("���ʽ����,���������Ų�ƥ��!");
												}
												if(GET_VALUE(pvalue,rt_v)){
													SET_VALUE(pflag_press,rt_v);
												}else{
													pflag_press->node.clone_and_clear_node(pvalue->node);
												}
												REMOVE_PRESS(pvalue);
												if(pprev==pvalue){
													v_stack.replace_top(pflag_press);
												}
											}else{
												REMOVE_PRESS(pflag_press);
											}
										}
										if(!GET_FLAG()){
											if(!blast && poper->get_type()==oper_shift_end){
												//����
												ERROR_COMPUTE("���ʽ����,����ջ���Ѿ�û�з�����,���Ų�ƥ��!");
											}
											pflag=NULL;
										}
										if(brevove)
											goto to_cont;
									}
								}
							}else{
								PUSH_FLAG();
							}
							break;
						}else if(pflag->is_single_oper()){
							//�ǵ�Ŀ�����
							POP_FLAG();
							if(!v_stack.pop(pprev)){
								ERROR_COMPUTE("���ʽ����,��Ŀ�������ʧ������!");
							}
							if(GET_VALUE(pprev,rt_v)){
								//һ���ǷǱ���
								if(!rt_v.single_calculate(*pflag,error)){
									//����ʧ��
									ERROR_RETURN();
								}else{
									SET_VALUE(pprev,rt_v);
									//����ɹ�(ɾ��һ���������ڵ�)
									REMOVE_PRESS(pflag_press);
								}
							}//else Ϊ����(���������ջ�������κβ���)
							v_stack.push(pprev);
							if(!GET_FLAG()){
								//����ջ���Ѿ�û�з�����
								ERROR_COMPUTE("���ʽ����,����ջ���Ѿ�û�з�����!");
							}
						}else{
							if(pflag->get_type()==oper_list_index_begin){
								if(poper->get_type()!=oper_list_index_end){
									PUSH_FLAG();
								}else{
									POP_FLAG();
									v_stack.pop(pprev);//���������
									pflag=NULL;
								}
								break;
							}else if(pflag->get_type()==oper_list_index_end){
								POP_FLAG();
								pflag=NULL;
								break;
							}
							//һ��˫Ŀ�����
							POP_FLAG();
							if(!v_stack.pop(pvalue)){
								ERROR_COMPUTE("���ʽ����,˫Ŀ�������ʧ�ڶ���������!");
							}
							if(!v_stack.pop(pprev)){
								ERROR_COMPUTE("���ʽ����,˫Ŀ�������ʧ��һ��������!");
							}
							if(GET_VALUE(pvalue,rt_v)){
								if(GET_VALUE(pprev,lft_v)){
									//һ���ǷǱ���
									if(!lft_v.calculate(&rt_v,*pflag,error)){
										//����ʧ��
										ERROR_RETURN();
									}else{
										SET_VALUE(pprev,lft_v);
										//����ɹ�(ɾ�������ڵ�)
										REMOVE_PRESS(pflag_press);
										REMOVE_PRESS(pvalue);
									}
								}
								v_stack.push(pprev);
							}else{
								v_stack.push(pvalue);
							}
							//���������
							if(!GET_FLAG()){
								//����ջ���Ѿ�û�з�����
								ERROR_COMPUTE("���ʽ����,����ջ���Ѿ�û�з�����!");
							}
						}
					}
					//�Ƿ���Ҫѹջ
					ASSERT(poper!=NULL);
					if(pflag && (*poper)>*pflag){
						PUSH_FLAG();
					}
				}
			}
			break;
		default:
			v_stack.push((tag_press*)ptr);
			break;
		}
	}
#ifdef LZY_INVOID_ERROR
	}catch(...){
		ERROR_COMPUTE("����ʱ��������޷���ɼ���");
	}
#endif	
to_exit:
	return ret_v;
}
//����ʱ����
BOOL press_computer::computer(tag_runtime_context* pcontext,const tag_press &press
							  , tag_computer_result &ret,string& error,LPVOID pparam)
{
#ifdef LZY_COUNT
	computer_count++;//�������
#endif
	BOOL ret_v=TRUE,blast=FALSE,bIndex,bis_refer;
	register LStack<tag_operator*>	f_stack;//������ջ
	register INT var_index=0;
	register CLMapBase<INT,tag_computer_result>	 var_pool;//���ʽ���ر���ջ(�ṩ�������)
	register LStack<tag_computer_result*>	v_stack;//���ʽ�ڵ�ջ
	tag_computer_result*	pret=NULL,*pvalue=NULL,*pprev=NULL;
	register tag_operator* pflag=NULL,*poper;
	//ѹ���ջһ����
#ifdef LZY_INVOID_ERROR
	try{
#endif
	f_stack.push(&g_oper_max);
	for(const tag_press* ptr=&press;ptr || !blast;(ptr?ptr=ptr->next:ptr)){
		blast=(ptr==NULL);
		switch((blast?type_operator:ptr->node.type)){
		case type_number:
			SET_NUMBER_VAR(ptr->node.dbl_value);
			break;
		case type_operator:
			{
				poper=(blast?(&g_oper_min):ptr->node.p_oper);
				ASSERT(poper!=NULL);
				if(NULL==pflag){
					SAVE_CURR_OPER();
					break;
				}
				//�ǲ�����
				if(*poper>*pflag || 
					(*poper==*pflag && poper->is_single_oper())){
					//���ǵ�Ŀ���������ջ
					SAVE_CURR_OPER();
				}else{
					while(*poper<=*pflag){
						//��ǰ���������ȼ���ͣ��������ݶ�ջ��
						if(!pflag->is_computer_oper()){
							//�Ǽ������
							if(!poper->is_computer_oper()){
								//�ж������߼��Ķ�·�㷨
								if(pflag->get_type()==oper_list_index_end){
									f_stack.pop(pflag);
								}else if(poper->get_type()==oper_dot_f ){
									//�Ƕ��Ų�����
									if(!v_stack.pop(pprev)){
										pprev=NULL;
									}//���������������ԣ�
								}else{
									if(blast && pflag!=(&g_oper_max)){
										ERROR_COMPUTE("���ʽ����,���Ų�ƥ��!");
									}
									f_stack.pop(pflag);
									if(!f_stack.value(pflag)){
										if(!blast && poper->get_type()==oper_shift_end){
											//����
											ERROR_COMPUTE("���ʽ����,����ջ���Ѿ�û�з�����,���Ų�ƥ��!");
										}
										pflag=NULL;
									}
								}
							}else{
								SAVE_CURR_OPER();
							}
							break;
						}else if(pflag->is_single_oper()){
							//�ǵ�Ŀ�����
							f_stack.pop(pflag);
							if(!v_stack.pop(pvalue)){
								ERROR_COMPUTE("���ʽ����,��Ŀ�������ʧ������!");
							}
							if(!pvalue->single_calculate(*pflag,error)){
								//����ʧ��
								ERROR_RETURN();
							}
							//�ɹ��򽫽����ջ
							v_stack.push(pvalue);
							if(!f_stack.value(pflag)){
								//����ջ���Ѿ�û�з�����
								ERROR_COMPUTE("���ʽ����,����ջ���Ѿ�û�з�����!");
							}
						}else{
							//һ��˫Ŀ�����
							if(pflag->get_type()==oper_list_index_begin){
								if(poper->get_type()!=oper_list_index_end){
									f_stack.pop(poper);
									break;
								}
							}/*else if(pflag->get_type()==oper_list_index_end){
								f_stack.pop(poper);
								pflag=NULL;
								break;
							}*/
							f_stack.pop(pflag);
							if(!v_stack.pop(pvalue)){
								ERROR_COMPUTE("���ʽ����,˫Ŀ�������ʧ�ڶ���������!");
							}
							if(!v_stack.pop(pprev)){
								ERROR_COMPUTE("���ʽ����,˫Ŀ�������ʧ��һ��������!");
							}
							if(bIndex=(pflag->get_type()==oper_list_index_begin)){
								if(!pprev->is_list_type()){
									ERROR_COMPUTE("�±����ֻ������������!");
								}
								//�����±����
								tag_computer_result rst;
								tag_result_value* p_arr;
								vector<tag_result_value>* parr=(vector<tag_result_value>*)pprev->get_refer_list();
								if(!(bis_refer=(NULL!=parr)))parr=(vector<tag_result_value>*)pprev->get_list();
								if(parr){
									if(!pvalue->is_number_type()){
										ERROR_COMPUTE("���ʽ����,�����±����Ϊ��ֵ!");
									}
									int idx=(int)pvalue->get_number();
									if(idx<0 || idx>=parr->size()){
										ERROR_COMPUTE("����ʱ����,�����±�Խ��!");
									}
									p_arr=&(parr->at(idx));
									if(bis_refer)pprev->set_refer_object((tag_computer_result*)p_arr);
									else{
										if(p_arr->is_list_type()){
											rst.set_list((vector<tag_result_value>*)p_arr->get_list());
										}else{
											rst.set_string(p_arr->get_string());
										}
										*pprev=rst;
									}
								}
							}else if(!pprev->calculate(pvalue,*pflag,error)){
								//����ʧ��
								ERROR_RETURN();
							}
							//���������
							v_stack.push(pprev);
							if(!f_stack.value(pflag)){
								//����ջ���Ѿ�û�з�����
								ERROR_COMPUTE("���ʽ����,����ջ���Ѿ�û�з�����!");
							}
							if(bIndex){
								//pflag=NULL;
								break;
							}
						}
					}
					//�Ƿ���Ҫѹջ
					ASSERT(poper!=NULL);
					if(pflag && (*poper)>*pflag && poper->get_type()!=oper_list_index_end){
						SAVE_CURR_OPER();
					}
				}
			}
			break;
		case type_string: 
			SET_STRING_VAR(ptr->node.str_value);
			break;
		default:
			pret=var_pool.GetValue(++var_index,TRUE);
			if(computer_node(pcontext,ptr->node,pret,error,pparam)){
				//����������ջ
				v_stack.push(pret);
			}else{
				//������
				if(error.empty() || error==""){
					if(ptr->node.type==type_function){
						error=string("�޷�ʶ��ĺ���\"")
							+ptr->node.ptr_func->name+string("\"!");
					}else{
						error=string("�޷�ʶ��ı�ʶ��\"")
							+string(*(ptr->node.str_value))+string("\"!");
					}
				}
				ret_v=FALSE;
				goto to_exit;
			}
			break;
		}
	}
	//�����ջ
	if(!v_stack.pop(pret)){
		if(v_stack.value(pvalue) || f_stack.value(pflag)){
			ERROR_COMPUTE("���ʽ�����޷���ɼ���");
		}
	}
	ret=(*pret);
#ifdef LZY_INVOID_ERROR
	}catch(...){
		ERROR_COMPUTE("����ʱ��������޷���ɼ���");
	}
#endif
	
to_exit:
	return ret_v;
}
//������ʽ�ڵ�
BOOL press_computer::computer_node(tag_runtime_context* pcontext,const tag_node &node
								   , tag_computer_result* &pret,string& error,LPVOID pparam)
{
	BOOL ret_v=TRUE;
	string* pstr=NULL;
	tag_computer_result* pbak=pret;
#ifdef LZY_INVOID_ERROR
	try{
#endif
		switch(node.type){
		case type_normal:
			//�����Ǳ��������Ǻ�������
#ifdef LZY_COUNT
			get_var_count++;//ȡ��������
#endif
			if(node.var_index>0){
				if(!(pcontext && pcontext->get_recognise_flag(node.var_index,*pret))){
					ret_v=FALSE;
					goto to_exit;
				}
			}else{
				//�����Ǻ궨��
				pstr=node.str_value;
				ASSERT(pstr!=NULL);
				ASSERT(m_ptrOuter!=NULL);
#ifdef LZY_COUNT
				if(!get_count_var(*pstr,pbak)){
#endif
				if(!pcontext->get_macro_define(*pstr,*pbak,error,pparam)){
					//������
					ret_v=FALSE;
					goto to_exit;
				}
#ifdef LZY_COUNT
				}
#endif
			}
			pret=pbak;
			break;
		case type_function:
			{
				ASSERT(m_ptrOuter!=NULL);
				tag_func* pfun=node.ptr_func;
				ASSERT(pfun!=NULL);
				tag_func_param_value_chain hd_param
					,*ptr=NULL;
				BOOL isfirst=TRUE;
				for(tag_param* param=pfun->param;param;param=param->next){
					if(isfirst){
						isfirst=FALSE;
						ptr=&hd_param;
					}else{
						ptr->next=new tag_func_param_value_chain();
						ptr=ptr->next;
					}
					ASSERT(param->press!=NULL);
					if(!computer(pcontext,(*param->press),ptr->param,error,pparam)){
						//������
						ret_v=FALSE;
						goto to_exit;
					}
				}
				ptr=(isfirst?NULL:&hd_param);		
				if(pfun->fun_index>0){
					//�û��Զ��庯��
					tag_user_function* ptr_func=pcontext->get_user_function(pfun->fun_index);
					if(!ptr_func || !run_user_function(pcontext,ptr_func,ptr,*pret,error)){
						//������
						ret_v=FALSE;
						goto to_exit;
					}
				}else{
					//�����ⲿ����
					//ע�������Ҫ����һ���µ�����ʱ����
					if(!pcontext->call_function(pfun->name,ptr,pfun->ret_type,*pret,error,pparam)){
						//������
						ret_v=FALSE;
						goto to_exit;
					}
				}
			}
			break;
		case type_null:
			pret->set_type(type_result_null);
			break;
		default:
			ret_v=FALSE;
			break;
		}
#ifdef LZY_INVOID_ERROR
	}catch(...){
		ret_v=FALSE;
		error="�������г����������!";
	}
#endif
to_exit:
	return ret_v;
}
#ifdef LZY_COUNT
BOOL press_computer::get_count_var(const string& name,tag_computer_result* &pret){
	BOOL ret=TRUE;
	DOUBLE val=0;
	if(name=="COMPUTE_COUNT"){
		val=computer_count;
	}else if(name=="FETCH_VALUE_COUNT"){
		val=get_var_count;
	}else if(name=="CALL_FUNC_COUNT"){
		val=run_func_count;
	}else if(name=="RUN_BLOCK_COUNT"){
		val=run_block_count;
	}else{
		ret=FALSE;
	}
	if(ret){
		pret->set_number(val);
	}
	return ret;
}
#endif