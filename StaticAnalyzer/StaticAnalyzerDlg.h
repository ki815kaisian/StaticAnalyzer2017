
// StaticAnalyzerDlg.h : 헤더 파일
//

#pragma once


// CStaticAnalyzerDlg 대화 상자
class CStaticAnalyzerDlg : public CDialogEx
{
// 생성입니다.
public:
	CStaticAnalyzerDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_STATICANALYZER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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
