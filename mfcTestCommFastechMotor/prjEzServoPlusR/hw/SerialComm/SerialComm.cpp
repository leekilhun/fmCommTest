#include "pch.h"
#include "system.h"

#include "_inc/comport_serial.h"
#include "_inc/SerialComm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

SerialComm::SerialComm(common::Attribute_t common_data, SerialComm::cfg_t* cfg)
  :Icommon(common_data) 
{
  m_Serialcfg = cfg;
  m_TrdLife = false;
  m_pSerial = nullptr;
  m_IsConnected = false;
  m_hThread = {};
  m_TrdId = 0;

  SerialComport::cfg_t comm_cfg = { 0, };
  comm_cfg.SetPortName("COM24");
  comm_cfg.baud = 230400;
    
  m_pSerial = new SerialComport(&comm_cfg);
}



SerialComm::~SerialComm()
{
  if (m_pSerial) delete m_pSerial; m_pSerial = nullptr;
  m_TrdLife = false;
  m_IsConnected = false;
  m_hThread = {};
  m_TrdId = 0;
}





errno_t SerialComm::OpenPort()
{
  m_pSerial->AttCallbackFunc(this, cbFunc);
  m_pSerial->Open();
  return 0;
}
void SerialComm::ClosePort()
{

}
bool SerialComm::IsConnected()
{
  return false;
}
uint32_t SerialComm::SendReceiveData()
{
  return 0;
}

errno_t SerialComm::SendCmd(uint8_t cmd, uint8_t* p_data, uint8_t length)
{
  return 0;
}
errno_t SerialComm::SendResp(uint8_t cmd, uint8_t* p_data, uint8_t length)
{
  return 0;
}
errno_t SerialComm::SendCmdRxResp(uint8_t cmd, uint8_t* p_data, uint8_t length)
{
  return 0;
}

uint32_t SerialComm::ReadByte(uint8_t* resp, uint32_t len)
{
  return 0;
}
uint32_t SerialComm::WriteByte(uint8_t* send, uint32_t len)
{
  return 0;
}
void SerialComm::AttCallbackFunc(void* obj, evt_cb cb)
{
}


void SerialComm::threadStop(void)
{
  ClosePort();
}

void SerialComm::threadRun(void)
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

void SerialComm::threadJob(void)
{
  SendReceiveData();
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

    util::delay(2);
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
