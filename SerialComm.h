#pragma once

#define	BUFF_SIZE		8192
#define	WM_COMM_READ	(WM_USER+1)		

#define	ASCII_LF		0x0a
#define	ASCII_CR		0x0d
#define	ASCII_XON		0x11
#define	ASCII_XOFF		0x13

class CSerialComm
{
public:
	CSerialComm(HWND hwnd);
	virtual ~CSerialComm();

public:
	BYTE	m_Queuebuff[BUFF_SIZE];
	long	m_QueueHead, m_QueueTail;

	HANDLE		m_hComm;				// 통신 포트 파일 핸들
	CString		m_sPortName;			// 포트 이름 (COM1 ..)
	BOOL		m_bConnected;			// 포트가 열렸는지 유무를 나타냄.
	OVERLAPPED	m_osRead, m_osWrite;	// 포트 파일 Overlapped structure
	HANDLE		m_hThreadWatchComm;		// Watch함수 Thread 핸들.
	WORD		m_wPortID;				// WM_COMM_READ와 함께 보내는 인수.

	CString	m_RecvMessage;
	HWND		hRecvCom;

	CWinThread* m_pThread;
	HANDLE	m_hSleep;
	int	m_Sleep_time;
	BOOL m_thread_flag;

public:
	void QueueClear();
	long QueueGetSize();
	BOOL QueuePutByte(BYTE b);
	BOOL QueueGetByte(BYTE* pb);

	DWORD ReadComm(BYTE* pBuff, DWORD nToRead);
	DWORD WriteComm(BYTE* pBuff, DWORD nToWrite);
	void ClosePort();
	BOOL OpenPort(DWORD dwBaud, WORD wPortID);

	void SleepClose();
	void SleepThread();
	void SleepSetup(int arg_sleep);
	void SetThreadFlag(BOOL arg_flag);
	void ThreadTerminate();
	void ThreadRestart();
	void ThreadStop();
	void ThreadStart();

	static UINT ThreadProc(LPVOID lParam);

	int is_connect();
};

