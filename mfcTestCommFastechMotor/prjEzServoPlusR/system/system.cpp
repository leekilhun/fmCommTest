#include "pch.h"
# include "system.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

apSystem::apSystem()
{
  memset(m_ProjectDir, 0x00, _MAX_DIR);
  //setbuf(stdout, NULL);
  init();
}

apSystem::~apSystem()
{
  terminate();
  init();
}

void apSystem::init()
{
  m_pObjinfo = nullptr;
  m_pCfgDat = nullptr;
  m_pApDat = nullptr;
  m_pAxisDat = nullptr;
  m_pCylDat = nullptr;
  m_pVacDat = nullptr;
  m_pSeqDat = nullptr;
  m_pSerialComm = nullptr;
  m_pNetComm = nullptr;
  m_pIo = nullptr;
}

void apSystem::terminate()
{
  stopThreads();
  destroy();
}

void apSystem::destroy()
{
  if (m_pObjinfo) { delete m_pObjinfo; } m_pObjinfo = nullptr;
  if (m_pCfgDat) { delete m_pCfgDat; }  m_pCfgDat = nullptr;
  if (m_pApDat) { delete m_pApDat; } m_pApDat = nullptr;
  if (m_pAxisDat) { delete m_pAxisDat; } m_pAxisDat = nullptr;
  if (m_pCylDat) { delete m_pCylDat; } m_pCylDat = nullptr;
  if (m_pVacDat) { delete m_pVacDat; } m_pVacDat = nullptr;
  if (m_pSeqDat) { delete m_pSeqDat; } m_pSeqDat = nullptr;

  if (m_pSerialComm) { delete m_pSerialComm; } m_pSerialComm = nullptr;
  if (m_pNetComm) { delete m_pNetComm; } m_pNetComm = nullptr;
  if (m_pIo) { delete m_pIo; } m_pIo = nullptr;

}

void apSystem::startThreads()
{
}

void apSystem::stopThreads()
{
}

void apSystem::getMcuData()
{
}

errno_t apSystem::_createSerialCommComponents()
{
  SerialComm::cfg_t cfg = {};

  m_pSerialComm = new SerialComm(m_pObjinfo->GetCommonData(HL_SERIAL_BASE), &cfg);

  errno_t result = m_pSerialComm->OpenPort();

  if (m_pObjinfo->RegisterComponent(HL_SERIAL_BASE, m_pSerialComm))
    TRACE("RegisterComponent Result Fail!(HL_SERIAL_BASE, m_pSerialComm)\n");

  return ERR_SUCCESS;
}

errno_t apSystem::_createEthernetCommComponents()
{
  netSocket::cfg_t cfg = {};
  cfg.mode = netSocket::mode_e::TCP_Client;
  cfg.port_no = 2002;
  cfg.SetIpStr("192.168.0.4");

  m_pNetComm = new netSocket(cfg);


  m_pNetComm->Open();
  /*
  if (m_pObjinfo->RegisterComponent(HL_ETHERNET_BASE, m_pNetComm))
    TRACE("RegisterComponent Result Fail!(HL_ETHERNET_BASE, m_pNetComm)\n");
*/


  return errno_t();
}

errno_t apSystem::_createBasicHWComponents()
{
  return ERR_SUCCESS;
}

errno_t apSystem::_createEngineComponets()
{
  return ERR_SUCCESS;
}

errno_t apSystem::_createApComponts()
{
  return ERR_SUCCESS;
}

errno_t apSystem::Initialize(openExe_cb cd_increase)
{
  m_pObjinfo = new sysObj();

  m_pCfgDat = new conf_dat();
  m_pCfgDat->LoadData();


  Table_IO* io  = new Table_IO;

  io->LoadData();

  delete io;

  _createSerialCommComponents();

  //_createEthernetCommComponents();

  return ERR_SUCCESS;
}



IComm* apSystem::GetSerialCommComponent()
{
  return nullptr;
}

conf_dat* apSystem::GetCfgDat()
{
  return nullptr;
}

ap_dat* apSystem::GetApDat()
{
  return nullptr;
}

axis_dat* apSystem::GetAxisDat()
{
  return nullptr;
}

cyl_dat* apSystem::GetCylDat()
{
  return nullptr;
}

vac_dat* apSystem::GetVacDat()
{
  return nullptr;
}

seq_dat* apSystem::GetSeqDat()
{
  return nullptr;
}
