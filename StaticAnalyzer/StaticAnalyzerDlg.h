
// StaticAnalyzerDlg.h : ��� ����
//

#pragma once


// CStaticAnalyzerDlg ��ȭ ����
class CStaticAnalyzerDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CStaticAnalyzerDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_STATICANALYZER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedFileOpen();
	afx_msg void OnBnClickedSavePath();
	afx_msg void OnBnClickedSection();
	afx_msg void OnBnClickedSymbol();
	afx_msg void OnBnClickedFunction();
	afx_msg void OnBnClickedObject();

	CWinThread* sectionTabelThread;
	CWinThread* symbolListThread;
	CWinThread* functionListThread;
	CWinThread* objectFileListThread;

	PInfo TargetPath;
};
