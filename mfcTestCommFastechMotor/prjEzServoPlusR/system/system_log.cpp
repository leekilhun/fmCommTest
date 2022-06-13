#include "pch.h"
#include "system_def.h"
#include "system_log.h"

#include <mutex>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace buffer;


/* 전역 함수로 구현 */
_que<logdat_t> log_table[static_cast<int>(loglevel::_max)]{SYS_LOG_BUFF_LIST_MAX, };
bool is_init = false;

void syslog_Init()
{
  is_init = true;
  //TODO:: 초기화 타임에 뭘 해야하지?
}

void syslog_Del()
{
  //TODO:: 삭제 기능 ???
}

uint32_t syslog_Put(loglevel level, const int obj, const char* file, const char* func, const int line, const char* fmt, ...)
{
  uint32_t ret = 0;
  if (is_init != true) return ret;

  mutex _mutex;
  logdat_t data;

  _mutex.lock();
  va_list args;
  va_start(args, fmt);

  int len = vsnprintf(nullptr, 0, fmt, args);
  if (len < 0) {
    va_end(args);
    _mutex.unlock();
    return ret;
  }
  else if (len > 0)
  {
    //obj id
    data.SetObjId((uint32_t)obj);
    //obj id
    data.SetLineNo((uint32_t)line);
    //file
    size_t size = strnlen_s(file, SYS_LOG_DATA_FILE_STR_MAX) + 1;
    char tmp_str[SYS_LOG_DATA_FILE_STR_MAX] = { 0, };
    strcpy_s(tmp_str, (size % SYS_LOG_DATA_FILE_STR_MAX), file);
    char* arg_tbl[SYS_LOG_ARG_TBL_CNT_MAX] = {};
    uint8_t arg_count = trans::SplitArgs(tmp_str, arg_tbl, "\\", SYS_LOG_ARG_TBL_CNT_MAX);
    if (arg_tbl[arg_count - 1] != 0) {
      data.SetFile(arg_tbl[arg_count - 1]);
    }
    //func
    data.SetFunc(func);
    //date, time
    char datetime[SYS_LOG_DATE_LENGTH_MAX] = { 0, };
    trans::DateFormStr(&datetime[0], SYS_LOG_DATE_LENGTH_MAX);
    arg_count = trans::SplitArgs(&datetime[0], arg_tbl, " ", SYS_LOG_ARG_TBL_CNT_MAX);
    if (arg_count > 1) {
      if (arg_tbl[0] != 0) {
        data.SetDate(arg_tbl[0]);
      }
      if (arg_tbl[1] != 0) {
        data.SetTime(arg_tbl[1]);
      }
    }

    uint32_t vec_len = (uint32_t)len + SYS_LOG_DATA_DATE_STR_MAX + 1;
    vector<char> buffer(vec_len);
    len = vsnprintf(&buffer[0], buffer.size(), fmt, args);
    //result = strcpy_s(p_data->message, LOG_MANAGE_DATA_MSG_STR_MAX, &buffer[0]);
    data.SetMsg(&buffer[0]);

    switch (level)
    {
    case loglevel::err:
      data.SetLevel("[ERR]");
      ret = log_table[static_cast<int>(loglevel::err)].Put(data);
      break;
    case loglevel::warning:
      data.SetLevel("[WAR]");
      ret = log_table[static_cast<int>(loglevel::warning)].Put(data);
      break;
    default: // info
      data.SetLevel("[INF]");
      ret = log_table[static_cast<int>(loglevel::info)].Put(data);
      break;
    }
  }

  va_end(args);
  _mutex.unlock();

  return ret;
}

logdat_t syslog_Get(int level)
{
  logdat_t ret;
  switch (level)
  {
  case static_cast<int>(loglevel::err):
  {
    log_table[static_cast<int>(loglevel::err)].Get(&ret);
  }
  break;
  case static_cast<int>(loglevel::warning):
  {
    log_table[static_cast<int>(loglevel::warning)].Get(&ret);
  }
  break;
  default:
  {
    log_table[static_cast<int>(loglevel::info)].Get(&ret);
  }
  break;
  }
  return ret;
}

logdat_t syslog_Pop(uint32_t addr, int level)
{
  logdat_t ret;
  switch (level)
  {
  case static_cast<int>(loglevel::err):
  {
    log_table[static_cast<int>(loglevel::err)].Pop(addr, &ret);
  }
  break;
  case static_cast<int>(loglevel::warning):
  {
    log_table[static_cast<int>(loglevel::warning)].Pop(addr, &ret);
  }
  break;
  default:
  {
    log_table[static_cast<int>(loglevel::info)].Pop(addr, &ret);
  }
  break;
  }
  return ret;
}

uint32_t syslog_Available(int level)
{
  uint32_t ret;
  switch (level)
  {
  case static_cast<int>(loglevel::err):
  {
    ret = log_table[static_cast<int>(loglevel::err)].Available();
  }
  break;
  case static_cast<int>(loglevel::warning):
  {
    ret = log_table[static_cast<int>(loglevel::warning)].Available();
  }
  break;
  default:
  {
    ret = log_table[static_cast<int>(loglevel::info)].Available();
  }
  break;
  }
  return ret;
}




sysLog::sysLog(uint32_t size):m_isInit(false),m_size(size), m_logTable(nullptr)
{
  m_logTable = new _que<logdat_t>[static_cast<int>(loglevel::_max)]{ size, };
}

sysLog::~sysLog()
{
  if (m_logTable)
  {
    delete m_logTable;
  }
  m_logTable = nullptr;
}

void sysLog::initLog()
{
  //TODO:: 초기화 기능은 뭘 해야하지?
}

void sysLog::delLog()
{
  //TODO:: 삭제 기능 ???
}

uint32_t sysLog::Put(loglevel level, const int obj, const char* file, const char* func, const int line, const char* fmt, ...)
{
  uint32_t ret = 0;
  if (is_init != true) return ret;

  mutex _mutex;
  logdat_t data;

  _mutex.lock();
  va_list args;
  va_start(args, fmt);

  int len = vsnprintf(nullptr, 0, fmt, args);
  if (len < 0) {
    va_end(args);
    _mutex.unlock();
    return ret;
  }
  else if (len > 0)
  {
    //obj id
    data.SetObjId((uint32_t)obj);
    //obj id
    data.SetLineNo((uint32_t)line);
    //file
    size_t size = strnlen_s(file, SYS_LOG_DATA_FILE_STR_MAX) + 1;
    char tmp_str[SYS_LOG_DATA_FILE_STR_MAX] = { 0, };
    strcpy_s(tmp_str, (size % SYS_LOG_DATA_FILE_STR_MAX), file);
    char* arg_tbl[SYS_LOG_ARG_TBL_CNT_MAX] = {};
    uint8_t arg_count = trans::SplitArgs(tmp_str, arg_tbl, "\\", SYS_LOG_ARG_TBL_CNT_MAX);
    if (arg_tbl[arg_count - 1] != 0) {
      data.SetFile(arg_tbl[arg_count - 1]);
    }
    //func
    data.SetFunc(func);
    //date, time
    char datetime[SYS_LOG_DATE_LENGTH_MAX] = { 0, };
    trans::DateFormStr(&datetime[0], SYS_LOG_DATE_LENGTH_MAX);
    arg_count = trans::SplitArgs(&datetime[0], arg_tbl, " ", SYS_LOG_ARG_TBL_CNT_MAX);
    if (arg_count > 1) {
      if (arg_tbl[0] != 0) {
        data.SetDate(arg_tbl[0]);
      }
      if (arg_tbl[1] != 0) {
        data.SetTime(arg_tbl[1]);
      }
    }

    uint32_t vec_len = (uint32_t)len + SYS_LOG_DATA_DATE_STR_MAX + 1;
    vector<char> buffer(vec_len);
    len = vsnprintf(&buffer[0], buffer.size(), fmt, args);
    //result = strcpy_s(p_data->message, LOG_MANAGE_DATA_MSG_STR_MAX, &buffer[0]);
    data.SetMsg(&buffer[0]);

    switch (level)
    {
    case loglevel::err:
      data.SetLevel("[ERR]");
      ret = m_logTable[static_cast<int>(loglevel::err)].Put(data);
      break;
    case loglevel::warning:
      data.SetLevel("[WAR]");
      ret = m_logTable[static_cast<int>(loglevel::warning)].Put(data);
      break;
    default: // info
      data.SetLevel("[INF]");
      ret = m_logTable[static_cast<int>(loglevel::info)].Put(data);
      break;
    }
  }

  va_end(args);
  _mutex.unlock();

  return ret;
}

logdat_t sysLog::Get(int level)
{
  return logdat_t();
}

logdat_t sysLog::Pop(uint32_t addr, int level)
{
  return logdat_t();
}

uint32_t sysLog::Available(int level)
{
  return uint32_t();
}


