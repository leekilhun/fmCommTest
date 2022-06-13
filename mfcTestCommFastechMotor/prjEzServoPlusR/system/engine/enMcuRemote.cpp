#include "pch.h"

#include "system_def.h"
#include "enMcuRemote.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define EN_MCU_REG_BANK_ERR_H            (uint8_t)0x00
#define EN_MCU_REG_BANK_ERR_L            (uint8_t)0x01
#define EN_MCU_REG_BANK_SETTING          (uint8_t)0x02
#define EN_MCU_REG_BANK_RUN_STATE        (uint8_t)0x03
#define EN_MCU_IO_IN_BANK_0              (uint8_t)0x04
#define EN_MCU_IO_IN_BANK_1              (uint8_t)0x05
#define EN_MCU_IO_IN_BANK_2              (uint8_t)0x06
#define EN_MCU_IO_IN_BANK_3              (uint8_t)0x07
#define EN_MCU_IO_OUT_BANK_0             (uint8_t)0x08
#define EN_MCU_IO_OUT_BANK_1             (uint8_t)0x09
#define EN_MCU_IO_OUT_BANK_2             (uint8_t)0x0a
#define EN_MCU_IO_OUT_BANK_3             (uint8_t)0x0b
#define EN_MCU_MOT_STATE_BANK_ERR_0      (uint8_t)0x0c
#define EN_MCU_MOT_STATE_BANK_ERR_1      (uint8_t)0x0d
#define EN_MCU_MOT_STATE_BANK_STATE_0    (uint8_t)0x0e
#define EN_MCU_MOT_STATE_BANK_STATE_1    (uint8_t)0x0f


#define EN_MCU_REG_ERR_CLEAR             REG_BIT(0)
#define EN_MCU_REG_ERR_NO_RESP_LCD       REG_BIT(1)
#define EN_MCU_REG_ERR_NO_RESP_MOT       REG_BIT(2)
#define EN_MCU_REG_ERR_B03               REG_BIT(3)
#define EN_MCU_REG_ERR_B04               REG_BIT(4)
#define EN_MCU_REG_ERR_B05               REG_BIT(5)
#define EN_MCU_REG_ERR_B06               REG_BIT(6)
#define EN_MCU_REG_ERR_B07               REG_BIT(7)

#define EN_MCU_REG_ERR_HW_PLUS_LIMIT     REG_BIT(0)
#define EN_MCU_REG_ERR_HW_MINU_LIMIT     REG_BIT(1)
#define EN_MCU_REG_ERR_SW_PLUS_LIMIT     REG_BIT(2)
#define EN_MCU_REG_ERR_SW_MINU_LIMIT     REG_BIT(3)
#define EN_MCU_REG_ERR_B14               REG_BIT(4)
#define EN_MCU_REG_ERR_B15               REG_BIT(5)
#define EN_MCU_REG_ERR_B16               REG_BIT(6)
#define EN_MCU_REG_ERR_B17               REG_BIT(7)

#define EN_MCU_REG_USE_BEEP              REG_BIT(0)
#define EN_MCU_REG_B21                   REG_BIT(1)
#define EN_MCU_REG_B22                   REG_BIT(2)
#define EN_MCU_REG_B23                   REG_BIT(3)
#define EN_MCU_REG_B24                   REG_BIT(4)
#define EN_MCU_REG_B25                   REG_BIT(5)
#define EN_MCU_REG_B26                   REG_BIT(6)
#define EN_MCU_REG_B27                   REG_BIT(7)

#define EN_MCU_REG_AUTO_RUNNING          REG_BIT(0)
#define EN_MCU_REG_EMG_STOP              REG_BIT(1)
#define EN_MCU_REG_AUTO_READY            REG_BIT(2)
#define EN_MCU_REG_ERROR_STOP            REG_BIT(3)
#define EN_MCU_REG_B34                   REG_BIT(4)
#define EN_MCU_REG_MOTOR_ON              REG_BIT(5)
#define EN_MCU_REG_ORG_COMPLETED         REG_BIT(6)
#define EN_MCU_REG_ALARM_STATUS          REG_BIT(7)

#define EN_MCU_MOT_REG_ERR_ALL           REG_BIT(0)
#define EN_MCU_MOT_REG_HW_PLUS_LIMIT     REG_BIT(1)
#define EN_MCU_MOT_REG_HW_MINUS_LIMIT    REG_BIT(2)
#define EN_MCU_MOT_REG_SW_PLUS_LIMIT     REG_BIT(3)
#define EN_MCU_MOT_REG_SW_MINUS_LIMIT    REG_BIT(4)
#define EN_MCU_MOT_REG_ERR_RESERVED      REG_BIT(5)
#define EN_MCU_MOT_REG_POS_CNT_OVER      REG_BIT(6)
#define EN_MCU_MOT_REG_SERVO_ALARM       REG_BIT(7)

#define EN_MCU_MOT_REG_ERR_OVER_CURRENT  REG_BIT(0)
#define EN_MCU_MOT_REG_ERR_OVER_SPEED    REG_BIT(1)
#define EN_MCU_MOT_REG_ERR_STEP_OUT      REG_BIT(2)
#define EN_MCU_MOT_REG_ERR_OVER_LOAD     REG_BIT(3)
#define EN_MCU_MOT_REG_ERR_OVER_HEAT     REG_BIT(4)
#define EN_MCU_MOT_REG_ERR_BACK_EMF      REG_BIT(5)
#define EN_MCU_MOT_REG_ERR_POWER         REG_BIT(6)
#define EN_MCU_MOT_REG_ERR_INPOSITION    REG_BIT(7)

#define EN_MCU_MOT_REG_EMG_STOP          REG_BIT(0)
#define EN_MCU_MOT_REG_SLOW_STOP         REG_BIT(1)
#define EN_MCU_MOT_REG_ORG_RETURN        REG_BIT(2)
#define EN_MCU_MOT_REG_INPOSITION        REG_BIT(3)
#define EN_MCU_MOT_REG_SERVO_ON          REG_BIT(4)
#define EN_MCU_MOT_REG_ALARM_RESET       REG_BIT(5)
#define EN_MCU_MOT_REG_PT_STOPPED        REG_BIT(6)
#define EN_MCU_MOT_REG_ORIGIN_SENSOR     REG_BIT(7)

#define EN_MCU_MOT_REG_Z_PLUS            REG_BIT(0)
#define EN_MCU_MOT_REG_ORG_RET_OK        REG_BIT(1)
#define EN_MCU_MOT_REG_MOTION_DIR        REG_BIT(2)
#define EN_MCU_MOT_REG_MOTIONING         REG_BIT(3)
#define EN_MCU_MOT_REG_MOTION_PAUSE      REG_BIT(4)
#define EN_MCU_MOT_REG_MOTION_ACCEL      REG_BIT(5)
#define EN_MCU_MOT_REG_MOTION_DECEL      REG_BIT(6)
#define EN_MCU_MOT_REG_MOTION_CONST      REG_BIT(7)

#define EN_MCU_ACT_MOTOR_ON              0x01
#define EN_MCU_ACT_MOTOR_OFF             0x00


#define RBTRO_CMD_STX                    0x96
#define RBTRO_CMD_ETX                    0x69

#define RBTRO_CMD_STATE_WAIT_STX          0
#define RBTRO_CMD_STATE_WAIT_ID           1
#define RBTRO_CMD_STATE_WAIT_TYPE         2
#define RBTRO_CMD_STATE_WAIT_LENGTH       3
#define RBTRO_CMD_STATE_WAIT_DATA         4
#define RBTRO_CMD_STATE_WAIT_CHECKSUM     5
#define RBTRO_CMD_STATE_WAIT_ETX          6


#define EN_MCU_STEP_RETRY_CNT_MAX                   3

#define EN_MCU_STEP_INIT                            0
#define EN_MCU_STEP_TODO                            1
#define EN_MCU_STEP_TIMEOUT                         2
#define EN_MCU_STEP_WAIT_MOTOR_ORIGIN_CPLT          3

#define EN_MCU_STEP_SEND_MCU_ALL_DATA               4
#define EN_MCU_STEP_SEND_MCU_ALL_DATA_START         5
#define EN_MCU_STEP_SEND_MCU_ALL_DATA_WAIT          6
#define EN_MCU_STEP_SEND_MCU_ALL_DATA_END           7
#define EN_MCU_STEP_CTRL_IO_OUT                     8
#define EN_MCU_STEP_CTRL_IO_OUT_START               9
#define EN_MCU_STEP_CTRL_IO_OUT_WAIT                10
#define EN_MCU_STEP_CTRL_IO_OUT_END                 11
#define EN_MCU_STEP_CTRL_AXIS_ONOFF                 12
#define EN_MCU_STEP_CTRL_AXIS_ONOFF_START           13
#define EN_MCU_STEP_CTRL_AXIS_ONOFF_WAIT            14
#define EN_MCU_STEP_CTRL_AXIS_ONOFF_END             15
#define EN_MCU_STEP_CTRL_AXIS_MOVE                  16
#define EN_MCU_STEP_CTRL_AXIS_MOVE_START            17
#define EN_MCU_STEP_CTRL_AXIS_MOVE_WAIT             18
#define EN_MCU_STEP_CTRL_AXIS_MOVE_END              19
#define EN_MCU_STEP_CTRL_AXIS_STOP                  20
#define EN_MCU_STEP_CTRL_AXIS_STOP_START            21
#define EN_MCU_STEP_CTRL_AXIS_STOP_WAIT             22
#define EN_MCU_STEP_CTRL_AXIS_STOP_END              23
#define EN_MCU_STEP_CTRL_AP_CONFIG                  24
#define EN_MCU_STEP_CTRL_AP_CONFIG_START            25
#define EN_MCU_STEP_CTRL_AP_CONFIG_WAIT             26
#define EN_MCU_STEP_CTRL_AP_CONFIG_END              27
#define EN_MCU_STEP_READ_MCU_DATA                   28
#define EN_MCU_STEP_READ_MCU_DATA_START             29
#define EN_MCU_STEP_READ_MCU_DATA_WAIT              30
#define EN_MCU_STEP_READ_MCU_DATA_END               31
#define EN_MCU_STEP_WRITE_MOTOR_DATA                32
#define EN_MCU_STEP_WRITE_MOTOR_DATA_START          33
#define EN_MCU_STEP_WRITE_MOTOR_DATA_WAIT           34
#define EN_MCU_STEP_WRITE_MOTOR_DATA_END            35
#define EN_MCU_STEP_WRITE_AP_CFG                    36
#define EN_MCU_STEP_WRITE_AP_CFG_START              37
#define EN_MCU_STEP_WRITE_AP_CFG_WAIT               38
#define EN_MCU_STEP_WRITE_AP_CFG_END                39
#define EN_MCU_STEP_WRITE_CYL_DATA                  40
#define EN_MCU_STEP_WRITE_CYL_DATA_START            41
#define EN_MCU_STEP_WRITE_CYL_DATA_WAIT             42
#define EN_MCU_STEP_WRITE_CYL_DATA_END              43
#define EN_MCU_STEP_WRITE_VAC_DATA                  44
#define EN_MCU_STEP_WRITE_VAC_DATA_START            45
#define EN_MCU_STEP_WRITE_VAC_DATA_WAIT             46
#define EN_MCU_STEP_WRITE_VAC_DATA_END              47
#define EN_MCU_STEP_WRITE_SEQ_DATA                  48
#define EN_MCU_STEP_WRITE_SEQ_DATA_START            49
#define EN_MCU_STEP_WRITE_SEQ_DATA_WAIT             50
#define EN_MCU_STEP_WRITE_SEQ_DATA_END              51
#define EN_MCU_STEP_READ_MOTOR_PARM_DATA            52
#define EN_MCU_STEP_READ_MOTOR_PARM_DATA_START      53
#define EN_MCU_STEP_READ_MOTOR_PARM_DATA_WAIT       54
#define EN_MCU_STEP_READ_MOTOR_PARM_DATA_END        55
#define EN_MCU_STEP_REQ_SEND_STATE_ONOFF            56
#define EN_MCU_STEP_REQ_SEND_STATE_ONOFF_START      57
#define EN_MCU_STEP_REQ_SEND_STATE_ONOFF_WAIT       58
#define EN_MCU_STEP_REQ_SEND_STATE_ONOFF_END        59
#define EN_MCU_STEP_GET_MOTOR_PARM_DATA             60
#define EN_MCU_STEP_GET_MOTOR_PARM_DATA_START       61
#define EN_MCU_STEP_GET_MOTOR_PARM_DATA_WAIT        62
#define EN_MCU_STEP_GET_MOTOR_PARM_DATA_END         63
#define EN_MCU_STEP_CTRL_AXIS_RUN_ORIGIN            64
#define EN_MCU_STEP_CTRL_AXIS_RUN_ORIGIN_START      65
#define EN_MCU_STEP_CTRL_AXIS_RUN_ORIGIN_WAIT       66
#define EN_MCU_STEP_CTRL_AXIS_RUN_ORIGIN_END        67
#define EN_MCU_STEP_CTRL_AXIS_MOVE_JOG              68
#define EN_MCU_STEP_CTRL_AXIS_MOVE_JOG_START        69
#define EN_MCU_STEP_CTRL_AXIS_MOVE_JOG_WAIT         70
#define EN_MCU_STEP_CTRL_AXIS_MOVE_JOG_END          71
#define EN_MCU_STEP_CTRL_AXIS_MOVE_LIMIT            72
#define EN_MCU_STEP_CTRL_AXIS_MOVE_LIMIT_START      73
#define EN_MCU_STEP_CTRL_AXIS_MOVE_LIMIT_WAIT       74
#define EN_MCU_STEP_CTRL_AXIS_MOVE_LIMIT_END        75


#define EN_MCU_DEF_STEPBUFF_INDEX_MAX               0
#define EN_MCU_DEF_STEPBUFF_INDEX_INCREASE          1
#define EN_MCU_DEF_STEPBUFF_ONOFF_VALUE             2
#define EN_MCU_DEF_STEPBUFF_WAIT_RETURN_STEP        (EN_MCU_MAX_BUFFER_LENGTH-1)


#define EN_MCU_DEF_MOVE_TIMEOUT_MS                  1000*5

enMcuRemote::enMcuRemote(common::Attribute_t common_data, cfg_t* cfg)
  :Icommon(common_data), m_Cmd(), m_pSerialComm(nullptr)/*, m_pIo(nullptr)*/
  , m_IsReceiveCplt(true), m_prev_ms(0), m_scan_ms(0), m_IsResponsed(false), m_IsSendMcuStateLocked(false)
{
  m_retryCnt = 0;
  m_pre_time = 0;
  m_step = {};
  m_id = 0;
  m_pApDat = nullptr;
  m_pAxisDat = nullptr;
  m_pCylDat = nullptr;
  m_pVacDat = nullptr;
  m_pSeqDat = nullptr;


  memset(&m_stepBuffer, 0, EN_MCU_MAX_BUFFER_LENGTH);

  m_pSerialComm = cfg->p_serial;
  //m_pIo = cfg->p_Io;
  m_pApDat = cfg->p_apDat;
  m_pAxisDat = cfg->p_axisDat;
  m_pCylDat = cfg->p_cylDat;
  m_pVacDat = cfg->p_vacDat;
  m_pSeqDat = cfg->p_seqDat;


  m_Cmd.state = RBTRO_CMD_STATE_WAIT_STX;
  m_Cmd.rx_packet.data = &m_Cmd.rx_packet.buffer[RBTRO_CMD_STATE_WAIT_DATA];
  m_Cmd.tx_packet.data = &m_Cmd.tx_packet.buffer[RBTRO_CMD_STATE_WAIT_DATA];

  m_pSerialComm->AttCallbackFunc(this, receiveData);
}

enMcuRemote::~enMcuRemote()
{

}


void enMcuRemote::receiveData(void* obj, void* w_parm, void* l_param)
{
  enMcuRemote* pThis = (enMcuRemote*)obj;
  /* parsing packet data */
  uint8_t rx_data = *((uint8_t*)w_parm);

  robotro_t* p_cmd = &pThis->m_Cmd;

  if (util::millis() - p_cmd->pre_time >= 100)
  {
    p_cmd->state = RBTRO_CMD_STATE_WAIT_STX;

  }
  p_cmd->pre_time = util::millis();


  switch (p_cmd->state)
  {
  case RBTRO_CMD_STATE_WAIT_STX:
  {
    /*robotro packet parsing*/
    if (rx_data == RBTRO_CMD_STX)
    {
      p_cmd->data_len = 0;
      p_cmd->index = 0;
      memset(&p_cmd->rx_packet.buffer[0], 0, EN_MCU_MAX_PACKET_BUFF_LENGTH);

      p_cmd->rx_packet.buffer[p_cmd->data_len++] = rx_data;
      p_cmd->rx_packet.check_sum = 0;
      p_cmd->state = RBTRO_CMD_STATE_WAIT_ID;
    }
  }
  break;
  case RBTRO_CMD_STATE_WAIT_ID:
  {
    p_cmd->rx_packet.mot_id = rx_data;
    p_cmd->rx_packet.buffer[p_cmd->data_len++] = rx_data;
    p_cmd->rx_packet.check_sum += rx_data;
    p_cmd->state = RBTRO_CMD_STATE_WAIT_TYPE;
  }
  break;
  case RBTRO_CMD_STATE_WAIT_TYPE:
    p_cmd->rx_packet.cmd_type = rx_data;
    p_cmd->rx_packet.buffer[p_cmd->data_len++] = rx_data;
    p_cmd->rx_packet.check_sum += rx_data;
    p_cmd->rx_packet.length = 0;
    p_cmd->state = RBTRO_CMD_STATE_WAIT_LENGTH;
    break;

  case RBTRO_CMD_STATE_WAIT_LENGTH:
    p_cmd->rx_packet.length = rx_data % EN_MCU_MAX_BUFFER_LENGTH; // prevent overflow
    p_cmd->rx_packet.buffer[p_cmd->data_len++] = rx_data;
    p_cmd->rx_packet.check_sum += rx_data;
    if (p_cmd->rx_packet.length == 0)
      p_cmd->state = RBTRO_CMD_STATE_WAIT_CHECKSUM;
    else
      p_cmd->state = RBTRO_CMD_STATE_WAIT_DATA;
    break;

  case RBTRO_CMD_STATE_WAIT_DATA:
    if (p_cmd->index + 1 > p_cmd->rx_packet.length)
    {
      p_cmd->state = RBTRO_CMD_STATE_WAIT_STX;
      p_cmd->index = 0;

      break;
    }

    p_cmd->rx_packet.data[p_cmd->index++] = rx_data;
    p_cmd->rx_packet.buffer[p_cmd->data_len++] = rx_data;
    p_cmd->rx_packet.check_sum += rx_data;

    if (p_cmd->index == p_cmd->rx_packet.length)
    {
      p_cmd->state = RBTRO_CMD_STATE_WAIT_CHECKSUM;
    }
    break;

  case RBTRO_CMD_STATE_WAIT_CHECKSUM:
    p_cmd->rx_packet.buffer[p_cmd->data_len++] = rx_data;
    p_cmd->rx_packet.check_sum_recv = rx_data;
    if (p_cmd->rx_packet.check_sum == p_cmd->rx_packet.check_sum_recv)
    {
      //TODO:
      pThis->receiveCplt();
    }
    p_cmd->state = RBTRO_CMD_STATE_WAIT_STX;
    break;

  case RBTRO_CMD_STATE_WAIT_ETX:
  default:
    p_cmd->state = RBTRO_CMD_STATE_WAIT_STX;
    break;
  }// switch (p_cmd->state)

}

bool enMcuRemote::receiveCplt()
{
  okResponse();
  m_IsReceiveCplt = true;

  switch (m_Cmd.rx_packet.cmd_type)
  {
  case EN_MCU_CMD_OK_RESPONSE:
  {
    m_IsResponsed = true;
  }
  break;
  case EN_MCU_CMD_FASTECH_MOTOR_PARM_GET:
  {
    int value = conv::DwToInt(&m_Cmd.rx_packet.data[1]);
    m_fmParamer.Set((uint16_t)m_Cmd.rx_packet.data[0], value);
    m_IsResponsed = true;
  }
  break;
  case EN_MCU_CMD_READ_ALL_STATE:
  {
    for (uint8_t i = 0; i < m_Cmd.rx_packet.length; i++)
    {
      if (i < 16) {
        m_mcuRegis[i] = m_Cmd.rx_packet.data[i];
      }
      else
      {
        switch (i)
        {
        case 16:
        {
          m_mcuMotor.cmd_pos = conv::DwToInt(&m_Cmd.rx_packet.data[i]);
          i = i + 3;
        }
        break;

        case 20:
        {
          m_mcuMotor.act_pos = conv::DwToInt(&m_Cmd.rx_packet.data[i]);
          i = i + 3;
        }
        break;
        case 24:
        {
          m_mcuMotor.act_vel = conv::DwToUint(&m_Cmd.rx_packet.data[i]);
          i = i + 3;
        }
        break;
        case 27:
        {
          m_mcuMotor.pos_err = conv::DwToInt(&m_Cmd.rx_packet.data[i]);
          i = i + 3;
        }
        break;
        default:
          break;
        }
      }
    }
  }
  break;
  case EN_MCU_CMD_READ_BOOT_INFO:
  {

  }
  break;
  case EN_MCU_CMD_READ_FIRM_INFO:
  {

  }
  break;
  case EN_MCU_CMD_CONTROL_IO_OUT:
  {

  }
  break;
  case EN_MCU_CMD_CONTROL_MOT_ORIGIN:
  {

  }
  break;
  case EN_MCU_CMD_READ_MCD_DATA:
  {
    uint8_t data_type = m_Cmd.rx_packet.data[0];

    switch (data_type)
    {
    case EN_MCU_DATA_TYPE_MOTOR_POS:
    {
      axis_dat::dat_t send_data;
      uint8_t idx = 0;
      uint8_t const data_len = 8;
      axis_dat::addr_e addr = static_cast<axis_dat::addr_e>(idx);
      send_data.cmd_pos = conv::DwToInt(&m_Cmd.rx_packet.data[1 + (idx * data_len)]);
      send_data.cmd_vel = conv::DwToUint(&m_Cmd.rx_packet.data[5 + (idx * data_len)]);
      m_pAxisDat->WriteData(addr, send_data);

      idx++;
      addr = static_cast<axis_dat::addr_e>(idx);
      send_data.cmd_pos = conv::DwToInt(&m_Cmd.rx_packet.data[1 + (idx * data_len)]);
      send_data.cmd_vel = conv::DwToUint(&m_Cmd.rx_packet.data[5 + (idx * data_len)]);
      m_pAxisDat->WriteData(addr, send_data);

      idx++;
      addr = static_cast<axis_dat::addr_e>(idx);
      send_data.cmd_pos = conv::DwToInt(&m_Cmd.rx_packet.data[1 + (idx * data_len)]);
      send_data.cmd_vel = conv::DwToUint(&m_Cmd.rx_packet.data[5 + (idx * data_len)]);
      m_pAxisDat->WriteData(addr, send_data);

      idx++;
      addr = static_cast<axis_dat::addr_e>(idx);
      send_data.cmd_pos = conv::DwToInt(&m_Cmd.rx_packet.data[1 + (idx * data_len)]);
      send_data.cmd_vel = conv::DwToUint(&m_Cmd.rx_packet.data[5 + (idx * data_len)]);
      m_pAxisDat->WriteData(addr, send_data);
    }
    break;
    case EN_MCU_DATA_TYPE_AP_DAT:
    {
      ap_dat::dat_t data;
      uint8_t const data_len = 4;
      for (uint8_t i = 0; i < static_cast<uint8_t>(ap_dat::addr_e::_max); i++)
      {
        data.parm1 = m_Cmd.rx_packet.data[(i * data_len) + 1] | (m_Cmd.rx_packet.data[(i * data_len) + 2] << 8);
        data.parm2 = m_Cmd.rx_packet.data[(i * data_len) + 3] | (m_Cmd.rx_packet.data[(i * data_len) + 4] << 8);
        ap_dat::addr_e addr = static_cast<ap_dat::addr_e>(i);
        m_pApDat->WriteData(addr, data);
      }
    }
    break;
    case EN_MCU_DATA_TYPE_CYL_DAT:
    {
      cyl_dat::dat_t data;
      uint8_t const data_len = 4;
      for (uint8_t i = 0; i < static_cast<uint8_t>(cyl_dat::addr_e::_max); i++)
      {
        data.timeout = m_Cmd.rx_packet.data[(i * data_len) + 1] | (m_Cmd.rx_packet.data[(i * data_len) + 2] << 8);
        data.on_settling = m_Cmd.rx_packet.data[(i * data_len) + 3] | (m_Cmd.rx_packet.data[(i * data_len) + 4] << 8);
        cyl_dat::addr_e addr = static_cast<cyl_dat::addr_e>(i);
        m_pCylDat->WriteData(addr, data);
      }
    }
    break;

    case EN_MCU_DATA_TYPE_VAC_DAT:
    {
      vac_dat::dat_t data;
      uint8_t const data_len = 4;
      for (uint8_t i = 0; i < static_cast<uint8_t>(vac_dat::addr_e::_max); i++)
      {
        data.timeout = m_Cmd.rx_packet.data[(i * data_len) + 1] | (m_Cmd.rx_packet.data[(i * data_len) + 2] << 8);
        data.on_settling = m_Cmd.rx_packet.data[(i * data_len) + 3] | (m_Cmd.rx_packet.data[(i * data_len) + 4] << 8);
        vac_dat::addr_e addr = static_cast<vac_dat::addr_e>(i);
        m_pVacDat->WriteData(addr, data);
      }
    }
    break;
    case EN_MCU_DATA_TYPE_SEQ_DAT:
    {
      seq_dat::dat_t data;
      uint8_t const data_len = 4;
      for (uint8_t i = 0; i < static_cast<uint8_t>(seq_dat::addr_e::_max); i++)
      {
        data.parm1 = m_Cmd.rx_packet.data[(i * data_len) + 1] | (m_Cmd.rx_packet.data[(i * data_len) + 2] << 8);
        data.parm2 = m_Cmd.rx_packet.data[(i * data_len) + 3] | (m_Cmd.rx_packet.data[(i * data_len) + 4] << 8);
        seq_dat::addr_e addr = static_cast<seq_dat::addr_e>(i);
        m_pSeqDat->WriteData(addr, data);
      }
    }
    break;
    default:
      m_step.SetStep(EN_MCU_STEP_READ_MCU_DATA);
      break;
    }
  }
  break;
  default:
    break;
  }
  m_scan_ms = util::millis() - m_prev_ms;
  m_prev_ms = util::millis();
  return m_IsReceiveCplt;
}



bool enMcuRemote::IsConnected()
{
  return m_pSerialComm->IsConnected();
}

void enMcuRemote::SendCmd(robotro_t* p_cmd, uint8_t cmd, uint8_t* p_data, uint32_t length)
{
  uint32_t index;


  index = 0;
  uint8_t check_sum = 0;
  p_cmd->tx_packet.buffer[index++] = RBTRO_CMD_STX;
  p_cmd->tx_packet.buffer[index++] = m_id;//p_cmd->rx_packet.mot_id;
  p_cmd->tx_packet.buffer[index++] = cmd;
  p_cmd->tx_packet.buffer[index++] = length;
  check_sum = m_id + cmd + length;
  for (uint32_t i = 0; i < length; i++)
  {
    p_cmd->tx_packet.buffer[index++] = p_data[i];
    check_sum += p_data[i];
  }

  p_cmd->tx_packet.buffer[index++] = check_sum;
  p_cmd->tx_packet.buffer[index++] = RBTRO_CMD_ETX;

  m_pSerialComm->WriteByte((byte*)p_cmd->tx_packet.buffer, index);
  m_IsReceiveCplt = false;
  //uartWrite(p_cmd->ch, p_cmd->tx_packet.buffer, index);
}



bool enMcuRemote::SendCmdRxResp(robotro_t* p_cmd, uint8_t cmd, uint8_t* p_data, uint32_t length, uint32_t timeout)
{
  bool  ret = false;
  uint32_t time_pre;

  SendCmd(p_cmd, cmd, p_data, length);

  time_pre = util::millis();

  while (1)
  {
    if (m_IsReceiveCplt)
    {
      ret = true;
      break;
    }

    if (util::millis() - time_pre >= timeout)
    {
      break;
    }
  }

  return ret;
}

uint32_t enMcuRemote::GetScanTime()
{
  return m_scan_ms;
}

errno_t enMcuRemote::RequestMcuState()
{

  doRunStep();

  if (m_IsReceiveCplt)
  {
    //SendCmdRxResp(&m_Cmd, EN_MCU_CMD_READ_ALL_STATE, 0, 0, 100);
  }
  else
  {
    uint32_t pre_ms = util::millis();
    while (util::millis() - pre_ms < 1000 * 2)
    {
      if (m_IsReceiveCplt)
        break;
    }
    m_IsReceiveCplt = true;
  }

  return errno_t();
}

void enMcuRemote::doRunStep()
{

  if (util::millis() - m_pre_time >= 1000 * 3)
  {
    m_step.SetStep(EN_MCU_STEP_INIT);
    m_pre_time = util::millis();
  }

  switch (m_step.GetStep())
  {
  case EN_MCU_STEP_INIT:
  {
    memset(&m_stepBuffer, 0, EN_MCU_MAX_BUFFER_LENGTH);
    m_IsSendMcuStateLocked = false;
    m_step.SetStep(EN_MCU_STEP_TODO);
    m_pre_time = util::millis();
  }
  break;
  /*######################################################
     to do
    ######################################################*/
  case EN_MCU_STEP_TODO:
  {
    m_IsReceiveCplt = true;
    m_retryCnt = 0;
  }
  break;
  /*######################################################
     timeout/ page check
    ######################################################*/
  case EN_MCU_STEP_TIMEOUT:
  {
    if (util::millis() - m_pre_time < 5)
    {
      break;
    }
    m_retryCnt = 0;
    m_step.SetStep(EN_MCU_STEP_TODO);
  }
  break;
  /*######################################################
     wait motor origin cplt
    ######################################################*/
  case EN_MCU_STEP_WAIT_MOTOR_ORIGIN_CPLT:
  {
    if (util::millis() - m_pre_time >= 1000 * 10)
    {
      m_step.SetStep(EN_MCU_STEP_TIMEOUT);
    }
    else if (IsAxisDone())
    {
      m_step.SetStep(EN_MCU_STEP_TODO);
    }
  }
  break;
  /*######################################################
     EN_MCU_STEP_SEND_MCU_ALL_DATA
    ######################################################*/
  case EN_MCU_STEP_SEND_MCU_ALL_DATA:
  {
    m_retryCnt = 0;
    m_step.SetStep(EN_MCU_STEP_SEND_MCU_ALL_DATA_START);
    m_pre_time = util::millis();
  }
  break;
  case EN_MCU_STEP_SEND_MCU_ALL_DATA_START:
  {
    /* send all state*/
    m_IsReceiveCplt = false;
    m_step.SetStep(EN_MCU_STEP_SEND_MCU_ALL_DATA_WAIT);
    m_pre_time = util::millis();

  }
  break;
  case EN_MCU_STEP_SEND_MCU_ALL_DATA_WAIT:
  {
    if (util::millis() - m_pre_time >= 50)
    {
      if (m_retryCnt++ < EN_MCU_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(EN_MCU_STEP_SEND_MCU_ALL_DATA_START);
        m_pre_time = util::millis();
      }
      else
      {
        m_retryCnt = 0;
        m_pre_time = util::millis();
        //
        m_step.SetStep(EN_MCU_STEP_SEND_MCU_ALL_DATA_END);
      }
    }

    if (m_IsReceiveCplt)
    {
      m_step.SetStep(EN_MCU_STEP_SEND_MCU_ALL_DATA_END);
      m_pre_time = util::millis();
    }
  }
  break;
  case EN_MCU_STEP_SEND_MCU_ALL_DATA_END:
  {
    if (m_retryCnt > EN_MCU_STEP_RETRY_CNT_MAX)
    {
      m_retryCnt = 0;
      m_step.SetStep(EN_MCU_STEP_TIMEOUT);
    }
    else
    {
      m_step.SetStep(EN_MCU_STEP_TODO);
    }
    m_pre_time = util::millis();
  }
  break;
  /*######################################################
     EN_MCU_STEP_CTRL_IO_OUT
    ######################################################*/
  case EN_MCU_STEP_CTRL_IO_OUT:
  {
    m_retryCnt = 0;
    m_step.SetStep(EN_MCU_STEP_CTRL_IO_OUT_START);
    m_pre_time = util::millis();
  }
  break;
  case EN_MCU_STEP_CTRL_IO_OUT_START:
  {
    /* io control*/

    SendCmd(&m_Cmd, EN_MCU_CMD_CONTROL_IO_OUT, &m_stepBuffer[0], 4);

    m_IsReceiveCplt = false;
    m_step.SetStep(EN_MCU_STEP_CTRL_IO_OUT_WAIT);
    m_pre_time = util::millis();

  }
  break;
  case EN_MCU_STEP_CTRL_IO_OUT_WAIT:
  {
    if (util::millis() - m_pre_time >= 200)
    {
      if (m_retryCnt++ < EN_MCU_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(EN_MCU_STEP_CTRL_IO_OUT_START);
        m_pre_time = util::millis();
      }
      else
      {
        m_retryCnt = 0;
        m_pre_time = util::millis();
        //
        m_step.SetStep(EN_MCU_STEP_CTRL_IO_OUT_END);
      }
    }

    bool is_equal = m_stepBuffer[0] == m_mcuRegis[EN_MCU_IO_OUT_BANK_0].get();
    is_equal &= m_stepBuffer[1] == m_mcuRegis[EN_MCU_IO_OUT_BANK_1].get();
    is_equal &= m_stepBuffer[2] == m_mcuRegis[EN_MCU_IO_OUT_BANK_2].get();
    is_equal &= m_stepBuffer[3] == m_mcuRegis[EN_MCU_IO_OUT_BANK_3].get();
    if (is_equal)
    {
      m_step.SetStep(EN_MCU_STEP_CTRL_IO_OUT_END);
      m_pre_time = util::millis();
    }

  }
  break;
  case EN_MCU_STEP_CTRL_IO_OUT_END:
  {
    if (m_retryCnt > EN_MCU_STEP_RETRY_CNT_MAX)
    {
      m_retryCnt = 0;
      m_step.SetStep(EN_MCU_STEP_TIMEOUT);
    }
    else
    {
      m_step.SetStep(EN_MCU_STEP_TODO);
    }
    memset(&m_stepBuffer, 0, EN_MCU_MAX_BUFFER_LENGTH);
    m_pre_time = util::millis();
  }
  break;
  /*######################################################
     EN_MCU_STEP_CTRL_AXIS_ONOFF
    ######################################################*/
  case EN_MCU_STEP_CTRL_AXIS_ONOFF:
  {
    m_retryCnt = 0;
    m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_ONOFF_START);
    m_pre_time = util::millis();
  }
  break;
  case EN_MCU_STEP_CTRL_AXIS_ONOFF_START:
  {
    /* control*/
    SendCmd(&m_Cmd, EN_MCU_CMD_CONTROL_MOT_ONOFF, &m_stepBuffer[0], 1);

    m_IsReceiveCplt = false;
    m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_ONOFF_WAIT);
    m_pre_time = util::millis();

  }
  break;
  case EN_MCU_STEP_CTRL_AXIS_ONOFF_WAIT:
  {
    if (util::millis() - m_pre_time >= 200)
    {
      if (m_retryCnt++ < EN_MCU_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_ONOFF_START);
        m_pre_time = util::millis();
      }
      else
      {
        m_retryCnt = 0;
        m_pre_time = util::millis();
        //
        m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_ONOFF_END);
      }
    }

    bool is_equal = (bool)m_stepBuffer[0] == IsMotorOn();
    if (is_equal)
    {
      m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_ONOFF_END);
      m_pre_time = util::millis();
    }

  }
  break;
  case EN_MCU_STEP_CTRL_AXIS_ONOFF_END:
  {
    if (m_retryCnt > EN_MCU_STEP_RETRY_CNT_MAX)
    {
      m_retryCnt = 0;
      m_step.SetStep(EN_MCU_STEP_TIMEOUT);
    }
    else
    {
      m_step.SetStep(EN_MCU_STEP_TODO);
    }
    memset(&m_stepBuffer, 0, EN_MCU_MAX_BUFFER_LENGTH);
    m_pre_time = util::millis();
  }
  break;
  /*######################################################
     EN_MCU_STEP_CTRL_AXIS_STOP
    ######################################################*/
  case EN_MCU_STEP_CTRL_AXIS_STOP:
  {
    m_retryCnt = 0;
    m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_STOP_START);
    m_pre_time = util::millis();
  }
  break;
  case EN_MCU_STEP_CTRL_AXIS_STOP_START:
  {
    /* control*/
    SendCmd(&m_Cmd, EN_MCU_CMD_CONTROL_MOT_STOP, 0, 0);

    m_IsReceiveCplt = false;
    m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_STOP_WAIT);
    m_pre_time = util::millis();

  }
  break;
  case EN_MCU_STEP_CTRL_AXIS_STOP_WAIT:
  {
    if (util::millis() - m_pre_time >= 200)
    {
      if (m_retryCnt++ < EN_MCU_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_STOP_START);
        m_pre_time = util::millis();
      }
      else
      {
        m_retryCnt = 0;
        m_pre_time = util::millis();
        //
        m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_STOP_END);
      }
    }

    if (IsMoving())
      break;

    m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_STOP_END);
    m_pre_time = util::millis();


  }
  break;
  case EN_MCU_STEP_CTRL_AXIS_STOP_END:
  {
    if (m_retryCnt > EN_MCU_STEP_RETRY_CNT_MAX)
    {
      m_retryCnt = 0;
      m_step.SetStep(EN_MCU_STEP_TIMEOUT);
    }
    else
    {
      m_step.SetStep(EN_MCU_STEP_TODO);
    }
    memset(&m_stepBuffer, 0, EN_MCU_MAX_BUFFER_LENGTH);
    m_pre_time = util::millis();
  }
  break;


  /*######################################################
     EN_MCU_STEP_CTRL_AXIS_MOVE
    ######################################################*/
  case EN_MCU_STEP_CTRL_AXIS_MOVE:
  {
    m_retryCnt = 0;
    m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_MOVE_START);
    m_pre_time = util::millis();
  }
  break;
  case EN_MCU_STEP_CTRL_AXIS_MOVE_START:
  {
    /* control*/
    SendCmd(&m_Cmd, EN_MCU_CMD_CONTROL_MOT_RUN, &m_stepBuffer[0], 16);

    m_IsReceiveCplt = false;
    m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_MOVE_WAIT);
    m_pre_time = util::millis();

  }
  break;
  case EN_MCU_STEP_CTRL_AXIS_MOVE_WAIT:
  {
    if (util::millis() - m_pre_time >= 200)
    {
      if (m_retryCnt++ < EN_MCU_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_MOVE_START);
        m_pre_time = util::millis();
      }
      else
      {
        m_retryCnt = 0;
        m_pre_time = util::millis();
        //
        m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_MOVE_END);
      }
    }

    if (false == IsMoving())
      break;

    m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_MOVE_END);
    m_pre_time = util::millis();


  }
  break;
  case EN_MCU_STEP_CTRL_AXIS_MOVE_END:
  {
    if (m_retryCnt > EN_MCU_STEP_RETRY_CNT_MAX)
    {
      m_retryCnt = 0;
      m_step.SetStep(EN_MCU_STEP_TIMEOUT);
    }
    else
    {
      m_step.SetStep(EN_MCU_STEP_TODO);
    }
    memset(&m_stepBuffer, 0, EN_MCU_MAX_BUFFER_LENGTH);
    m_pre_time = util::millis();
  }
  break;

  /*######################################################
     EN_MCU_STEP_CTRL_AP_CONFIG
    ######################################################*/
  case EN_MCU_STEP_CTRL_AP_CONFIG:
  {
    m_retryCnt = 0;
    m_step.SetStep(EN_MCU_STEP_CTRL_AP_CONFIG_START);
    m_pre_time = util::millis();
  }
  break;
  case EN_MCU_STEP_CTRL_AP_CONFIG_START:
  {
    /* control*/
    SendCmd(&m_Cmd, EN_MCU_CMD_AP_CONFIG_WRITE, &m_stepBuffer[0], 1);

    m_IsReceiveCplt = false;
    m_step.SetStep(EN_MCU_STEP_CTRL_AP_CONFIG_WAIT);
    m_pre_time = util::millis();

  }
  break;
  case EN_MCU_STEP_CTRL_AP_CONFIG_WAIT:
  {
    if (util::millis() - m_pre_time >= 200)
    {
      if (m_retryCnt++ < EN_MCU_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(EN_MCU_STEP_CTRL_AP_CONFIG_START);
        m_pre_time = util::millis();
      }
      else
      {
        m_retryCnt = 0;
        m_pre_time = util::millis();
        //
        m_step.SetStep(EN_MCU_STEP_CTRL_AP_CONFIG_END);
      }
    }

    bool is_equal = m_stepBuffer[0] == m_mcuRegis[EN_MCU_REG_BANK_SETTING].get();

    if (false == is_equal)
      break;

    m_step.SetStep(EN_MCU_STEP_CTRL_AP_CONFIG_END);
    m_pre_time = util::millis();


  }
  break;
  case EN_MCU_STEP_CTRL_AP_CONFIG_END:
  {
    if (m_retryCnt > EN_MCU_STEP_RETRY_CNT_MAX)
    {
      m_retryCnt = 0;
      m_step.SetStep(EN_MCU_STEP_TIMEOUT);
    }
    else
    {
      m_step.SetStep(EN_MCU_STEP_TODO);
    }
    memset(&m_stepBuffer, 0, EN_MCU_MAX_BUFFER_LENGTH);
    m_pre_time = util::millis();
  }
  break;

  /*######################################################
     EN_MCU_STEP_READ_MCU_DATA
    ######################################################*/
  case EN_MCU_STEP_READ_MCU_DATA:
  {
    m_retryCnt = 0;
    m_step.SetStep(EN_MCU_STEP_READ_MCU_DATA_START);
    m_pre_time = util::millis();
  }
  break;
  case EN_MCU_STEP_READ_MCU_DATA_START:
  {
    /* control*/
    SendCmd(&m_Cmd, EN_MCU_CMD_READ_MCD_DATA, 0, 0);

    m_IsReceiveCplt = false;
    m_step.SetStep(EN_MCU_STEP_READ_MCU_DATA_WAIT);
    m_pre_time = util::millis();

  }
  break;
  case EN_MCU_STEP_READ_MCU_DATA_WAIT:
  {
    if (util::millis() - m_pre_time >= 200)
    {
      if (m_retryCnt++ < EN_MCU_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(EN_MCU_STEP_READ_MCU_DATA_START);
        m_pre_time = util::millis();
      }
      else
      {
        m_retryCnt = 0;
        m_pre_time = util::millis();
        //
        m_step.SetStep(EN_MCU_STEP_READ_MCU_DATA_END);
      }
    }

    if (m_IsReceiveCplt)
    {
      m_step.SetStep(EN_MCU_STEP_READ_MCU_DATA_END);
      m_pre_time = util::millis();
    }

  }
  break;
  case EN_MCU_STEP_READ_MCU_DATA_END:
  {
    if (m_retryCnt > EN_MCU_STEP_RETRY_CNT_MAX)
    {
      m_retryCnt = 0;
      m_step.SetStep(EN_MCU_STEP_TIMEOUT);
    }
    else
    {
      m_step.SetStep(EN_MCU_STEP_TODO);
    }
    memset(&m_stepBuffer, 0, EN_MCU_MAX_BUFFER_LENGTH);
    m_pre_time = util::millis();
  }
  break;

  /*######################################################
     EN_MCU_STEP_WRITE_MOTOR_DATA
    ######################################################*/
  case EN_MCU_STEP_WRITE_MOTOR_DATA:
  {
    m_retryCnt = 0;

    m_stepBuffer[0] = 0;
    m_stepBuffer[1] = static_cast<uint8_t>(axis_dat::addr_e::_max);

    m_step.SetStep(EN_MCU_STEP_WRITE_MOTOR_DATA_START);
    m_pre_time = util::millis();
  }
  break;
  case EN_MCU_STEP_WRITE_MOTOR_DATA_START:
  {
    /*  */
    axis_dat::dat_t data = m_pAxisDat->ReadData(static_cast<axis_dat::addr_e>(m_stepBuffer[0]));
    m_stepBuffer[2] = m_stepBuffer[0];
    memcpy(&m_stepBuffer[6], (uint8_t*)&data, sizeof(axis_dat::dat_t));
    axis_dat::addr_e addr = static_cast<axis_dat::addr_e>(conv::DwToUint(&m_stepBuffer[2]));

    SendCmd(&m_Cmd, EN_MCU_CMD_WRITE_MOTOR_POS_DATA, &m_stepBuffer[2], 12);
    m_stepBuffer[0]++;
    m_step.SetStep(EN_MCU_STEP_WRITE_MOTOR_DATA_WAIT);
    m_pre_time = util::millis();
  }
  break;
  case EN_MCU_STEP_WRITE_MOTOR_DATA_WAIT:
  {
    if (util::millis() - m_pre_time < 100)
      break;

    if (m_stepBuffer[0] < m_stepBuffer[1])
    {
      m_step.SetStep(EN_MCU_STEP_WRITE_MOTOR_DATA_START);
    }
    else
    {
      m_step.SetStep(EN_MCU_STEP_WRITE_MOTOR_DATA_END);
    }
    m_pre_time = util::millis();


  }
  break;
  case EN_MCU_STEP_WRITE_MOTOR_DATA_END:
  {
    m_step.SetStep(EN_MCU_STEP_TODO);
    memset(&m_stepBuffer, 0, EN_MCU_MAX_BUFFER_LENGTH);
    m_pre_time = util::millis();
  }
  break;
  /*######################################################
     EN_MCU_STEP_WRITE_AP_CFG
    ######################################################*/
  case EN_MCU_STEP_WRITE_AP_CFG:
  {
    m_retryCnt = 0;
    m_IsResponsed = false;
    m_stepBuffer[0] = static_cast<uint8_t>(ap_dat::addr_e::_max) * sizeof(ap_dat::dat_t);

    m_step.SetStep(EN_MCU_STEP_WRITE_AP_CFG_START);
    m_pre_time = util::millis();
  }
  break;
  case EN_MCU_STEP_WRITE_AP_CFG_START:
  {
    /*  */
    ap_dat::dat_t data;
    uint8_t data_size = sizeof(ap_dat::dat_t);
    for (uint8_t i = 0; i < static_cast<uint8_t>(ap_dat::addr_e::_max); i++)
    {
      data = m_pApDat->ReadData(static_cast<ap_dat::addr_e>(i));
      memcpy(&m_stepBuffer[1 + (i * data_size)], (uint8_t*)&data, data_size);
    }
    SendCmd(&m_Cmd, EN_MCU_CMD_WRITE_AP_DATA, &m_stepBuffer[0], uint32_t(m_stepBuffer[0] + 1));

    m_step.SetStep(EN_MCU_STEP_WRITE_AP_CFG_WAIT);
    m_pre_time = util::millis();
  }
  break;
  case EN_MCU_STEP_WRITE_AP_CFG_WAIT:
  {
    if (util::millis() - m_pre_time >= 50)
    {
      if (m_retryCnt++ < EN_MCU_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(EN_MCU_STEP_WRITE_AP_CFG_START);
        m_pre_time = util::millis();
      }
      else
      {
        m_retryCnt = 0;
        m_pre_time = util::millis();
        //
        m_step.SetStep(EN_MCU_STEP_WRITE_AP_CFG_END);
      }
    }

    if (m_IsResponsed)
    {
      m_step.SetStep(EN_MCU_STEP_WRITE_AP_CFG_END);
      m_pre_time = util::millis();
    }
  }
  break;
  case EN_MCU_STEP_WRITE_AP_CFG_END:
  {
    m_step.SetStep(EN_MCU_STEP_TODO);
    memset(&m_stepBuffer, 0, EN_MCU_MAX_BUFFER_LENGTH);
    m_pre_time = util::millis();
  }
  break;

  /*######################################################
     EN_MCU_STEP_WRITE_CYL_DATA
    ######################################################*/
  case EN_MCU_STEP_WRITE_CYL_DATA:
  {
    m_retryCnt = 0;
    m_IsResponsed = false;
    m_stepBuffer[0] = static_cast<uint8_t>(cyl_dat::addr_e::_max) * sizeof(cyl_dat::dat_t);

    m_step.SetStep(EN_MCU_STEP_WRITE_CYL_DATA_START);
    m_pre_time = util::millis();
  }
  break;
  case EN_MCU_STEP_WRITE_CYL_DATA_START:
  {
    /*  */
    cyl_dat::dat_t data;
    uint8_t data_size = sizeof(cyl_dat::dat_t);
    for (uint8_t i = 0; i < static_cast<uint8_t>(cyl_dat::addr_e::_max); i++)
    {
      data = m_pCylDat->ReadData(static_cast<cyl_dat::addr_e>(i));
      memcpy(&m_stepBuffer[1 + (i * data_size)], (uint8_t*)&data, data_size);
    }
    SendCmd(&m_Cmd, EN_MCU_CMD_WRITE_CYL_DATA, &m_stepBuffer[0], uint32_t(m_stepBuffer[0] + 1));

    m_step.SetStep(EN_MCU_STEP_WRITE_CYL_DATA_WAIT);
    m_pre_time = util::millis();
  }
  break;
  case EN_MCU_STEP_WRITE_CYL_DATA_WAIT:
  {
    if (util::millis() - m_pre_time >= 50)
    {
      if (m_retryCnt++ < EN_MCU_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(EN_MCU_STEP_WRITE_CYL_DATA_START);
        m_pre_time = util::millis();
      }
      else
      {
        m_retryCnt = 0;
        m_pre_time = util::millis();
        //
        m_step.SetStep(EN_MCU_STEP_WRITE_CYL_DATA_END);
      }
    }

    if (m_IsResponsed)
    {
      m_step.SetStep(EN_MCU_STEP_WRITE_CYL_DATA_END);
      m_pre_time = util::millis();
    }
  }
  break;
  case EN_MCU_STEP_WRITE_CYL_DATA_END:
  {
    m_step.SetStep(EN_MCU_STEP_TODO);
    memset(&m_stepBuffer, 0, EN_MCU_MAX_BUFFER_LENGTH);
    m_pre_time = util::millis();
  }
  break;

  /*######################################################
    EN_MCU_STEP_WRITE_VAC_DATA
    ######################################################*/
  case EN_MCU_STEP_WRITE_VAC_DATA:
  {
    m_retryCnt = 0;
    m_IsResponsed = false;
    m_stepBuffer[0] = static_cast<uint8_t>(vac_dat::addr_e::_max) * sizeof(vac_dat::dat_t);

    m_step.SetStep(EN_MCU_STEP_WRITE_VAC_DATA_START);
    m_pre_time = util::millis();
  }
  break;
  case EN_MCU_STEP_WRITE_VAC_DATA_START:
  {
    /*  */
    vac_dat::dat_t data;
    uint8_t data_size = sizeof(vac_dat::dat_t);
    for (uint8_t i = 0; i < static_cast<uint8_t>(vac_dat::addr_e::_max); i++)
    {
      data = m_pVacDat->ReadData(static_cast<vac_dat::addr_e>(i));
      memcpy(&m_stepBuffer[1 + (i * data_size)], (uint8_t*)&data, data_size);
    }
    SendCmd(&m_Cmd, EN_MCU_CMD_WRITE_VAC_DATA, &m_stepBuffer[0], uint32_t(m_stepBuffer[0] + 1));

    m_step.SetStep(EN_MCU_STEP_WRITE_VAC_DATA_WAIT);
    m_pre_time = util::millis();
  }
  break;
  case EN_MCU_STEP_WRITE_VAC_DATA_WAIT:
  {
    if (util::millis() - m_pre_time >= 50)
    {
      if (m_retryCnt++ < EN_MCU_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(EN_MCU_STEP_WRITE_VAC_DATA_START);
        m_pre_time = util::millis();
      }
      else
      {
        m_retryCnt = 0;
        m_pre_time = util::millis();
        //
        m_step.SetStep(EN_MCU_STEP_WRITE_VAC_DATA_END);
      }
    }

    if (m_IsResponsed)
    {
      m_step.SetStep(EN_MCU_STEP_WRITE_VAC_DATA_END);
      m_pre_time = util::millis();
    }
  }
  break;
  case EN_MCU_STEP_WRITE_VAC_DATA_END:
  {
    m_step.SetStep(EN_MCU_STEP_TODO);
    memset(&m_stepBuffer, 0, EN_MCU_MAX_BUFFER_LENGTH);
    m_pre_time = util::millis();
  }
  break;

  /*######################################################
     EN_MCU_STEP_WRITE_SEQ_DATA
    ######################################################*/
  case EN_MCU_STEP_WRITE_SEQ_DATA:
  {
    m_retryCnt = 0;
    m_IsResponsed = false;
    m_stepBuffer[0] = static_cast<uint8_t>(seq_dat::addr_e::_max) * sizeof(seq_dat::dat_t);

    m_step.SetStep(EN_MCU_STEP_WRITE_SEQ_DATA_START);
    m_pre_time = util::millis();
  }
  break;
  case EN_MCU_STEP_WRITE_SEQ_DATA_START:
  {
    /*  */
    seq_dat::dat_t data;
    uint8_t data_size = sizeof(seq_dat::dat_t);
    for (uint8_t i = 0; i < static_cast<uint8_t>(seq_dat::addr_e::_max); i++)
    {
      data = m_pSeqDat->ReadData(static_cast<seq_dat::addr_e>(i));
      memcpy(&m_stepBuffer[1 + (i * data_size)], (uint8_t*)&data, data_size);
    }
    SendCmd(&m_Cmd, EN_MCU_CMD_WRITE_SEQ_DATA, &m_stepBuffer[0], uint32_t(m_stepBuffer[0] + 1));

    m_step.SetStep(EN_MCU_STEP_WRITE_SEQ_DATA_WAIT);
    m_pre_time = util::millis();
  }
  break;
  case EN_MCU_STEP_WRITE_SEQ_DATA_WAIT:
  {
    if (util::millis() - m_pre_time >= 50)
    {
      if (m_retryCnt++ < EN_MCU_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(EN_MCU_STEP_WRITE_SEQ_DATA_START);
        m_pre_time = util::millis();
      }
      else
      {
        m_retryCnt = 0;
        m_pre_time = util::millis();
        //
        m_step.SetStep(EN_MCU_STEP_WRITE_SEQ_DATA_END);
      }
    }

    if (m_IsResponsed)
    {
      m_step.SetStep(EN_MCU_STEP_WRITE_SEQ_DATA_END);
      m_pre_time = util::millis();
    }
  }
  break;
  case EN_MCU_STEP_WRITE_SEQ_DATA_END:
  {
    m_step.SetStep(EN_MCU_STEP_TODO);
    memset(&m_stepBuffer, 0, EN_MCU_MAX_BUFFER_LENGTH);
    m_pre_time = util::millis();
  }
  break;

  /*######################################################
     EN_MCU_STEP_READ_MOTOR_PARM_DATA
    ######################################################*/
  case EN_MCU_STEP_READ_MOTOR_PARM_DATA:
  {
    m_retryCnt = 0;
    m_IsResponsed = false;
    m_stepBuffer[EN_MCU_DEF_STEPBUFF_INDEX_MAX] = static_cast<uint8_t>(fm_parameter::param::_max);
    m_stepBuffer[EN_MCU_DEF_STEPBUFF_INDEX_INCREASE] = 0;

    m_stepBuffer[EN_MCU_DEF_STEPBUFF_ONOFF_VALUE] = (uint8_t)false;
    m_stepBuffer[EN_MCU_DEF_STEPBUFF_WAIT_RETURN_STEP] = EN_MCU_STEP_READ_MOTOR_PARM_DATA_START;
    m_step.SetStep(EN_MCU_STEP_REQ_SEND_STATE_ONOFF_START);
    m_pre_time = util::millis();
  }
  break;
  case EN_MCU_STEP_READ_MOTOR_PARM_DATA_START:
  {
    /*  */
    SendCmd(&m_Cmd, EN_MCU_CMD_FASTECH_MOTOR_PARM_GET, &m_stepBuffer[EN_MCU_DEF_STEPBUFF_INDEX_INCREASE], 1);
    m_step.SetStep(EN_MCU_STEP_READ_MOTOR_PARM_DATA_WAIT);
    m_pre_time = util::millis();
  }
  break;
  case EN_MCU_STEP_READ_MOTOR_PARM_DATA_WAIT:
  {
    if (util::millis() - m_pre_time >= 100)
    {
      if (m_retryCnt++ < EN_MCU_STEP_RETRY_CNT_MAX + 3)
      {
        m_step.SetStep(EN_MCU_STEP_READ_MOTOR_PARM_DATA_START);
        m_pre_time = util::millis();
      }
      else
      {
        m_retryCnt = 0;
        m_pre_time = util::millis();
        //
        m_step.SetStep(EN_MCU_STEP_READ_MOTOR_PARM_DATA_END);
      }
    }

    if (m_IsResponsed)
    {
      m_IsResponsed = false;
      //int value = conv::DwToInt(&m_Cmd.rx_packet.data[0]);
      //m_fmParamer.Set((uint16_t)(m_stepBuffer[1]++), value);
      m_stepBuffer[EN_MCU_DEF_STEPBUFF_INDEX_INCREASE]++;
      if (m_stepBuffer[EN_MCU_DEF_STEPBUFF_INDEX_INCREASE] < m_stepBuffer[EN_MCU_DEF_STEPBUFF_INDEX_MAX])
      {
        m_step.SetStep(EN_MCU_STEP_READ_MOTOR_PARM_DATA_START);
      }
      else
      {
        m_step.SetStep(EN_MCU_STEP_READ_MOTOR_PARM_DATA_END);
      }
      m_pre_time = util::millis();
    }
  }
  break;
  case EN_MCU_STEP_READ_MOTOR_PARM_DATA_END:
  {
    if (m_step.pre_step == EN_MCU_STEP_READ_MOTOR_PARM_DATA_WAIT)
    {
      m_stepBuffer[EN_MCU_DEF_STEPBUFF_ONOFF_VALUE] = (uint8_t)true;
      m_stepBuffer[EN_MCU_DEF_STEPBUFF_WAIT_RETURN_STEP] = EN_MCU_STEP_TODO;

      m_step.SetStep(EN_MCU_STEP_REQ_SEND_STATE_ONOFF_START);
    }
    else
    {
      m_step.SetStep(EN_MCU_STEP_TODO);
      memset(&m_stepBuffer, 0, EN_MCU_MAX_BUFFER_LENGTH);
    }
    m_pre_time = util::millis();
  }
  break;
  //
  /*######################################################
     EN_MCU_STEP_REQ_SEND_STATE_ONOFF
    ######################################################*/
  case EN_MCU_STEP_REQ_SEND_STATE_ONOFF:
  {
    m_retryCnt = 0;
    m_IsResponsed = false;

    m_step.SetStep(EN_MCU_STEP_REQ_SEND_STATE_ONOFF_START);
    m_pre_time = util::millis();
  }
  break;
  case EN_MCU_STEP_REQ_SEND_STATE_ONOFF_START:
  {
    /*  */
    if (m_stepBuffer[EN_MCU_DEF_STEPBUFF_ONOFF_VALUE] == (uint8_t)true)
      SendCmd(&m_Cmd, EN_MCU_CMD_START_SEND_MCU_STATE, 0, 0);
    else
      SendCmd(&m_Cmd, EN_MCU_CMD_STOP_SEND_MCU_STATE, 0, 0);

    m_step.SetStep(EN_MCU_STEP_REQ_SEND_STATE_ONOFF_WAIT);
    m_pre_time = util::millis();
  }
  break;
  case EN_MCU_STEP_REQ_SEND_STATE_ONOFF_WAIT:
  {
    if (util::millis() - m_pre_time >= 100)
    {
      if (m_retryCnt++ < EN_MCU_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(EN_MCU_STEP_REQ_SEND_STATE_ONOFF_START);
        m_pre_time = util::millis();
      }
      else
      {
        m_retryCnt = 0;
        m_pre_time = util::millis();
        //
        m_step.SetStep(EN_MCU_STEP_REQ_SEND_STATE_ONOFF_END);
      }
    }

    if (m_IsResponsed)
    {
      if (m_stepBuffer[EN_MCU_DEF_STEPBUFF_ONOFF_VALUE])
        m_IsSendMcuStateLocked = false;
      else
        m_IsSendMcuStateLocked = true;

      m_step.SetStep(EN_MCU_STEP_REQ_SEND_STATE_ONOFF_END);
      m_pre_time = util::millis();
    }
  }
  break;
  case EN_MCU_STEP_REQ_SEND_STATE_ONOFF_END:
  {
    if (m_stepBuffer[EN_MCU_DEF_STEPBUFF_WAIT_RETURN_STEP] != 0)
    {
      m_step.SetStep(m_stepBuffer[EN_MCU_DEF_STEPBUFF_WAIT_RETURN_STEP]);
    }
    else
    {
      m_step.SetStep(EN_MCU_STEP_TODO);
    }
    m_pre_time = util::millis();

  }
  break;
  //
  /*######################################################
     EN_MCU_STEP_GET_MOTOR_PARM_DATA
    ######################################################*/
  case EN_MCU_STEP_GET_MOTOR_PARM_DATA:
  {
    m_retryCnt = 0;
    m_IsResponsed = false;

    m_step.SetStep(EN_MCU_STEP_GET_MOTOR_PARM_DATA_START);
    m_pre_time = util::millis();
  }
  break;
  case EN_MCU_STEP_GET_MOTOR_PARM_DATA_START:
  {
    /*  */
    SendCmd(&m_Cmd, EN_MCU_CMD_FASTECH_MOTOR_PARM_GET, &m_stepBuffer[EN_MCU_DEF_STEPBUFF_INDEX_INCREASE], 1);

    m_step.SetStep(EN_MCU_STEP_GET_MOTOR_PARM_DATA_WAIT);
    m_pre_time = util::millis();
  }
  break;
  case EN_MCU_STEP_GET_MOTOR_PARM_DATA_WAIT:
  {
    if (util::millis() - m_pre_time >= 100)
    {
      if (m_retryCnt++ < EN_MCU_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(EN_MCU_STEP_GET_MOTOR_PARM_DATA_START);
        m_pre_time = util::millis();
      }
      else
      {
        m_retryCnt = 0;
        m_pre_time = util::millis();
        //
        m_step.SetStep(EN_MCU_STEP_GET_MOTOR_PARM_DATA_END);
      }
    }

    if (m_IsResponsed)
    {
      m_step.SetStep(EN_MCU_STEP_GET_MOTOR_PARM_DATA_END);
      m_pre_time = util::millis();
    }
  }
  break;
  case EN_MCU_STEP_GET_MOTOR_PARM_DATA_END:
  {

    m_step.SetStep(EN_MCU_STEP_TODO);
    m_pre_time = util::millis();

  }
  break;

  /*######################################################
     EN_MCU_STEP_CTRL_AXIS_RUN_ORIGIN
    ######################################################*/
  case EN_MCU_STEP_CTRL_AXIS_RUN_ORIGIN:
  {
    m_retryCnt = 0;
    m_IsResponsed = false;

    m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_RUN_ORIGIN_START);
    m_pre_time = util::millis();
  }
  break;
  case EN_MCU_STEP_CTRL_AXIS_RUN_ORIGIN_START:
  {
    /*  */
    SendCmd(&m_Cmd, EN_MCU_CMD_CONTROL_MOT_ORIGIN, 0, 0);

    m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_RUN_ORIGIN_WAIT);
    m_pre_time = util::millis();
  }
  break;
  case EN_MCU_STEP_CTRL_AXIS_RUN_ORIGIN_WAIT:
  {
    if (util::millis() - m_pre_time >= 100)
    {
      if (m_retryCnt++ < EN_MCU_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_RUN_ORIGIN_START);
        m_pre_time = util::millis();
      }
      else
      {
        m_retryCnt = 0;
        m_pre_time = util::millis();
        //
        m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_RUN_ORIGIN_END);
      }
    }

    if (m_IsResponsed)
    {
      m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_RUN_ORIGIN_END);
      m_pre_time = util::millis();
    }
  }
  break;
  case EN_MCU_STEP_CTRL_AXIS_RUN_ORIGIN_END:
  {

    m_step.SetStep(EN_MCU_STEP_WAIT_MOTOR_ORIGIN_CPLT);
    m_pre_time = util::millis();

  }
  break;
  //
  /*######################################################
     EN_MCU_STEP_CTRL_AXIS_MOVE_JOG
    ######################################################*/
  case EN_MCU_STEP_CTRL_AXIS_MOVE_JOG:
  {
    m_retryCnt = 0;
    m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_MOVE_JOG_START);
    m_pre_time = util::millis();
  }
  break;
  case EN_MCU_STEP_CTRL_AXIS_MOVE_JOG_START:
  {
    /*  */
    m_IsReceiveCplt = false;
    SendCmd(&m_Cmd, EN_MCU_CMD_CONTROL_MOT_JOG, &m_stepBuffer[0], 5);

    m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_MOVE_JOG_WAIT);
    m_pre_time = util::millis();
  }
  break;
  case EN_MCU_STEP_CTRL_AXIS_MOVE_JOG_WAIT:
  {
    if (util::millis() - m_pre_time >= 100)
    {
      if (m_retryCnt++ < EN_MCU_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_MOVE_JOG_START);
        m_pre_time = util::millis();
      }
      else
      {
        m_retryCnt = 0;
        m_pre_time = util::millis();
        //
        m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_MOVE_JOG_END);
      }
    }

    if (m_IsReceiveCplt)
    {
      m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_MOVE_JOG_END);
      m_pre_time = util::millis();
    }
  }
  break;
  case EN_MCU_STEP_CTRL_AXIS_MOVE_JOG_END:
  {

    m_step.SetStep(EN_MCU_STEP_TODO);
    m_pre_time = util::millis();
  }
  break;
  /*######################################################
     EN_MCU_STEP_CTRL_AXIS_MOVE_LIMIT
    ######################################################*/
  case EN_MCU_STEP_CTRL_AXIS_MOVE_LIMIT:
  {
    m_retryCnt = 0;
    m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_MOVE_LIMIT_START);
    m_pre_time = util::millis();
  }
  break;
  case EN_MCU_STEP_CTRL_AXIS_MOVE_LIMIT_START:
  {
    /*  */
    m_IsReceiveCplt = false;
    SendCmd(&m_Cmd, EN_MCU_CMD_CONTROL_MOT_LIMIT, &m_stepBuffer[0], 5);

    m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_MOVE_LIMIT_WAIT);
    m_pre_time = util::millis();
  }
  break;
  case EN_MCU_STEP_CTRL_AXIS_MOVE_LIMIT_WAIT:
  {
    if (util::millis() - m_pre_time >= 100)
    {
      if (m_retryCnt++ < EN_MCU_STEP_RETRY_CNT_MAX)
      {
        m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_MOVE_LIMIT_START);
        m_pre_time = util::millis();
      }
      else
      {
        m_retryCnt = 0;
        m_pre_time = util::millis();
        //
        m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_MOVE_LIMIT_END);
      }
    }

    if (m_IsReceiveCplt)
    {
      m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_MOVE_LIMIT_END);
      m_pre_time = util::millis();
    }
  }
  break;
  case EN_MCU_STEP_CTRL_AXIS_MOVE_LIMIT_END:
  {

    m_step.SetStep(EN_MCU_STEP_TODO);
    m_pre_time = util::millis();
  }
  break;
  default:
    break;
  }//switch

}

errno_t enMcuRemote::ReadMcuData(uint32_t data_type, void* parm)
{
  //m_pEnMcuRemote->SendCmd
  if (m_step.GetStep() == EN_MCU_STEP_TODO)
  {
    switch (data_type)
    {
    case EN_MCU_DATA_TYPE_MOTOR_POS:
      break;
    case EN_MCU_DATA_TYPE_AP_DAT:
      break;
    case EN_MCU_DATA_TYPE_CYL_DAT:
      break;
    case EN_MCU_DATA_TYPE_VAC_DAT:
      break;
    case EN_MCU_DATA_TYPE_SEQ_DAT:
      break;
    default:
      m_step.SetStep(EN_MCU_STEP_READ_MCU_DATA);
      break;
    }

    m_pre_time = util::millis();
  }
  return errno_t();
}


void enMcuRemote::WriteMcuData(uint32_t data_type, void* p_data)
{
  if (m_step.GetStep() == EN_MCU_STEP_TODO)
  {
    memset(&m_stepBuffer[0], 0, EN_MCU_MAX_BUFFER_LENGTH);
    switch (data_type)
    {
    case EN_MCU_DATA_TYPE_MOTOR_POS:
      m_step.SetStep(EN_MCU_STEP_WRITE_MOTOR_DATA);
      break;

    case EN_MCU_DATA_TYPE_AP_DAT:
      m_step.SetStep(EN_MCU_STEP_WRITE_AP_CFG);
      break;

    case EN_MCU_DATA_TYPE_CYL_DAT:
      m_step.SetStep(EN_MCU_STEP_WRITE_CYL_DATA);
      break;

    case EN_MCU_DATA_TYPE_VAC_DAT:
      m_step.SetStep(EN_MCU_STEP_WRITE_VAC_DATA);
      break;

    case EN_MCU_DATA_TYPE_SEQ_DAT:
      m_step.SetStep(EN_MCU_STEP_WRITE_SEQ_DATA);
      break;

    default:
      m_step.SetStep(EN_MCU_STEP_READ_MCU_DATA);
      break;
    }
    m_pre_time = util::millis();
  }

}

void enMcuRemote::UpdateMcuOpVirtualSw(op_virual_sw sw)
{
  m_IsResponsed = false;
  uint8_t data = static_cast<uint8_t>(sw);
  SendCmd(&m_Cmd, EN_MCU_CMD_EVENT_MCU_VIRTUAL_SW, &data, 1);
  uint32_t pre_ms = util::millis();
  while (m_IsResponsed != false)
  {
    if ((util::millis() - pre_ms) >= 100)
    {
      break;
    }
  }
}

void enMcuRemote::BuzzerOnOff(bool on_off)
{
  if (on_off)
    m_stepBuffer[0] = m_mcuRegis[EN_MCU_REG_BANK_SETTING].on_bit(EN_MCU_REG_USE_BEEP);
  else
    m_stepBuffer[0] = m_mcuRegis[EN_MCU_REG_BANK_SETTING].off_bit(EN_MCU_REG_USE_BEEP);

  if (m_step.GetStep() == EN_MCU_STEP_TODO)
  {
    m_pre_time = util::millis();
    m_step.SetStep(EN_MCU_STEP_CTRL_AP_CONFIG);
  }

}


errno_t enMcuRemote::OutputOn(uint32_t addr)
{
  int min = static_cast<uint32_t>(out_addr::o_00);
  int max = static_cast<uint32_t>(out_addr::_max);
  if (scope::between(addr, min, max) != true)
    return -1;

  div_t div_result = div(addr, 8); // 몫 -bank no, 나머지 - bit no

  if (m_mcuRegis[(uint8_t)div_result.quot][REG_BIT(div_result.rem)] == false)
  {
    m_mcuRegis[(uint8_t)div_result.quot][REG_BIT(div_result.rem)] = true;

    m_stepBuffer[0] = m_mcuRegis[EN_MCU_IO_OUT_BANK_0].get();
    m_stepBuffer[1] = m_mcuRegis[EN_MCU_IO_OUT_BANK_1].get();
    m_stepBuffer[2] = m_mcuRegis[EN_MCU_IO_OUT_BANK_2].get();
    m_stepBuffer[3] = m_mcuRegis[EN_MCU_IO_OUT_BANK_3].get();

    if (m_step.GetStep() == EN_MCU_STEP_TODO)
    {
      m_step.SetStep(EN_MCU_STEP_CTRL_IO_OUT);
    }

    //SendCmdRxResp(&m_Cmd, EN_MCU_CMD_CONTROL_IO_OUT, &data[0], 4, 50);
  }

  return 0;
}

errno_t enMcuRemote::OutputOff(uint32_t addr)
{
  int min = static_cast<uint32_t>(out_addr::o_00);
  int max = static_cast<uint32_t>(out_addr::_max);
  if (scope::between(addr, min, max) != true)
    return -1;

  div_t div_result = div(addr, 8); // 몫 -bank no, 나머지 - bit no

  if (m_mcuRegis[(uint8_t)div_result.quot][REG_BIT(div_result.rem)] == true)
  {
    m_mcuRegis[(uint8_t)div_result.quot][REG_BIT(div_result.rem)] = false;
    m_stepBuffer[0] = m_mcuRegis[EN_MCU_IO_OUT_BANK_0].get();
    m_stepBuffer[1] = m_mcuRegis[EN_MCU_IO_OUT_BANK_1].get();
    m_stepBuffer[2] = m_mcuRegis[EN_MCU_IO_OUT_BANK_2].get();
    m_stepBuffer[3] = m_mcuRegis[EN_MCU_IO_OUT_BANK_3].get();

    if (m_step.GetStep() == EN_MCU_STEP_TODO)
    {
      m_step.SetStep(EN_MCU_STEP_CTRL_IO_OUT);
    }

    // SendCmdRxResp(&m_Cmd, EN_MCU_CMD_CONTROL_IO_OUT, &data[0], 4, 50);
  }

  return 0;
}

errno_t enMcuRemote::OutputToggle(uint32_t addr)
{
    return errno_t();
}


bool enMcuRemote::IsOn(uint32_t addr)
{
  int min = static_cast<uint32_t>(in_addr::i_00);
  int max = static_cast<uint32_t>(in_addr::_max);
  if (scope::between(addr, min, max) != true)
    return false;

  div_t div_result = div(addr, 8); // 몫 -bank no, 나머지 - bit no

  return m_mcuRegis[(uint8_t)div_result.quot][REG_BIT(div_result.rem)];
}


bool enMcuRemote::IsOff(uint32_t addr)
{
  return !IsOn(addr);
}


errno_t enMcuRemote::Initialize()
{
  return errno_t();
}

errno_t enMcuRemote::MotorOnOff(bool on_off)
{
  if (IsMotorOn() == on_off)
    return 0;

  m_stepBuffer[0] = (uint8_t)on_off;
  //m_stepBuffer[1] = m_mcuRegis[EN_MCU_MOT_STATE_BANK_STATE_0].get();

  if (m_step.GetStep() == EN_MCU_STEP_TODO)
  {
    m_pre_time = util::millis();
    m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_ONOFF);
  }

  return errno_t();
}

void enMcuRemote::okResponse()
{
  SendCmd(&m_Cmd, EN_MCU_CMD_OK_RESPONSE, 0, 0);
}

bool enMcuRemote::IsMotorOn()
{
  return m_mcuRegis[EN_MCU_MOT_STATE_BANK_STATE_0][EN_MCU_MOT_REG_SERVO_ON];
}

errno_t enMcuRemote::WaitDone(uint8_t mode)
{
  if (!IsMoving())
    return 0;
  errno_t ret = 0;
  uint32_t pre_ms = util::millis();
  while (!IsAxisDone())
  {
    if (util::millis() - pre_ms >= EN_MCU_DEF_MOVE_TIMEOUT_MS)
    {
      ret = -1;
      break;
    }
  }

  return ret;
}

bool enMcuRemote::IsMoving()
{
  return m_mcuRegis[EN_MCU_MOT_STATE_BANK_STATE_1][EN_MCU_MOT_REG_MOTIONING];
}

bool enMcuRemote::IsAxisDone()
{
  return m_mcuRegis[EN_MCU_MOT_STATE_BANK_STATE_0][EN_MCU_MOT_REG_INPOSITION];
}

errno_t enMcuRemote::ClearState()
{
  return errno_t();
}

uint32_t enMcuRemote::GetMotorState()
{
  return 0;
}


int enMcuRemote::Move(int cmd_pos, uint32_t cmd_vel /*= 100*/, uint32_t acc /*= 100*/, uint32_t decel /*= 100*/)
{
  if (false == IsMotorOn())
    return -1;

  memset(&m_stepBuffer, 0, EN_MCU_MAX_BUFFER_LENGTH);

  m_stepBuffer[0] = (uint8_t)(cmd_pos >> 0);
  m_stepBuffer[1] = (uint8_t)(cmd_pos >> 8);
  m_stepBuffer[2] = (uint8_t)(cmd_pos >> 16);
  m_stepBuffer[3] = (uint8_t)(cmd_pos >> 24);

  m_stepBuffer[4] = (uint8_t)(cmd_vel >> 0);
  m_stepBuffer[5] = (uint8_t)(cmd_vel >> 8);
  m_stepBuffer[6] = (uint8_t)(cmd_vel >> 16);
  m_stepBuffer[7] = (uint8_t)(cmd_vel >> 24);

  m_stepBuffer[8] = (uint8_t)(acc >> 0);
  m_stepBuffer[9] = (uint8_t)(acc >> 8);
  m_stepBuffer[10] = (uint8_t)(acc >> 16);
  m_stepBuffer[11] = (uint8_t)(acc >> 24);

  m_stepBuffer[12] = (uint8_t)(decel >> 0);
  m_stepBuffer[13] = (uint8_t)(decel >> 8);
  m_stepBuffer[14] = (uint8_t)(decel >> 16);
  m_stepBuffer[15] = (uint8_t)(decel >> 24);

  if (m_step.GetStep() == EN_MCU_STEP_TODO)
  {
    m_pre_time = util::millis();
    m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_MOVE);
  }
  return 0;
}
//축을 정지한다. (한개의 축에 대한 정지)
int enMcuRemote::Stop()
{
  if (m_step.GetStep() == EN_MCU_STEP_TODO)
  {
    m_pre_time = util::millis();
    m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_STOP);
  }
  return 0;
}
//축을 등속이동에 대해 정지한다.
int enMcuRemote::VStop()
{
  return 0;
}
// 축을 비상정지한다.
int enMcuRemote::EStop()
{
  return 0;
}

int enMcuRemote::JogMove(uint32_t cmd_vel /*= 1000*/, bool is_cw /*= true*/)
{
  if (false == IsMotorOn())
    return -1;
  memset(&m_stepBuffer, 0, EN_MCU_MAX_BUFFER_LENGTH);

  m_stepBuffer[0] = (uint8_t)(cmd_vel >> 0);
  m_stepBuffer[1] = (uint8_t)(cmd_vel >> 8);
  m_stepBuffer[2] = (uint8_t)(cmd_vel >> 16);
  m_stepBuffer[3] = (uint8_t)(cmd_vel >> 24);

  m_stepBuffer[4] = (uint8_t)(is_cw);

  if (m_step.GetStep() == EN_MCU_STEP_TODO)
  {
    m_pre_time = util::millis();
    m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_MOVE_JOG);
  }
  return 0;
}

int enMcuRemote::JogMove(uint32_t jog_pos, uint32_t cmd_vel /*= 1000*/, bool is_cw /*= true*/)
{
  if (false == IsMotorOn())
    return -1;
  return 0;
}

errno_t enMcuRemote::SetFastechMotorParm(fm_parameter::param type, int value)
{
  errno_t ret = 0;
  m_stepBuffer[0] = static_cast<uint8_t>(type);
  m_stepBuffer[1] = uint8_t((value >> 0) & 0x000000ff);
  m_stepBuffer[2] = uint8_t((value >> 8) & 0x000000ff);
  m_stepBuffer[3] = uint8_t((value >> 16) & 0x000000ff);
  m_stepBuffer[4] = uint8_t((value >> 24) & 0x000000ff);

  SendCmd(&m_Cmd, EN_MCU_CMD_STOP_SEND_MCU_STATE, 0, 0);
  util::delay(50);
  m_IsResponsed = false;
  SendCmd(&m_Cmd, EN_MCU_CMD_FASTECH_MOTOR_PARM_SET, &m_stepBuffer[0], 5);

  uint32_t pre_ms = util::millis();
  while (m_IsResponsed != false)
  {
    if ((util::millis() - pre_ms) >= 100)
    {
      ret = -1;
      break;
    }
  }
  SendCmd(&m_Cmd, EN_MCU_CMD_START_SEND_MCU_STATE, 0, 0);
  util::delay(50);

  return ret;
}

int enMcuRemote::GetFastechMotorParm(fm_parameter::param cmd)
{
  return m_fmParamer.Get(static_cast<uint16_t>(cmd));
}

void enMcuRemote::RequestMotorParamer(fm_parameter::param type)
{
  if (type == fm_parameter::param::_max)
  {
    m_step.SetStep(EN_MCU_STEP_READ_MOTOR_PARM_DATA);
  }
  else
  {
    m_stepBuffer[EN_MCU_DEF_STEPBUFF_INDEX_INCREASE] = static_cast<uint8_t>(type);
    //SendCmd(&m_Cmd, EN_MCU_CMD_STOP_SEND_MCU_STATE, 0, 0);
    //util::delay(50);
    m_IsResponsed = false;
    SendCmd(&m_Cmd, EN_MCU_CMD_FASTECH_MOTOR_PARM_GET, &m_stepBuffer[EN_MCU_DEF_STEPBUFF_INDEX_INCREASE], 1);
    uint8_t err_cnt = 0;
    uint32_t pre_ms = util::millis();
    while (m_IsResponsed == false)
    {
      if ((util::millis() - pre_ms) >= 50)
      {
        if (err_cnt++ < EN_MCU_STEP_RETRY_CNT_MAX)
        {
          SendCmd(&m_Cmd, EN_MCU_CMD_FASTECH_MOTOR_PARM_GET, &m_stepBuffer[EN_MCU_DEF_STEPBUFF_INDEX_INCREASE], 1);
          pre_ms = util::millis();
        }
        else
        {
          break;
        }
      }
    }
    //SendCmd(&m_Cmd, EN_MCU_CMD_START_SEND_MCU_STATE, 0, 0);
    //util::delay(50);
  }

  m_pre_time = util::millis();
}


bool enMcuRemote::IsSendMcuStateLocked()
{
  return m_IsSendMcuStateLocked;
}

void enMcuRemote::ResquestSendMcuStateOnOff(bool on_off)
{
  if (m_step.GetStep() == EN_MCU_STEP_TODO)
  {
    m_stepBuffer[EN_MCU_DEF_STEPBUFF_ONOFF_VALUE] = (uint8_t)on_off;

    m_step.SetStep(EN_MCU_STEP_REQ_SEND_STATE_ONOFF);
    m_pre_time = util::millis();
  }
  else
  {
    m_pre_time = util::millis();
  }
}

errno_t enMcuRemote::MotorOrigin()
{
  if (m_step.GetStep() == EN_MCU_STEP_TODO)
  {
    m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_RUN_ORIGIN);
    m_pre_time = util::millis();
  }

  return errno_t();
}

errno_t enMcuRemote::ZereSetMotorEncoder()
{
  if (IsMotorOn())
    return -1;

  SendCmd(&m_Cmd, EN_MCU_CMD_MOT_ENCODER_ZEROSET, 0, 0);
  return 0;
}

int enMcuRemote::MoveToLimit(uint32_t cmd_vel, bool is_cw)
{
  if (false == IsMotorOn())
    return -1;
  memset(&m_stepBuffer, 0, EN_MCU_MAX_BUFFER_LENGTH);

  m_stepBuffer[0] = (uint8_t)(cmd_vel >> 0);
  m_stepBuffer[1] = (uint8_t)(cmd_vel >> 8);
  m_stepBuffer[2] = (uint8_t)(cmd_vel >> 16);
  m_stepBuffer[3] = (uint8_t)(cmd_vel >> 24);

  m_stepBuffer[4] = (uint8_t)(is_cw);

  if (m_step.GetStep() == EN_MCU_STEP_TODO)
  {
    m_pre_time = util::millis();
    m_step.SetStep(EN_MCU_STEP_CTRL_AXIS_MOVE_LIMIT);
  }
  return 0;
}