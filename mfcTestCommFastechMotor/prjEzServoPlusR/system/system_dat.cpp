#include "pch.h"
#include "system_def.h"
#include "system_dat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


conf_dat::conf_dat(char* file_info) :m_pParser(nullptr)
{
  sprintf_s(m_dirFile, PRJ_FILE_DIR_STR_MAX - 1, file_info);
  m_pParser = new parser::json(m_dirFile, static_cast<uint8_t>(conf_dat::addr_e::_max));
}

conf_dat::~conf_dat()
{
  if (m_pParser)
  {
    delete m_pParser;
  }
}

int conf_dat::LoadData()
{  
  return m_pParser->LoadData();;
}

void conf_dat::PrintData()
{
  m_pParser->PrintData();
}

void conf_dat::WriteData(conf_dat::addr_e addr, parser::json_t data)
{
}

parser::json_t conf_dat::ReadData(conf_dat::addr_e addr)
{  
  return m_pParser->ReadData(static_cast<uint32_t>(addr));
}

parser::json_t* conf_dat::GetData(conf_dat::addr_e addr)
{
  return m_pParser->GetData(static_cast<uint32_t>(addr));
}


//
//bool conf_dat::putData(char** pdata, uint8_t cnt)
//{
//  bool is_input = false;
//  bool ret = false;
//  dat_t data = {};
//  for (uint8_t i = 0; i < cnt; i++) {
//    switch (i)
//    {
//    case 0:
//    {
//      trans::Replace(pdata[i], ' ');
//      trans::Replace(pdata[i], '\t', true);
//      is_input = compare::IsEqualStr(pdata[i], "INPUT");
//    }
//    break;
//    case 1:
//    {
//      trans::Replace(pdata[i], ' ');
//      trans::Replace(pdata[i], '\t', true);
//      if (is_input) {
//        //data.Set_type(IO_ADDR_BASE_INPUT);
//      }
//      else {
//        //data.Set_type(IO_ADDR_BASE_OUTPUT);
//      }
//      // data.Set_name(pdata[i]);
//    }
//    break;
//    case 2:
//    {
//      trans::Replace(pdata[i], ' ');
//      trans::Replace(pdata[i], '\t');
//      //data.addr = atoi(pdata[i]);
//      ret = true;
//    }
//    break;
//    }
//  }
//  if (ret) {
//    //  if (is_input)
//        //input[input_idx++] = data;
//    //  else
//        //output[output_idx++] = data;
//  }
//  return ret;
//}
//


#define APDAT_START_ADDRESS               0x0000

#define APDAT_START_ADDR                  APDAT_START_ADDRESS
#define APDAT_LENGTH                      4
#define APDAT_ADDR(x)                     APDAT_START_ADDR + ((x) * APDAT_LENGTH)
#define APDAT_CNT_MAX                     8
#define APDAT_END_ADDR                    APDAT_START_ADDR + (APDAT_LENGTH * APDAT_CNT_MAX)

#define APDAT_AXIS_POS_START_ADDR         APDAT_END_ADDR
#define APDAT_AXIS_POS_DATA_LENGTH        8
#define APDAT_AXIS_POS_DATA_ADDR(x)       APDAT_AXIS_POS_START_ADDR + ((x) * APDAT_AXIS_POS_DATA_LENGTH)
#define APDAT_AXIS_POS_DATA_CNT_MAX       8
#define APDAT_AXIS_POS_END_ADDR           APDAT_AXIS_POS_START_ADDR + (APDAT_AXIS_POS_DATA_LENGTH * APDAT_AXIS_POS_DATA_CNT_MAX)

#define APDAT_CYL_ACT_START_ADDR          APDAT_AXIS_POS_END_ADDR
#define APDAT_CYL_ACT_DATA_LENGTH         4
#define APDAT_CYL_ACT_DATA_ADDR(x)        APDAT_CYL_ACT_START_ADDR + ((x) * APDAT_CYL_ACT_DATA_LENGTH)
#define APDAT_CYL_ACT_DATA_CNT_MAX        8
#define APDAT_CYL_ACT_END_ADDR            APDAT_CYL_ACT_START_ADDR + (APDAT_CYL_ACT_DATA_LENGTH * APDAT_CYL_ACT_DATA_CNT_MAX)

#define APDAT_VAC_ACT_START_ADDR          APDAT_CYL_ACT_END_ADDR
#define APDAT_VAC_ACT_DATA_LENGTH         4
#define APDAT_VAC_ACT_DATA_ADDR(x)        APDAT_VAC_ACT_START_ADDR + ((x) * APDAT_VAC_ACT_DATA_LENGTH)
#define APDAT_VAC_ACT_DATA_CNT_MAX        8
#define APDAT_VAC_ACT_END_ADDR            APDAT_VAC_ACT_START_ADDR + (APDAT_VAC_ACT_DATA_LENGTH * APDAT_VAC_ACT_DATA_CNT_MAX)

#define APDAT_SEQ_START_ADDR              APDAT_VAC_ACT_END_ADDR
#define APDAT_SEQ_DATA_LENGTH             4
#define APDAT_SEQ_DATA_ADDR(x)            APDAT_SEQ_START_ADDR + ((x) * APDAT_SEQ_DATA_LENGTH)
#define APDAT_SEQ_DATA_CNT_MAX            8
#define APDAT_SEQ_END_ADDR                APDAT_SEQ_START_ADDR + (APDAT_SEQ_DATA_LENGTH * APDAT_SEQ_DATA_CNT_MAX)


#define APDAT_LOG_START_ADDR              APDAT_SEQ_END_ADDR
#define APDAT_LOG_DATA_LENGTH             64
#define APDAT_LOG_DATA_ADDR(x)            APDAT_LOG_START_ADDR + ((x) * APDAT_LOG_DATA_LENGTH)
#define APDAT_LOG_DATA_CNT_MAX            32
#define APDAT_LOG_END_ADDR                APDAT_LOG_START_ADDR + (APDAT_LOG_DATA_LENGTH * APDAT_LOG_DATA_CNT_MAX)



void ap_dat::WriteData(ap_dat::addr_e addr, ap_dat::dat_t data)
{
  apcfg_dat[static_cast<uint8_t>(addr)] = data;
  uint16_t rcv_data[2] = { data.parm1, data.parm2 };
  //TODO:: write mcu eeprom
}

ap_dat::dat_t ap_dat::ReadData(ap_dat::addr_e addr)
{
  return apcfg_dat[static_cast<uint8_t>(addr)];
}

ap_dat::dat_t* ap_dat::GetData(ap_dat::addr_e addr)
{
  return &apcfg_dat[static_cast<uint8_t>(addr)];
}

void ap_dat::LoadRomData()
{
  uint16_t rcv_data[2] = { 0, };
  for (uint8_t i = 0; i < static_cast<uint8_t>(ap_dat::addr_e::_max); i++) {
    //TODO:: read mcu eeprom
    apcfg_dat[i].parm1 = rcv_data[0];
    apcfg_dat[i].parm2 = rcv_data[1];
  }
}

bool ap_dat::ClearRomData()
{
  uint8_t data[APDAT_LENGTH] = { 0, };
  bool ret = false;
  for (uint8_t i = 0; i < static_cast<uint8_t>(ap_dat::addr_e::_max); i++) {
    //TODO:: write mcu eeprom
  }
  return ret;
}

void ap_dat::Write_MotCfg(ap_dat::dat_t data)
{
  apcfg_dat[static_cast<uint8_t>(ap_dat::addr_e::ap_mot_cfg)] = data;
  uint16_t rcv_data[2] = { data.parm1, data.parm2 };
  //TODO::write mcu eeprom
}

ap_dat::dat_t ap_dat::Read_MotCfg()
{
  return apcfg_dat[static_cast<uint8_t>(ap_dat::addr_e::ap_mot_cfg)];
}

void axis_dat::WriteData(axis_dat::addr_e addr, axis_dat::dat_t data)
{
  cmd_pos_dat[static_cast<uint8_t>(addr)] = data;
  //TODO:: write mcu eeprom
}

axis_dat::dat_t* axis_dat::GetData(axis_dat::addr_e addr)
{
  return &cmd_pos_dat[static_cast<uint8_t>(addr)];
}

axis_dat::dat_t axis_dat::ReadData(axis_dat::addr_e addr)
{
  return cmd_pos_dat[static_cast<uint8_t>(addr)];
}

void axis_dat::LoadRomData()
{
  uint32_t rcv_data[2] = { 0, };
  for (uint8_t i = 0; i < static_cast<uint8_t>(axis_dat::addr_e::_max); i++) {
    //TODO:: read mcu eeprom
    cmd_pos_dat[i].cmd_pos = (int)rcv_data[0];
    cmd_pos_dat[i].cmd_vel = rcv_data[1];
  }
}

bool axis_dat::ClearRomData()
{
  uint8_t data[8] = { 0, };
  bool ret = false;
  for (uint8_t i = 0; i < static_cast<uint8_t>(axis_dat::addr_e::_max); i++) {
    //TODO:: write mcu eeprom
  }
  return ret;
}

void cyl_dat::WriteData(cyl_dat::addr_e addr, cyl_dat::dat_t data)
{
  cyl_act_dat[static_cast<uint8_t>(addr)] = data;
  uint16_t rcv_data[2] = { data.timeout, data.on_settling };
  //TODO:: write mcu eeprom
}

cyl_dat::dat_t* cyl_dat::GetData(cyl_dat::addr_e addr)
{
  return &cyl_act_dat[static_cast<uint8_t>(addr)];
}

cyl_dat::dat_t cyl_dat::ReadData(cyl_dat::addr_e addr)
{
  return cyl_act_dat[static_cast<uint8_t>(addr)];
}

void cyl_dat::LoadRomData()
{
  uint16_t rcv_data[2] = { 0, };
  for (uint8_t i = 0; i < static_cast<uint8_t>(cyl_dat::addr_e::_max); i++) {
    //TODO:: read mcu eeprom
    cyl_act_dat[i].timeout = rcv_data[0];
    cyl_act_dat[i].on_settling = rcv_data[1];
  }
}

bool cyl_dat::ClearRomData()
{
  uint8_t data[APDAT_CYL_ACT_DATA_LENGTH] = { 0, };
  bool ret = false;
  for (uint8_t i = 0; i < static_cast<uint8_t>(cyl_dat::addr_e::_max); i++) {
    //TODO:: write mcu eeprom
  }
  return ret;
}

void vac_dat::WriteData(vac_dat::addr_e addr, vac_dat::dat_t data)
{
  vac_act_dat[static_cast<uint8_t>(addr)] = data;
  uint16_t rcv_data[2] = { data.timeout, data.on_settling };
  //TODO:: write mcu eeprom
}

vac_dat::dat_t* vac_dat::GetData(vac_dat::addr_e addr)
{
  return &vac_act_dat[static_cast<uint8_t>(addr)];
}

vac_dat::dat_t vac_dat::ReadData(vac_dat::addr_e addr)
{
  return vac_act_dat[static_cast<uint8_t>(addr)];
}

void vac_dat::LoadRomData()
{
  uint16_t rcv_data[2] = { 0, };
  for (uint8_t i = 0; i < static_cast<uint8_t>(vac_dat::addr_e::_max); i++) {
    //TODO:: read mcu eeprom
    vac_act_dat[i].timeout = rcv_data[0];
    vac_act_dat[i].on_settling = rcv_data[1];
  }
}

bool vac_dat::ClearRomData()
{
  uint8_t data[APDAT_VAC_ACT_DATA_LENGTH] = { 0, };
  bool ret = false;
  for (uint8_t i = 0; i < static_cast<uint8_t>(vac_dat::addr_e::_max); i++) {
    //TODO:: write mcu eeprom
  }
  return ret;
}

void seq_dat::WriteData(seq_dat::addr_e addr, seq_dat::dat_t data)
{
  sequencing_dat[static_cast<uint8_t>(addr)] = data;
  uint16_t rcv_data[2] = { data.parm1, data.parm2 };
  //TODO:: write mcu eeprom
}

seq_dat::dat_t* seq_dat::GetData(seq_dat::addr_e addr)
{
  return &sequencing_dat[static_cast<uint8_t>(addr)];
}

seq_dat::dat_t seq_dat::ReadData(seq_dat::addr_e addr)
{
  return sequencing_dat[static_cast<uint8_t>(addr)];
}

void seq_dat::LoadRomData()
{
  uint16_t rcv_data[2] = { 0, };
  for (uint8_t i = 0; i < static_cast<uint8_t>(seq_dat::addr_e::_max); i++) {
    //TODO:: read mcu eeprom
    sequencing_dat[i].parm1 = rcv_data[0];
    sequencing_dat[i].parm2 = rcv_data[1];
  }
}

bool seq_dat::ClearRomData()
{
  uint8_t data[APDAT_SEQ_DATA_LENGTH] = { 0, };
  bool ret = false;
  for (uint8_t i = 0; i < static_cast<uint8_t>(seq_dat::addr_e::_max); i++) {
    //TODO:: write mcu eeprom
  }
  return ret;
}
