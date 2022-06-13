#pragma once
#include "afxdialogex.h"


// tapMain 대화 상자

class tapMain : public CDialogEx
{
	DECLARE_DYNAMIC(tapMain)

public:
	tapMain(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~tapMain();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB_MAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
