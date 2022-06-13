#pragma once

class ioFastech;

#define USE_IO_DATA_FORMAT_DAT
#ifndef USE_IO_DATA_FORMAT_DAT
#define USE_IO_DATA_FORMAT_JSON
#define IO_DATA_DEFAULT_PATH "./io_dat.json"
#else
#define IO_DATA_DEFAULT_PATH "./io.dat"
#endif

#define IO_NAME_STR_MAX				PRJ_NAME_STR_LENGTH_MAX
#define IO_OUTPUT_MAX             3000
#define IO_INPUT_MAX              3000
#define IO_DAT_ARG_TBL_CNT_MAX    3000
class Table_IO
{
  enum class type_e
  {
    in_t,
    out_t,
  };
  struct dat_t {
    Table_IO::type_e type{};
    char name[IO_NAME_STR_MAX]{};
    uint32_t addr{};
  };

private:
  dat_t* m_pIn[IO_INPUT_MAX]{};
  vector< dat_t*> m_vtIn;
  uint32_t m_inCnt{};

  dat_t* m_pOut[IO_OUTPUT_MAX]{};
  vector< dat_t*> m_vtOut;
  uint32_t m_outCnt{};


  char m_fileInfo[PRJ_FILE_DIR_STR_MAX]{};

  /****************************************************
   *	Constructor
   ****************************************************/
public:
  Table_IO(char* file_info = IO_DATA_DEFAULT_PATH)
  {
    strcpy_s(m_fileInfo, strlen(file_info) + 1, file_info);
  }

  virtual ~Table_IO()
  {
    while (m_inCnt)
    {
      if (m_pIn[m_inCnt])
      {
        delete m_pIn[m_inCnt];
      }
      m_pIn[m_inCnt--] = nullptr;

    }
    while (m_outCnt)
    {
      if (m_pOut[m_outCnt])
      {
        delete m_pOut[m_outCnt];
      }
      m_pOut[m_outCnt--] = nullptr;
    }
  }

  /****************************************************
   *	func
   ****************************************************/
private:
  inline bool putData(char** pdata, uint8_t cnt) {
    return false;
  }

  inline void parsingJson(char* p_data, uint32_t size)
  {
    enum class state_t {
      section,
      key,
      value,
      partition,
    };

    type_e data_type = type_e::in_t;
    uint32_t curr = 0;
    char data;
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
        char tmp_str[IO_NAME_STR_MAX] = { 0, };
        size_t stringLength = end - begin;
        memcpy(tmp_str, begin, stringLength % IO_NAME_STR_MAX);
        curr += (uint32_t)stringLength;

        state = state_t::partition;

        // check type
        if (compare::IsEqualStr("input", tmp_str, true))
          data_type = type_e::in_t;
        else if (compare::IsEqualStr("output", tmp_str, true))
          data_type = type_e::out_t;
        else
          state = state_t::section;

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
        switch (data)
        {
        case '[':        // 문자가 [이면 배열
        {
          char* begin = p_data + curr;
          char* end = strchr(begin, ']');
          size_t length = end - begin;
          curr = curr + (uint32_t)length;
          size_t str_len = 0, loc = 1;
          uint32_t idx = 0;

          enum class _state_t {
            section,
            name,
            addr,
            partition,
          } _state;

          _state = _state_t::section;
          char* p_tmp = new char[length] {};
          memcpy(p_tmp, begin, length);
          //char* tmp_p[50] = { 0, };
          char** pp_tmp = new char* [IO_DAT_ARG_TBL_CNT_MAX] {0, };
          char tmp_addr[IO_NAME_STR_MAX] = { 0, };
          uint8_t cnt = trans::SplitArgs(p_tmp, pp_tmp, ",", 50);
          for (int i = 0; i < cnt; i++)
          {
            char* p_char = pp_tmp[i];
            str_len = strlen(p_char);
            loc = 1;
            while (str_len > loc)
            {
              char data = p_char[loc];

              switch (_state)
              {
              case _state_t::section:
              {
                switch (data)
                {
                case '\"':
                {
                  _state = _state_t::name;
                }
                break;
                }
              }
              break;
              case _state_t::name:
              {
                char* begin = p_char + loc;
                char* end = strchr(begin, '\"');
                size_t length = end - begin;
                if (data_type == type_e::in_t)
                {
                  m_pIn[m_inCnt % IO_INPUT_MAX] = new dat_t();
                  m_pIn[m_inCnt % IO_INPUT_MAX]->type = data_type;
                  memcpy(m_pIn[m_inCnt % IO_INPUT_MAX]->name, begin, length% IO_NAME_STR_MAX);
                }
                else
                {
                  m_pOut[m_outCnt % IO_INPUT_MAX] = new dat_t();
                  m_pOut[m_outCnt % IO_INPUT_MAX]->type = data_type;
                  memcpy(m_pOut[m_outCnt % IO_INPUT_MAX]->name, begin, length% IO_NAME_STR_MAX);
                }   

                loc += length;
                _state = _state_t::partition;
              }
              break;
              case _state_t::partition:
              {
                if (data == ':')
                {
                  _state = _state_t::addr;
                  idx = 0;
                }
              }
              break;
              case _state_t::addr:
              {

                switch (data)
                {
                case '0': case '1': case '2': case '3': case '4': case '5':    // 문자가숫자이면
                case '6': case '7': case '8': case '9':
                  tmp_addr[idx++] = data;
                  break;

                case '}':
                {
                  if (data_type == type_e::in_t)
                  {
                    m_pIn[m_inCnt]->addr = atoi(tmp_addr);
                    m_inCnt++;
                  }
                  else
                  {
                    m_pOut[m_outCnt]->addr = atoi(tmp_addr);
                    m_outCnt++;
                  }                  
                  _state = _state_t::section;
                  loc = str_len;
                }
                break;
                default:
                  break;
                }//switch (data)

              }
              break;
              default:
                break;
              } // switch

              loc++;
            }// while

            p_char++;
            
          }// for 
          state = state_t::section;
          if (p_tmp) { delete[] p_tmp; p_tmp = nullptr; }
          if (pp_tmp) { delete[] pp_tmp; pp_tmp = nullptr; }
        }
        break;
        default:
          break;
        } //switch
      }
      break;
      default:
        break;
      }//switch

      curr++;

    }//while
  }

  inline void parsingDat(char* p_data, uint32_t size)
  {
    enum class state_t {
      type,
      name,
      addr,
      wait,
    };

    type_e data_type = type_e::in_t;
    uint32_t curr = 0;
    char data;
    state_t state = state_t::type;

    curr++;
    // 문서 크기만큼 반복
    while (curr < size)
    {
      data = p_data[curr];
      switch (state)
      {
      case state_t::type:
      {
        switch (data)
        {
        case 'I':
        case 'i':
        {
          state = state_t::name;
          char* begin = p_data + curr;
          char* end = strchr(begin, ',');
          size_t stringLength = end - begin;
          curr += (uint32_t)stringLength + 1;
          data_type = type_e::in_t;
        }
        break;
        case 'O':
        case 'o':
        {
          state = state_t::name;
          char* begin = p_data + curr;
          char* end = strchr(begin, ',');
          size_t stringLength = end - begin;
          curr += (uint32_t)stringLength + 1;
          data_type = type_e::out_t;
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
      case state_t::name:
      {
        switch (data)
        {
        case '\n':case '\t':case ' ': //
          break;
        default:
        {
          
          char* begin = p_data + curr;
          char* end = strchr(begin, ',');
          size_t stringLength = end - begin;
          if (data_type == type_e::in_t)
          {
            m_pIn[m_inCnt % IO_INPUT_MAX] = new dat_t();

            memcpy(m_pIn[m_inCnt]->name, begin, stringLength% IO_NAME_STR_MAX);
            m_pIn[m_inCnt]->type = type_e::in_t;
          }
          else
          {
            m_pOut[m_outCnt % IO_INPUT_MAX] = new dat_t();

            memcpy(m_pOut[m_outCnt]->name, begin, stringLength% IO_NAME_STR_MAX);
            m_pOut[m_outCnt]->type = type_e::out_t;
          }

          curr = curr + (uint32_t)stringLength + 1;
          state = state_t::addr;
        }
        break;
        }//switch        
      }
      break;
      case state_t::wait:
        break;
      case state_t::addr:
      {

        char* begin = p_data + curr;
        char* end = strchr(begin, '\n');
        size_t length = end - begin;
        curr = curr + (uint32_t)length;
        char tmp_addr[IO_NAME_STR_MAX] = { 0, };
        uint8_t idx = 0, loc = 0;
        while (length > 0)
        {
          switch (*(begin + idx))
          {
          case '0': case '1': case '2': case '3': case '4': case '5':    // 문자가숫자이면
          case '6': case '7': case '8': case '9':
            tmp_addr[loc++] = *(begin + idx);
            break;
          case '/':// 주석 처리
          {
            length = 1;
          }
          break;
          default:
            break;
          }//switch
          idx++;
          length--;
        }
        if (data_type == type_e::in_t)
        {
          m_pIn[m_inCnt]->addr = atoi(tmp_addr);
          m_inCnt++;
        }
        else
        {
          m_pOut[m_outCnt]->addr = atoi(tmp_addr);
          m_outCnt++;
        }
        
        state = state_t::type;
      }

      break;
      }//switch

      curr++;

    }//while
  }


public:
  inline uint32_t LoadData() {
    FILE* p_file = nullptr;
    /* Open for read (will fail if file "data" does not exist) */
    fopen_s(&p_file, m_fileInfo, "rb");
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
#ifndef USE_IO_DATA_FORMAT_DAT
        parsingJson(file_binary, (uint32_t)size);
#else
        parsingDat(file_binary, (uint32_t)size);
#endif
    }
      else {
        size = 0;
      }
  }
    delete[] file_binary;
    return (uint32_t)size;
  }


#if 0

private:
  inline bool putData(char** pdata, uint8_t cnt) {
    bool is_input = false;
    bool ret = false;
    Table_IO::data_t data = {};
    for (uint8_t i = 0; i < cnt; i++) {
      switch (i)
      {
      case 0:
      {
        trans::Replace(pdata[i], ' ');
        trans::Replace(pdata[i], '\t', true);
        is_input = equal::EqualStr(pdata[i], "INPUT");
      }
      break;
      case 1:
      {
        trans::Replace(pdata[i], ' ');
        trans::Replace(pdata[i], '\t', true);
        if (is_input) {
          data.Set_type(IO_ADDR_BASE_INPUT);
        }
        else {
          data.Set_type(IO_ADDR_BASE_OUTPUT);
        }
        data.Set_name(pdata[i]);
      }
      break;
      case 2:
      {
        trans::Replace(pdata[i], ' ');
        trans::Replace(pdata[i], '\t');
        data.addr = atoi(pdata[i]);
        ret = true;
      }
      break;
      }
    }
    if (ret) {
      if (is_input)
        input[input_idx++] = data;
      else
        output[output_idx++] = data;
    }
    return ret;
  }
  inline void parsing(char* p_data, uint32_t size) {
    enum class state_t {
      _init,
      find_type,
      parsing,
    };

    uint32_t curr = 0;
    char data;
    char tmp[IO_PARSING_LINE_STR_MAX] = { 0 };
    char* tmp_p[IO_ARG_TBL_CNT_MAX] = { 0, };
    state_t state = state_t::find_type;

    // 문서 크기만큼 반복
    while (curr < size) {
      data = p_data[curr];

      switch (state)
      {
      case state_t::find_type:
      {
        switch (data)
        {
        case '\n': //
        {
          goto pass_parsing;
        }
        break;
        case ' ': // 공백 처리
        {
          curr++;
          if (p_data[curr] == ' ') {
            char* begin = p_data + curr + 1;
            char* end = strchr(begin, '\n');
            size_t stringLength = end - begin;
            curr = curr + (uint32_t)stringLength + 1;
          }
        }
        case '/': //주석 처리
        {
          curr++;
          if (p_data[curr] == '/') {
            char* begin = p_data + curr + 1;
            char* end = strchr(begin, '\n');
            size_t stringLength = end - begin;
            curr = curr + (uint32_t)stringLength + 1;
          }
        }
        break;
        case 'i':
        case 'I':
        case 'o':
        case 'O':
        {
          state = state_t::parsing;
          goto skip_cnt;
        }
        break;
        }
      }
      break;
      case state_t::parsing:
      {
        char* begin = p_data + curr + 1;
        char* end = strchr(begin, '\n');
        size_t stringLength = end - begin;
        memcpy_s(tmp, IO_PARSING_LINE_STR_MAX, &p_data[curr], scope::constrain((int)stringLength, 0, IO_PARSING_LINE_STR_MAX - 1));
        curr = curr + (uint32_t)stringLength + 1;
        uint8_t cnt = trans::SplitArgs(tmp, tmp_p, ", /");
        if (cnt > 0) {
          putData(tmp_p, cnt);
        }
        state = state_t::find_type;
      }
      break;
      }//switch
    pass_parsing:
      curr++;

    skip_cnt:
      curr;
    }//while
  }

public:
  inline uint32_t ReadMapFile() {
    FILE* p_file = nullptr;
    /* Open for read (will fail if file "data" does not exist) */
    fopen_s(&p_file, map_data, "rb");
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

  inline uint32_t GetInputIndex() {
    return input_idx;
  }

  inline uint32_t GetOutputIndex() {
    return output_idx;
  }

  inline dat_t* operator[](uint32_t type) {
    dat_t* ret = nullptr;
    switch (type)
    {
    case IO_ADDR_BASE_INPUT:
      ret = input;
      break;
    case IO_ADDR_BASE_OUTPUT:
      ret = output;
      break;
}
    return ret;
  }

#endif
};




class systemIO :public IIO
{
  /****************************************************
   *	data
   ****************************************************/
public:
  enum class err_e
  {
    SUCCESS,
    TIMEOUT = 1,
  };

  struct cfg_t
  {
    uint32_t base_addr_in{};
    uint32_t base_addr_out{};
  };



  /****************************************************
   *	Constructor
   ****************************************************/
public:
  systemIO(common::Attribute_t common_data, systemIO::cfg_t& cfg);
  virtual ~systemIO();


  /****************************************************
   *	func
   ****************************************************/
private:


public:
  bool IsOn(uint32_t addr);
  bool IsOff(uint32_t addr);
  errno_t OutputOn(uint32_t addr);
  errno_t OutputOff(uint32_t addr);
  errno_t OutputToggle(uint32_t addr);



};

