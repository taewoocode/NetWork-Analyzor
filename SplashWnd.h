#pragma once

#include "resource.h"

// CSplashWnd 대화 상자

class CSplashWnd : public CDialogEx
{
	DECLARE_DYNAMIC(CSplashWnd)

public:
	CSplashWnd(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CSplashWnd();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CSplashWnd };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	void Show(UINT uMilliseconds = 3000);

public:
	CDC MemDC;
	BITMAP bmpInfo;
	CBitmap bmp;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
