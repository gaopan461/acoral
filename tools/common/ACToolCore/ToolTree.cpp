#include "ToolTree.h"

namespace actools
{
	ToolTree::ToolTree()
	{
		m_strCurrDatabaseName = "";
	}

	ToolTree::~ToolTree()
	{}

	BOOL ToolTree::Create(const std::string& strDatabaseName, RECT rcRect, CWnd* pWnd, UINT nID)
	{
		DWORD dwStyle, dwOptions;

		// Setup the window style
		dwStyle = WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

		// Setup the tree options 
		// OT_OPTIONS_SHOWINFOWINDOW
		dwOptions = OT_OPTIONS_SHADEEXPANDCOLUMN | OT_OPTIONS_SHADEROOTITEMS;

		// Create tree options
		BOOL ret = COptionTree::Create(dwStyle, rcRect, pWnd, dwOptions, nID);

		// Want to be notified
		SetNotify(TRUE, this);
		m_strCurrDatabaseName = strDatabaseName;
		return ret;
	}
}