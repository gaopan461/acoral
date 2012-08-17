// PopWindow.cpp : 实现文件
//

#include "stdafx.h"
#include "ToolTest.h"
#include "PopWindow.h"
#include "CheckComboBox.h"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

bool ValidPopMainStyle(const CString& style)
{
	if(style == "radio" || style == "checkbox" || style == "static")
		return true;
	else
		return false;
}

bool ValidPopParamStyle(const CString& style)
{
	if(style == "edit" || style == "combobox" || style == "comcheckbox")
		return true;
	else
		return false;
}

//===================================================================================
bool g_bIsPopMenu = true;

//===================================================================================

int OriginToDefType(CWnd* pOriginWnd, CollectMainDefTypesT& mapDefType)
{
	//获取该原始控件的xml配置信息
	int dlgId = pOriginWnd->GetDlgCtrlID();
	ASSERT(g_mapOriginInfos.find(dlgId) != g_mapOriginInfos.end());
	std::string xmlName = g_mapOriginInfos[dlgId].m_strName;
	ASSERT(g_mapPopInfos.find(xmlName) != g_mapPopInfos.end());
	CollectionPopControlInfosT& vtMainInfos = g_mapPopInfos[xmlName];

	//将该原始控件中的内容进行解析，变成一种中间格式
	CollectionMainTextsT mapMidData;
	CPopWindow::OriginToMidData(pOriginWnd,mapMidData);

	//对中间格式进行翻译（把文字转成内部表示）
	for(CollectionMainTextsT::iterator mainTextItem = mapMidData.begin(); mainTextItem != mapMidData.end(); mainTextItem++)
	{
		CString mainName = mainTextItem->first;
		for(CollectionPopControlInfosT::iterator mainInfoItem = vtMainInfos.begin(); mainInfoItem != vtMainInfos.end(); mainInfoItem++)
		{
			//主控件匹配
			if(mainName == (*mainInfoItem)->m_strName.c_str())
			{
				//主控件转换为内部值
				int mainValue = (*mainInfoItem)->m_nValue;

				CollectionParamTextsT& vtParamsText = mainTextItem->second;
				std::vector<SPopControlParamInfo*>& vtParamsInfo = (*mainInfoItem)->m_vtParams;
				ASSERT(vtParamsText.size() == vtParamsInfo.size());
				std::vector<SParamDefType> vtParamDefTypes;
				vtParamDefTypes.clear();
				for(size_t paramIdx = 0; paramIdx < vtParamsText.size(); paramIdx++)
				{
					//参数控件名字需匹配
					ASSERT(vtParamsText[paramIdx].first == vtParamsInfo[paramIdx]->m_strName.c_str());

					SParamDefType paramDefType;
					paramDefType.m_strCName = vtParamsInfo[paramIdx]->m_strCName;
					paramDefType.m_strDlgStyle = vtParamsInfo[paramIdx]->m_strDlgStyle;
					if(paramDefType.m_strDlgStyle == "edit")
					{
						paramDefType.m_strEditReverse = vtParamsInfo[paramIdx]->m_strReverse;
						if(!vtParamsText[paramIdx].second.empty())
							paramDefType.m_strEditValue = vtParamsText[paramIdx].second[0].GetBuffer();
					}
					else if(paramDefType.m_strDlgStyle == "combobox")
					{
						if(!vtParamsText[paramIdx].second.empty())
						{
							std::string comboboxName = vtParamsText[paramIdx].second[0].GetBuffer();
							std::map<std::string, std::string>& mapValue = vtParamsInfo[paramIdx]->m_mapValue;
							ASSERT(mapValue.find(comboboxName) != mapValue.end());
							paramDefType.m_nComboboxValue = atoi(mapValue[comboboxName].c_str());
						}
					}
					else if(paramDefType.m_strDlgStyle == "comcheckbox")
					{
						CollectionParamValuesT& vtParamValuesText = vtParamsText[paramIdx].second;
						for(CollectionParamValuesT::iterator paramValueItem = vtParamValuesText.begin(); paramValueItem != vtParamValuesText.end(); paramValueItem++)
						{
							std::string comcheckboxName = paramValueItem->GetBuffer();
							std::map<std::string, std::string>& mapValue = vtParamsInfo[paramIdx]->m_mapValue;
							ASSERT(mapValue.find(comcheckboxName) != mapValue.end());
							paramDefType.m_vtComCheckboxValue.push_back(atoi(mapValue[comcheckboxName].c_str()));
						}
					}
					else
						ASSERT(false);

					vtParamDefTypes.push_back(paramDefType);
				}

				mapDefType.insert(std::make_pair(mainValue,vtParamDefTypes));
				break;
			}
		}
	}
	return 0;
}

int DefTypeToOrigin(CWnd* pOriginWnd, CollectMainDefTypesT& mapDefType, std::vector<CString>& vtTexts)
{
	ASSERT(pOriginWnd);
	int dlgId = pOriginWnd->GetDlgCtrlID();
	ASSERT(g_mapOriginInfos.find(dlgId) != g_mapOriginInfos.end());
	std::string xmlName = g_mapOriginInfos[dlgId].m_strName;
	ASSERT(g_mapPopInfos.find(xmlName) != g_mapPopInfos.end());
	CollectionPopControlInfosT& mainInfos = g_mapPopInfos[xmlName];
	for(CollectMainDefTypesT::iterator mainItem = mapDefType.begin(); mainItem != mapDefType.end(); mainItem++)
	{
		int mainValue = mainItem->first;
		for(size_t mainIdx = 0; mainIdx < mainInfos.size(); mainIdx++)
		{
			//主控件匹配
			if(mainInfos[mainIdx]->m_nValue ==  mainValue)
			{
				//main
				CString text = mainInfos[mainIdx]->m_strName.c_str();
				std::vector<SParamDefType>& vtParams = mainItem->second;
				std::vector<SPopControlParamInfo*>& vtParamInfos = mainInfos[mainIdx]->m_vtParams;
				//参数控件个数不匹配，返回
				if(vtParams.size() != vtParamInfos.size())
					return -1;

				//main:
				if(!vtParams.empty())
					text += ":";

				for(size_t paramIdx = 0; paramIdx < vtParams.size(); paramIdx++)
				{
					//参数名不匹配，返回
					if(vtParams[paramIdx].m_strCName != vtParamInfos[paramIdx]->m_strCName)
						return -1;

					//参数间用","分隔
					if(paramIdx != 0)
						text += ",";

					text += vtParamInfos[paramIdx]->m_strName.c_str();
					text += "=";

					std::string dlgStyle = vtParamInfos[paramIdx]->m_strDlgStyle;
					if(dlgStyle == "edit")
						text += vtParams[paramIdx].m_strEditValue.c_str();
					else if(dlgStyle == "combobox")
					{
						for(std::map<std::string,std::string>::iterator paramValueItem = vtParamInfos[paramIdx]->m_mapValue.begin(); 
							paramValueItem != vtParamInfos[paramIdx]->m_mapValue.end(); 
							paramValueItem++)
							if(atoi(paramValueItem->second.c_str()) == vtParams[paramIdx].m_nComboboxValue)
							{
								text += paramValueItem->first.c_str();
								break;
							}
					}
					else if(dlgStyle == "comcheckbox")
					{
						for(size_t idx = 0; idx < vtParams[paramIdx].m_vtComCheckboxValue.size(); idx++)
						{
							for(std::map<std::string,std::string>::iterator paramValueItem = vtParamInfos[paramIdx]->m_mapValue.begin(); 
								paramValueItem != vtParamInfos[paramIdx]->m_mapValue.end(); 
								paramValueItem++)
								if(atoi(paramValueItem->second.c_str()) == vtParams[paramIdx].m_vtComCheckboxValue[idx])
								{
									text += paramValueItem->first.c_str();
									break;
								}
						}
					}
				}
				break;
			}
		}
	}

	return 0;
}

// CPopWindow 对话框

IMPLEMENT_DYNAMIC(CPopWindow, CDialog)

CPopWindow::CPopWindow(int resID, CWnd* pParent /*=NULL*/)
	: CDialog(resID, pParent)
	, m_nId(USER_CTRL_ID_START)
{

}

CPopWindow::~CPopWindow()
{
	for(std::vector<SPopItem*>::iterator iter1 = m_vtPopItems.begin(); iter1 != m_vtPopItems.end(); iter1++)
		delete (*iter1);

	for(std::vector<CWnd*>::iterator iter2 = m_vtWnds.begin(); iter2 != m_vtWnds.end(); iter2++)
		delete (*iter2);
}

void CPopWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CPopWindow::CreatePopMain(SPopControlMainInfo* pMainInfo)
{
	ASSERT(pMainInfo);
	if(pMainInfo->m_strDlgStyle == "static")
	{
		CStatic* pStatic = new CStatic();
		pStatic->Create(pMainInfo->m_strName.c_str(),WS_CHILD|WS_VISIBLE,CRect(pMainInfo->m_nStartWidth,pMainInfo->m_nStartHeight,pMainInfo->m_nStartWidth + pMainInfo->m_nWidth,pMainInfo->m_nStartHeight + pMainInfo->m_nHeight),this,m_nId++);
		m_vtWnds.push_back(pStatic);
		return;
	}

	DWORD dwStyle = WS_CHILD | WS_VISIBLE;
	if(pMainInfo->m_strDlgStyle == "radio")
		dwStyle |= BS_AUTORADIOBUTTON;
	else if(pMainInfo->m_strDlgStyle == "checkbox")
		dwStyle |= BS_AUTOCHECKBOX;

	CButton* pMainCtrl = new CButton();
	pMainCtrl->Create(pMainInfo->m_strName.c_str(),dwStyle,CRect(pMainInfo->m_nStartWidth,pMainInfo->m_nStartHeight,pMainInfo->m_nStartWidth+pMainInfo->m_nWidth,pMainInfo->m_nStartHeight+20),this,m_nId++);
	m_vtWnds.push_back(pMainCtrl);
	SPopItem* pPopItem = new SPopItem();
	pPopItem->m_pMainWnd = pMainCtrl;
	pPopItem->m_pMainInfo = pMainInfo;

	for(std::vector<SPopControlParamInfo*>::iterator paramIter = pMainInfo->m_vtParams.begin();
		paramIter != pMainInfo->m_vtParams.end();
		paramIter++)
	{
		CWnd* pParamWnd = NULL;
		SPopControlParamInfo* pParamInfo = *paramIter;
		int nNameL,nNameT,nNameR,nNameB;
		int nCtrlL,nCtrlT,nCtrlR,nCtrlB;
		nNameL = pParamInfo->m_nStartWidth;
		nNameT = pParamInfo->m_nStartHeight;
		nNameR = nNameL + pParamInfo->m_nWidth;
		nNameB = nNameT + DEFAULT_HEIGHT;
		nCtrlL = nNameR;
		nCtrlT = nNameT;
		nCtrlR = nCtrlL + pParamInfo->m_nWidth1;
		nCtrlB = nNameB;

		CStatic* pParamNameWnd = new CStatic();
		pParamNameWnd->Create(pParamInfo->m_strName.c_str(),WS_CHILD|WS_VISIBLE,CRect(nNameL,nNameT,nNameR,nNameB),this,m_nId++);
		m_vtWnds.push_back(pParamNameWnd);

		if(pParamInfo->m_strDlgStyle == "edit")
		{
			pParamWnd = new CEdit();
			pParamWnd->Create("Edit",NULL,WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,CRect(nCtrlL,nCtrlT,nCtrlR,nCtrlB),this,m_nId++);
		}
		else if(pParamInfo->m_strDlgStyle == "combobox")
		{
			nCtrlB += pParamInfo->m_mapValue.size() * DEFAULT_HEIGHT;
			pParamWnd = new CComboBox();
			pParamWnd->Create("ComboBox",NULL,WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST,CRect(nCtrlL,nCtrlT,nCtrlR,nCtrlB),this,m_nId++);
			for (std::map<std::string,std::string>::iterator itemIter = pParamInfo->m_mapValue.begin();
				itemIter != pParamInfo->m_mapValue.end();
				itemIter++)
			{
				((CComboBox*)pParamWnd)->AddString(itemIter->first.c_str());
			}
		}
		else if(pParamInfo->m_strDlgStyle == "comcheckbox")
		{
			nCtrlB += pParamInfo->m_mapValue.size() * DEFAULT_HEIGHT;
			CCheckComboBox* pCheckComboBox = new CCheckComboBox();
			pCheckComboBox->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWNLIST,CRect(nCtrlL,nCtrlT,nCtrlR,nCtrlB), this, m_nId++);
			pParamWnd = pCheckComboBox;
			for (std::map<std::string,std::string>::iterator itemIter = pParamInfo->m_mapValue.begin();
				itemIter != pParamInfo->m_mapValue.end();
				itemIter++)
			{
				((CCheckComboBox*)pParamWnd)->AddString(itemIter->first.c_str());
			}
		}

		m_vtWnds.push_back(pParamWnd);
		pPopItem->m_vtParamInfos.push_back(std::make_pair(pParamWnd,pParamInfo));
	}
	m_vtPopItems.push_back(pPopItem);
}

BOOL CPopWindow::OnInitDialog()
{
	CDialog::OnInitDialog();
	int originDlgId = m_pOriginWnd->GetDlgCtrlID();
	if(g_mapOriginInfos.find(originDlgId) == g_mapOriginInfos.end())
		return TRUE;

	SOriginControlInfo& originInfo = g_mapOriginInfos[originDlgId];
	if(g_mapPopInfos.find(originInfo.m_strName) == g_mapPopInfos.end())
		return TRUE;

	CollectionPopControlInfosT& popInfo = g_mapPopInfos[originInfo.m_strName];
	for(CollectionPopControlInfosT::iterator mainIter = popInfo.begin(); mainIter != popInfo.end(); mainIter++)
		CreatePopMain(*mainIter);

	UpdateOriginToPop();

	return TRUE;
}

BEGIN_MESSAGE_MAP(CPopWindow, CDialog)
	ON_WM_CLOSE()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

void CPopWindow::SetOrigin(CWnd* pWnd)
{
	m_pOriginWnd = pWnd;
}

BOOL CPopWindow::Show()
{
	DoModal();
	return TRUE;
}

int CPopWindow::OriginToMidData(CollectionMainTextsT& mapData)
{
	return CPopWindow::OriginToMidData(m_pOriginWnd,mapData);
}

int CPopWindow::OriginToMidData(CWnd* pOriginWnd, CollectionMainTextsT& mapData)
{
	DWORD originId = pOriginWnd->GetDlgCtrlID();
	if(g_mapOriginInfos.find(originId) == g_mapOriginInfos.end())
		return -1;

	int originDlgStyle = g_mapOriginInfos[originId].m_nDlgStyle;
	mapData.clear();
	std::vector<CString> vtItems;
	switch(originDlgStyle)
	{
	case ORIGIN_STYLE_EDITRADIO:
	case ORIGIN_STYLE_EDITTUPLE:
		{
			CString originText;
			((CEdit*)pOriginWnd)->GetWindowText(originText);

			CString itemText;
			int itemPos = 0;
			//每一行控件通过|隔开
			itemText = originText.Tokenize(_T("|"),itemPos);
			while (itemText != _T(""))
			{
				vtItems.push_back(itemText);
				itemText = originText.Tokenize(_T("|"), itemPos);
			};
		}
		break;
	case ORIGIN_STYLE_LISTBOX:
		{
			CString itemText;
			for(int i = 0; i < ((CListBox*)pOriginWnd)->GetCount(); i++)
			{
				((CListBox*)pOriginWnd)->GetText(i,itemText);
				vtItems.push_back(itemText);
			}
		}
		break;
	}

	for(std::vector<CString>::iterator iterItem = vtItems.begin(); iterItem != vtItems.end(); iterItem++)
	{
		iterItem->Trim();
		CollectionParamTextsT vtParamTexts;
		vtParamTexts.clear();

		//主控件和参数控件通过:隔开
		int mainPos = iterItem->FindOneOf(":");
		if(mainPos != -1)
		{
			CString mainText = iterItem->Left(mainPos);mainText.Trim();
			CString paramsText = iterItem->Mid(mainPos + 1);paramsText.Trim();

			CString paramText;
			int paramPos = 0;
			//参数控件之间通过,隔开
			paramText = paramsText.Tokenize(_T(","),paramPos);paramText.Trim();
			while(paramText != _T(""))
			{
				CollectionParamValuesT vtParamValues;
				vtParamValues.clear();

				//参数名和参数值通过=隔开
				int paramNamePos = paramText.FindOneOf("=");
				if(paramNamePos != -1)
				{
					CString paramName = paramText.Left(paramNamePos);paramName.Trim();
					CString paramValues = paramText.Mid(paramNamePos + 1);paramValues.Trim();

					CString paramValue;
					int paramValuePos = 0;
					//参数的多个值通过`隔开
					paramValue = paramValues.Tokenize(_T("`"),paramValuePos);paramValue.Trim();
					while(paramValue != _T(""))
					{
						vtParamValues.push_back(paramValue);
						paramValue = paramValues.Tokenize(_T("`"),paramValuePos);paramValue.Trim();
					}

					vtParamTexts.push_back(std::make_pair(paramName,vtParamValues));
				}
				else
					vtParamTexts.push_back(std::make_pair(paramText,vtParamValues));

				paramText = paramsText.Tokenize(_T(","),paramPos);paramText.Trim();
			}

			mapData.insert(std::make_pair(mainText,vtParamTexts));
		}
		else
			mapData.insert(std::make_pair(*iterItem,vtParamTexts));

	}
	return 0;
}

int CPopWindow::UpdateOriginToPop()
{
	if(!m_pOriginWnd)
		return -1;

	CollectionMainTextsT mapData;
	if(OriginToMidData(mapData) != 0)
		return -1;

	for(std::vector<SPopItem*>::iterator itemIter = m_vtPopItems.begin(); itemIter != m_vtPopItems.end(); itemIter++)
	{
		CString mainName = (*itemIter)->m_pMainInfo->m_strName.c_str();
		if(mapData.find(mainName) != mapData.end())
		{
			((CButton*)((*itemIter)->m_pMainWnd))->SetCheck(BST_CHECKED);
			CollectionParamTextsT& vtParamTexts = mapData[mainName];
			std::vector<std::pair<CWnd*,SPopControlParamInfo*>>& vtParamInfos = (*itemIter)->m_vtParamInfos;
			if(vtParamTexts.size() != vtParamInfos.size())
				continue;

			for(size_t paramIdx = 0; paramIdx < vtParamTexts.size(); paramIdx++)
			{
				if(vtParamTexts[paramIdx].first != vtParamInfos[paramIdx].second->m_strName.c_str())
					break;

				if(vtParamTexts[paramIdx].second.empty())
					continue;

				if(vtParamInfos[paramIdx].second->m_strDlgStyle == "edit")
				{
					CString paramValue = vtParamTexts[paramIdx].second[0];
					vtParamInfos[paramIdx].first->SetWindowText(paramValue.GetBuffer());
				}
				else if(vtParamInfos[paramIdx].second->m_strDlgStyle == "combobox")
				{
					CString paramValue = vtParamTexts[paramIdx].second[0];
					CComboBox* pParamWnd = (CComboBox*)(vtParamInfos[paramIdx].first);
					pParamWnd->SelectString(-1,paramValue.GetBuffer());
				}
				else if(vtParamInfos[paramIdx].second->m_strDlgStyle == "comcheckbox")
				{
					CCheckComboBox* pParamWnd = (CCheckComboBox*)(vtParamInfos[paramIdx].first);
					CollectionParamValuesT& vtParamValues = vtParamTexts[paramIdx].second;
					for(std::vector<CString>::iterator valueItem = vtParamValues.begin(); valueItem != vtParamValues.end(); valueItem++)
					{
						int idx = pParamWnd->FindStringExact(-1,valueItem->GetBuffer());
						if(idx != CB_ERR)
							pParamWnd->SetCheck(idx,TRUE);
					}
				}
			}
		}
	}

	return 0;
}

int CPopWindow::PopToMidData(std::vector<CString>& vtData)
{
	for(std::vector<SPopItem*>::iterator mainItem = m_vtPopItems.begin(); mainItem != m_vtPopItems.end(); mainItem++)
	{
		if(((CButton*)((*mainItem)->m_pMainWnd))->GetCheck() == 1)
		{
			CString text = (*mainItem)->m_pMainInfo->m_strName.c_str();
			if(!(*mainItem)->m_vtParamInfos.empty())
				text = text + ":";

			for(std::vector<std::pair<CWnd*,SPopControlParamInfo*>>::iterator paramItem = (*mainItem)->m_vtParamInfos.begin(); 
				paramItem != (*mainItem)->m_vtParamInfos.end(); 
				paramItem++)
			{
				if(paramItem != (*mainItem)->m_vtParamInfos.begin())
					text = text + ",";

				text = text + paramItem->second->m_strName.c_str();
				CString paramVal;
				paramItem->first->GetWindowText(paramVal);
				text = text + "=" + paramVal;
			}

			vtData.push_back(text);
		}
	}

	return 0;
}

int CPopWindow::WritePopToOrigin()
{
	if(!m_pOriginWnd)
		return -1;

	DWORD originId = m_pOriginWnd->GetDlgCtrlID();
	if(g_mapOriginInfos.find(originId) == g_mapOriginInfos.end())
		return -1;

	int originDlgStyle = g_mapOriginInfos[originId].m_nDlgStyle;
	std::vector<CString> vtData;
	PopToMidData(vtData);
	switch(originDlgStyle)
	{
	case ORIGIN_STYLE_EDITRADIO:
	case ORIGIN_STYLE_EDITTUPLE:
		{
			CString text = "";
			for(std::vector<CString>::iterator iter = vtData.begin(); iter != vtData.end(); iter++)
			{
				if(iter != vtData.begin())
					text = text + "|";

				text = text + iter->GetBuffer();
			}
			((CEdit*)m_pOriginWnd)->SetWindowText(text.GetBuffer());
		}
		break;
	case ORIGIN_STYLE_LISTBOX:
		{
			((CListBox*)m_pOriginWnd)->ResetContent();
			for(std::vector<CString>::iterator iter = vtData.begin(); iter != vtData.end(); iter++)
				((CListBox*)m_pOriginWnd)->InsertString(-1,iter->GetBuffer());
		}
		break;
	}

	return 0;
}

// CPopWindow 消息处理程序
afx_msg void CPopWindow::OnClose()
{
	CDialog::OnClose();
}

afx_msg void CPopWindow::OnRButtonDown(UINT nFlags, CPoint point)
{
	WritePopToOrigin();
	CDialog::EndDialog(IDOK);
}

//===================================================================================
CollectionOriginInfosT g_mapOriginInfos;

bool REG_PROPERTY(int PID, const char* name, int dlgStyle)
{
	ASSERT(g_mapOriginInfos.find(PID) == g_mapOriginInfos.end());
	SOriginControlInfo originInfo;
	originInfo.m_strName = name;
	originInfo.m_nDlgStyle = dlgStyle;
	g_mapOriginInfos.insert(std::make_pair(PID,originInfo));
	return true;
}

//===================================================================================
CollectionPopInfosT g_mapPopInfos;

//===================================================================================
bool LoadPopConfig(std::string name)
{
	rapidxml::xml_document<> XMLDoc;
	rapidxml::file<> fdoc(name.c_str());
	rapidxml::xml_node<>* XMLRoot;
	XMLDoc.parse<0>(fdoc.data());
	XMLRoot = XMLDoc.first_node("root");
	
	//遍历所有配置
	for(rapidxml::xml_node<>* propertyNode = XMLRoot->first_node(); propertyNode != NULL; propertyNode = propertyNode->next_sibling())
	{
		std::string propertyName = propertyNode->name();
		CollectionPopControlInfosT vtPopControlInfos;
		vtPopControlInfos.clear();
		int startHeight = 0;
		int startWidth = 0;
		for(rapidxml::xml_node<>* mainItem = propertyNode->first_node("item"); mainItem != NULL; mainItem = mainItem->next_sibling("item"))
		{
			rapidxml::xml_node<>* nodeM;
			nodeM				= mainItem->first_node("dlgStyle");
			if(!nodeM)
				continue;

			CString dlgStyleM	= nodeM->value();					dlgStyleM.Trim();
			if(!ValidPopMainStyle(dlgStyleM))
				continue;

			SPopControlMainInfo* mainInfo = new SPopControlMainInfo();

			mainInfo->m_strDlgStyle = dlgStyleM.GetBuffer();

			nodeM				= mainItem->first_node("name");
			CString nameM		= nodeM ? nodeM->value() : "";		nameM.Trim();ConvertUTF8ToGBK(nameM);
			mainInfo->m_strName	= nameM.GetBuffer();
			nodeM				= mainItem->first_node("value");
			CString valueM		= nodeM ? nodeM->value() : "0";		valueM.Trim();
			mainInfo->m_nValue	= atoi(valueM.GetBuffer());
			nodeM				= mainItem->first_node("checked");
			CString checkedM	= nodeM ? nodeM->value() : "0";		checkedM.Trim();
			mainInfo->m_bChecked = (bool)atoi(checkedM.GetBuffer());
			nodeM				= mainItem->first_node("nonewline");
			CString nonewlineM	= nodeM ? nodeM->value() : "0";		nonewlineM.Trim();
			mainInfo->m_bNonewline = (bool)atoi(nonewlineM.GetBuffer());
			nodeM				= mainItem->first_node("width");
			CString widthM		= nodeM ? nodeM->value() : MAIN_DEFAULT_WIDTH;	widthM.Trim();
			mainInfo->m_nWidth	= atoi(widthM.GetBuffer());
			nodeM				= mainItem->first_node("descr");
			CString descrM		= nodeM ? nodeM->value() : "";		descrM.Trim();ConvertUTF8ToGBK(descrM);
			mainInfo->m_strDescr	= descrM.GetBuffer();
			mainInfo->m_nHeight	= DEFAULT_HEIGHT;
			mainInfo->m_nStartWidth = startWidth;
			startWidth = startWidth + mainInfo->m_nWidth;
			mainInfo->m_nStartHeight = startHeight;

			for(rapidxml::xml_node<>* paramItem = mainItem->first_node("param"); paramItem != NULL; paramItem = paramItem->next_sibling("param"))
			{
				SPopControlParamInfo* paramInfo = new SPopControlParamInfo();

				rapidxml::xml_node<>* nodeP;
				nodeP				= paramItem->first_node("dlgStyle");
				if(!nodeP)
					continue;

				CString dlgStyleP	= nodeP->value();				dlgStyleP.Trim();
				if(!ValidPopParamStyle(dlgStyleP))
					continue;
				paramInfo->m_strDlgStyle = dlgStyleP.GetBuffer();
				for(rapidxml::xml_node<> * paramDlgItem = nodeP->first_node("item"); paramDlgItem != NULL; paramDlgItem = paramDlgItem->next_sibling())
				{
					rapidxml::xml_node<>* dlgItemNodeP;
					dlgItemNodeP = paramDlgItem->first_node("name");
					CString dlgItemNameP = dlgItemNodeP ? dlgItemNodeP->value() : ""; dlgItemNameP.Trim();ConvertUTF8ToGBK(dlgItemNameP);
					dlgItemNodeP = paramDlgItem->first_node("value");
					CString dlgItemValueP = dlgItemNodeP ? dlgItemNodeP->value() : ""; dlgItemValueP.Trim();
					paramInfo->m_mapValue.insert(std::make_pair(dlgItemNameP.GetBuffer(),dlgItemValueP.GetBuffer()));
				}

				nodeP				= paramItem->first_node("name");
				CString nameP		= nodeP ? nodeP->value() : "";	nameP.Trim();ConvertUTF8ToGBK(nameP);
				paramInfo->m_strName	= nameP.GetBuffer();
				nodeP				= paramItem->first_node("cname");
				CString cnameP		= nodeP ? nodeP->value() : "";	cnameP.Trim();
				paramInfo->m_strCName= cnameP.GetBuffer();
				nodeP				= paramItem->first_node("reverse");
				CString reverseP	= nodeP ? nodeP->value() : "";	reverseP.Trim();
				paramInfo->m_strReverse = reverseP.GetBuffer();
				nodeP				= paramItem->first_node("default");
				CString defaultP	= nodeP ? nodeP->value() : "";	defaultP.Trim();ConvertUTF8ToGBK(defaultP);
				paramInfo->m_strDefault = defaultP.GetBuffer();
				nodeP				= paramItem->first_node("descr");
				CString descrP		= nodeP ? nodeP->value() : "";	descrP.Trim();ConvertUTF8ToGBK(descrP);
				paramInfo->m_strDescr = descrP.GetBuffer();
				nodeP				= paramItem->first_node("width");
				CString widthP		= nodeP ? nodeP->value() : PARAM_DEFAULT_WIDTH;	 widthP.Trim();
				paramInfo->m_nWidth	= atoi(widthP.GetBuffer());
				nodeP				= paramItem->first_node("width1");
				CString width1P		= nodeP ? nodeP->value() : PARAM_DEFAULT_WIDTH1; width1P.Trim();
				paramInfo->m_nWidth1	= atoi(width1P.GetBuffer());
				paramInfo->m_nHeight = DEFAULT_HEIGHT;
				paramInfo->m_nHeight1= DEFAULT_HEIGHT;
				paramInfo->m_nStartWidth = startWidth;
				startWidth = startWidth + paramInfo->m_nWidth + paramInfo->m_nWidth1;
				paramInfo->m_nStartHeight= startHeight;
				mainInfo->m_vtParams.push_back(paramInfo);
			}

			if((!mainInfo->m_vtParams.empty()) || (!mainInfo->m_bNonewline))
			{
				startWidth = 0;
				startHeight += DEFAULT_HEIGHT + DEFAULT_HEIGHT_INTER;
			}

			vtPopControlInfos.push_back(mainInfo);
		}
		g_mapPopInfos.insert(std::make_pair(propertyName,vtPopControlInfos));
	}

	return true;
}

void UnloadPopConfig()
{
	for(CollectionPopInfosT::iterator iterPop = g_mapPopInfos.begin(); iterPop != g_mapPopInfos.end(); iterPop++)
	{
		for(CollectionPopControlInfosT::iterator iterPopCtrl = iterPop->second.begin(); iterPopCtrl != iterPop->second.end(); iterPopCtrl++)
		{
			for(std::vector<SPopControlParamInfo*>::iterator iterPopParamCtrl = (*iterPopCtrl)->m_vtParams.begin(); iterPopParamCtrl != (*iterPopCtrl)->m_vtParams.end(); iterPopParamCtrl++)
				delete (*iterPopParamCtrl);
			
			(*iterPopCtrl)->m_vtParams.clear();
			delete (*iterPopCtrl);
		}
		iterPop->second.clear();
	}
	g_mapPopInfos.clear();
}

void DeclareNo(CWnd* pWnd, lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME)
{
	ASSERT(lua_istable(L, -1));
	CString strID;
	if(ISWRITETODB)
	{
		pWnd->GetDlgItem(DLGID)->GetWindowText(strID);strID.Trim();
		ASSERT(!strID.IsEmpty());

		lua_pushinteger(L, atoi(strID.GetBuffer()));
		lua_setfield(L, -2, NAME);
	}
	else
	{
		lua_getfield(L, -1, NAME);
		ASSERT(lua_isnumber(L,-1));
		strID = lua_tostring(L, -1);
		lua_pop(L,1);
		pWnd->GetDlgItem(DLGID)->SetWindowText(strID.GetBuffer());
	}
}

void DeclareListBoxDefType(CWnd* pWnd, lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME)
{
	ASSERT(lua_istable(L, -1));

	if(ISWRITETODB)
	{
		CollectMainDefTypesT mapDefType;
		OriginToDefType(pWnd->GetDlgItem(DLGID),mapDefType);

		lua_newtable(L);
		int num = 1;
		for (CollectMainDefTypesT::iterator mainItem = mapDefType.begin(); mainItem != mapDefType.end(); mainItem++)
		{
			lua_pushinteger(L, num);
			lua_pushinteger(L, mainItem->first);
			lua_settable(L, -3);
			num++;
		}

		lua_setfield(L, -2, NAME);
	}
	else
	{
		CollectMainDefTypesT mapDefType;
		lua_pushnil(L);
		while (lua_next(L, -2) != 0) 
		{
			ASSERT(lua_isnumber(L, -1));
			int mainValue = lua_tointeger(L, -1);
			lua_pop(L, 1);
			std::vector<SParamDefType> vtParams;
			vtParams.clear();
			mapDefType.insert(std::make_pair(mainValue, vtParams));
		}

		std::vector<CString> vtTexts;
		DefTypeToOrigin(pWnd->GetDlgItem(DLGID), mapDefType, vtTexts);
	}
}

void DeclareListBoxDefTypeAndParams(CWnd* pWnd, lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME)
{
	ASSERT(lua_istable(L, -1));
	CollectMainDefTypesT mapDefType;
	OriginToDefType(pWnd->GetDlgItem(DLGID),mapDefType);

	lua_newtable(L);
	for(CollectMainDefTypesT::iterator mainItem = mapDefType.begin(); mainItem != mapDefType.end(); mainItem++)
	{
		lua_pushinteger(L, mainItem->first);
		lua_newtable(L);
		for (std::vector<SParamDefType>::iterator paramItem = mainItem->second.begin(); paramItem != mainItem->second.end(); paramItem++)
		{
			if(paramItem->m_strDlgStyle == "edit")
			{
				lua_pushstring(L, paramItem->m_strCName.c_str());
				if(paramItem->m_strEditReverse == "number")
					lua_pushnumber(L, atof(paramItem->m_strEditValue.c_str()));
				else
					lua_pushstring(L, paramItem->m_strEditValue.c_str());
				lua_settable(L, -3);
			}
			else if(paramItem->m_strDlgStyle == "combobox")
			{
				lua_pushstring(L, paramItem->m_strCName.c_str());
				lua_pushinteger(L, paramItem->m_nComboboxValue);
				lua_settable(L, -3);
			}
			else if(paramItem->m_strDlgStyle == "comcheckbox")
			{
				lua_pushstring(L, paramItem->m_strCName.c_str());
				lua_newtable(L);
				for(size_t num = 0; num < paramItem->m_vtComCheckboxValue.size(); num++)
				{
					lua_pushinteger(L, num);
					lua_pushinteger(L, paramItem->m_vtComCheckboxValue[num]);
					lua_settable(L, -3);
				}
				lua_settable(L ,-3);
			}
		}
		lua_settable(L, -3);
	}

	lua_setfield(L, -2, NAME);	
}

void DeclareListBoxInt(CWnd* pWnd, lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME)
{
	ASSERT(lua_istable(L, -1));
	CListBox* pListBox = (CListBox*)(pWnd->GetDlgItem(DLGID));
	lua_newtable(L);
	for(int i = 0; i < pListBox->GetCount(); i++)
	{
		CString strValue;
		pListBox->GetText(i,strValue);
		lua_pushinteger(L, i);
		lua_pushinteger(L, atoi(strValue.GetBuffer()));
		lua_settable(L, -3);
	}
	lua_setfield(L, -2, NAME);
}

void DeclareEditInt(CWnd* pWnd, lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME, long DEFVAL)
{
	ASSERT(lua_istable(L, -1));
	CString strInt;
	pWnd->GetDlgItem(DLGID)->GetWindowText(strInt);strInt.Trim();
	int value = strInt.IsEmpty() ? DEFVAL : atoi(strInt.GetBuffer());

	lua_pushinteger(L, value);
	lua_setfield(L, -2, NAME);
}

void DeclareEditStr(CWnd* pWnd, lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME, const char* DEFVAL)
{
	ASSERT(lua_istable(L, -1));
	CString strStr;
	pWnd->GetDlgItem(DLGID)->GetWindowText(strStr);strStr.Trim();
	const char* value = strStr.IsEmpty() ? DEFVAL : strStr.GetBuffer();

	lua_pushstring(L, value);
	lua_setfield(L, -2, NAME);
}

void DeclareEditDouble(CWnd* pWnd, lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME, float DEFVAL)
{
	ASSERT(lua_istable(L, -1));
	CString strDouble;
	pWnd->GetDlgItem(DLGID)->GetWindowText(strDouble);strDouble.Trim();
	double value = strDouble.IsEmpty() ? DEFVAL : atof(strDouble.GetBuffer());

	lua_pushnumber(L, value);
	lua_setfield(L, -2, NAME);
}

void DeclareEditDefType(CWnd* pWnd, lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME, const char* DEFVAL)
{
	ASSERT(lua_istable(L, -1));
	CString strDefType;
	pWnd->GetDlgItem(DLGID)->GetWindowText(strDefType);strDefType.Trim();
	if(strDefType.IsEmpty())
		strDefType = DEFVAL;

	//空且无默认值，设置为-1
	if(strDefType.IsEmpty())
	{
		lua_pushinteger(L, -1);
		lua_setfield(L, -2, NAME);
		return;
	}

	CollectMainDefTypesT mapDefType;
	OriginToDefType(pWnd->GetDlgItem(DLGID),mapDefType);

	ASSERT(!mapDefType.empty());
	lua_pushinteger(L, mapDefType.begin()->first);
	lua_setfield(L, -2, NAME);
}

void DeclareEditDefTypeAndParams(CWnd* pWnd, lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME, const char* DEFVAL)
{
	ASSERT(lua_istable(L, -1));
	CString strDefType;
	pWnd->GetDlgItem(DLGID)->GetWindowText(strDefType);strDefType.Trim();
	if(strDefType.IsEmpty())
		strDefType = DEFVAL;

	//空且无默认值，设置为空表
	if(strDefType.IsEmpty())
	{
		lua_newtable(L);
		lua_setfield(L, -2, NAME);
		return;
	}

	return DeclareListBoxDefTypeAndParams(pWnd,L,ISWRITETODB,DLGID,NAME);
}

void DeclareTupleEditDefType(CWnd* pWnd, lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME, const char* DEFVAL)
{
	ASSERT(lua_istable(L, -1));
	CString strDefType;
	pWnd->GetDlgItem(DLGID)->GetWindowText(strDefType);strDefType.Trim();
	if(strDefType.IsEmpty())
		strDefType = DEFVAL;

	//空且无默认值，设置为空表
	if(strDefType.IsEmpty())
	{
		lua_newtable(L);
		lua_setfield(L, -2, NAME);
		return;
	}

	CollectMainDefTypesT mapDefType;
	OriginToDefType(pWnd->GetDlgItem(DLGID),mapDefType);

	lua_newtable(L);
	int num = 0;
	for(CollectMainDefTypesT::iterator mainItem = mapDefType.begin(); mainItem != mapDefType.end(); mainItem++)
	{
		lua_pushinteger(L, num);
		lua_pushinteger(L, mainItem->first);
		lua_settable(L, -3);
		num++;
	}

	lua_setfield(L, -2, NAME);	
}

void DeclareCheckBox(CWnd* pWnd, lua_State* L, bool ISWRITETODB, int DLGID, const char* NAME, bool DEFVAL)
{
	ASSERT(lua_istable(L, -1));
	CButton* pButton = (CButton*)(pWnd->GetDlgItem(DLGID));
	bool isCheck;
	if(pButton->GetCheck() == 0)
		isCheck = DEFVAL;
	else
		isCheck = true;

	lua_pushboolean(L, isCheck);
	lua_setfield(L, -2, NAME);
}

//===================================================================================
void ConvertGBKToUTF8(CString& strGBK)
{
	int len = MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, NULL, 0);
	unsigned short* wszUTF8 = new unsigned short[len + 1];
	memset(wszUTF8, 0, len * 2 + 2);
	MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, (LPWSTR)wszUTF8, len);

	len = WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)wszUTF8, -1, NULL, 0, NULL, NULL);
	char* szUTF8 = new char[len + 1];
	memset(szUTF8, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)wszUTF8, -1, szUTF8, len, NULL, NULL);

	strGBK = szUTF8;
	delete[] szUTF8;
	delete[] wszUTF8;
}

void ConvertUTF8ToGBK(CString& strUTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUTF8, -1, NULL, 0);
	unsigned short* wszGBK = new unsigned short[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUTF8, -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL, NULL);

	strUTF8 = szGBK;
	delete[] szGBK;
	delete[] wszGBK;
}