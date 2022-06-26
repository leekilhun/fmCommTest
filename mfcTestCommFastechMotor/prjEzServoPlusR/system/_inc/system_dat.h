#pragma once

/*
 * system_dat.h
 *
 * 작성자 : 이길훈
 * 22.04.05
 *  data  객체
 */


/****************************************************
 *	
 ****************************************************/

/// <summary>
/// system config 정보 데이터
/// </summary>
struct conf_dat
{

  enum class addr_e //
  {
    rem_portname, rem_baudrate, conf_2, conf_3, conf_4, conf_5, conf_6, conf_7,
    conf_10, conf_11, conf_12, conf_13, conf_14, conf_15, conf_16, conf_17,
    conf_20, conf_21, conf_22, conf_23, conf_24, conf_25, conf_26, conf_27,
    _max
  };


  parser::json* m_pParser;
  char m_dirFile[PRJ_FILE_DIR_STR_MAX]{};

  conf_dat(char* file_info = "./conf_dat.json");
  virtual ~conf_dat();

  int LoadData();
  
  void PrintData();
  void WriteData(conf_dat::addr_e addr, parser::json_t data);
 
  parser::json_t ReadData(conf_dat::addr_e addr);
  parser::json_t* GetData(conf_dat::addr_e addr); 


private:
  //void parsing(char* p_data, uint32_t size);
  //void parsingArray(char* p_data, uint32_t size);
  //bool putData(char** pdata, uint8_t cnt);

};


/****************************************************
 *	controller의 mcu eeprom data 정보를 읽는다
 ****************************************************/

/// <summary>
/// eeprom ap config data
/// </summary>
struct ap_dat
{
  struct dat_t
  {
    uint16_t  parm1;
    uint16_t  parm2;
  };

  enum class addr_e //
  {
    ap_mot_cfg, ap_1, ap_2, ap_3, ap_4, ap_5, ap_6, ap_7,
    _max
  };

  ap_dat::dat_t apcfg_dat[static_cast<uint8_t>(ap_dat::addr_e::_max)];

  void WriteData(ap_dat::addr_e addr, ap_dat::dat_t data);
  ap_dat::dat_t ReadData(ap_dat::addr_e addr);
  ap_dat::dat_t* GetData(ap_dat::addr_e addr);
  void LoadRomData();
  bool ClearRomData();
  void Write_MotCfg(ap_dat::dat_t data);
  ap_dat::dat_t Read_MotCfg();
};


struct axis_dat
{
  struct dat_t
  {
    int       cmd_pos{};
    uint32_t  cmd_vel{};
  };

  /*multiply by data length 8*/
  enum class addr_e
  {
    ready_pos, pos_peel_start, pos_vinyl_cplt, pos_tans,
    pos_vinyl_reattach, pos_reserved, pos_6, pos_7,
    _max
  };

  axis_dat::dat_t cmd_pos_dat[static_cast<uint8_t>(axis_dat::addr_e::_max)];

  void WriteData(axis_dat::addr_e addr, axis_dat::dat_t data);
  axis_dat::dat_t* GetData(axis_dat::addr_e addr);
  axis_dat::dat_t ReadData(axis_dat::addr_e addr);
  void LoadRomData();
  bool ClearRomData();

};

struct cyl_dat 
{
  struct dat_t
  {
    /** 동작 제한 시간 ms */
    uint16_t timeout;

    /** 동작 후 안정화 시간 ms*/
    uint16_t on_settling;
  };

  /* multiply by length 4*/
  enum class addr_e //
  {
    phone_clamp_cyl,
    cyl_1, cyl_2, cyl_3,
    cyl_4, cyl_5, cyl_6, cyl_7,
    _max
  };

  cyl_dat::dat_t cyl_act_dat[static_cast<uint8_t>(cyl_dat::addr_e::_max)];

  void WriteData(cyl_dat::addr_e addr, cyl_dat::dat_t data);
  cyl_dat::dat_t* GetData(cyl_dat::addr_e addr);
  cyl_dat::dat_t ReadData(cyl_dat::addr_e addr);
  void LoadRomData();
  bool ClearRomData();
};

struct vac_dat {
  struct dat_t
  {
    /** 동작 제한 시간 */
    uint16_t timeout;
    /** ON 동작 후 안정화 시간 */
    uint16_t  on_settling;
  };

  enum class addr_e //
  {
    peel_drum_vac,
    vac_1, vac_2, vac_3,
    vac_4, vac_5, vac_6, vac_7,
    _max
  };

  vac_dat::dat_t vac_act_dat[static_cast<uint8_t>(vac_dat::addr_e::_max)];

  void WriteData(vac_dat::addr_e addr, vac_dat::dat_t data);
  vac_dat::dat_t* GetData(vac_dat::addr_e addr);
  vac_dat::dat_t ReadData(vac_dat::addr_e addr);
  void LoadRomData();
  bool ClearRomData();

};



struct seq_dat {
  struct dat_t
  {
    uint16_t  parm1;
    uint16_t  parm2;
  };

  enum class addr_e //
  {
    seq_0, seq_1, seq_2, seq_3,
    seq_4, seq_5, seq_6, seq_7,
    _max
  };

  seq_dat::dat_t sequencing_dat[static_cast<uint8_t>(seq_dat::addr_e::_max)];

  void WriteData(seq_dat::addr_e addr, seq_dat::dat_t data);
  seq_dat::dat_t* GetData(seq_dat::addr_e addr);
  seq_dat::dat_t ReadData(seq_dat::addr_e addr);
  void LoadRomData();
  bool ClearRomData();
};