#include "ac_behavior_tree.h"
namespace acoral
{
	BTNode::BTNode(BTNode* pParent)
		: m_pParent(pParent)
		, m_pEvaluteScript(NULL)
		, m_pEnterScript(NULL)
		, m_pExcuteScript(NULL)
		, m_pExitScript(NULL)
		, m_bNeedExit(false)
		, m_nActiveChild(-1)
	{

	}

	BTNode::~BTNode()
	{
		_safe_delete(m_pEvaluteScript);
		_safe_delete(m_pEnterScript);
		_safe_delete(m_pExcuteScript);
		_safe_delete(m_pExitScript);
		for(size_t i = 0; i < m_vtChilds.size(); i++)
			_safe_delete(m_vtChilds[i]);
	}

	BTNode& BTNode::AddChild(BTNode* pChild)
	{
		ACCHECK(pChild);
		m_vtChilds.push_back(pChild);
		return (*this);
	}

	BTNode& BTNode::SetName(const char* pName)
	{
		ACCHECK(pName);
		m_strName = pName;
		return (*this);
	}

	BTNode& BTNode::AddEvaluateScript(BTEvaluateScript* pScript)
	{
		if(pScript != m_pEvaluteScript)
		{
			_safe_delete(m_pEvaluteScript);
			m_pEvaluteScript = pScript;
		}

		return (*this);
	}

	BTNode& BTNode::AddEnterScript(BTEnterScript* pScript)
	{
		if(pScript != m_pEnterScript)
		{
			_safe_delete(m_pEnterScript);
			m_pEnterScript = pScript;
		}

		return (*this);
	}

	BTNode& BTNode::AddExcuteScript(BTExcuteScript* pScript)
	{
		ACCHECK(pScript);
		if(pScript != m_pExcuteScript)
		{
			_safe_delete(m_pExcuteScript);
			m_pExcuteScript = pScript;
		}

		return (*this);
	}

	BTNode& BTNode::AddExitScript(BTExitScript* pScript)
	{
		if(pScript != m_pExitScript)
		{
			_safe_delete(m_pExitScript);
			m_pExitScript = pScript;
		}

		return (*this);
	}

	void BTNode::SetActiveChild(void* unit, sint32 nActiveChild)
	{
		ACCHECK(nActiveChild >= -1 && nActiveChild < (sint32)m_vtChilds.size());

		if(nActiveChild != m_nActiveChild)
		{
			if(m_nActiveChild != -1)
				m_vtChilds[m_nActiveChild]->Exit(unit);

			if(nActiveChild != -1)
				m_vtChilds[nActiveChild]->Enter(unit);

			m_nActiveChild = nActiveChild;
		}
	}

	bool BTNode::Evaluate(void* unit)
	{
		if(m_pEvaluteScript)
			return m_pEvaluteScript->Excute(unit);

		return true;
	}

	bool BTNode::Enter(void* unit)
	{
		m_bNeedExit = true;
		m_nActiveChild = -1;
		if(m_pEnterScript)
			return m_pEnterScript->Excute(unit);

		return true;
	}

	EBTNodeStatus BTNode::Excute(void* unit)
	{
		ACCHECK(m_pExcuteScript);
		return m_pExcuteScript->Excute(unit, m_vtChilds);
	}

	bool BTNode::Exit(void* unit)
	{
		if(m_bNeedExit)
		{
			m_bNeedExit = false;
			if(m_nActiveChild != -1)
				m_vtChilds[m_nActiveChild]->Exit(unit);

			if(m_pExitScript)
				return m_pExitScript->Excute(unit);
		}

		return true;
	}

	EBTNodeStatus BTSeletor::Excute(void* unit, const std::vector<BTNode*>& vtChilds)
	{
		sint32 nCurSel = -1;
		EBTNodeStatus nStatus = BT_NODE_STATUS_FINISH;
		for(size_t i = 0; i < vtChilds.size(); i++)
		{
			if(vtChilds[i]->Evaluate(unit))
			{
				nCurSel = i;
				break;
			}
		}

		m_pOwner->SetActiveChild(unit, nCurSel);

		if(m_pOwner->GetActiveChild() != -1)
			nStatus = vtChilds[m_pOwner->GetActiveChild()]->Excute(unit);

		return nStatus;;
	}

}