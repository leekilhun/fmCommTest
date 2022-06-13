#pragma once


#define EN_MCU_MAX_BUFFER_LENGTH                    46
#define EN_MCU_MAX_PACKET_BUFF_LENGTH               EN_MCU_MAX_BUFFER_LENGTH + 5

#define EN_MCU_CMD_READ_ALL_STATE                   0x00
#define EN_MCU_CMD_READ_BOOT_INFO                   0x01
#define EN_MCU_CMD_READ_FIRM_INFO                   0x02
#define EN_MCU_CMD_CONTROL_IO_OUT                   0x10
#define EN_MCU_CMD_CONTROL_MOT_ORIGIN               0x11
#define EN_MCU_CMD_CONTROL_MOT_ONOFF                0x12
#define EN_MCU_CMD_CONTROL_MOT_RUN                  0x13
#define EN_MCU_CMD_CONTROL_MOT_STOP                 0x14
#define EN_MCU_CMD_CONTROL_MOT_JOG                  0x15
#define EN_MCU_CMD_CONTROL_MOT_LIMIT                0x16
#define EN_MCU_CMD_MOT_ENCODER_ZEROSET              0x17
#define EN_MCU_CMD_AP_CONFIG_WRITE                  0x20
#define EN_MCU_CMD_READ_MCD_DATA                    0x21
#define EN_MCU_CMD_WRITE_MOTOR_POS_DATA             0x30
#define EN_MCU_CMD_WRITE_AP_DATA                    0x31
#define EN_MCU_CMD_WRITE_CYL_DATA                   0x32
#define EN_MCU_CMD_WRITE_VAC_DATA                   0x33
#define EN_MCU_CMD_WRITE_SEQ_DATA                   0x34
#define EN_MCU_CMD_EVENT_MCU_VIRTUAL_SW             0x40


#define EN_MCU_CMD_START_SEND_MCU_STATE             0xB0
#define EN_MCU_CMD_STOP_SEND_MCU_STATE              0xB1

#define EN_MCU_CMD_FASTECH_MOTOR_PARM_SET           0xAC
#define EN_MCU_CMD_FASTECH_MOTOR_PARM_GET           0xAE
#define EN_MCU_CMD_OK_RESPONSE                      0xAA


#define EN_MCU_DATA_TYPE_MOTOR_POS                  35
#define EN_MCU_DATA_TYPE_AP_DAT                     39
#define EN_MCU_DATA_TYPE_CYL_DAT                    43
#define EN_MCU_DATA_TYPE_VAC_DAT                    47
#define EN_MCU_DATA_TYPE_SEQ_DAT                    51


class enMcuRemote :public IAxis, public IIO, public virtual Icommon
{
  /****************************************************
   *	data
   ****************************************************/
  struct robotro_t {
    struct robotro_packet_t {
      uint8_t   mot_id{};
      uint8_t   cmd_type{};
      uint8_t   error{};
      uint16_t  length{};
      uint8_t   check_sum{};
      uint8_t   check_sum_recv{};
      uint8_t   buffer[EN_MCU_MAX_PACKET_BUFF_LENGTH]{};
      uint8_t* data{};
    };

    uint8_t   state{};
    uint32_t  pre_time{};
    uint32_t  index{};
    uint32_t  data_len{};
    uint8_t   error{};

    robotro_packet_t  rx_packet;
    robotro_packet_t  tx_packet;
  };
public:
  struct cfg_t {
    //IIO* p_Io;
    IComm* p_serial{};
    ap_dat* p_apDat{};
    axis_dat* p_axisDat{};
    cyl_dat* p_cylDat{};
    vac_dat* p_vacDat{};
    seq_dat* p_seqDat{};

    inline void operator = (const cfg_t* p_cfg) {
      //this->p_Io = p_cfg->p_Io;
      this->p_serial = p_cfg->p_serial;
    }

  };

  enum class reg_e {
    ap_b0, ap_b1, ap_b2, ap_b3,
    in_b0, in_b1, in_b2, in_b3,
    out_b0, out_b1, out_b2, out_b3,
    mot_b0, mot_b1, mot_b2, mot_b3,
    _max
  };

  enum class in_addr {
    i_00 = 32, i_01, i_02, i_03, i_04, i_05, i_06, i_07,
    i_10, i_11, i_12, i_13, i_14, i_15, i_16, i_17,
    i_20, i_21, i_22, i_23, i_24, i_25, i_26, i_27,
    i_30, i_31, i_32, i_33, i_34, i_35, i_36, i_37,
    _max
  };

  enum class op_virual_sw {
    start = 2, stop, reset, e_stop
  };

  enum class out_addr {
    o_00 = 64, o_01, o_02, o_03, o_04, o_05, o_06, o_07,
    o_10, o_11, o_12, o_13, o_14, o_15, o_16, o_17,
    o_20, o_21, o_22, o_23, o_24, o_25, o_26, o_27,
    o_30, o_31, o_32, o_33, o_34, o_35, o_36, o_37,
    _max
  };

  struct mot_data_t
  {
    int       act_pos{};
    int       cmd_pos{};
    uint32_t  act_vel{};
    int       pos_err{};
  };


private:
  IComm* m_pSerialComm;
  ap_dat* m_pApDat;
  axis_dat* m_pAxisDat;
  cyl_dat* m_pCylDat;
  vac_dat* m_pVacDat;
  seq_dat* m_pSeqDat;
  //IIO*  m_pIo;
  bool  m_IsReceiveCplt;
  uint32_t m_prev_ms;
  uint32_t m_scan_ms;

  uint8_t m_retryCnt;
  uint32_t m_pre_time;
  uint8_t m_stepBuffer[EN_MCU_MAX_BUFFER_LENGTH];
  bool m_IsResponsed;
  bool m_IsSendMcuStateLocked;

public:
  prc_step_t m_step;
  flag::fdata m_mcuRegis{ static_cast<int>(reg_e::_max) };
  mot_data_t m_mcuMotor;
  uint8_t m_id;
  robotro_t m_Cmd;
  fm_parameter m_fmParamer;

  /****************************************************
   *	Constructor
   ****************************************************/
public:
  enMcuRemote(common::Attribute_t common_data, cfg_t* cfg);

  virtual ~enMcuRemote();

  /****************************************************
   *	func
   ****************************************************/
private:
  static void receiveData(void* obj, void* w_parm, void* l_param);
  bool receiveCplt();
  void doRunStep();
  void okResponse();
public:

  bool IsConnected();
  
  void SendCmd(robotro_t* p_cmd, uint8_t cmd, uint8_t* p_data, uint32_t length);
  bool SendCmdRxResp(robotro_t* p_cmd, uint8_t cmd, uint8_t* p_data, uint32_t length, uint32_t timeout);
  
  uint32_t GetScanTime();
  errno_t RequestMcuState();
  void ResquestSendMcuStateOnOff(bool on_off = true);
  bool IsSendMcuStateLocked();
  void RequestMotorParamer(fm_parameter::param type = fm_parameter::param::_max);
  int GetFastechMotorParm(fm_parameter::param type);
  errno_t SetFastechMotorParm(fm_parameter::param type, int value);
  void BuzzerOnOff(bool on_off);
  errno_t ReadMcuData(uint32_t data_type = 0, void* parm = nullptr);
  void WriteMcuData(uint32_t data_type = 0, void* parm = nullptr);
  void UpdateMcuOpVirtualSw(op_virual_sw sw);
  errno_t ZereSetMotorEncoder();
  errno_t MotorOrigin();

  /* IAxis */
  errno_t Initialize();
  errno_t MotorOnOff(bool on_off);
  bool IsMotorOn();
  errno_t WaitDone(uint8_t mode);
  bool IsMoving();

  bool IsAxisDone();
  errno_t  ClearState();
  uint32_t  GetMotorState();
  int Move(int cmd_pos, uint32_t cmd_vel = 100, uint32_t acc = 100, uint32_t decel = 100);
  //축을 정지한다. (한개의 축에 대한 정지)
  int Stop();
  //축을 등속이동에 대해 정지한다.
  int VStop();
  // 축을 비상정지한다.
  int EStop();

  int JogMove(uint32_t cmd_vel = 1000, bool is_cw = true);
  int JogMove(uint32_t jog_pos, uint32_t cmd_vel = 1000, bool is_cw = true);
  int MoveToLimit(uint32_t cmd_vel = 1000, bool is_plus = true);


  /* IIO */
  bool IsOn(uint32_t addr);
  bool IsOff(uint32_t addr);
  errno_t OutputOn(uint32_t addr);
  errno_t OutputOff(uint32_t addr);
  errno_t OutputToggle(uint32_t addr);






};