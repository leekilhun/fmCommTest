#include "pch.h"
#include "system_def.h"
#include "_inc/comport_serial.h"

SerialComport::SerialComport(cfg_t* cfg)
  :m_Serial_handle(), m_IsOpened(false), m_TrdLife(false), m_received_data(0), m_cbObj(nullptr),m_func(nullptr)
{
  m_Serialcfg = cfg;
}

SerialComport::~SerialComport()
{
  if (m_IsOpened)
    Close();
}

errno_t SerialComport::openPort(char* port_name, uint32_t baud)
{
  uint32_t err_code = 0;

  DCB dcb;
  COMMTIMEOUTS timeouts;
  DWORD dwError;

  char name[MAX_PATH] = {0,};
  sprintf_s(name,MAX_PATH,"//./%s", port_name);

  m_Serialcfg.serial_handle = CreateFileA(name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  if (m_Serialcfg.serial_handle == INVALID_HANDLE_VALUE)
  {
    TRACE("Error opening serial port!\n");
    return 2;
  }

  while (1)
  {
    dcb.DCBlength = sizeof(DCB);
    if (GetCommState(m_Serialcfg.serial_handle, &dcb) == FALSE)
    {
      TRACE("Error GetCommState\n");
      err_code = 1;
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
      err_code = 2;

      TRACE("SetCommState err: %d\n", (int)dwError);
      break;
    }

    if (SetCommMask(m_Serialcfg.serial_handle, 0) == FALSE) // Not using Comm event
    {
      err_code = 3;
      break;
    }
    if (SetupComm(m_Serialcfg.serial_handle, 4096, 4096) == FALSE) // Buffer size (Rx,Tx)
    {
      err_code = 4;
      break;
    }
    if (PurgeComm(m_Serialcfg.serial_handle, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR) == FALSE) // Clear buffer
    {
      err_code = 5;
      break;
    }
    if (ClearCommError(m_Serialcfg.serial_handle, &dwError, NULL) == FALSE)
    {
      err_code = 6;
      break;
    }

    if (GetCommTimeouts(m_Serialcfg.serial_handle, &timeouts) == FALSE)
    {
      err_code = 7;
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
      err_code = 8;
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
    threadRun();
    m_IsOpened = true;
  }

  return err_code;
}

bool SerialComport::closePort()
{
  bool ret = false;
  ret = CloseHandle(m_Serialcfg.serial_handle);
  m_IsOpened = false;
  return ret;
}


errno_t SerialComport::Open()
{
  return openPort(m_Serialcfg.GetPortName(), m_Serialcfg.baud);
}

void SerialComport::Close()
{
  if (m_Serial_handle == nullptr) 
    return;
  m_TrdLife = false;
  while (m_TrdId);
  closePort();
}

void SerialComport::Recovery()
{
}

bool SerialComport::IsOpened() const
{
  return m_IsOpened;
}

uint32_t SerialComport::Available()
{
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
        m_func(m_cbObj, 0, &m_received_data);
      }
    }
  }
  ret = length;

  return ret;
}

uint8_t SerialComport::Read()
{
  return m_received_data;
}

uint32_t SerialComport::Write(uint8_t* p_data, uint32_t length)
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

uint32_t SerialComport::CommPrintf(char* fmt, ...)
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

void SerialComport::AttCallbackFunc(void* obj, evt_cb cb)
{
  m_cbObj = obj;
  m_func = cb;
}


void SerialComport::threadStop(void)
{
  closePort();
}

void SerialComport::threadRun(void)
{
  m_TrdLife = true;
  // 포트 감시 스레드 생성
  m_hThread = CreateThread(
    (LPSECURITY_ATTRIBUTES)NULL,
    0,
    (LPTHREAD_START_ROUTINE)threadFunc,
    (LPVOID)this,
    0,
    &m_TrdId
  );
}

void SerialComport::threadJob(void)
{
  Available();  
}

UINT SerialComport::threadFunc(LPVOID pParam)
{
  SerialComport* pThis = (SerialComport*)pParam;

  TRACE("Serial Comport Thread Start\n\r");

  while (pThis->m_TrdLife)
  {
    pThis->threadJob();

    util::delay(10);
  }

  TRACE("Serial Comport Thread Terminatet\n\r");
  pThis->m_TrdId = 0;
  pThis->m_hThread = NULL;
  return 0;
}