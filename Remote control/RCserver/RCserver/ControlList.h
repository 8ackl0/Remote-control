#pragma once
#include "afxwin.h"


// CControlList �Ի���

class CControlList : public CDialogEx
{
	DECLARE_DYNAMIC(CControlList)

public:
	CControlList(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CControlList();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

public:
		enum CmdType {CT_Confirm ,CT_HideDesk,CT_ShowDesk,CT_ShutDown,
				CT_ReRun,CT_Logout,CT_SendText,CT_CloseClient};
	typedef struct DataPackage
	{
		CmdType	MsgType;		//��������
		char	Content[512];	//�ı���Ϣ,ֻ������������ΪCT_SendTextʱ�Ϸ�
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
