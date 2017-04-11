#include "stdafx.h"
#include "StaticAnalyzer.h"
#include "StaticAnalyzerDlg.h"

UINT FunctionList(LPVOID IParam)
{
	CStaticAnalyzerDlg* Dlg = (CStaticAnalyzerDlg* )IParam;
	CString savePath = Dlg->TargetPath.targetSavePath;
	CString tmpFilePath = savePath + "\\tmpFunction.txt";
	CString tmpDisasmPath = savePath + "\\tmpDisasm.txt";
	CString functionFilePath = savePath + "\\Function.txt";
	CString binaryFile = Dlg->TargetPath.targetBinaryFile;
	CString cmdStr;
	cmdStr.Format(_T(".\\nm.exe > %s %s -a -n -S"),tmpFilePath,binaryFile);
	system((CStringA)(cmdStr));
	cmdStr.Format(_T(".\\objdump.exe > %s %s -d"),tmpDisasmPath,binaryFile);
	system((CStringA)(cmdStr));


	while(!PathFileExists(tmpFilePath));
	CStdioFile tmpFuncFile;
	CStdioFile tmpDisasmFile;
	CFile functionFile;
	tmpFuncFile.Open(tmpFilePath,CFile::shareDenyNone | CFile::modeRead);
	tmpDisasmFile.Open(tmpDisasmPath,CFile::shareDenyNone | CFile::modeRead);
	functionFile.Open(functionFilePath,CFile::modeCreate | CFile::shareDenyNone | CFile::modeWrite);

	CString readBuf;
	while(tmpFuncFile.ReadString(readBuf)){
		if(readBuf.Find(_T(" T "),16) >= 0){
			readBuf.Append(_T("\r\n"));
			CT2CA outputString(readBuf, CP_UTF8);
			functionFile.Seek(0L, CFile::end);
			functionFile.Write(outputString,strlen(outputString));
		}else if(readBuf.Find(_T(" t "),16) >= 0){
			readBuf.Append(_T("\r\n"));
			CT2CA outputString(readBuf, CP_UTF8);
			functionFile.Seek(0L, CFile::end);
			functionFile.Write(outputString,strlen(outputString));
		}
	}
	readBuf.Format(_T("\r\n\r\n\r\n"));
	CT2CA outputString(readBuf, CP_UTF8);
	functionFile.Seek(0L, CFile::end);
	functionFile.Write(outputString,strlen(outputString));

	while(tmpDisasmFile.ReadString(readBuf)){
		if(readBuf.Find(_T(">:")) >= 0 && readBuf.Find(_T("<.L")) < 0){
			readBuf.Append(_T("\r\n"));
			CT2CA outputString(readBuf, CP_UTF8);
			functionFile.Seek(0L, CFile::end);
			functionFile.Write(outputString,strlen(outputString));
		}else if(readBuf.Find(_T("	call	")) >= 0){
			readBuf.Append(_T("\r\n"));
			CT2CA outputString(readBuf, CP_UTF8);
			functionFile.Seek(0L, CFile::end);
			functionFile.Write(outputString,strlen(outputString));
		}
	}

	tmpFuncFile.Close();
	tmpDisasmFile.Close();
	functionFile.Close();
	cmdStr.Format(_T("del %s"),tmpFilePath);
	system((CStringA)(cmdStr));
	cmdStr.Format(_T("del %s"),tmpDisasmPath);
	system((CStringA)(cmdStr));

	AfxMessageBox(_T("Function List Gen"));
	AfxGetMainWnd()->GetDlgItem(IDC_FUNCTION)->EnableWindow(TRUE);
	return 0;
}