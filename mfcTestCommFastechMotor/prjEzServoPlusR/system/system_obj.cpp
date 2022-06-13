#include "pch.h"
#include "system_def.h"

#include "system_obj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static ObjDataDef::Info_t systemObj[] =
{
  { false,ObjType::NONE					    ,NULL							    ,0	,"None"						,""												,NULL},
  { false,ObjType::HL_IO				    ,HL_IO_BASE				    ,0	,"IO"							,"./Log/IO.log"						,NULL},
  { false,ObjType::HL_SERIAL		    ,HL_SERIAL_BASE		    ,0	,"Serial_1"				,"./Log/Serial.log"				,NULL},
  { false,ObjType::HL_ETHERNET      ,HL_ETHERNET_BASE	    ,0	,"Ethernet_1"			,"./Log/Ethernet.log"			,NULL},
  { false,ObjType::HL_MOTION		    ,HL_AXIS_BASE				  ,0	,"FestechMotor"		,"./Log/Axis.log"					,NULL},
  { false,ObjType::ML_OP_PANEL	    ,ML_OPPANEL_BASE	    ,0	,"Op Panel"				,"./Log/OpPanel.log"			,NULL},
  { false,ObjType::PL_AUTO_MANAGER	,TRS_AUTOMANAGER_BASE	,0	,"Auto Manager"		,"./Log/AutoManager.log"	,NULL},
};



sysObj::sysObj() :m_idx(0), m_infoObject(nullptr), m_iNumObjects(0)
{
  makeSystemInfo();
}

sysObj::~sysObj()
{
  if (m_infoObject)
  {
    delete[] m_infoObject;
  }
}

void sysObj::makeSystemInfo()
{
  ObjDataDef::Info_t* pSys = &systemObj[0];
  int len = sizeof(systemObj) / sizeof(ObjDataDef::Info_t);
  m_infoObject = new ObjDataDef[len];
  int i = 0;
  while (i < len)
  {
    if (pSys->type == ObjType::NONE)
      goto _exit;

    m_infoObject[i].SetData(pSys);

    m_iNumObjects++;
  _exit:
    i++;
    pSys++;
  }
}

ObjDataDef* sysObj::getObjectData(int obj_id) const
{
  ObjDataDef* p_data = &m_infoObject[0];
  int max = m_iNumObjects;
  while ((p_data->IsMatchingID(obj_id)) == false)
  {
    p_data++;
    if (0 == max--)
    {
      return nullptr;
    }
  }
  return p_data;
}

common::Attribute_t sysObj::GetCommonData(int id)
{
  common::Attribute_t commonData;
  ObjDataDef* p_data = getObjectData(id);
  if (p_data == nullptr)
    return commonData;

  commonData.SetObjectID(id);
  commonData.SetInstanceNo(p_data->GetObjectInstanceNo());
  commonData.SetObjectName(p_data->GetObjectName());
  commonData.SetLogFileName(p_data->GetLogFileName());
  return commonData;
}

ObjType sysObj::GetObjectType(int id) const
{
  ObjDataDef* p_data = getObjectData(id);
  if (p_data == nullptr)
    return ObjType();
  return p_data->GetObjectType();
}

char* sysObj::GetObjectName(int id) const
{
  ObjDataDef* p_data = getObjectData(id);
  if (p_data == nullptr)
    return (char*)nullptr;
  return p_data->GetObjectName();
}

char* sysObj::GetLogFileName(int id) const
{
  ObjDataDef* p_data = getObjectData(id);
  if (p_data == nullptr)
    return (char*)nullptr;
  return p_data->GetLogFileName();
}

Icommon* sysObj::GetSystemObject(int id)
{
  ObjDataDef* p_data = getObjectData(id);
  if (p_data == nullptr)
    return (Icommon*)nullptr;
  return p_data->GetSystemObject();
}

errno_t sysObj::RegisterComponent(int id, Icommon* pComp)
{
  ObjDataDef* p_data = getObjectData(id);
  if (p_data == nullptr)
    return ERR_SYSTEMINFO_INVALID_OBJECTID;

  p_data->SetSystemObject(pComp);
  return ERR_SYSTEMINFO_SUCCESS;
}
