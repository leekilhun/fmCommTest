#include "pch.h"
#include "system_def.h"
#include "system_dat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define INC_NO(x)         (x++)%(static_cast<uint8_t>(conf_dat::addr_e::_max))

conf_dat::conf_dat(char* file_info) :m_incNo(0)
{
  sprintf_s(m_dirFile, PRJ_FILE_DIR_STR_MAX - 1, file_info);
}

conf_dat::~conf_dat()
{
  while (m_incNo > 0)
  {
    if (m_sysCfgDat[m_incNo].type== valueType_e::key_value)
    {
      delete m_sysCfgDat[m_incNo].key_value;
    }
    --m_incNo;
  }
}

int conf_dat::LoadData()
{
  FILE* p_file = nullptr;
  /* Open for read (will fail if file "data" does not exist) */
  fopen_s(&p_file, m_dirFile, "rb");
  if (p_file == nullptr) {
    TRACE("The file 'data' was not opened! \n\r");
    return -1;
  }
  else
    TRACE("The file 'data' was opened \n\r");

  // 파일 크기 구하기
  fseek(p_file, 0, SEEK_END);
  size_t size = ftell(p_file);
  fseek(p_file, 0, SEEK_SET);

  // 파일 크기 + NULL 공간만큼 메모리를 할당하고 0으로 초기화
  char* file_binary = nullptr;
  file_binary = new char[size + 1]{ 0 };
  if (file_binary != nullptr)
  {
    // 파일 내용 읽기
    if (fread(file_binary, size, 1, p_file) > 0) {
      //파싱하여 
      parsing(file_binary, (uint32_t)size);
    }
    else {
      size = 0;
    }
  }
  delete[] file_binary;
  return (uint32_t)size;
}

void conf_dat::PrintData()
{
}

void conf_dat::WriteData(conf_dat::addr_e addr, conf_dat::json_t data)
{
}

conf_dat::json_t conf_dat::ReadData(conf_dat::addr_e addr)
{
  return m_sysCfgDat[static_cast<uint8_t>(addr)];
}

conf_dat::json_t* conf_dat::GetData(conf_dat::addr_e addr)
{
  return &m_sysCfgDat[static_cast<uint8_t>(addr)];
}

void conf_dat::parsingArray(char* p_data, uint32_t size)
{
  enum class state_t {
    section,
    key,
    value,
    partition,
  };

  char tmp[256] = { 0, };
  memcpy(tmp, p_data, size % 256);
  char* tmp_p[SYS_DAT_ARG_TBL_CNT_MAX] = { 0, };
  state_t state = state_t::section;
  char* key_str = m_sysCfgDat[m_incNo].key;

  uint8_t cnt = trans::SplitArgs(tmp, tmp_p, ",", SYS_DAT_ARG_TBL_CNT_MAX);

  for (int i = 0; i < cnt; i++)
  {
    char data;
    uint32_t t_curr = 0;
    uint32_t t_arg_size = 0;
    if (tmp_p[i] != NULL)
    {
      uint32_t t_arg_size = (uint32_t)strlen(tmp_p[i]);
      while (t_curr < t_arg_size)
      {
        data = tmp_p[i][t_curr];
        // fine key and value
        {
          switch (data)       // 문자의 종류에 따라 분기
          {
          case '"':           // 문자가 "이면 문자열
          {
            memcpy(m_sysCfgDat[m_incNo].key, key_str, strlen(key_str));
            m_sysCfgDat[m_incNo].is_array = true;
            char* begin = tmp_p[i] + 1;
            char* end = strchr(begin, '\"');
            size_t stringLength = end - begin;
            memcpy(m_sysCfgDat[m_incNo].str_value, begin, stringLength % SYS_DAT_VALUE_STR_MAX);
            m_sysCfgDat[INC_NO(m_incNo)].type = valueType_e::string;
            t_curr = t_arg_size;
          }
          break;
          case '{':        // key- value       
          {
            memcpy(m_sysCfgDat[m_incNo].key, key_str, strlen(key_str));
            m_sysCfgDat[m_incNo].is_array = true;
            m_sysCfgDat[m_incNo].key_value = new keyValue_t();
            m_sysCfgDat[m_incNo].type = valueType_e::key_value;

            uint32_t curr = 1;
            uint32_t arg_size = (uint32_t)strlen(tmp_p[i]);
            state = state_t::section;
            while (curr < arg_size)
            {
              char data = tmp_p[i][curr];
              switch (state)
              {
              case state_t::section:
              {
                switch (data)
                {
                case '\"':
                  state = state_t::key;
                  break;

                default:
                  break;
                }//switch (data)
              }
              break;
              case state_t::key:
              {
                char* begin = tmp_p[i] + curr;
                char* end = strchr(begin, '\"');
                size_t stringLength = end - begin;
                memcpy(m_sysCfgDat[m_incNo].key_value->key, begin, stringLength % SYS_DAT_KEY_STR_MAX);
                curr = curr + (uint32_t)stringLength;
                state = state_t::partition;
              }
              break;
              case state_t::partition:
              {
                if (data == ':')
                {
                  state = state_t::value;
                }
              }
              break;
              case state_t::value:
              {
                switch (data)       // 문자의 종류에 따라 분기
                {
                case '"':           // 문자가 "이면 문자열
                {
                  char* begin = tmp_p[i] + curr + 1;
                  char* end = strchr(begin, '\"');
                  size_t stringLength = end - begin;
                  memcpy(m_sysCfgDat[m_incNo].key_value->str_value, begin, stringLength % SYS_DAT_VALUE_STR_MAX);
                  m_sysCfgDat[INC_NO(m_incNo)].key_value->type = valueType_e::string;
                  curr = arg_size;
                  t_curr = t_arg_size;

                }
                break;
                case '[':        // 문자가 [이면 배열
                  break;

                case 't': case 'T':
                case 'f': case 'F':
                {
                  char* begin = tmp_p[i] + curr;
                  char* end = strchr(begin, ',');
                  size_t stringLength = end - begin;
                  char tmp_bool[SYS_DAT_VALUE_STR_MAX] = { 0, };
                  memcpy(tmp_bool, begin, stringLength % SYS_DAT_VALUE_STR_MAX);

                  bool is_true = compare::IsEqualStr(tmp_bool, "true", true);
                  m_sysCfgDat[m_incNo].key_value->yesorno = is_true;
                  m_sysCfgDat[INC_NO(m_incNo)].key_value->type = valueType_e::boolian;
                  curr = arg_size;
                  t_curr = t_arg_size;
                }
                break;
                case '0': case '1': case '2': case '3': case '4': case '5':    // 문자가숫자이면
                case '6': case '7': case '8': case '9': case '-':              // -는 수일 때
                {
                  char* begin = tmp_p[i] + curr;
                  char* end = strchr(begin, '}');
                  size_t stringLength = end - begin;
                  char tmp_num[SYS_DAT_VALUE_STR_MAX] = { 0, };
                  memcpy(tmp_num, begin, stringLength % SYS_DAT_VALUE_STR_MAX);

                  char* dot_loc = strchr(tmp_num, '.');
                  if (dot_loc != NULL)
                  {
                    m_sysCfgDat[m_incNo].key_value->realnumber = atof(tmp_num);
                    m_sysCfgDat[INC_NO(m_incNo)].key_value->type = valueType_e::realnumber;
                  }
                  else
                  {
                    m_sysCfgDat[m_incNo].key_value->number = atoi(tmp_num);
                    m_sysCfgDat[INC_NO(m_incNo)].key_value->type = valueType_e::integer;
                  }
                  curr = arg_size;
                  t_curr = t_arg_size;
                }
                break;
                default:
                  break;
                }// switch (data) 
              }
              break;
              default:
                break;
              }//switch (state)

              curr++;
            }//while
          }
          break;
          case 't': case 'T':
          case 'f': case 'F':
          {
            memcpy(m_sysCfgDat[m_incNo].key, key_str, strlen(key_str));
            m_sysCfgDat[m_incNo].is_array = true;
            bool is_true = compare::IsEqualStr(tmp_p[i], "true", true);
            m_sysCfgDat[m_incNo].yesorno = is_true;
            m_sysCfgDat[INC_NO(m_incNo)].type = valueType_e::boolian;
            t_curr = t_arg_size;
          }
          break;
          case '0': case '1': case '2': case '3': case '4': case '5':    // 문자가숫자이면
          case '6': case '7': case '8': case '9': case '-':              // -는 수일 때
          {
            memcpy(m_sysCfgDat[m_incNo].key, key_str, strlen(key_str));
            m_sysCfgDat[m_incNo].is_array = true;   

            char* dot_loc = strchr(tmp_p[i], '.');
            if (dot_loc != NULL)
            {
              m_sysCfgDat[m_incNo].realnumber = atof(tmp_p[i]);
              m_sysCfgDat[INC_NO(m_incNo)].type = valueType_e::realnumber;
            }
            else
            {
              m_sysCfgDat[m_incNo].number = atoi(tmp_p[i]);
              m_sysCfgDat[INC_NO(m_incNo)].type = valueType_e::integer;
            }
            t_curr = t_arg_size;
          }
          break;
          default:
            break;
          }

        }
        t_curr++;
      }// while (arg_size < curr)    
    }//if (tmp_p[i] != NULL)
  }//for (int i = 0; i < cnt; i++)
}



void conf_dat::parsing(char* p_data, uint32_t size)// uint8_t section_no/*= 0*/, uint32_t curr_no /*= 0*/
{
  enum class state_t {
    section,
    key,
    value,
    partition,
  };

  uint32_t curr = 0;
  //uint32_t addr = 0;
  bool is_pass_cnt = false;
  char data;
  char tmp[SYS_DAT_PARSING_LINE_STR_MAX] = { 0 };
  char* tmp_p[SYS_DAT_ARG_TBL_CNT_MAX] = { 0, };
  state_t state = state_t::section;
  if (p_data[curr] != '{' && p_data[curr] != '[')
    return;

  curr++;
  // 문서 크기만큼 반복
  while (curr < size)
  {
    data = p_data[curr];
    switch (state)
    {
    case state_t::section:
    {
      switch (data)
      {
      case '\"':
      {
        state = state_t::key;
      }
      break;
      case '/':
      {
        if (p_data[curr + 1] == '/') // 주석 처리
        {
          char* begin = p_data + curr + 1;
          char* end = strchr(begin, '\n');
          size_t stringLength = end - begin;
          curr = curr + (uint32_t)stringLength + 1;
        }
      }
      break;
      case '\n':case '\t':case ' ': //
      default:
        break;
      }//switch

    }
    break;
    case state_t::key:
    {
      char* begin = p_data + curr;
      char* end = strchr(begin, '\"');
      size_t stringLength = end - begin;
      memcpy(m_sysCfgDat[m_incNo].key, begin, stringLength % SYS_DAT_KEY_STR_MAX);
      curr = curr + (uint32_t)stringLength;
      state = state_t::partition;
    }
    break;
    case state_t::partition:
    {
      if (data == ':')
      {
        state = state_t::value;
      }
      if (data == ',')
      {
        state = state_t::section;
      }
    }
    break;
    case state_t::value:
    {
      switch (data)       // 문자의 종류에 따라 분기
      {
      case '"':           // 문자가 "이면 문자열
      {
        char* begin = p_data + curr + 1;
        char* end = strchr(begin, '\"');
        size_t stringLength = end - begin;
        memcpy(m_sysCfgDat[m_incNo].str_value, begin, stringLength % SYS_DAT_VALUE_STR_MAX);
        curr = curr + (uint32_t)stringLength;
        m_sysCfgDat[INC_NO(m_incNo)].type = valueType_e::string;
        state = state_t::partition;
      }
      break;
      case '[':        // 문자가 [이면 배열
      {
        //parsing(p_data, size, curr);
        m_sysCfgDat[m_incNo].is_array = true;
        char* begin = p_data + curr + 1;
        char* end = strchr(begin, ']');
        size_t stringLength = end - begin;

        parsingArray(begin, (uint32_t)stringLength);
        curr = curr + (uint32_t)stringLength;
        state = state_t::partition;

      }
      break;
      case 't': case 'T':
      case 'f': case 'F':
      {
        char* begin = p_data + curr ;
        char* end = strchr(begin, ',');
        size_t stringLength = end - begin;
        char tmp_bool[SYS_DAT_VALUE_STR_MAX] = { 0, };
        memcpy(tmp_bool, begin, stringLength% SYS_DAT_VALUE_STR_MAX);

        bool is_true = compare::IsEqualStr(tmp_bool, "true", true);
        m_sysCfgDat[m_incNo].yesorno = is_true;
        m_sysCfgDat[INC_NO(m_incNo)].type = valueType_e::boolian;
        curr = curr + (uint32_t)stringLength;
        state = state_t::partition;


      }
      break;
      case '0': case '1': case '2': case '3': case '4': case '5':    // 문자가숫자이면
      case '6': case '7': case '8': case '9': case '-':              // -는 수일 때
      {
        char* begin = p_data + curr;
        char* end = strchr(begin, ',');
        if (end == 0)
        {
          // } 가 나오면 문자열이 끝남
          end = strchr(p_data + curr, '}');
          if (end == NULL)    // }가 없으면 잘못된 문법이므로
          {
            TRACE("fail json format : not found '}'! \n\r");
            return;          // 반복을 종료
          }
        }
        size_t stringLength = end - begin;
        char tmp_num[SYS_DAT_VALUE_STR_MAX] = { 0, };
        memcpy(tmp_num, begin, stringLength % SYS_DAT_VALUE_STR_MAX);
        curr = curr + (uint32_t)stringLength;

        char* dot_loc = strchr(tmp_num, '.');
        if (dot_loc != NULL)
        {
          m_sysCfgDat[m_incNo].realnumber = atof(tmp_num);
          m_sysCfgDat[INC_NO(m_incNo)].type = valueType_e::realnumber;
        }
        else
        {
          m_sysCfgDat[m_incNo].number = atoi(tmp_num);
          m_sysCfgDat[INC_NO(m_incNo)].type = valueType_e::integer;
        }
        state = state_t::partition;
        is_pass_cnt = true;
      }
      break;
      default:
        break;
      }
    }
    break;
    }//switch


    if (is_pass_cnt)
      is_pass_cnt = false;
    else
      curr++;

  }//while






}

bool conf_dat::putData(char** pdata, uint8_t cnt)
{
  bool is_input = false;
  bool ret = false;
  dat_t data = {};
  for (uint8_t i = 0; i < cnt; i++) {
    switch (i)
    {
    case 0:
    {
      trans::Replace(pdata[i], ' ');
      trans::Replace(pdata[i], '\t', true);
      is_input = compare::IsEqualStr(pdata[i], "INPUT");
    }
    break;
    case 1:
    {
      trans::Replace(pdata[i], ' ');
      trans::Replace(pdata[i], '\t', true);
      if (is_input) {
        //data.Set_type(IO_ADDR_BASE_INPUT);
      }
      else {
        //data.Set_type(IO_ADDR_BASE_OUTPUT);
      }
      // data.Set_name(pdata[i]);
    }
    break;
    case 2:
    {
      trans::Replace(pdata[i], ' ');
      trans::Replace(pdata[i], '\t');
      //data.addr = atoi(pdata[i]);
      ret = true;
    }
    break;
    }
  }
  if (ret) {
    //  if (is_input)
        //input[input_idx++] = data;
    //  else
        //output[output_idx++] = data;
  }
  return ret;
}



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
