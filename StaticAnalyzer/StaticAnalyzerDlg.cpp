
// StaticAnalyzerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "StaticAnalyzer.h"
#include "StaticAnalyzerDlg.h"
#include "afxdialogex.h"
#include <Shlobj.h>
//#include "functions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static int CALLBACK BrowseForFolder_CallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

// CStaticAnalyzerDlg 대화 상자
CStaticAnalyzerDlg::CStaticAnalyzerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStaticAnalyzerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStaticAnalyzerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CStaticAnalyzerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_FILE_OPEN, &CStaticAnalyzerDlg::OnBnClickedFileOpen)
	ON_BN_CLICKED(IDC_SAVE_PATH, &CStaticAnalyzerDlg::OnBnClickedSavePath)
	ON_BN_CLICKED(IDC_SECTION, &CStaticAnalyzerDlg::OnBnClickedSection)
	ON_BN_CLICKED(IDC_SYMBOL, &CStaticAnalyzerDlg::OnBnClickedSymbol)
	ON_BN_CLICKED(IDC_FUNCTION, &CStaticAnalyzerDlg::OnBnClickedFunction)
	ON_BN_CLICKED(IDC_OBJECT, &CStaticAnalyzerDlg::OnBnClickedObject)
END_MESSAGE_MAP()


// CStaticAnalyzerDlg 메시지 처리기

BOOL CStaticAnalyzerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	
	GetDlgItem(IDC_EDIT_BINARY)->EnableWindow(FALSE);
	GetDlgItem(IDC_FILE_OPEN)->EnableWindow(FALSE);
	GetDlgItem(IDC_SECTION)->EnableWindow(FALSE);
	GetDlgItem(IDC_SYMBOL)->EnableWindow(FALSE);
	GetDlgItem(IDC_FUNCTION)->EnableWindow(FALSE);
	GetDlgItem(IDC_OBJECT)->EnableWindow(FALSE);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CStaticAnalyzerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CStaticAnalyzerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CStaticAnalyzerDlg::OnBnClickedFileOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog insDlg(TRUE, NULL, _T("*.elf"), OFN_HIDEREADONLY | OFN_NOCHANGEDIR, _T("ELF Files (*.elf)|*.elf||"));
	CString currentDirName;
	GetDlgItemText(IDC_EDIT_PATH, currentDirName);
	insDlg.m_ofn.lpstrInitialDir =  currentDirName; 
	// 파일 대화상자 실행
	if(insDlg.DoModal() == IDOK){
		TargetPath.targetBinaryFile = insDlg.m_ofn.lpstrFile;
		SetDlgItemText(IDC_EDIT_BINARY, TargetPath.targetBinaryFile);
		GetDlgItem(IDC_EDIT_BINARY)->EnableWindow(FALSE);
		GetDlgItem(IDC_SECTION)->EnableWindow(TRUE);
		GetDlgItem(IDC_SYMBOL)->EnableWindow(TRUE);
		GetDlgItem(IDC_FUNCTION)->EnableWindow(TRUE);
		GetDlgItem(IDC_OBJECT)->EnableWindow(TRUE);
	}  
}

void CStaticAnalyzerDlg::OnBnClickedSavePath()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	wchar_t currentDirName[MAX_PATH];
	wchar_t selectPath[MAX_PATH];
    BROWSEINFO   pathInfo; 
    LPITEMIDLIST  pathIdList;
 
    memset(&pathInfo, 0x00, sizeof(BROWSEINFO));
    pathInfo.hwndOwner   =   this->m_hWnd; 
	GetCurrentDirectory(MAX_PATH, currentDirName);
	pathInfo.lpfn = BrowseForFolder_CallbackProc;
	pathInfo.lParam = (LPARAM)(LPCSTR)currentDirName;
    pathInfo.ulFlags  =  BIF_EDITBOX| BIF_NEWDIALOGSTYLE; 
 
    pathIdList = SHBrowseForFolder(&pathInfo);
    if (pathIdList) { 
		SHGetPathFromIDList(pathIdList, selectPath);
		TargetPath.targetSavePath = selectPath;
		SetDlgItemText(IDC_EDIT_PATH, TargetPath.targetSavePath);
		GetDlgItem(IDC_EDIT_PATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BINARY)->EnableWindow(TRUE);
		GetDlgItem(IDC_FILE_OPEN)->EnableWindow(TRUE);
    }
}


void CStaticAnalyzerDlg::OnBnClickedSection()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_SECTION)->EnableWindow(FALSE);
	sectionTabelThread = AfxBeginThread(SectionTable, (LPVOID)this);	

}


void CStaticAnalyzerDlg::OnBnClickedSymbol()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_SYMBOL)->EnableWindow(FALSE);
	symbolListThread = AfxBeginThread(SymbolList, (LPVOID)this);	
}


void CStaticAnalyzerDlg::OnBnClickedFunction()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_FUNCTION)->EnableWindow(FALSE);
	functionListThread = AfxBeginThread(FunctionList, (LPVOID)this);	
}


void CStaticAnalyzerDlg::OnBnClickedObject()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_OBJECT)->EnableWindow(FALSE);
	objectFileListThread = AfxBeginThread(ObjectFileList, (LPVOID)this);	

}

static int CALLBACK BrowseForFolder_CallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if(uMsg == BFFM_INITIALIZED)
	SendMessage(hwnd, BFFM_SETSELECTION, (WPARAM)TRUE, (LPARAM)lpData);
	return 0;
}
