// tapMain.cpp: 구현 파일
//

#include "pch.h"
#include "prjEzServoPlusR.h"
#include "afxdialogex.h"
#include "tapMain.h"


// tapMain 대화 상자

IMPLEMENT_DYNAMIC(tapMain, CDialogEx)

tapMain::tapMain(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAB_MAIN, pParent)
{

}

tapMain::~tapMain()
{
}

void tapMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(tapMain, CDialogEx)
END_MESSAGE_MAP()


// tapMain 메시지 처리기
