
// SCHtuningView.h: CSCHtuningView 클래스의 인터페이스
//

#pragma once

#include "SCHtuningDOC.h"

#include "SerialComm.h"

class CSCHtuningView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CSCHtuningView() noexcept;
	DECLARE_DYNCREATE(CSCHtuningView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_SCHTUNING_FORM };
#endif

// 특성입니다.
public:
	CSCHtuningDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CSCHtuningView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnEnd();
	//afx_msg void OnBnClickedBtnInit();
	afx_msg void OnBnClickedBtnCw01();
	afx_msg void OnBnClickedBtnCw02();
	afx_msg void OnBnClickedBtnCw03();
	afx_msg void OnBnClickedBtnCw04();
	afx_msg void OnBnClickedBtnCw05();
	afx_msg void OnBnClickedBtnCw06();
	afx_msg void OnBnClickedBtnCcw01();
	afx_msg void OnBnClickedBtnCcw02();
	afx_msg void OnBnClickedBtnCcw03();
	afx_msg void OnBnClickedBtnCcw04();
	afx_msg void OnBnClickedBtnCcw05();
	afx_msg void OnBnClickedBtnCcw06();
	afx_msg void OnBnClickedBtnServerStart();
	afx_msg void OnBnClickedBtnServerStop();
	afx_msg void OnBnClickedRadioMat();
	afx_msg void OnBnClickedRadioMfc();
	afx_msg void OnEnChangeEditRecv();
	afx_msg void OnLbnSelchangeList01();
	virtual void OnDraw(CDC* pDC);

public:
	CListBox m_List;

public:
	int m_endian_flag;

	void proc_recv(TCHAR buf[]);

public:
	CDC m_dc;
	CBitmap m_bitmap;

	int m_dc_x, m_dc_y;
	int m_dc_w, m_dc_h;

	CDC m_dc_image;
	CBitmap m_bitmap_image;

	int m_dc_x_image, m_dc_y_image;
	int m_dc_w_image, m_dc_h_image;

	int m_cur;
	CWinThread* m_pThread;

	CImage m_Image_filter;

	CImage m_Image_cw[8];
	CImage m_Image_ccw[8];

	CImage m_Image_graph;
	int m_graph_id_prev;
	int m_graph_id_cur;

	int m_screw_id;
	int m_rot_id;

	int m_thread_flag;

	int m_thread_delay;

	CPoint m_pt_filter;
	CPoint m_pt_screw[6];
	void init_dc_buffer(CWnd* pWnd);
	void proc_unit();
	void change_screw();

	void proc_stop();
	void font_unit();
	int m_font_id;
	CString m_screw_value[5];

	afx_msg LRESULT OnCommunication(WPARAM wParam, LPARAM lPararm);
	
	int m_port;
	int m_baud_rate;
	int m_screw[6];

	CSerialComm* m_SerialComm;

	void display(CString arg_msg);
	CString load_file(CString filename);
	void load_config();
	void save_file(CString filename, CString arg_msg);
	void save_config();
	void search_usb_port();
	void add_usb_port(int port_id);
	void open_serial();
	void close_serial();
	void send_serial(CString arg_msg);
	void receive_serial(CString arg_msg);
	void anay_proc(CString arg_msg);
	afx_msg void OnBnClickedBtnUsbOpen();
	afx_msg void OnBnClickedBtnUsbClose();
	void init_dc_buffer_image(CWnd* pWnd);
};

#ifndef _DEBUG  // SCHtuningView.cpp의 디버그 버전
inline CSCHtuningDoc* CSCHtuningView::GetDocument() const
   { return reinterpret_cast<CSCHtuningDoc*>(m_pDocument); }
#endif

