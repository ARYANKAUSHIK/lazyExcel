///////////////////////////////////////////////////////////////////////////////////////////////
//@ģ��		:	������ѧ���ʽ������(ʵ��)
//@����		:	������
//@����ʱ��	:	2005-11-16
///////////////////////////////////////////////////////////////////////////////////////////////

#include "LComputer.h"
#include <new>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
bool IsNumeric(const char* src)
{
	if(!src)return 0;
	bool bPre=1;
	int	dotCount=0;
	char val;
	while(val=*src++){
		if(bPre && val==' ')continue;//�ӹ�ǰ���ո�
		if(bPre){
			bPre=0;
			if(val=='+' || val=='-')continue;
		}
		if((val<'0' || val>'9') 
			&& !(val=='.' && !dotCount++))
			return 0;
	}
	return 1;
}
template<class T>
LStack<T>::LStack():m_pmem(0),m_pcurr(0),m_esp(0)
{

}
template<class T>
LStack<T>::~LStack()
{
	if(m_pmem)
		FreeMemList(m_pmem);
}

template<class T>
pmem_block_list LStack<T>::ExtentListSize()
{
	long size=sizeof(mem_block_list)+sizeof(info_item_list<T>)*BLOCK_SIZE;
	char* ptr=new char[size];
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
			delete ((char*)pList->pNext);
		delete ((char*)pList);
	}
}
template<class T>
bool LStack<T>::get_value(T& val,bool bpopup)
{
	bool ret=0;
	long oldp=m_esp;
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
		ret=1;
	}
_to_exit:
	if(!bpopup){
		m_esp=oldp;
		m_pcurr=pOld;
	}
	return ret;
}

template<class T>
bool LStack<T>::value(T& val)
{
	return get_value(val,0);
}
template<class T>
bool LStack<T>::pop(T& val)
{
	return get_value(val,1);
}

template<class T>
bool LStack<T>::push(T val)
{
	bool bret=0;
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
	bret=1;
	return bret;
}
//////////////////////////////////////////////////////////////////////
// LComputer Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

LComputer::LComputer()
{

}

LComputer::~LComputer()
{

}

DOUBLE LComputer::Calculate(char *ptrPress,BOOL& b_error)
{
	DOUBLE ret=0,dblT=0;
	oper_flag level,next_level;
	b_error=FALSE;
	char val;
	BOOL bSave_value=TRUE,bCacl=TRUE;
	LStack<oper_flag> stack_f;
	LStack<DOUBLE> stack_v;
	if(!ptrPress)return ret;
	LONG nLen=strlen(ptrPress);
	char *pPress=new char[nLen+1];
	strcpy(pPress,ptrPress);
	char *ptr=pPress,*ptrHead=pPress,*pBak;
	while((val=*pPress++) || val==0){
		//if(val==' ')continue;
		switch(val){
		case '+':
			next_level.level=LEVEL_ADD;
			next_level.oper=FLAG_ADD;
			break;
		case '-':
			next_level.level=LEVEL_SUB;
			next_level.oper=FLAG_SUB;
			break;
		case '*':
			next_level.level=LEVEL_MUL;
			next_level.oper=FLAG_MUL;
			break;
		case '/':
			next_level.level=LEVEL_DIV;
			next_level.oper=FLAG_DIV;
			break;
		case '(':
			next_level.level=MAX_LEVEL;
			next_level.oper=FLAG_NONE;
			break;
		case ')':
			next_level.level=MIN_LEVEL;
			next_level.oper=FLAG_NONE;
			break;
		default:
			if(val==0)
				next_level.level=MIN_LEVEL;
			else
				next_level.level=NO_LEVEL;
			next_level.oper=FLAG_NONE;
			break;
		}
		if((int)next_level.level!=(int)NO_LEVEL || val==0){
			if(val!=0)
				*(pPress-1)='\0';
			char *ptrEnd=0;
			bSave_value=(ptr[0]!='\0');
			if(!IsNumeric(ptr) && bCacl){
				try{
					/*��������*/
					pBak=pPress;
					if(next_level.level=MAX_LEVEL){
						/*�����Ǹ�����*/
						char* ptrName=new char[strlen(ptr)+1];
						strcpy(ptrName,ptr);
						
						dblT=OnFunction(ptrName,pPress,b_error);
						delete[] ptrName;
						if(b_error)
							break;
						bCacl=FALSE;
					}else{
						//�����Ƿ��ǿ�ʶ��ĳ���
						//����
						dblT=OnDefinedValue(ptr,b_error);
						bCacl=FALSE;
						if(b_error)
							break;
					}
					*(pBak-1)=val;
				}catch(...){
					/*���ʽ���ܳ�����*/
					b_error=TRUE;
					break;
				}
			}else{
				if(bSave_value && bCacl){
					dblT=strtod(ptr,&ptrEnd);
					if(val==0 || next_level.level==MIN_LEVEL)
						ret=dblT;
				}
				
				if(next_level.level==MAX_LEVEL){
					/*���� ��ջ*/
					stack_f.push(next_level);
				}else{
					if(!stack_f.value(level)){
						if(next_level.level!=MAX_LEVEL)
							stack_v.push(dblT);
						stack_f.push(next_level);
					}else if(level.level>next_level.level){
						stack_v.push(dblT);
						stack_f.push(next_level);
					}else{
						while(level.level<=next_level.level){
							/*��Ҫ������*/
							/*���������ӹ�*/
							if(level.level==MAX_LEVEL){
								/*��һ���Ƿ�Ϊ������*/
								if(next_level.level==MIN_LEVEL){
									/*��������*/
									stack_f.pop(level);
								}else{
									/*���ڼ�����ʽ*/
									stack_f.push(next_level);
									stack_v.push(dblT);
								}
								break;
							}else{
								if(!stack_v.pop(ret))
									ret=0;
								if(level.oper==FLAG_ADD){
									ret+=dblT;
								}else if(level.oper==FLAG_SUB){
									ret-=dblT;
								}else if(level.oper==FLAG_MUL){
									ret*=dblT;
								}else if(level.oper==FLAG_DIV){
									ret/=dblT;
								}
								dblT=ret;
								stack_f.pop(level);
								if(!stack_f.value(level))break;
							}
						}
						if(level.level!=MAX_LEVEL && val!=0){
							/*�����м���*/
							stack_v.push(dblT);
							if(next_level.level!=MIN_LEVEL)
								stack_f.push(next_level);
						}
					}
				}
			}
			if(val!=0 ){
				if(bCacl)
					*(pPress-1)=val;
			}else{
				break;
			}
			if(!bCacl)
				bCacl=TRUE;
			ptr=pPress;
		}
	}
	delete[] ptrHead;
	return ret;
}
