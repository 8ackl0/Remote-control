#pragma once
#include "afxwin.h"


// CControlList 对话框

class CControlList : public CDialogEx
{
	DECLARE_DYNAMIC(CControlList)

public:
	CControlList(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CControlList();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

public:
		enum CmdType {CT_Confirm ,CT_HideDesk,CT_ShowDesk,CT_ShutDown,
				CT_ReRun,CT_Logout,CT_SendText,CT_CloseClient};
	typedef struct DataPackage
	{
		CmdType	MsgType;		//命令类型
		char	Content[512];	//文本信息,只有在命令类型为CT_SendText时合法
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Text;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
};
