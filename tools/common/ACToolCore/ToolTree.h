#ifndef _TOOL_TREE_H_
#define _TOOL_TREE_H_

#define _CRT_SECURE_NO_WARNINGS

#include <afx.h>
#include <afxwin.h>
#include <afxcmn.h>
#include <afxdtctl.h>
#include <string>
#include "OptionTree.h"
#include "ac_lua.h"

namespace actools
{
	class ToolTree : public COptionTree
	{
	public:
		ToolTree();
		virtual ~ToolTree();
	public:
		BOOL Create(RECT rcRect, CWnd* pWnd, UINT nID);
		int SetDB(const std::string& strDBName);
	public:
		int UpdateDBToTree(const std::string& strDBName);
	private:
		std::string m_strCurrDBName;
	};
}

#endif//_TOOL_TREE_H_