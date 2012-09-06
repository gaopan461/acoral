#ifndef _TOOL_TREE_H_
#define _TOOL_TREE_H_

#define _CRT_SECURE_NO_WARNINGS

#include <afx.h>
#include <afxwin.h>
#include <afxcmn.h>
#include <afxdtctl.h>
#include <string>
#include "OptionTree.h"

namespace actools
{
	class ToolTree : public COptionTree
	{
	public:
		ToolTree();
		virtual ~ToolTree();
	public:
		BOOL Create(const std::string& strDatabaseName, RECT rcRect, CWnd* pWnd, UINT nID);
	private:
		std::string m_strCurrDatabaseName;
	};
}

#endif//_TOOL_TREE_H_