#pragma once

class enMcuRemote;

class apMcuRemote :public Icommon
{
  /****************************************************
   *	data
   ****************************************************/
public:
  struct cfg_t {
    IIO* p_Io;
    enMcuRemote* p_enMcuRemote;

    inline void operator = (const cfg_t* p_cfg) {
      this->p_Io = p_cfg->p_Io;
      this->p_enMcuRemote = p_cfg->p_enMcuRemote;
    }

  };
private:
  enMcuRemote* m_pEnMcuRemote;


  bool m_TrdLife;
  DWORD m_TrdId;
  HANDLE m_hThread;
public:
  /****************************************************
   *	Constructor
   ****************************************************/
public:
  apMcuRemote(common::Attribute_t common_data, cfg_t* cfg);

  virtual ~apMcuRemote();


  /****************************************************
   *	func
   ****************************************************/
private:
  void threadJob(void);
  static UINT threadFunc(LPVOID pParam);
public:

  void ThreadStop(void);
  void ThreadRun(void);

  bool IsConnected();

  //errno_t SendCmd(uint8_t cmd, uint8_t* p_data, uint8_t length);
  //errno_t SendResp(uint8_t cmd, uint8_t* p_data, uint8_t length);
  //errno_t SendCmdRxResp(uint8_t cmd, uint8_t* p_data, uint8_t length);
  //void AttCallbackFunc(void* obj, comm_rcv_cb cb);

};