#include "pch.h"
#include "system_def.h"

#include "_inc/motor_fastech_comm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

fm_comm::fm_comm(fm_comm::cfg_t &cfg):m_commCfg(cfg)
{

  m_commCfg.pComm->AttCallbackFunc(this, receiveDataCB);
  threadRun();
}

fm_comm::~fm_comm()
{
  threadStop();
}


void fm_comm::threadStop(void)
{
  m_TrdLife = false;
  while (m_TrdId);
}
void fm_comm::threadRun(void)
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


void fm_comm::threadJob(void)
{
  uint8_t data[4096] = { 0, };
  uint8_t tmp[480] = { 0, };
  uint8_t hex2char[80] = { 0, };

  int length = m_Que.Available();
  if (length > 16)
  {
    int index = 0;
    for (index = 0; index < length; index++)
    {
      uint8_t data = 0;
      m_Que.Get(&data);
      sprintf_s((char* const)hex2char, sizeof(hex2char), "0x%02X ", data);
      if ((index > 0) && (index % 16 == 0))
      {

      }
      else
      {
        //	memcpy()
        strncat_s((char* const)tmp, sizeof(tmp), (const char*)hex2char, 5);
      }
    };
    std::cout << tmp << std::endl;
  }

}

UINT fm_comm::threadFunc(LPVOID pParam)
{
  fm_comm* pThis = (fm_comm*)pParam;

  TRACE("motor commucation Thread Start\n\r");

  while (pThis->m_TrdLife)
  {
    pThis->threadJob();

    util::delay(10);
  }

  TRACE("motor commucation Thread Terminatet\n\r");
  pThis->m_TrdId = 0;
  pThis->m_hThread = NULL;
  return 0;
}

void fm_comm::receiveDataCB(void* obj, void* w_parm, void* l_parm)
{
  fm_comm* p_this = (fm_comm*)obj;
  int length = *((int*)w_parm);
  int index = 0;
  for (index = 0; index < length; index++)
  {
    uint8_t data = *((uint8_t*)l_parm + index);
    p_this->m_Que.Put(data);
  }

}
