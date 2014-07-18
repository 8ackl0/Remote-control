
// RCserverDlg.h : ͷ�ļ�
//

#pragma once
#include"ControlList.h"

#define CM_RECEIVED  WM_USER+1001
// CRCserverDlg �Ի���
class CRCserverDlg : public CDialogEx
{
// ����
public:
	CRCserverDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RCSERVER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	/**************************������������***********************
CT_Confirm: ȷ����Ϣ
CT_HideDesk:��������
CT_ShowDesk:��ʾ����
CT_ShutDown:�ػ�
CT_ReRun:��������
CT_Logout:ע��
CT_SendText:�����ı�
CT_CloseClient:�����ͻ��˳���
**************************************************************/
	enum CmdType {CT_Confirm ,CT_HideDesk,CT_ShowDesk,CT_ShutDown,
				CT_ReRun,CT_Logout,CT_SendText,CT_CloseClient};
	typedef struct DataPackage
	{
		CmdType	MsgType;		//��������
		char	Content[512];	//�ı���Ϣ,ֻ������������ΪCT_SendTextʱ�Ϸ�
	};

	CControlList  m_CtrlList;
	ULONG_PTR m_pGdiToken;
    GdiplusStartupInput m_gdiplusStartupInput;

	SOCKET  m_Socket;					//�����׽���
	char*	m_BmpData;					//λͼ����ָ��
	char*   m_Header;					//��ʱָ��
	BOOL    m_ShowBmp;					//�Ƿ���ʾλͼ
	char*   m_TempData;					//��ʱ����ָ��
	int     m_BmpSize;					//��¼λͼ��С
	int     m_RecSize;					//ʵ�ʽ��յĴ�С
	CString m_ClientIP;					//�ͻ���IP
	UINT    m_ClientPort;				//�ͻ��˶˿�
	DWORD   m_JPGSize;					//JPG���ݵ��ܴ�С	
	BOOL    m_IsScreen;					//�Ƿ�ȫ����ʾ
	WINDOWPLACEMENT m_OldPlacement;		//ԭ���Ĵ���ģʽ
	CRect m_FullScreenRect;				//ȫ����ʾ����
	IStream *m_pStm;
	Bitmap  *m_pNewBmp;
	sockaddr_in m_Addr;

	LRESULT OnReceived(WPARAM wParam,LPARAM lParam);
	void ShowJPEG(void* pData,int DataSize);
	virtual BOOL PreTranslateMessage(MSG* pMsg);//��д��Ϣ������ʵ�ֿ�ݼ�

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};
