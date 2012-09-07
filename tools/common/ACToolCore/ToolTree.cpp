  #include "ToolTree.h"
#include "ToolBase.h"

namespace actools
{
	ToolTree::ToolTree()
	{
		m_strCurrDBName = "";
	}

	ToolTree::~ToolTree()
	{}

	BOOL ToolTree::Create(RECT rcRect, CWnd* pWnd, UINT nID)
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
		return ret;
	}

	int ToolTree::SetDB(const std::string& strDBName)
	{
		if(UpdateDBToTree(strDBName) != 0)
			return -1;

		m_strCurrDBName = strDBName;
		return 0;
	}

	int ToolTree::UpdateDBToTree(const std::string& strDBName)
	{
		lua_State* pLua = ToolBase::Instance().GetLuaState();
		return 0;
	}
}