
// SCHtuningView.cpp: CSCHtuningView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "SCHtuning.h"
#endif

#include "SCHtuningDoc.h"
#include "SCHtuningView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

UINT ThreadProc(LPVOID lParam)
{
	CSCHtuningView* pView = (CSCHtuningView*)lParam;

	while (true)
	{
		if (pView->m_thread_flag == 1)
			pView->proc_unit();
	}
	return 0;
}


// CSCHtuningView

IMPLEMENT_DYNCREATE(CSCHtuningView, CFormView)

BEGIN_MESSAGE_MAP(CSCHtuningView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDC_BTN_START, &CSCHtuningView::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_END, &CSCHtuningView::OnBnClickedBtnEnd)
	//ON_BN_CLICKED(IDC_BTN_INIT, &CSCHtuningView::OnBnClickedBtnInit)
	ON_BN_CLICKED(IDC_BTN_CW_01, &CSCHtuningView::OnBnClickedBtnCw01)
	ON_BN_CLICKED(IDC_BTN_CW_02, &CSCHtuningView::OnBnClickedBtnCw02)
	ON_BN_CLICKED(IDC_BTN_CW_03, &CSCHtuningView::OnBnClickedBtnCw03)
	ON_BN_CLICKED(IDC_BTN_CW_04, &CSCHtuningView::OnBnClickedBtnCw04)
	ON_BN_CLICKED(IDC_BTN_CW_05, &CSCHtuningView::OnBnClickedBtnCw05)
	ON_BN_CLICKED(IDC_BTN_CW_06, &CSCHtuningView::OnBnClickedBtnCw06)
	ON_BN_CLICKED(IDC_BTN_CCW_01, &CSCHtuningView::OnBnClickedBtnCcw01)
	ON_BN_CLICKED(IDC_BTN_CCW_02, &CSCHtuningView::OnBnClickedBtnCcw02)
	ON_BN_CLICKED(IDC_BTN_CCW_03, &CSCHtuningView::OnBnClickedBtnCcw03)
	ON_BN_CLICKED(IDC_BTN_CCW_04, &CSCHtuningView::OnBnClickedBtnCcw04)
	ON_BN_CLICKED(IDC_BTN_CCW_05, &CSCHtuningView::OnBnClickedBtnCcw05)
	ON_BN_CLICKED(IDC_BTN_CCW_06, &CSCHtuningView::OnBnClickedBtnCcw06)
	//ON_BN_CLICKED(IDC_BTN_SERVER_START, &CSCHtuningView::OnBnClickedBtnServerStart)
	//ON_BN_CLICKED(IDC_BTN_SERVER_STOP, &CSCHtuningView::OnBnClickedBtnServerStop)
	//ON_BN_CLICKED(IDC_RADIO_MAT, &CSCHtuningView::OnBnClickedRadioMat)
	//ON_BN_CLICKED(IDC_RADIO_MFC, &CSCHtuningView::OnBnClickedRadioMfc)
	//ON_EN_CHANGE(IDC_EDIT_RECV, &CSCHtuningView::OnEnChangeEditRecv)
	//ON_LBN_SELCHANGE(IDC_LIST_01, &CSCHtuningView::OnLbnSelchangeList01)
	ON_BN_CLICKED(IDC_BTN_USB_OPEN, &CSCHtuningView::OnBnClickedBtnUsbOpen)
	ON_BN_CLICKED(IDC_BTN_USB_CLOSE, &CSCHtuningView::OnBnClickedBtnUsbClose)
	ON_MESSAGE(WM_COMM_READ, OnCommunication)
END_MESSAGE_MAP()

// CSCHtuningView 생성/소멸

CSCHtuningView::CSCHtuningView() noexcept
	: CFormView(IDD_SCHTUNING_FORM)
{
	// TODO: 여기에 생성 코드를 추가합니다.

	m_pThread = NULL;

	m_SerialComm = NULL;

}

CSCHtuningView::~CSCHtuningView()
{
	if (m_pThread != NULL)
	{
		delete m_pThread;
		m_pThread = NULL;
	}

	if (m_SerialComm != NULL)
	{
		delete m_SerialComm;
		m_SerialComm = NULL;
	}
}

void CSCHtuningView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_LIST_01, m_List);
}

BOOL CSCHtuningView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	pView = this;

	return CFormView::PreCreateWindow(cs);
}

void CSCHtuningView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	RECT rect_win;
	AfxGetMainWnd()->GetWindowRect(&rect_win);
	AfxGetMainWnd()->MoveWindow(rect_win.left, rect_win.top, 1920, 1200);

	GetParentFrame()->SetWindowText(_T("Tuning"));

	m_endian_flag = 1;

	CButton* pBtn = NULL;

	//pBtn = (CButton*)GetDlgItem(IDC_RADIO_MFC);
	//pBtn->SetCheck(false);

	//pBtn = (CButton*)GetDlgItem(IDC_RADIO_MATLAB);
	//pBtn->SetCheck(true);


	CWnd* pWnd = NULL;

	//pWnd = GetDlgItem(IDC_BTN_SERVER_START);
	//pWnd->EnableWindow(true);

	//pWnd = GetDlgItem(IDC_BTN_SERVER_STOP);
	//pWnd->EnableWindow(false);

	m_dc_w = 840;
	m_dc_h = 200+50;

	m_dc_x = 600;
	m_dc_y = 30;

	m_pt_filter = CPoint(40, 40);

	m_pt_screw[0] = CPoint(3, 2);
	m_pt_screw[1] = CPoint(130, 2);
	m_pt_screw[2] = CPoint(257, 2);
	m_pt_screw[3] = CPoint(384, 2);
	m_pt_screw[4] = CPoint(511, 2);
	m_pt_screw[5] = CPoint(638, 2);
	//m_pt_screw[6] = CPoint(1048, 50);

	m_thread_delay = 200;

	m_cur = 0;

	m_screw_id = 0;
	m_rot_id = 0;

	init_dc_buffer(this);

	m_dc_x_image = 200;
	m_dc_y_image = 300;
	m_dc_w_image = 1110;
	m_dc_h_image = 470;

	init_dc_buffer_image(this);


	m_thread_flag = 0;

	m_Image_filter.Load(_T("filter_image.png"));

	CString str;

	for (int i = 0; i < 60; i++)
	{
		str.Format(_T("arrow_cw_0%d.png"), i + 1);
		m_Image_cw[i].Load(str);

		str.Format(_T("arrow_ccw_0%d.png"), i + 1);
		m_Image_ccw[i].Load(str);
	}

	proc_unit();

	m_port = 1;
	m_baud_rate = 9600;

	search_usb_port();

	load_config();

	m_graph_id_cur = 0;
	m_graph_id_prev = 0;
}

void CSCHtuningView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSCHtuningView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CSCHtuningView 진단

#ifdef _DEBUG
void CSCHtuningView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSCHtuningView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSCHtuningDoc* CSCHtuningView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSCHtuningDoc)));
	return (CSCHtuningDoc*)m_pDocument;
}
#endif //_DEBUG


// CSCHtuningView 메시지 처리기

void CSCHtuningView::proc_recv(TCHAR buf[])
{
	// TODO: 여기에 구현 코드 추가.

	// mfc client : 0, matlab client : 1
	/*
	if (m_endian_flag == 0)
	{
		CString str_buf;

		str_buf.Format(_T("%s"), buf);

		//CWnd* pWnd = GetDlgItem(IDC_EDIT_RECV);
		//pWnd->SetWindowTextW(str_buf);
	}
	else
	{
		double a_value[8 * 8];

		for (int i = 0; i < 8 * 8; i++)
			a_value[i] = 0;

		CString str_buf;
		CString str;

		str_buf.Format(_T("%s"), buf);

		int cur_01 = 0;
		int cur_02 = 0;

		int cur_value = 0;

		while (true)
		{
			if (buf[cur_02] == 0)
			{
				if (cur_value < 8 * 8)
				{
					str = str_buf.Mid(cur_01, str_buf.GetLength() - cur_01);
					a_value[cur_value] = _ttof(str);
				}
				break;
			}

			if (buf[cur_02] == ',')
			{
				str = str_buf.Mid(cur_01, cur_02 - cur_01);
				a_value[cur_value++] = _ttof(str);
				cur_01 = cur_02 + 1;
			}
			cur_02++;
		}

		//CWnd* pWnd = GetDlgItem(IDC_EDIT_RECV);

		str_buf = _T("");

		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				if (i < 8 - 1)
				{
					if (j < 8 - 1)
						str.Format(_T("%f, "), a_value[i * 8 + j]);
					else
						str.Format(_T("%f, \r\n"), a_value[i * 8 + j]);
				}
				else
				{
					if (j < 8 - 1)
						str.Format(_T("%f, "), a_value[i * 8 + j]);
					else
						str.Format(_T("%f\r\n"), a_value[i * 8 + j]);
				}

				str_buf += str;
			}
		}
		pWnd->SetWindowTextW(str_buf);
	}*/
}

void CSCHtuningView::OnBnClickedBtnStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_thread_flag = 1;
	m_cur = 0;

	if (m_pThread == NULL)
		m_pThread = AfxBeginThread(ThreadProc, this);
}


void CSCHtuningView::OnBnClickedBtnEnd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	if (m_pThread != NULL)
	{
		proc_stop();

		m_thread_flag = 0;

		HANDLE nThread = m_pThread->m_hThread;
		DWORD nExitCode = 0;
		::TerminateThread(nThread, nExitCode);

		delete m_pThread;
		m_pThread = NULL;
	}

}


/*void CSCHtuningView::OnBnClickedBtnInit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	m_thread_flag = 0;

	m_screw_id = 0;
	m_rot_id = 0;

	proc_unit();
}*/


void CSCHtuningView::OnBnClickedBtnCw01()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	change_screw();

	m_screw_id = 1;
	m_rot_id = 0;

	m_cur = 0;

	m_screw_value[0] = "시계 방향으로 회전하세요";
}


void CSCHtuningView::OnBnClickedBtnCw02()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	change_screw();

	m_screw_id = 2;
	m_rot_id = 0;

	m_cur = 0;

	m_screw_value[0] = "시계 방향으로 회전하세요";
}


void CSCHtuningView::OnBnClickedBtnCw03()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	change_screw();

	m_screw_id = 3;
	m_rot_id = 0;

	m_cur = 0;

	m_screw_value[0] = "시계 방향으로 회전하세요";
}


void CSCHtuningView::OnBnClickedBtnCw04()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	change_screw();

	m_screw_id = 4;
	m_rot_id = 0;

	m_cur = 0;

	m_screw_value[0] = "시계 방향으로 회전하세요";
}


void CSCHtuningView::OnBnClickedBtnCw05()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	change_screw();

	m_screw_id = 5;
	m_rot_id = 0;

	m_cur = 0;

	m_screw_value[0] = "시계 방향으로 회전하세요";
}


void CSCHtuningView::OnBnClickedBtnCw06()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	change_screw();

	m_screw_id = 6;
	m_rot_id = 0;

	m_cur = 0;

	m_screw_value[0] = "시계 방향으로 회전하세요";
}


void CSCHtuningView::OnBnClickedBtnCcw01()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	change_screw();

	m_screw_id = 1;
	m_rot_id = 1;

	m_cur = 0;

	m_screw_value[0] = "시계 반대 방향으로 회전하세요";
}


void CSCHtuningView::OnBnClickedBtnCcw02()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	change_screw();

	m_screw_id = 2;
	m_rot_id = 1;

	m_cur = 0;

	m_screw_value[0] = "시계 반대 방향으로 회전하세요";
}


void CSCHtuningView::OnBnClickedBtnCcw03()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	change_screw();

	m_screw_id = 3;
	m_rot_id = 1;

	m_cur = 0;

	m_screw_value[0] = "시계 반대 방향으로 회전하세요";
}


void CSCHtuningView::OnBnClickedBtnCcw04()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	change_screw();

	m_screw_id = 4;
	m_rot_id = 1;

	m_cur = 0;

	m_screw_value[0] = "시계 반대 방향으로 회전하세요";
}


void CSCHtuningView::OnBnClickedBtnCcw05()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	change_screw();

	m_screw_id = 5;
	m_rot_id = 1;

	m_cur = 0;

	m_screw_value[0] = "시계 반대 방향으로 회전하세요";
}


void CSCHtuningView::OnBnClickedBtnCcw06()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	change_screw();

	m_screw_id = 6;
	m_rot_id = 1;

	m_cur = 0;

	m_screw_value[0] = "시계 반대 방향으로 회전하세요";
}


void CSCHtuningView::OnBnClickedBtnServerStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	/*
	CWnd* pWnd = NULL;

	pWnd = GetDlgItem(IDC_BTN_SERVER_START);
	pWnd->EnableWindow(false);

	pWnd = GetDlgItem(IDC_BTN_SERVER_STOP);
	pWnd->EnableWindow(true);

	//m_endian_flag = 0;

	CButton* pWnd_01 = (CButton*)GetDlgItem(IDC_RADIO_MFC);
	CButton* pWnd_02 = (CButton*)GetDlgItem(IDC_RADIO_MATLAB);

	bool flag_01 = pWnd_01->GetCheck();
	bool flag_02 = pWnd_02->GetCheck();

	if (flag_01)
		m_endian_flag = 0;

	if (flag_02)
		m_endian_flag = 1;

	if (m_ListenSocket.Create(21000, SOCK_STREAM))
	{
		if (!m_ListenSocket.Listen())
		{
			AfxMessageBox(L"ERROR: Listen() return FALSE");
		}
	}
	else
	{
		AfxMessageBox(L"ERROR: Failed to create server socket!");
	}*/
}


void CSCHtuningView::OnBnClickedBtnServerStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	/*
	CWnd* pWnd = NULL;

	pWnd = GetDlgItem(IDC_BTN_SERVER_START);
	pWnd->EnableWindow(true);

	pWnd = GetDlgItem(IDC_BTN_SERVER_STOP);
	pWnd->EnableWindow(false);

	POSITION pos;
	pos = m_ListenSocket.m_ptrClientSocketList.GetHeadPosition();
	CClientSocket* pClient = NULL;

	while (pos != NULL)
	{
		pClient = (CClientSocket*)m_ListenSocket.m_ptrClientSocketList.GetNext(pos);

		if (pClient != NULL)
		{
			pClient->ShutDown();
			pClient->Close();

			delete pClient;
		}
	}
	m_ListenSocket.ShutDown();
	m_ListenSocket.Close();*/
}


void CSCHtuningView::OnBnClickedRadioMat()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//m_endian_flag = 1;
}


void CSCHtuningView::OnBnClickedRadioMfc()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//m_endian_flag = 0;
}


void CSCHtuningView::OnEnChangeEditRecv()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CFormView::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CSCHtuningView::OnLbnSelchangeList01()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CSCHtuningView::OnDraw(CDC* pDC)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (m_dc)
		pDC->BitBlt(m_dc_x, m_dc_y, m_dc_w, m_dc_h, &m_dc, 0, 0, SRCCOPY);

	if (m_dc_image)
		pDC->BitBlt(m_dc_x_image, m_dc_y_image, m_dc_w_image, m_dc_h_image, &m_dc_image, 0, 0, SRCCOPY);

}

void CSCHtuningView::init_dc_buffer(CWnd* pWnd)
{
	// TODO: 여기에 구현 코드 추가.

	CDC* pDC = pWnd->GetDC();

	m_dc.CreateCompatibleDC(pDC);
	m_bitmap.CreateCompatibleBitmap(pDC, m_dc_w, m_dc_h);
	CBitmap* pOldBitmap = (CBitmap*)m_dc.SelectObject(&m_bitmap);

	CRect rect(0, 0, m_dc_w, m_dc_h);
	CBrush Brush;
	Brush.CreateSolidBrush(RGB(255, 255, 255));

	m_dc.FillRect(rect, &Brush);
}

void CSCHtuningView::init_dc_buffer_image(CWnd* pWnd)
{
	// TODO: 여기에 구현 코드 추가.

	CDC* pDC = pWnd->GetDC();

	m_dc_image.CreateCompatibleDC(pDC);
	m_bitmap_image.CreateCompatibleBitmap(pDC, m_dc_w_image, m_dc_h_image);
	CBitmap* pOldBitmap = (CBitmap*)m_dc_image.SelectObject(&m_bitmap_image);

	CRect rect(0, 0, m_dc_w_image, m_dc_h_image);
	CBrush Brush;
	Brush.CreateSolidBrush(RGB(255, 255, 255));

	m_dc_image.FillRect(rect, &Brush);
}

void CSCHtuningView::proc_unit()
{
	// TODO: 여기에 구현 코드 추가.

	if (m_cur > 100)
		m_cur = 0;

	//m_Image_filter.BitBlt(m_dc, m_pt_filter.x, m_pt_filter.y);
	m_Image_filter.BitBlt(m_dc, 0, 0);


	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	m_dc.SelectObject(&pen);

	CBrush brush;
	brush.CreateSolidBrush(RGB(255, 255, 255));
	m_dc.SelectObject(&brush);

	m_dc.FillRect(CRect(250, 200, 350+300, 250), &brush);


	CFont font;
	font.CreatePointFont(50000, _T("굴림체"));
	CString str_screw[7];
	str_screw[0].Format(_T("%s"), m_screw_value[0]);

	m_dc.TextOutW(320, 200, str_screw[0]);

	if (m_screw_id == 0)
	{
		InvalidateRect(CRect(m_dc_x, m_dc_y, m_dc_x + m_dc_w, m_dc_y + m_dc_h), false);

		m_cur++;

		Sleep(m_thread_delay);

		return;
	}

	int a_cur = m_cur % 8;

	if (m_rot_id == 0)
		m_Image_cw[a_cur].TransparentBlt(m_dc, m_pt_screw[m_screw_id - 1].x,
			m_pt_screw[m_screw_id - 1].y, m_Image_cw[a_cur].GetWidth(),
			m_Image_cw[a_cur].GetHeight(), RGB(255, 255, 255));
	else
		m_Image_ccw[a_cur].TransparentBlt(m_dc, m_pt_screw[m_screw_id - 1].x,
			m_pt_screw[m_screw_id - 1].y, m_Image_ccw[a_cur].GetWidth(),
			m_Image_ccw[a_cur].GetHeight(), RGB(255, 255, 255));

	m_cur++;

	InvalidateRect(CRect(m_dc_x, m_dc_y, m_dc_x + m_dc_w, m_dc_y + m_dc_h), false);

	Sleep(m_thread_delay);
}

void CSCHtuningView::change_screw()
{
	// TODO: 여기에 구현 코드 추가.

	m_thread_flag = 0;
	Sleep(m_thread_delay * 2);

	m_thread_flag = 1;
}

void CSCHtuningView::proc_stop()
{
	// TODO: 여기에 구현 코드 추가.

	m_screw_id = 0;
	m_Image_filter.BitBlt(m_dc, m_pt_filter.x, m_pt_filter.y);
	Invalidate(true);
}

LRESULT CSCHtuningView::OnCommunication(WPARAM wParam, LPARAM lPararm)
{
	display(m_SerialComm->m_RecvMessage);
	return TRUE;
}

void CSCHtuningView::display(CString arg_msg)
{
	// TODO: 여기에 구현 코드 추가.

	CString str_unit;

	int cur_01 = 0;
	int cur_02 = 2;

	int count = 0;
	while (1)
	{
		cur_02 = arg_msg.Find(_T("\n"), cur_01);

		if (cur_02 >= 0)
		{
			cur_01 = cur_02 + 1;
			count++;
		}
		else
		{
			break;
		}
	}

	cur_01 = 0;
	int len = 0;
	for (int i = 0; i < count; i++)
	{
		cur_02 = arg_msg.Find(_T("\n"), cur_01);
		str_unit = arg_msg.Mid(cur_01, cur_02 - cur_01);

		len = str_unit.GetLength();

		if (len >= 0)
			receive_serial(str_unit);


		cur_01 = cur_02 + 1;
	}
}

CString CSCHtuningView::load_file(CString filename)
{
	// TODO: 여기에 구현 코드 추가.

	CString str;

	CFile file;
	if (!file.Open(filename, CFile::modeRead))
	{
		MessageBox(_T("파일을 열지 못했습니다."), _T("경고!"), MB_OK | MB_ICONHAND);
		return str;
	}

	UINT len = (UINT)file.GetLength();
	char* buf = new char[len];

	file.Read(buf, len);
	file.Close();

	buf[len] = '\0';
	len++;

	str = (LPSTR)buf;

	return str;
}

void CSCHtuningView::load_config()
{
	// TODO: 여기에 구현 코드 추가.

	CString str_total = load_file(_T("01_config.txt"));

	int len = str_total.GetLength();

	if (len == 0)
	{
		CString str;

		str.Format(_T("%d"), m_baud_rate);

		CWnd* pWnd_02 = GetDlgItem(IDC_EDIT_BAUD_RATE);
		pWnd_02->SetWindowText(str);

		return;
	}

	CString str_column;
	CString str_data;
	int cur_01, cur_02;

	cur_01 = 0;
	cur_02 = 0;

	for (int i = 0; i < 2; i++)
	{
		cur_02 = str_total.Find(_T(":"), cur_01);
		str_column = str_total.Mid(cur_01, cur_02 - cur_01);
		cur_01 = cur_02 + 1;

		cur_02 = str_total.Find(_T("\n"), cur_01);
		str_data = str_total.Mid(cur_01, cur_02 - cur_01);
		cur_01 = cur_02 + 1;

		str_data.TrimLeft();

		if (i == 0)
		{
			add_usb_port(_ttoi(str_data));
		}
		else if (i == 1)
		{
			CWnd* pWnd_02 = GetDlgItem(IDC_EDIT_BAUD_RATE);
			pWnd_02->SetWindowText(str_data);

			m_baud_rate = _ttoi(str_data);
		}
	}

}

void CSCHtuningView::save_file(CString filename, CString arg_msg)
{
	// TODO: 여기에 구현 코드 추가.

	int len = arg_msg.GetLength();

	char buf[500];
	WideCharToMultiByte(CP_ACP, 0, arg_msg, -1, buf, len, NULL, NULL);

	CFile file;
	if (!file.Open(filename, CFile::modeCreate | CFile::modeWrite))
	{
		MessageBox(_T("파일을 열지 못했습니다."), _T("경고!"), MB_OK | MB_ICONHAND);
		return;
	}

	file.Write(buf, len);
	file.Close();
}

void CSCHtuningView::save_config()
{
	// TODO: 여기에 구현 코드 추가.

	CString str;
	CString str_total;

	CComboBox* pWnd_01 = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
	int cur = pWnd_01->GetCurSel();

	pWnd_01->GetLBText(cur, str);

	str = str.Mid(3, str.GetLength() - 3);

	str_total += _T("port : ");
	str_total += str;
	str_total += _T("\n");

	CWnd* pWnd_02 = GetDlgItem(IDC_EDIT_BAUD_RATE);
	pWnd_02->GetWindowText(str);

	str_total += _T("baud rate : ");
	str_total += str;
	str_total += _T("\n");

	save_file(_T("01_config.txt"), str_total);
}

void CSCHtuningView::search_usb_port()
{
	// TODO: 여기에 구현 코드 추가.

	CComboBox* pWnd = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);

	int count = pWnd->GetCount();

	for (int i = 0; i < count; i++)
		pWnd->DeleteString(0);
	pWnd->SetCurSel(0);


	HKEY hKey;
	RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), &hKey);

	TCHAR szData[20];
	TCHAR szName[100];
	DWORD index = 0;
	DWORD dwSize = 100;
	DWORD dwSize2 = 20;
	DWORD dwType = REG_SZ;
	memset(szData, 0x00, sizeof(szData));
	memset(szName, 0x00, sizeof(szName));

	CString str_buf;
	CString str;

	while (ERROR_SUCCESS == RegEnumValue(hKey, index, szName, &dwSize, NULL, NULL, NULL, NULL)) {
		index++;
		RegQueryValueEx(hKey, szName, NULL, &dwType, (LPBYTE)szData, &dwSize2);

		str.Format(_T("%s"), szData);

		pWnd->AddString(str);

		str_buf += str;
		str_buf += _T("\n");

		memset(szData, 0x00, sizeof(szData));
		memset(szName, 0x00, sizeof(szName));
		dwSize = 100;
		dwSize2 = 20;
	}
	RegCloseKey(hKey);

	pWnd->SetCurSel(0);
}

void CSCHtuningView::add_usb_port(int port_id)
{
	// TODO: 여기에 구현 코드 추가.

	CComboBox* pWnd_01 = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
	pWnd_01->SetCurSel(0);

	CString str_01;
	str_01.Format(_T("COM%d"), port_id);

	int count = pWnd_01->GetCount();

	CString str_02;

	for (int i = 0; i < count; i++)
	{
		pWnd_01->GetLBText(i, str_02);

		if (str_02.Compare(str_01) == 0)
		{
			pWnd_01->SetCurSel(i);
			m_port = port_id;
			break;
		}
	}
}

void CSCHtuningView::open_serial()
{
	// TODO: 여기에 구현 코드 추가.

	CComboBox* pWnd = (CComboBox*)GetDlgItem(IDC_COMBO_PORT);
	int cur = pWnd->GetCurSel();


	CString str_01;
	pWnd->GetLBText(cur, str_01);

	CString str_02 = str_01.Mid(3, str_01.GetLength() - 3);

	m_port = _ttoi(str_02);


	CWnd* pWnd_02 = NULL;
	pWnd_02 = GetDlgItem(IDC_EDIT_BAUD_RATE);
	pWnd_02->GetWindowTextW(str_01);

	m_baud_rate = _ttoi(str_01);

	if (m_SerialComm != NULL)
	{
		delete m_SerialComm;
		m_SerialComm = NULL;
	}

	m_SerialComm = new CSerialComm(m_hWnd);

	if (!m_SerialComm->OpenPort(m_baud_rate, m_port))
	{
		display(_T("포트를 초기화 실패."));
		m_SerialComm->ClosePort();
	}
}

void CSCHtuningView::close_serial()
{
	// TODO: 여기에 구현 코드 추가.

	m_SerialComm->ClosePort();

	if (m_SerialComm != NULL)
	{
		delete m_SerialComm;
		m_SerialComm = NULL;
	}

	display(_T("bye !!"));
}

void CSCHtuningView::send_serial(CString arg_msg)
{
	// TODO: 여기에 구현 코드 추가.

	char buf_char[1024];
	WideCharToMultiByte(CP_ACP, 0, arg_msg, -1, buf_char, 1024, NULL, NULL);

	int len = arg_msg.GetLength();

	m_SerialComm->WriteComm((unsigned char*)buf_char, len);

}

void CSCHtuningView::receive_serial(CString arg_msg)
{
	// TODO: 여기에 구현 코드 추가.

	anay_proc(arg_msg);

	if (m_screw[m_screw_id - 1] < 1 * (1024 / 10))
	{
		m_graph_id_cur = 1 + 10 * (m_screw_id - 1);
	}
	else if (m_screw[m_screw_id - 1] < 2 * (1024 / 10))
	{
		m_graph_id_cur = 2 + 10 * (m_screw_id - 1);
	}
	else if (m_screw[m_screw_id - 1] < 3 * (1024 / 10))
	{
		m_graph_id_cur = 3 + 10 * (m_screw_id - 1);
	}
	else if (m_screw[m_screw_id - 1] < 4 * (1024 / 10))
	{
		m_graph_id_cur = 4 + 10 * (m_screw_id - 1);
	}
	else if (m_screw[m_screw_id - 1] < 5 * (1024 / 10))
	{
		//		m_graph_id_cur = 5;
	}
	else if (m_screw[m_screw_id - 1] < 6 * (1024 / 10))
	{
		m_graph_id_cur = 6 + 10 * (m_screw_id - 1);
	}
	else if (m_screw[m_screw_id - 1] < 7 * (1024 / 10))
	{
		m_graph_id_cur = 7 + 10 * (m_screw_id - 1);
	}
	else if (m_screw[m_screw_id - 1] < 8 * (1024 / 10))
	{
		m_graph_id_cur = 8 + 10 * (m_screw_id - 1);
	}
	else if (m_screw[m_screw_id - 1] < 9 * (1024 / 10))
	{
		m_graph_id_cur = 9 + 10 * (m_screw_id - 1);
	}
	else if (m_screw[m_screw_id - 1] < 10 * (1024 / 10))
	{
		m_graph_id_cur = 10 + 10 * (m_screw_id - 1);
	}

	CString m_filename;

	if (m_graph_id_prev != m_graph_id_cur)
	{
		if (m_screw_id == 1)
		{
			if (m_graph_id_cur == 1 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw1_01_graph.png");
			}
			else if (m_graph_id_cur == 2 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw1_02_graph.png");
			}
			else if (m_graph_id_cur == 3 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw1_03_graph.png");
			}
			else if (m_graph_id_cur == 4 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw1_04_graph.png");
			}
			else if (m_graph_id_cur == 5 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw2_00_graph.png");
			}
			else if (m_graph_id_cur == 6 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw1_05_graph.png");
			}
			else if (m_graph_id_cur == 7 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw1_06_graph.png");
			}
			else if (m_graph_id_cur == 8 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw1_07_graph.png");
			}
			else if (m_graph_id_cur == 9 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw1_08_graph.png");
			}
			else if (m_graph_id_cur == 10 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw1_09_graph.png");
			}
		}

		else if (m_screw_id == 2)
		{
			if (m_graph_id_cur == 1 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw2_01_graph.png");
			}
			else if (m_graph_id_cur == 2 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw2_02_graph.png");
			}
			else if (m_graph_id_cur == 3 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw2_03_graph.png");
			}
			else if (m_graph_id_cur == 4 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw2_04_graph.png");
			}
			else if (m_graph_id_cur == 5 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw3_00_graph.png");
			}
			else if (m_graph_id_cur == 6 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw2_05_graph.png");
			}
			else if (m_graph_id_cur == 7 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw2_06_graph.png");
			}
			else if (m_graph_id_cur == 8 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw2_07_graph.png");
			}
			else if (m_graph_id_cur == 9 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw2_08_graph.png");
			}
			else if (m_graph_id_cur == 10 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw2_09_graph.png");
			}
		}

		else if (m_screw_id == 3)
		{
			if (m_graph_id_cur == 1 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw3_01_graph.png");
			}
			else if (m_graph_id_cur == 2 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw3_02_graph.png");
			}
			else if (m_graph_id_cur == 3 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw3_03_graph.png");
			}
			else if (m_graph_id_cur == 4 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw3_04_graph.png");
			}
			else if (m_graph_id_cur == 5 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw4_00_graph.png");
			}
			else if (m_graph_id_cur == 6 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw3_05_graph.png");
			}
			else if (m_graph_id_cur == 7 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw3_06_graph.png");
			}
			else if (m_graph_id_cur == 8 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw3_07_graph.png");
			}
			else if (m_graph_id_cur == 9 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw3_08_graph.png");
			}
			else if (m_graph_id_cur == 10 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw3_09_graph.png");
			}
		}

		else if (m_screw_id == 4)
		{
			if (m_graph_id_cur == 1 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw4_01_graph.png");
			}
			else if (m_graph_id_cur == 2 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw4_02_graph.png");
			}
			else if (m_graph_id_cur == 3 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw4_03_graph.png");
			}
			else if (m_graph_id_cur == 4 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw4_04_graph.png");
			}
			else if (m_graph_id_cur == 5 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw5_00_graph.png");
			}
			else if (m_graph_id_cur == 6 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw4_05_graph.png");
			}
			else if (m_graph_id_cur == 7 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw4_06_graph.png");
			}
			else if (m_graph_id_cur == 8 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw4_07_graph.png");
			}
			else if (m_graph_id_cur == 9 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw4_08_graph.png");
			}
			else if (m_graph_id_cur == 10 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw4_09_graph.png");
			}
		}

		else if (m_screw_id == 5)
		{
			if (m_graph_id_cur == 1 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw5_01_graph.png");
			}
			else if (m_graph_id_cur == 2 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw5_02_graph.png");
			}
			else if (m_graph_id_cur == 3 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw5_03_graph.png");
			}
			else if (m_graph_id_cur == 4 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw5_04_graph.png");
			}
			else if (m_graph_id_cur == 5 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw6_00_graph.png");
			}
			else if (m_graph_id_cur == 6 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw5_05_graph.png");
			}
			else if (m_graph_id_cur == 7 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw5_06_graph.png");
			}
			else if (m_graph_id_cur == 8 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw5_07_graph.png");
			}
			else if (m_graph_id_cur == 9 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw5_08_graph.png");
			}
			else if (m_graph_id_cur == 10 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw5_09_graph.png");
			}
		}
		else if (m_screw_id == 6)
		{
			if (m_graph_id_cur == 1 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw6_01_graph.png");
			}
			else if (m_graph_id_cur == 2 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw6_02_graph.png");
			}
			else if (m_graph_id_cur == 3 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw6_03_graph.png");
			}
			else if (m_graph_id_cur == 4 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw6_04_graph.png");
			}
			else if (m_graph_id_cur == 5 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw1_00_graph.png");
			}
			else if (m_graph_id_cur == 6 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw6_05_graph.png");
			}
			else if (m_graph_id_cur == 7 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw6_06_graph.png");
			}
			else if (m_graph_id_cur == 8 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw6_07_graph.png");
			}
			else if (m_graph_id_cur == 9 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw6_08_graph.png");
			}
			else if (m_graph_id_cur == 10 + 10 * (m_screw_id - 1))
			{
				m_filename = _T("zr_screw6_09_graph.png");
			}
		}

		m_Image_graph.Destroy();
		m_Image_graph.Load(m_filename);

		m_Image_graph.Draw(m_dc_image, 0, 0);

		m_dc_image.TextOutW(20, 18, m_filename);

		InvalidateRect(CRect(m_dc_x_image, m_dc_y_image, m_dc_x_image + m_dc_w_image, m_dc_y_image + m_dc_h_image), false);
	}
	m_graph_id_prev = m_graph_id_cur;
}

void CSCHtuningView::anay_proc(CString arg_msg)
{
	// TODO: 여기에 구현 코드 추가.

	int cur[5];

	cur[0] = arg_msg.Find(_T(","), 0);
	cur[1] = arg_msg.Find(_T(","), cur[0] + 1);
	cur[2] = arg_msg.Find(_T(","), cur[1] + 1);
	cur[3] = arg_msg.Find(_T(","), cur[2] + 1);
	cur[4] = arg_msg.Find(_T(","), cur[3] + 1);

	CString str;

	CWnd* pWnd = NULL;

	str = arg_msg.Mid(0, cur[0]);

	pWnd = GetDlgItem(IDC_EDIT3);
	pWnd->SetWindowTextW(str);

	m_screw[0] = _ttoi(str);

	str = arg_msg.Mid(cur[0] + 1, cur[1] - cur[0] - 1);

	pWnd = GetDlgItem(IDC_EDIT4);
	pWnd->SetWindowTextW(str);
	m_screw[1] = _ttoi(str);


	str = arg_msg.Mid(cur[1] + 1, cur[2] - cur[1] - 1);

	pWnd = GetDlgItem(IDC_EDIT5);
	pWnd->SetWindowTextW(str);
	m_screw[2] = _ttoi(str);


	str = arg_msg.Mid(cur[2] + 1, cur[3] - cur[2] - 1);

	pWnd = GetDlgItem(IDC_EDIT6);
	pWnd->SetWindowTextW(str);
	m_screw[3] = _ttoi(str);


	str = arg_msg.Mid(cur[3] + 1, cur[4] - cur[3] - 1);

	pWnd = GetDlgItem(IDC_EDIT7);
	pWnd->SetWindowTextW(str);
	m_screw[4] = _ttoi(str);


	str = arg_msg.Mid(cur[4] + 1, arg_msg.GetLength() - cur[4] - 1);

	pWnd = GetDlgItem(IDC_EDIT8);
	pWnd->SetWindowTextW(str);
	m_screw[5] = _ttoi(str);


	if (m_screw[0] < 4 * (1024 / 10))
	{
		m_screw_id = 1;
		m_rot_id = 1;

		m_screw_value[0] = "1번 나사를 시계 반대 방향으로 회전하세요";

		return;
	}

	if (m_screw[0] > 5 * (1024 / 10))
	{
		m_screw_id = 1;
		m_rot_id = 0;

		m_screw_value[0] = "1번 나사를 시계 방향으로 회전하세요";

		return;
	}

	if (m_screw[0] > 4 * (1024 / 10) && m_screw[0] < 5 * (1024 / 10))
	{
		m_screw_value[0] = "2번 나사로 넘어가세요.";

		m_screw_id = 0;
		m_rot_id = 0;

//		proc_unit();

	}

	if (m_screw[1] < 4 * (1024 / 10))
	{
		m_screw_id = 2;
		m_rot_id = 1;

		m_screw_value[0] = "2번 나사를 시계 반대 방향으로 회전하세요";

		return;
	}

	if (m_screw[1] > 5 * (1024 / 10))
	{
		m_screw_id = 2;
		m_rot_id = 0;

		m_screw_value[0] = "2번 나사를 시계 방향으로 회전하세요";

		return;
	}

	if (m_screw[1] > 4 * (1024 / 10) && m_screw[0] < 5 * (1024 / 10))
	{
		m_screw_value[0] = "3번 나사로 넘어가세요";

		m_screw_id = 0;
		m_rot_id = 0;

//		proc_unit();

	}

	if (m_screw[2] < 4 * (1024 / 10))
	{
		m_screw_id = 3;
		m_rot_id = 1;

		m_screw_value[0] = "3번 나사를 시계 반대 방향으로 회전하세요";

		return;
	}

	if (m_screw[2] > 5 * (1024 / 10))
	{
		m_screw_id = 3;
		m_rot_id = 0;

		m_screw_value[0] = "3번 나사를 시계 방향으로 회전하세요";

		return;
	}

	if (m_screw[2] > 4 * (1024 / 10) && m_screw[0] < 5 * (1024 / 10))
	{
		m_screw_value[0] = "4번 나사로 넘어가세요";

		m_screw_id = 0;
		m_rot_id = 0;

		//		proc_unit();

	}
	
	if (m_screw[3] < 4 * (1024 / 10))
	{
		m_screw_id = 4;
		m_rot_id = 1;

		m_screw_value[0] = "4번 나사를 시계 반대 방향으로 회전하세요";

		return;
	}

	if (m_screw[3] > 5 * (1024 / 10))
	{
		m_screw_id = 4;
		m_rot_id = 0;

		m_screw_value[0] = "4번 나사를 시계 방향으로 회전하세요";

		return;
	}

	if (m_screw[3] > 4 * (1024 / 10) && m_screw[0] < 5 * (1024 / 10))
	{
		m_screw_value[0] = "5번 나사로 넘어가세요";

		m_screw_id = 0;
		m_rot_id = 0;

		//		proc_unit();

	}

	if (m_screw[4] < 4 * (1024 / 10))
	{
		m_screw_id = 5;
		m_rot_id = 1;

		m_screw_value[0] = "5번 나사를 시계 반대 방향으로 회전하세요";

		return;
	}

	if (m_screw[4] > 5 * (1024 / 10))
	{
		m_screw_id = 5;
		m_rot_id = 0;

		m_screw_value[0] = "5번 나사를 시계 방향으로 회전하세요";

		return;
	}

	if (m_screw[4] > 4 * (1024 / 10) && m_screw[0] < 5 * (1024 / 10))
	{
		m_screw_value[0] = "6번 나사로 넘어가세요";

		m_screw_id = 0;
		m_rot_id = 0;

		//		proc_unit();

	}

	if (m_screw[5] < 4 * (1024 / 10))
	{
		m_screw_id = 6;
		m_rot_id = 1;

		m_screw_value[0] = "6번 나사를 시계 반대 방향으로 회전하세요";

		return;
	}

	if (m_screw[5] > 5 * (1024 / 10))
	{
		m_screw_id = 6;
		m_rot_id = 0;

		m_screw_value[0] = "6번 나사를 시계 방향으로 회전하세요";

		return;
	}

	if (m_screw[5] > 4 * (1024 / 10) && m_screw[0] < 5 * (1024 / 10))
	{
		m_screw_value[0] = "튜닝이 완료되었습니다";

		m_screw_id = 0;
		m_rot_id = 0;

		//		proc_unit();

	}

}

void CSCHtuningView::OnBnClickedBtnUsbOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	open_serial();
}


void CSCHtuningView::OnBnClickedBtnUsbClose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	close_serial();
}



