#pragma once

/*
 * system.h
 * 
 * 작성자 : 이길훈
 * 22.04.04 
 * system header
 */

 /* 아래 순서대로 절차를 진행하며 가능한 interface를 상속한다 */
 /*
  system_def.h   <-   system.h
                         |
                      hal_xx.h
                         |
                      engine_xx.h
                         |
                      control_xx.h
                         |
                      ap_xx.h

 */


#include "system_def.h"

/* hw object*/
#include "_inc/SerialComm.h"
#include "_inc/netSocket.h"
/* engine object*/

/* control object*/

/* ap object*/


class apSystem
{
 /****************************************************
  *	data
  ****************************************************/
  char m_ProjectDir[_MAX_DIR];
  sysObj* m_pObjinfo;

  conf_dat* m_pCfgDat;
  ap_dat* m_pApDat;
  axis_dat* m_pAxisDat;
  cyl_dat* m_pCylDat;
  vac_dat* m_pVacDat;
  seq_dat* m_pSeqDat;
  systemIO* m_pIo;

  SerialComm* m_pSerialComm;
  netSocket* m_pNetComm;


 /****************************************************
  *	Constructor
  ****************************************************/
public:
  apSystem();
	virtual ~apSystem();

 /****************************************************
  *	func
  ****************************************************/
private:
  void init();
  void terminate();
  void destroy();
  void startThreads();
  void stopThreads();
  void getMcuData();

  errno_t _createSerialCommComponents();
  errno_t _createEthernetCommComponents();
  errno_t _createBasicHWComponents();
  errno_t _createEngineComponets();
  errno_t _createApComponts();

public:
  errno_t Initialize(openExe_cb cd_increase = nullptr);
  //IIO* GetIOComponent();
  //enMcuRemote* GetEnMcuRemote();
  //apMcuRemote* GetApMcuRemote();

  IComm* GetSerialCommComponent();
  conf_dat* GetCfgDat();
  ap_dat* GetApDat();
  axis_dat* GetAxisDat();
  cyl_dat* GetCylDat();
  vac_dat* GetVacDat();
  seq_dat* GetSeqDat();
  inline netSocket* GetEthernetComponent() const{
    return m_pNetComm;
  }


};
