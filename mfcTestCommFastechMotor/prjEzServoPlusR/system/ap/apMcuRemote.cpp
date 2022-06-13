#include "pch.h"

#include "system_def.h"
#include "apMcuRemote.h"

#include "enMcuRemote.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

apMcuRemote::apMcuRemote(common::Attribute_t common_data, cfg_t* cfg)
  :Icommon(common_data), m_pEnMcuRemote(nullptr)
  , m_TrdLife(false), m_TrdId(0), m_hThread(0)
{
  m_pEnMcuRemote = cfg->p_enMcuRemote;

}

apMcuRemote::~apMcuRemote()
{
  if (m_TrdLife)
  {
    ThreadStop();
  }

}


bool apMcuRemote::IsConnected()
{
  return m_pEnMcuRemote->IsConnected();
}



#if 0

errno_t apMcuRemote::SendCmd(uint8_t cmd, uint8_t* p_data, uint8_t length)
{
  return errno_t();
}

errno_t apMcuRemote::SendResp(uint8_t cmd, uint8_t* p_data, uint8_t length)
{
  return errno_t();
}

errno_t apMcuRemote::SendCmdRxResp(uint8_t cmd, uint8_t* p_data, uint8_t length)
{
  return errno_t();
}

void apMcuRemote::AttCallbackFunc(void* obj, comm_rcv_cb cb)
{
}

#endif // 0



void apMcuRemote::ThreadStop(void)
{
  m_TrdLife = false;
  while (m_TrdId != 0);
}

void apMcuRemote::ThreadRun(void)
{
  if (m_TrdLife) {
    ThreadStop();
    Sleep(10);
  }

  // ½º·¹µå »ý¼º
  m_hThread = CreateThread(
    (LPSECURITY_ATTRIBUTES)NULL,
    0,
    (LPTHREAD_START_ROUTINE)threadFunc,
    (LPVOID)this,
    0,
    &m_TrdId
  );

  if (!m_hThread)
  {
    // ½ÇÆÐ
    TRACE(L"Error create thread for apMcuRemote! \n\r");
  }
  else
    m_TrdLife = true;
}

void apMcuRemote::threadJob(void)
{
  m_pEnMcuRemote->RequestMcuState();
}

/****************************************************
 *	thread
 ****************************************************/
UINT apMcuRemote::threadFunc(LPVOID pParam)
{
  apMcuRemote* pThis = (apMcuRemote*)pParam;
  TRACE(L"start thread for apMcuRemote! \n\r");
  while (pThis->m_TrdLife) {
    pThis->threadJob();
    Sleep(10);
  }
  TRACE(L"teminate thread for apMcuRemote! \n\r");
  pThis->m_TrdId = 0;
  return 0;
}