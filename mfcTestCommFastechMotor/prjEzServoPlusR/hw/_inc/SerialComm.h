#pragma once

/*
 * SerialComm.h
 *
 * 작성자 : 이길훈
 * 22.04.04
 *  rs232 통신을 위한 hal component
 *  interface IComm 상속
 */


#define SERIAL_COMM_BUFF_MAX	256

class SerialComport;


class SerialComm :public IComm
{
public:
	struct cfg_t
	{
		char portname[PRJ_NAME_STR_LENGTH_MAX]{};
		uint32_t baudrate{};
		uint8_t  port_no{};

		inline int SetPortName(const char* value) {
			return sprintf_s(portname, PRJ_NAME_STR_LENGTH_MAX, value);
		}


		inline char* GetPortName() {
			return &portname[0];
		}

		inline void GetPortName(TCHAR* p_value, uint32_t len = PRJ_NAME_STR_LENGTH_MAX) {
			TCHAR tmp[PRJ_NAME_STR_LENGTH_MAX] = { 0, };
			trans::CharToLPTSTR(portname, tmp, PRJ_NAME_STR_LENGTH_MAX);
			wsprintf(p_value, tmp);
		}

		inline void operator = (const cfg_t* p_cfg) {
			this->baudrate = p_cfg->baudrate;
			this->port_no = p_cfg->port_no;
			this->SetPortName(p_cfg->portname);
		}

	};


	/****************************************************
	 *	data
	 ****************************************************/
private:
	SerialComport* m_pSerial;
public:
	bool m_IsConnected;
	bool m_TrdLife;
	cfg_t m_Serialcfg;
	buffer::_que<byte> m_Que{SERIAL_COMM_BUFF_MAX};

	HANDLE m_hThread;
	DWORD m_TrdId;

	/****************************************************
	 *	Constructor
	 ****************************************************/
public:
	SerialComm(common::Attribute_t common_data, cfg_t* cfg);
	~SerialComm();


	/****************************************************
	 *	func
	 ****************************************************/
private:
	void threadStop(void);
	void threadRun(void);
	void threadJob(void);

	static UINT threadFunc(LPVOID pParam);
	static void cbFunc(void* obj, void* w_parm, void* l_parm);
public:
	errno_t OpenPort();
	void ClosePort();
	bool IsConnected();
	uint32_t SendReceiveData();

	errno_t SendCmd(uint8_t cmd, uint8_t* p_data, uint8_t length);
	errno_t SendResp(uint8_t cmd, uint8_t* p_data, uint8_t length);
	errno_t SendCmdRxResp(uint8_t cmd, uint8_t* p_data, uint8_t length);

	uint32_t ReadByte(uint8_t* resp, uint32_t len);
	uint32_t WriteByte(uint8_t* send, uint32_t len);
	void AttCallbackFunc(void* obj, evt_cb cb);

};



#if 0
class SerialComm :public IComm
{
public:
	struct cfg_t
	{
		char portname[PRJ_NAME_STR_LENGTH_MAX]{};
		uint32_t baudrate{};
		uint8_t  port_no{};
		HWND hwnd = {};

		inline int SetPortName(const char* value) {
			return sprintf_s(portname, PRJ_NAME_STR_LENGTH_MAX, value);
		}


		inline char* GetPortName() {
			return &portname[0];
		}

		inline void GetPortName(TCHAR* p_value, uint32_t len = PRJ_NAME_STR_LENGTH_MAX) {
			TCHAR tmp[PRJ_NAME_STR_LENGTH_MAX] = { 0, };
			trans::CharToLPTSTR(portname, tmp, PRJ_NAME_STR_LENGTH_MAX);
			wsprintf(p_value, tmp);
		}

		inline void operator = (const cfg_t* p_cfg) {
			this->baudrate = p_cfg->baudrate;
			this->port_no = p_cfg->port_no;
			this->SetPortName(p_cfg->portname);
		}

	};
private:
	/****************************************************
	 *	data
	 ****************************************************/
	enum class error_code
	{
		Success,
		GetCommState,
		SerCommState,
		SetCommMask,
		SetupComm,
		PurgeComm,
		ClearCommError,
		GetCommTimeouts,
		SetCommTimeouts,
		existPort,
		emptyPort,
		invalidHandleValue,
		failCreateEventRead,
		failCreateEventWrite,
		failCreateThread,
	};

private:

public:
	// 통신 포트 파일 핸들
	HANDLE m_Serial_handle;
	// 포트를 감시할 함수, 스레드의 핸들을 보관
	HANDLE m_hThread;

	// 포트 파일 IO를 위한 OverLapped structure
	OVERLAPPED m_osWrite, m_osRead;
	// 수신 감시 Event 객체 
	CEvent* m_pEvent;

	bool m_IsConnected;
	bool m_TrdLife;
	cfg_t m_Serialcfg;
	buffer::_que<byte> m_Quebuffer;

	DWORD m_TrdId;

	void* m_cbObj;
	comm_rcv_cb m_func;

	/****************************************************
	 *	Constructor
	 ****************************************************/
public:
	SerialComm(common::Attribute_t common_data, cfg_t* cfg);

	virtual ~SerialComm();

	/****************************************************
	 *	func
	 ****************************************************/
private:
	errno_t initPort(DWORD baudrate);

	void closeHandle();

	void threadStop(void);
	void threadRun(void);
	void threadJob(void);

	static UINT threadFunc(LPVOID pParam);
public:

	errno_t OpenPort();
	void ClosePort();
	bool IsConnected();

	uint32_t ReadByte(uint8_t* resp, uint32_t len);
	uint32_t WriteByte(uint8_t* send, uint32_t len);

	void AttCallbackFunc(void* obj, comm_rcv_cb cb);
	uint32_t SendReceiveData();


	void	AddStringPortList(CComboBox* combobox);
	void	AddStringBaudrateList(CComboBox* combobox);


};
#endif
