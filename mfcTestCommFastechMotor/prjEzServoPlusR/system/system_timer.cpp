#include "pch.h"
#include "system_def.h"
#include "system_timer.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

sysTimer::sysTimer():m_isExcute(false), m_availableCnt(0), m_indexTop(0)
, m_bTrdLife(false), m_TrdId(0), m_hThread(), m_counter(0)
{
  m_counter = 0;
}

sysTimer::~sysTimer()
{
  if (m_bTrdLife) {
    m_bTrdLife = false;

    while (m_TrdId != 0);
  }
}

UINT sysTimer::timerCount(void* pObj, void* p_arg)
{
  sysTimer* pThis = (sysTimer*)pObj;
  uint32_t pre_us = util::micros();
  uint32_t pass_us = 0;
  TRACE("Scheduler ISR Thread start\n");
  while (pThis->m_bTrdLife)
  {
    /* make 1ms tick */
    pass_us = util::micros() - pre_us;
    if (pass_us > 999) {
      pre_us = util::micros();
      pThis->ISR();
    }
    //Sleep(1);
  }
  TRACE("Scheduler ISR Thread Terminatet\n");
  pThis->m_TrdId = 0;
  return 0;
}

DWORD sysTimer::ThreadRun()
{
  m_hThread = CreateThread(
    (LPSECURITY_ATTRIBUTES)NULL,
    0,
    (LPTHREAD_START_ROUTINE)timerCount,
    (LPVOID)this,
    0,
    &this->m_TrdId
  );
  if (!m_hThread)
    TRACE("Fail Scheduler ISR Thread \n");
  else
    m_bTrdLife = true;

  return this->m_TrdId;
}

int sysTimer::Terminate()
{
  m_bTrdLife = false;
  while (m_TrdId != 0);
  return 0;
}

bool sysTimer::Init(void)
{
  if (m_isExcute == true)
    return false;  // 

  //m_sysTimer[DEF_SW_TIMER_MAX]
  for (int i = 0; i < DEF_SW_TIMER_MAX; i++) 
  {
    m_sysTimer[i].obj = nullptr;
    m_sysTimer[i].enable = false;
    m_sysTimer[i].mode = mode_e::ONE_TIME;
    m_sysTimer[i].init_ms = 0;
    m_sysTimer[i].cnt_ms = 0;
    m_sysTimer[i].func_cb = nullptr;
  }
  m_isExcute = true;
  m_availableCnt = DEF_SW_TIMER_MAX;
  m_indexTop = 0;

  return true;
}

int sysTimer::Set(sysTimer::obj_t* p_data)
{
  int ret = -1;
  if (p_data->obj == nullptr)
    return ret;

  if (m_availableCnt > 0) {
    int ch = GetHandle();
    m_sysTimer[ch].mode = p_data->mode;
    m_sysTimer[ch].obj = p_data->obj;
    m_sysTimer[ch].func_cb = p_data->func_cb;
    m_sysTimer[ch].init_ms = p_data->cnt_ms;
    m_sysTimer[ch].cnt_ms = p_data->cnt_ms;
    m_sysTimer[ch].enable = false;
    ret = ch;
    m_availableCnt--;
    m_indexTop++;
  }

  return ret;
}

bool sysTimer::Start(uint8_t ch)
{
  if (ch < DEF_SW_TIMER_MAX == false)
    return false;

  m_sysTimer[ch % DEF_SW_TIMER_MAX].cnt_ms = m_sysTimer[ch % DEF_SW_TIMER_MAX].init_ms;
  m_sysTimer[ch % DEF_SW_TIMER_MAX].enable = true;

  return true;
}

bool sysTimer::Stop(uint8_t ch)
{
  if (ch < DEF_SW_TIMER_MAX == false)
    return false;

  m_sysTimer[ch % DEF_SW_TIMER_MAX].enable = false;
  return true;
}

bool sysTimer::Reset(uint8_t ch)
{
  if (ch < DEF_SW_TIMER_MAX == false)
    return false;

  m_sysTimer[ch % DEF_SW_TIMER_MAX].enable = false;
  m_sysTimer[ch % DEF_SW_TIMER_MAX].cnt_ms = m_sysTimer[ch % DEF_SW_TIMER_MAX].init_ms;

  return true;
}

void sysTimer::ISR(void)
{
  int ret = 0;

  m_counter++;

  for (uint32_t i = 0; i < DEF_SW_TIMER_MAX && i < m_indexTop; i++)
  {
    if (m_sysTimer[i].enable == true) 
    {
      m_sysTimer[i].cnt_ms--; 

      if (m_sysTimer[i].cnt_ms == 0)      
      {
        if (m_sysTimer[i].mode == mode_e::ONE_TIME)        
        {
          m_sysTimer[i].enable = false;                 
        }

        m_sysTimer[i].cnt_ms = m_sysTimer[i].init_ms; 
        m_sysTimer[i].func_cb(m_sysTimer[i].obj);     
      }
    }
  }

  return;
}

bool sysTimer::CloseHandle(uint8_t ch)
{
  if (ch < DEF_SW_TIMER_MAX != false)
    return false;
  m_sysTimer[ch % DEF_SW_TIMER_MAX].obj = nullptr;
  m_sysTimer[ch % DEF_SW_TIMER_MAX].enable = false;
  m_sysTimer[ch % DEF_SW_TIMER_MAX].mode = mode_e::ONE_TIME;
  m_sysTimer[ch % DEF_SW_TIMER_MAX].init_ms = 0;
  m_sysTimer[ch % DEF_SW_TIMER_MAX].cnt_ms = 0;
  m_sysTimer[ch % DEF_SW_TIMER_MAX].func_cb = nullptr;

  m_availableCnt++;
  for (uint32_t i = 0; i < DEF_SW_TIMER_MAX; i++) {
    if (m_sysTimer[(DEF_SW_TIMER_MAX - i) % DEF_SW_TIMER_MAX].obj != nullptr) {
      m_indexTop = DEF_SW_TIMER_MAX - i + 1;
      goto out;
    }
  }
  m_indexTop = 0;
out:
  return true;
}



int sysTimer::GetHandle(void)
{
  int ret = -1;
  uint32_t idx = 0;
  while (idx < DEF_SW_TIMER_MAX) {
    if (m_sysTimer[idx++].obj == nullptr) {
      ret = --idx;
      break;
    }
  }
  return ret;
}