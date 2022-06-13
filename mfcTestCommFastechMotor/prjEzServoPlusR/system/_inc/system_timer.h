#pragma once

#define DEF_SW_TIMER_MAX   10

class sysTimer
{
  /****************************************************
   *	data
   ****************************************************/
public:
  enum class mode_e
  {
    ONE_TIME = 1,
    LOOP_TIME = 2,
  };

  struct obj_t
  {
    void* obj{};  // class 
    bool enable{ false };     // 타이머 인에이블 신호
    sysTimer::mode_e mode{};          // 타이머 모드
    uint64_t init_ms{};           // 현재의 타이머 값
    uint64_t cnt_ms{};         // 타이머 초기화될때의 카운트 값
    swTimer_cb func_cb{};            // 만료될때 실행될 함수
  };

private:
  sysTimer::obj_t m_sysTimer[DEF_SW_TIMER_MAX];
  uint32_t m_availableCnt;
  uint32_t m_indexTop;
  bool m_isExcute;
  volatile uint64_t m_counter;

public:
  bool	m_bTrdLife;
  DWORD	m_TrdId;
  HANDLE m_hThread;

  /****************************************************
   *	Constructor
   ****************************************************/
public:
  sysTimer();
  virtual ~sysTimer();

  /****************************************************
   *	func
   ****************************************************/
private:
  static UINT timerCount(void* pObj, void* p_arg);

public:
  DWORD ThreadRun();

  int Terminate();

  bool Init(void);

  int Set(sysTimer::obj_t* p_data);

  bool Start(uint8_t ch);

  bool Stop(uint8_t ch);

  bool Reset(uint8_t ch);

  void ISR(void);

  bool CloseHandle(uint8_t ch);

  int  GetHandle(void);

};
