// SplashWnd.cpp: 구현 파일
//

#include "pch.h"
#include "SCHtuning.h"
#include "SplashWnd.h"
#include "afxdialogex.h"


// CSplashWnd 대화 상자

IMPLEMENT_DYNAMIC(CSplashWnd, CDialogEx)

CSplashWnd::CSplashWnd(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CSplashWnd, pParent)
{

}

CSplashWnd::~CSplashWnd()
{
}

void CSplashWnd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSplashWnd, CDialogEx)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSplashWnd 메시지 처리기

void CSplashWnd::Show(UINT uMilliseconds)
{
    if (Create(IDD_SPLASH, GetDesktopWindow()))
    {
        int img_w = 792;
        int img_h = 792;

        CRect rect;
        GetWindowRect(&rect);
        MoveWindow(360,80, img_w, img_h);

        ShowWindow(SW_SHOWNORMAL);
        SetTimer(100, uMilliseconds, NULL);
    }
}

BOOL CSplashWnd::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  여기에 추가 초기화 작업을 추가합니다.

    SetLayeredWindowAttributes(RGB(255, 255, 255), 0, LWA_COLORKEY);

    CClientDC dc(this);

    MemDC.CreateCompatibleDC(&dc);
    bmp.LoadBitmapW(IDB_BITMAP_SCH_MARK);
    bmp.GetBitmap(&bmpInfo);

    Invalidate(false);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CSplashWnd::OnPaint()
{
	    CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.

        CBitmap* pOldBmp = NULL;

        pOldBmp = MemDC.SelectObject(&bmp);

        dc.BitBlt(0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, &MemDC, 0, 0, SRCCOPY);

        MemDC.SelectObject(pOldBmp);
}


void CSplashWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

    DestroyWindow();

	CDialogEx::OnTimer(nIDEvent);
}
