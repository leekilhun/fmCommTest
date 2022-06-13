#pragma once


/*
 * system_log.h
 *
 * 작성자 : 이길훈
 * 22.04.05
 * - system log 관련 기능 구현
 * - file write 기능은 어떤 방식으로 할 지 생각 중...
 */


#ifdef _USE_SYSTEM_CFG_LOG


#ifdef PRJ_CFG_USE_CSTRING
#define SYS_LOG_CFG_STRING  CString
#else
#define SYS_LOG_CFG_STRING  std::string
#endif


#define SYS_LOG_SPACE_LEN               1
#define SYS_LOG_BUFF_LIST_MAX					  4096  
#define SYS_LOG_TXT_LENGTH_MAX 				  256
#define SYS_LOG_ARG_TBL_CNT_MAX 				256

#define SYS_LOG_DATA_LEVEL_STR_MAX 		  16
#define SYS_LOG_DATA_DATE_STR_MAX 			16
#define SYS_LOG_DATA_TIME_STR_MAX 			16
#define SYS_LOG_DATA_MSG_STR_MAX				SYS_LOG_TXT_LENGTH_MAX
#define SYS_LOG_DATA_FILE_STR_MAX			  MAX_PATH
#define SYS_LOG_DATA_FUNC_STR_MAX			  26

#define SYS_LOG_DATE_LENGTH_MAX				  26


  enum class loglevel {
    info,
    err,
    warning,
    _max
  };
#define LOG_LEVEL_INT(x)         static_cast<int>(loglevel::x)



#define __LOGFUNC__				__FUNCTION__
#define SYSLOG_INFO(obj,fmt, ...)		syslog_Put(loglevel::info,obj, (__FILE__), __LOGFUNC__, __LINE__,  fmt, ##__VA_ARGS__)
#define SYSLOG_ERR(obj,fmt, ...)		syslog_Put(loglevel::err,obj, (__FILE__), __LOGFUNC__, __LINE__,  fmt, ##__VA_ARGS__)
#define SYSLOG_WAR(obj,fmt, ...)		syslog_Put(loglevel::warning,obj, (__FILE__), __LOGFUNC__, __LINE__,  fmt, ##__VA_ARGS__)


#define SYSLOG_AVAILABLE(level)     AvailableLog(JLOG_LEVEL_INT(level))

  struct logdat_t 
  {
    char level[SYS_LOG_DATA_LEVEL_STR_MAX]{ 0,};
    char date[SYS_LOG_DATA_DATE_STR_MAX]{ 0, };
    char time[SYS_LOG_DATA_TIME_STR_MAX]{ 0, };
    char message[SYS_LOG_DATA_MSG_STR_MAX]{ 0, };
    char file[SYS_LOG_DATA_FILE_STR_MAX]{ 0, };
    char func[SYS_LOG_DATA_FUNC_STR_MAX]{ 0, };
    uint32_t line_no{};
    uint32_t obj_id{};

    void Init() {
      memset(&level[0], 0x00, SYS_LOG_DATA_LEVEL_STR_MAX);
      memset(&date[0], 0x00, SYS_LOG_DATA_DATE_STR_MAX);
      memset(&time[0], 0x00, SYS_LOG_DATA_TIME_STR_MAX);
      memset(&message[0], 0x00, SYS_LOG_DATA_MSG_STR_MAX);
      memset(&file[0], 0x00, SYS_LOG_DATA_FILE_STR_MAX);
      memset(&func[0], 0x00, SYS_LOG_DATA_FUNC_STR_MAX);
      line_no = 0;
      obj_id = 0;
    }
    int SetLevel(const char* value) {
      return sprintf_s(level, SYS_LOG_DATA_LEVEL_STR_MAX, value);
    }
    int SetDate(const char* value) {
      return sprintf_s(date, SYS_LOG_DATA_DATE_STR_MAX, value);
    }
    int SetTime(const char* value) {
      return sprintf_s(time, SYS_LOG_DATA_TIME_STR_MAX, value);
    }
    int SetMsg(const char* value) {
      return sprintf_s(message, SYS_LOG_DATA_MSG_STR_MAX, value);
    }
    int SetFile(const char* value) {
      return sprintf_s(file, SYS_LOG_DATA_FILE_STR_MAX, value);
    }

    int SetFunc(const char* value) {
      return sprintf_s(func, SYS_LOG_DATA_FUNC_STR_MAX, value);
    }

    int SetObjId(uint32_t id) {
      obj_id = id;
      return 0;
    }

    int SetLineNo(uint32_t _no) {
      line_no = _no;
      return 0;
    }

    void GetLevel(SYS_LOG_CFG_STRING* p_str) {
#ifdef PRJ_CFG_USE_CSTRING 
      * p_str = trans::CharToCString(level);
#else
      p_str->assign(level);
#endif
    }
    void GetDate(SYS_LOG_CFG_STRING* p_str) {
#ifdef PRJ_CFG_USE_CSTRING 
      * p_str = trans::CharToCString(date);
#else
      p_str->assign(date);
#endif
    }
    void GetTime(SYS_LOG_CFG_STRING* p_str) {
#ifdef PRJ_CFG_USE_CSTRING 
      * p_str = trans::CharToCString(time);
#else
      p_str->assign(time);
#endif
    }
    void GetMsg(SYS_LOG_CFG_STRING* p_str) {
#ifdef PRJ_CFG_USE_CSTRING 
      * p_str = trans::CharToCString(message);
#else
      p_str->assign(message);
#endif
    }
    void GetFile(SYS_LOG_CFG_STRING* p_str) {
#ifdef PRJ_CFG_USE_CSTRING 
      * p_str = trans::CharToCString(file);
#else
      p_str->assign(file);
#endif
    }

    uint32_t GetObjid() {
      return obj_id;
    }

    uint32_t GetLineNo() {
      return line_no;
    }

  };


  /* 전역 함수로 구현 */
  static void syslog_Init();
  static void syslog_Del();
  static uint32_t syslog_Put(
    loglevel level,
    const int obj,
    const char* file,
    const char* func,
    const int line,
    const char* fmt, ...);
  static logdat_t syslog_Get(int level = static_cast<int>(loglevel::info));
  static logdat_t syslog_Pop(uint32_t addr, int level = static_cast<int>(loglevel::info));
  static uint32_t syslog_Available(int level = static_cast<int>(loglevel::info));

 
  /* 클래스 객체 */
  class sysLog
  {
    bool m_isInit;
    uint32_t m_size;
    //_que<SYS_LOG_CFG_STRING>* m_queList;
   buffer:: _que<logdat_t>* m_logTable;

  public:
    sysLog(uint32_t size = SYS_LOG_BUFF_LIST_MAX);
    ~sysLog();
  private:
    void initLog();
    void delLog();
  public:
    uint32_t Put(
      loglevel level,
      const int obj,
      const char* file,
      const char* func,
      const int line,
      const char* fmt, ...);

    logdat_t Get(int level = static_cast<int>(loglevel::info));
    logdat_t Pop(uint32_t addr, int level = static_cast<int>(loglevel::info));
    uint32_t Available(int level = static_cast<int>(loglevel::info));
  };






#endif