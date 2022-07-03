#include "pch.h"
#include "system_def.h"

#include "_inc/SerialComm.h"

#include <mutex>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



static std::mutex serialComm_mutex;

SerialComm::SerialComm(common::Attribute_t common_data, SerialComm::cfg_t* cfg)
  :Icommon(common_data), m_received_data(0), m_cbObj(nullptr) , m_func()
{
  m_Serialcfg = cfg;
  m_TrdLife = false;
  m_IsOpened = false;
  m_hThread = {};
  m_TrdId = 0;

  //SerialComport::cfg_t comm_cfg = { 0, };
  //comm_cfg.SetPortName(cfg->GetPortName());
  //comm_cfg.baud = cfg->baudrate;
  //
  ///*hw serial port�� ����*/
  //m_pSerial = new SerialComport(&comm_cfg);
}

SerialComm::~SerialComm()
{
  //if (m_pSerial) delete m_pSerial; m_pSerial = nullptr;
  m_TrdLife = false;
  m_IsOpened = false;
  m_hThread = {};
  m_TrdId = 0;
}

errno_t SerialComm::OpenPort()
{
  /*data ������ ���� �ݹ��Լ� ���*/
  //m_pSerial->AttCallbackFunc(this, cbFunc);
  //m_pSerial->Open();
  return openPort(m_Serialcfg.GetPortName(), m_Serialcfg.baudrate);
}
void SerialComm::ClosePort()
{
  if (m_Serialcfg.serial_handle == nullptr)
    return;
  m_TrdLife = false;
  while (m_TrdId);
  closePort();

}
void SerialComm::Recovery()
{
}
bool SerialComm::IsOpened()
{
  return m_IsOpened;
}
uint32_t SerialComm::serialAvailable()
{
  uint32_t ret = 0;
  //int32_t length = 0;
  //DWORD dwRead = 0;
  //uint8_t data;

  //if (ReadFile(m_Serialcfg.serial_handle, &data, (DWORD)1, &dwRead, NULL) == TRUE)
  //{
  //  if (dwRead != 1)
  //  {
  //    length = 0;
  //  }
  //  else
  //  {
  //    length = 1;
  //    m_received_data = data;
  //    if (m_cbObj)
  //    {
  //      m_func(m_cbObj, 0, &m_received_data);
  //    }
  //  }
  //}
  //ret = length;

  return (uint32_t)m_Que.Available();
}
uint8_t SerialComm::Read()
{
  return m_received_data;
}
uint32_t SerialComm::Write(uint8_t* p_data, uint32_t length)
{
  uint32_t ret = 0;
  DWORD dwWrite = 0;

  if (WriteFile(m_Serialcfg.serial_handle, p_data, (DWORD)length, &dwWrite, NULL) == FALSE)
  {
    ret = 0;
  }
  else
  {
    ret = dwWrite;
  }

  return ret;
}
uint32_t SerialComm::CommPrintf(char* fmt, ...)
{
  char buf[256];
  va_list args;
  int len;
  uint32_t ret;

  va_start(args, fmt);
  len = vsnprintf(buf, 256, fmt, args);

  ret = Write((uint8_t*)buf, len);

  va_end(args);


  return ret;
}

void SerialComm::AttCallbackFunc(void* obj, evt_cb cb)
{
  m_cbObj = obj;
  m_func = cb;
}


errno_t SerialComm::openPort(char* port_name, uint32_t baud)
{
  errno_t err_code = 0;

  DCB dcb;
  COMMTIMEOUTS timeouts;
  DWORD dwError;

  char name[MAX_PATH] = { 0, };
  sprintf_s(name, MAX_PATH, "//./%s", port_name);

  m_Serialcfg.serial_handle = CreateFileA(name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (m_Serialcfg.serial_handle == INVALID_HANDLE_VALUE)
  {
    TRACE("Error opening serial port!\n");
    return static_cast<errno_t>(error_code::emptyPort);
  }

  while (1)
  {
    dcb.DCBlength = sizeof(DCB);
    if (GetCommState(m_Serialcfg.serial_handle, &dcb) == FALSE)
    {
      TRACE("Error GetCommState\n");
      err_code = static_cast<errno_t>(error_code::GetCommState);
      break;
    }

    // Set baudrate
    dcb.BaudRate = (DWORD)baud;
    dcb.ByteSize = 8;                    // Data bit = 8bit
    dcb.Parity = NOPARITY;             // No parity
    dcb.StopBits = ONESTOPBIT;           // Stop bit = 1
    dcb.fParity = NOPARITY;             // No Parity check
    dcb.fBinary = 1;                    // Binary mode
    dcb.fNull = 0;                    // Get Null byte
    dcb.fAbortOnError = 0;
    dcb.fErrorChar = 0;
    // Not using XOn/XOff
    dcb.fOutX = 0;
    dcb.fInX = 0;
    // Not using H/W flow control
    dcb.fDtrControl = DTR_CONTROL_DISABLE;
    dcb.fRtsControl = RTS_CONTROL_DISABLE;
    dcb.fDsrSensitivity = 0;
    dcb.fOutxDsrFlow = 0;
    dcb.fOutxCtsFlow = 0;

    if (SetCommState(m_Serialcfg.serial_handle, &dcb) == FALSE)
    {
      DWORD dwError = GetLastError();
      err_code = static_cast<errno_t>(error_code::SerCommState);

      TRACE("SetCommState err: %d\n", (int)dwError);
      break;
    }

    if (SetCommMask(m_Serialcfg.serial_handle, 0) == FALSE) // Not using Comm event
    {
      err_code = static_cast<errno_t>(error_code::SetCommMask);
      break;
    }
    if (SetupComm(m_Serialcfg.serial_handle, SERIAL_COMM_BUFF_MAX, SERIAL_COMM_BUFF_MAX) == FALSE) // Buffer size (Rx,Tx)
    {
      err_code = static_cast<errno_t>(error_code::SetupComm);
      break;
    }
    if (PurgeComm(m_Serialcfg.serial_handle, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR) == FALSE) // Clear buffer
    {
      err_code = static_cast<errno_t>(error_code::PurgeComm);
      break;
    }
    if (ClearCommError(m_Serialcfg.serial_handle, &dwError, NULL) == FALSE)
    {
      err_code = static_cast<errno_t>(error_code::ClearCommError);
      break;
    }

    if (GetCommTimeouts(m_Serialcfg.serial_handle, &timeouts) == FALSE)
    {
      err_code = static_cast<errno_t>(error_code::GetCommTimeouts);
      break;
    }
    // Timeout (Not using timeout)
    // Immediatly return
    timeouts.ReadIntervalTimeout = 0;
    timeouts.ReadTotalTimeoutMultiplier = 0;
    timeouts.ReadTotalTimeoutConstant = 1; // must not be zero.
    timeouts.WriteTotalTimeoutMultiplier = 0;
    timeouts.WriteTotalTimeoutConstant = 0;
    if (SetCommTimeouts(m_Serialcfg.serial_handle, &timeouts) == FALSE)
    {
      err_code = static_cast<errno_t>(error_code::SetCommTimeouts);
      break;
    }
    EscapeCommFunction(m_Serialcfg.serial_handle, SETRTS);
    EscapeCommFunction(m_Serialcfg.serial_handle, SETDTR);
    break;
  }

  if (err_code != 0)
  {
    CloseHandle(m_Serialcfg.serial_handle);
  }
  else
  {
    SetCommMask(m_Serialcfg.serial_handle, EV_RXCHAR);
    threadRun();
    m_IsOpened = true;
  }

  return err_code;
}

bool SerialComm::closePort()
{
  bool ret = false;
  ret = CloseHandle(m_Serialcfg.serial_handle);
  m_IsOpened = false;
  return ret;
}

void SerialComm::threadStop(void)
{
  ClosePort();
}

void SerialComm::threadRun(void)
{
  m_TrdLife = true;
  // ��Ʈ ���� ������ ����
  m_hThread = CreateThread(
    (LPSECURITY_ATTRIBUTES)NULL,
    0,
    (LPTHREAD_START_ROUTINE)threadFunc,
    (LPVOID)this,
    0,
    &m_TrdId
  );
}


// https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfile
uint32_t SerialComm::readData()
{

#if 0
  DWORD dwRead, dwError, dwErrorFlags;
  COMSTAT comstat;
  char receive_byte[SERIAL_COMM_BUFF_MAX] = { 0, };

  ClearCommError(m_Serialcfg.serial_handle, &dwError, &comstat);
  // input Queue�� ���� �ִ� �������� ����
  dwRead = comstat.cbInQue;
  if (dwRead > 0)
  {
    if (!ReadFile(m_Serialcfg.serial_handle, &receive_byte, SERIAL_COMM_BUFF_MAX, &dwRead, NULL/*&m_osRead*/))
    {
      if (GetLastError() == ERROR_IO_PENDING)
      {
        // ��������� �����Ͱ� �ִ� ��� timeout�� ������ �ð���ŭ ��ٷ��ش�.
        while (!GetOverlappedResult(m_Serialcfg.serial_handle, &m_osRead, &dwRead, TRUE))
        {
          dwError = GetLastError();
          if (dwError != ERROR_IO_INCOMPLETE)
          {
            ClearCommError(m_Serialcfg.serial_handle, &dwErrorFlags, &comstat);
            break;
          }
        }
      }
      else
      {
        dwRead = 0;
        ClearCommError(m_Serialcfg.serial_handle, &dwErrorFlags, &comstat);
      }
    }
  }

#endif

  uint32_t ret = 0;
  int32_t length = 0;
  DWORD dwRead = 0;
  uint8_t data;

  if (ReadFile(m_Serialcfg.serial_handle, &data, (DWORD)1, &dwRead, NULL) == TRUE)
  {
    if (dwRead != 1)
    {
      length = 0;
    }
    else
    {
      length = 1;
      m_received_data = data;
      if (m_cbObj)
      {
        m_func(m_cbObj, &length, &m_received_data);
      }
    }
  }
  ret = length;

  return ret;
}



void SerialComm::threadJob(void)
{
#if 0
  DWORD       dwEvtMask;
  dwEvtMask = 0;
  // Event�� �߻��� �� ���� ��
  WaitCommEvent(m_Serialcfg.serial_handle, &dwEvtMask, NULL);
  if ((dwEvtMask & EV_RXCHAR) == EV_RXCHAR)
  {
    do
    {

    } while (1);

  }
  else
  {
    TRACE("Other Event\n\r");
  }
#endif
  //  serialAvailable();
  int32_t length = 0;
  DWORD dwRead = 0;
  uint8_t data;

  if (ReadFile(m_Serialcfg.serial_handle, &data, (DWORD)1, &dwRead, NULL) == TRUE)
  {
    if (dwRead != 1)
    {
      length = 0;
    }
    else
    {
      length = 1;
      m_received_data = data;
      m_Que.Put(data);
      if (m_cbObj)
      {
        m_func(m_cbObj, &length, &m_received_data);
      }
    }
  }

}




/****************************************************
 *	thread
 ****************************************************/
UINT SerialComm::threadFunc(LPVOID pParam)
{
  SerialComm* pThis = (SerialComm*)pParam;

  TRACE("CommWatch Thread Start\n\r");

  while (pThis->m_TrdLife)
  {
    pThis->threadJob();
    util::delay(10);
  }

  TRACE("CommWatch Thread Terminatet\n\r");
  pThis->m_TrdId = 0;
  pThis->m_hThread = NULL;
  return 0;
}

void SerialComm::cbFunc(void* obj, void* w_parm, void* l_parm)
{
  SerialComm* pThis = (SerialComm*)obj;

}
