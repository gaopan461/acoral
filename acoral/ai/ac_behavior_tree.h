#ifndef _ac_behavior_tree_
#define _ac_behavior_tree_

#include "ac_def.h"

namespace acoral
{
	class BTNode;

	//�ڵ��ִ��״̬
	enum EBTNodeStatus
	{
		BT_NODE_STATUS_READEY,		//׼��ִ��
		BT_NODE_STATUS_EXCUTING,	//����ִ��
		BT_NODE_STATUS_FINISH,				//ִ�н���
	};

	//�ڵ�ű�����
	class BTScript
	{
	public:
		BTScript(BTNode* pOwner):m_pOwner(pOwner){}
	protected:
		BTNode* m_pOwner;
	};

	//�ڵ���������ű�����
	class BTEvaluateScript : public BTScript
	{
	public:
		BTEvaluateScript(BTNode* pOwner):BTScript(pOwner){}
		virtual bool Excute(void* unit) = 0;
	};

	//�ڵ����ű�����
	class BTEnterScript : public BTScript
	{
	public:
		BTEnterScript(BTNode* pOwner):BTScript(pOwner){}
		virtual bool Excute(void* unit) = 0;
	};

	//�ڵ�ִ�нű�����
	class BTExcuteScript : public BTScript
	{
	public:
		BTExcuteScript(BTNode* pOwner):BTScript(pOwner){}
		virtual EBTNodeStatus Excute(void* unit, const std::vector<BTNode*>& vtChilds) = 0;
	};

	//�ڵ��˳��ű�����
	class BTExitScript : public BTScript
	{
	public:
		BTExitScript(BTNode* pOwner):BTScript(pOwner){}
		virtual bool Excute(void* unit) = 0;
	};

	//��Ϊ���ڵ�
	class BTNode
	{
	public:
		BTNode(BTNode* pParent);
		~BTNode();
		BTNode& AddChild(BTNode* pChild);
		BTNode& SetName(const char* pName);
		BTNode& AddEvaluateScript(BTEvaluateScript* pScript);
		BTNode& AddEnterScript(BTEnterScript* pScript);
		BTNode& AddExcuteScript(BTExcuteScript* pScript);
		BTNode& AddExitScript(BTExitScript* pScript);
		inline sint32 GetActiveChild(){return m_nActiveChild;}
		void SetActiveChild(void* unit, sint32 nActiveChild);
		bool Evaluate(void* unit);
		bool Enter(void* unit);
		EBTNodeStatus Excute(void* unit);
		bool Exit(void* unit);
	private:
		std::vector<BTNode*> m_vtChilds;			//�ӽڵ�
		BTNode* m_pParent;									//���ڵ�
		std::string m_strName;									//�ڵ���
		BTEvaluateScript* m_pEvaluteScript;	//���������ű�
		BTEnterScript* m_pEnterScript;				//����ű�
		BTExcuteScript* m_pExcuteScript;			//ִ�нű�
		BTExitScript* m_pExitScript;						//�˳��ű�
		bool m_bNeedExit;										//�Ƿ���Ҫִ���˳��ű�
		sint32 m_nActiveChild;
	};

	class BTSeletor : public BTExcuteScript
	{
	public:
		BTSeletor(BTNode* pOwner):BTExcuteScript(pOwner){}
		virtual EBTNodeStatus Excute(void* unit, const std::vector<BTNode*>& vtChilds);
	};

}
#endif