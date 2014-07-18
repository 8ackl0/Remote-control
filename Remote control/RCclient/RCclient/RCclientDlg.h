
// RCclientDlg.h : ͷ�ļ�
//

#pragma once


// CRCclientDlg �Ի���
class CRCclientDlg : public CDialogEx
{
// ����
public:
	CRCclientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RCCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	//������������
enum CmdType {CT_Confirm ,CT_HideDesk,CT_ShowDesk,CT_ShutDown,
              CT_ReRun,CT_Logout,CT_SendText,CT_CloseClient};
	typedef struct DataPackage
{
	CmdType  MsgType;	//��������
	char  Content[512]; //�ı���Ϣ
};

	ULONG_PTR m_pGdiToken;
	GdiplusStartupInput m_gdiplusStartupInput;

	int  SendData(UINT index, int mod , int bmpsize ,int totalsize ,int frames,char *pSendBuf, sockaddr_in &addr);
	void OnCancel();

	SOCKET  m_Socket;		//�ͻ����׽���
	CString m_ServerIP;		//������IP
	BOOL	m_Confirm;		//ȷ����Ϣ
	int     m_FrameIndex;	//��¼����
	int     m_Mod;			//��¼����
	int     m_Count;		//ÿһ��λͼ�ֱ�������
	int     m_Bmpsize;		//λͼ��ʵ�ʴ�С
	char*	m_pSendBuf;		//���ͻ�����ָ��
	char*	m_pHeader;		//��ʱָ��
	sockaddr_in  m_Addr;	//�׽��ֵ�ַ
	BOOL    m_Received;		//�Ƿ���յ�������������ȷ����Ϣ
	int     m_Counter;		//������
	DWORD	m_JpegSize;		//JPEG�����ܴ�С


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnReceived(WPARAM wParam,LPARAM lParam);
	virtual void OnOK();
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
};
