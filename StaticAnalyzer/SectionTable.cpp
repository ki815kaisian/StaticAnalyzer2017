#include "stdafx.h"
#include "StaticAnalyzer.h"
#include "StaticAnalyzerDlg.h"

UINT SectionTable(LPVOID IParam)
{
	CStaticAnalyzerDlg* Dlg = (CStaticAnalyzerDlg* )IParam;
	CString savePath = Dlg->TargetPath.targetSavePath;
	CString tmpFilePath = savePath + "\\tmpSection.txt";
	CString sectionFilePath = savePath + "\\Section.txt";
	CString binaryFile = Dlg->TargetPath.targetBinaryFile;
	CString cmdStr;
	cmdStr.Format(_T(".\\objdump.exe > %s -h -w %s"),tmpFilePath,binaryFile);
	system((CStringA)(cmdStr));
	
	while(!PathFileExists(tmpFilePath));
	CStdioFile tmpFile;
	CFile sectionFile;
	tmpFile.Open(tmpFilePath,CFile::shareDenyNone | CFile::modeRead);
	sectionFile.Open(sectionFilePath,CFile::modeCreate | CFile::shareDenyNone | CFile::modeWrite);

	CString readBuf;
	while(tmpFile.ReadString(readBuf)){
		if(readBuf.Find(_T("ALLOC")) >= 0){
			readBuf.Append(_T("\r\n"));
			CT2CA outputString(readBuf, CP_UTF8);
			sectionFile.Seek(0L, CFile::end);
			sectionFile.Write(outputString,strlen(outputString));
		}
	}
	tmpFile.Close();
	sectionFile.Close();
	cmdStr.Format(_T("del %s"),tmpFilePath);
	system((CStringA)(cmdStr));

	AfxMessageBox(_T("Section Table Gen"));
	AfxGetMainWnd()->GetDlgItem(IDC_SECTION)->EnableWindow(TRUE);
	return 0;
}