#include "pch.h"

#include "system_util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


int scope::constrain(int amt, int low, int high)
{
  return ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)));
}

bool scope::between(int amt, int low, int high)
{
  bool ret = true;
  if (amt < low) {
    ret = false;
  }
  else if (amt > high) {
    ret = false;
  }
  return ret;
}





int conv::DwToInt(uint8_t* bytes)
{
  int a = bytes[0] & 0xFF;
  a |= ((bytes[1] << 8) & 0xFF00);
  a |= ((bytes[2] << 16) & 0xFF0000);
  a |= ((bytes[3] << 24) & 0xFF000000);

  return a;
}

uint32_t conv::DwToUint(uint8_t* bytes)
{
  uint32_t a = bytes[0] & 0xFF;
  a |= ((bytes[1] << 8) & 0xFF00);
  a |= ((bytes[2] << 16) & 0xFF0000);
  a |= ((bytes[3] << 24) & 0xFF000000);

  return a;
}

void file::CurrDir(char* p_buff, int len)
{
  char* ret = _getcwd(p_buff, len);
}

int file::MakeDir(const char* dir_name)
{
  return _mkdir(dir_name);
}

int file::DeleteDir(const char* dir_name)
{
  return _rmdir(dir_name);
}

bool file::isFile(_finddata_t fd)
{
  if (fd.attrib & _A_SUBDIR)
    return false;
  return true;
}

bool file::isDir(_finddata_t fd)
{
  if (fd.attrib & _A_SUBDIR)
    return true;
  return false;
}

bool file::isDirExists(const char* dir)
{
  return (_access_s(dir, 0) == 0 ? true : false);
}

bool file::isFileExists(const char* file)
{
  return (_access_s(file, 0) == 0 ? true : false);
}

int file::SearchDir(const char* dir_str)
{
  std::string dir_name = dir_str;
  std::string path = dir_name + "\\*.*";
  int ret = 0;
  //path.append("\\*.*");
  struct _finddata_t fd;//디렉토리 내 파일 및 폴더 정보 저장 객체
  intptr_t handle;
  list<_finddata_t> fd_list;//디렉토리, 파일 정보 객체 리스트

  if ((handle = _findfirst(path.c_str(), &fd)) == -1L) //fd 구조체 초기화.
  {
    //파일이나 디렉토리가 없을 경우.
    TRACE("No file in directory!");
    goto returnOut;
  }

  do //폴더 탐색 반복 시작
  {
    bool is_file = isFile(fd);//현재 객체 종류 확인(파일 or 디렉토리)
    if (is_file == false && fd.name[0] != '.') {
      //디렉토리일 때의 로직        
      ret += SearchDir((dir_name + "\\" + fd.name).c_str());//재귀적 호출
    }
    else if (is_file == true && fd.name[0] != '.') {
      //파일일 때의 로직       
      fd_list.push_back(fd);
    }

  } while (_findnext(handle, &fd) == 0);
  ret += (int)fd_list.size();
returnOut:
  _findclose(handle);
  return ret;
}

int file::GetFileCount(const char* dir_str, bool inc_dir)
{
  std::string dir_name = dir_str;
  std::string path = dir_name + "\\*.*";
  int ret = 0;
  struct _finddata_t fd;//디렉토리 내 파일 및 폴더 정보 저장 객체
  intptr_t handle;
  if ((handle = _findfirst(path.c_str(), &fd)) == -1L) {
    //파일이나 디렉토리가 없을 경우.
    TRACE("No file in directory!");
    goto returnOut;
  }

  do //폴더 탐색 반복 시작
  {
    bool is_file = isFile(fd);//현재 객체 종류 확인(파일 or 디렉토리)
    if (is_file == false && fd.name[0] != '.') {
      //디렉토리일 때의 로직        
      if (inc_dir) {
        ret += GetFileCount((dir_name + "\\" + fd.name).c_str(), inc_dir); //재귀적 호출
      }
    }
    else if (is_file == true && fd.name[0] != '.') {
      //파일일 때의 로직  
      ret++;
    }

  } while (_findnext(handle, &fd) == 0);
returnOut:
  _findclose(handle);

  return ret;
}

bool file::isLeafDir(const char* dir_str)
{
  std::string dir_name = dir_str;
  std::string path = dir_name + "\\*.*";
  struct _finddata_t fd;//디렉토리 내 파일 및 폴더 정보 저장 객체
  intptr_t handle;
  bool ret = false;

  //fd 구조체 초기화.
  if ((handle = _findfirst(path.c_str(), &fd)) == -1L) {
    //파일이나 디렉토리가 없을 경우.
    TRACE("No file in directory!");
    ret = true;
    goto returnOut;
  }

  do //폴더 탐색 반복 시작
  {
    bool is_dir = isDir(fd);
    if (is_dir && fd.name[0] != '.') {
      goto returnOut;
    }
  } while (_findnext(handle, &fd) == 0);
  ret = true;
returnOut:
  _findclose(handle);
  return ret;
}

int file::DeleteFilesInDir(const char* dir_str, bool inc_dir)
{
  std::string dir_name = dir_str;
  std::string path = dir_name + "\\*.*";
  int ret = 0;
  struct _finddata_t fd;//디렉토리 내 파일 및 폴더 정보 저장 객체
  intptr_t handle;
  if ((handle = _findfirst(path.c_str(), &fd)) == -1L) {
    //파일이나 디렉토리가 없을 경우.
    TRACE("No file in directory!");
    goto returnOut;
  }

  do //폴더 탐색 반복 시작
  {
    bool is_file = isFile(fd);//현재 객체 종류 확인(파일 or 디렉토리)
    if (is_file == false && fd.name[0] != '.') {
      //디렉토리일 때의 로직        
      if (inc_dir) {
        ret += DeleteFilesInDir((dir_name + "\\" + fd.name).c_str(), inc_dir); //재귀적 호출
      }
    }
    else if (is_file == true && fd.name[0] != '.') {
      //파일일 때의 로직  
      remove((dir_name + "\\" + fd.name).c_str());
      ret++;
    }

  } while (_findnext(handle, &fd) == 0);
returnOut:
  _findclose(handle);

  return ret;
}

errno_t file::CopyFilesInDir_binary(const char* dst_dir, const char* src_dir, bool over_write, bool inc_dir)
{
  std::string _src_path = src_dir;
  _src_path += "\\*.*";
  struct _finddata_t fd;
  intptr_t handle;
  FILE* src_file = {}, * dst_file = {};
  errno_t err = FILE_ERR_SUCCESS;

  std::string _dst_path = dst_dir;

  if ((handle = _findfirst(_src_path.c_str(), &fd)) == -1L) {
    //소스 디렉토리에 파일이나 디렉토리가 없을 경우.
    TRACE("No file in directory!");
    err = FILE_ERR_NO_FILE_IN_DIR;
    goto returnOut;
  }

  _src_path = src_dir;
  do //폴더 탐색 반복 시작
  {
    bool is_file = isFile(fd);//현재 객체 종류 확인(파일 or 디렉토리)

    if (fd.name[0] == '.')
      continue;

    if (is_file) {
      // 파일 카피 
      if (FILE_ERR_SUCCESS == (fopen_s(&src_file, (_src_path + "\\" + fd.name).c_str(), "rb"))) {
        // 파일의 마지막 위치로 이동한다. 
        fseek(src_file, 0, SEEK_END);
        // 마지막 위치에서 위치 값을 얻어서 파일의 크기를 계산한다.
        long pos = ftell(src_file);
        // 파일의 처음 위치로 복귀한다.
        char* buff = new char[size_t(pos) + 1];
        fseek(src_file, 0, SEEK_SET);
        // 파일의 크기만큼 데이터를 읽는다.
        fread(buff, pos, 1, src_file);
        fclose(src_file);

        if (isDirExists(_dst_path.c_str()) == false) {
          MakeDir((_dst_path /*+ "\\" + fd.name*/).c_str());
        }

        // 같은 파일이 있으면
        if (isFileExists((_dst_path + "\\" + fd.name).c_str())) {
          if (over_write) {
            if (FILE_ERR_SUCCESS == (fopen_s(&dst_file, (_dst_path + "\\" + fd.name).c_str(), "wb"))) {
              // 대상 파일 만들기
              fwrite((const void*)buff, sizeof(char), size_t(pos) + 1, dst_file);
              fclose(dst_file);
            }
          }
        }
        else {
          if (FILE_ERR_SUCCESS == (fopen_s(&dst_file, (_dst_path + "\\" + fd.name).c_str(), "wb"))) {
            // 대상 파일 만들기
            //fprintf(dst_file, buff);
            fwrite((const void*)buff, sizeof(char), size_t(pos) + 1, dst_file);
            fclose(dst_file);
          }
        }

        delete[] buff;
        buff = nullptr;
      }
    }
    else {
      if (inc_dir) {
        if (isDirExists(_dst_path.c_str()) == false) {
          MakeDir((_dst_path /*+ "\\" + fd.name*/).c_str());
        }

        CopyFilesInDir_binary((_dst_path + "\\" + fd.name).c_str()
          , (_src_path + "\\" + fd.name).c_str()
          , over_write
          , inc_dir);
      }
    }

  } while (_findnext(handle, &fd) == 0);

returnOut:
  _findclose(handle);

  return err;
}

uint8_t trans::SplitArgs(char* arg_str, char** argv, char* delim_chars, int max)
{
  uint8_t argc = 0;
  char* tok = nullptr;
  char* next_ptr = nullptr;

  for (tok = strtok_s(arg_str, delim_chars, &next_ptr); tok; tok = strtok_s(NULL, delim_chars, &next_ptr))
  {
    argv[argc++] = tok;
    if (argc >= max)
      return argc - 1;
  }
  return argc;
}

void trans::NowStr(char* p_str, int max_len)
{
  SYSTEMTIME st;
  GetLocalTime(&st);

  sprintf_s(p_str, max_len, "[%04d/%02d/%02d] %02d:%02d:%02d (%02dmsec)\n"
    , st.wYear
    , st.wMonth
    , st.wDay
    , st.wHour
    , st.wMinute
    , st.wSecond
    , st.wMilliseconds);

}


void trans::DateFormStr(char* p_str, int length, DataTimeFormat_e type)
{
  time_t timetClock;
  struct tm tmNewTime;

  // Get time in seconds 
  time(&timetClock);
  // Convert time to struct  tm form 
  localtime_s(&tmNewTime, &timetClock);
  switch (type)
  {
  case DataTimeFormat_e::YYYYMMDD_HHMMSS_space:
  {
    sprintf_s(p_str, length, "%04d-%02d-%02d %02d:%02d:%02d "
      , tmNewTime.tm_year + 1900
      , tmNewTime.tm_mon + 1
      , tmNewTime.tm_mday
      , tmNewTime.tm_hour
      , tmNewTime.tm_min
      , tmNewTime.tm_sec
    );
  }
  break;
  case DataTimeFormat_e::YYYYMMDD_HHMMSS_square_brackets:
  default:
  {
    sprintf_s(p_str, length, "[%04d-%02d-%02d] [%02d:%02d:%02d] "
      , tmNewTime.tm_year + 1900
      , tmNewTime.tm_mon + 1
      , tmNewTime.tm_mday
      , tmNewTime.tm_hour
      , tmNewTime.tm_min
      , tmNewTime.tm_sec
    );
  }
  break;
  }

}

void trans::Left(const char* pdata, char* pdest, int size, int count)
{
  count = scope::constrain(count, 0, (int)strlen(pdata));
  int i = 0;
  for (; i < count; i++) {
    pdest[i] = pdata[i];
  }
  pdest[i] = '\0';
}

void trans::Replace(char* src, char find, bool upcase, char replace)
{
  size_t size = strlen(src);
  size_t pos = 0;
  size_t idx = 0;
  char tmp = 0x0;
  while (size) {
    tmp = src[pos];
    src[pos] = 0x00;
    if (tmp == find) {
      if (replace != ' ') {
        src[idx++] = replace;
      }
    }
    else {
      if (upcase)
        src[idx++] = toupper(tmp);
      else
        src[idx++] = tmp;
    }
    pos++;
    size--;
  }
}


#ifdef _AFX

CString trans::CharToCString(char* str)
{
  CString cStr;
#if defined(UNICODE) || defined(_UNICODE)
  int nLen = (int)strlen(str) + 1;
  TCHAR* tszTemp = nullptr;
  tszTemp = new TCHAR[nLen];
  memset(tszTemp, 0x00, nLen * sizeof(TCHAR));
  MultiByteToWideChar(
    CP_ACP,
    0,
    str,
    -1,
    tszTemp,
    nLen
  );

  cStr.Format(_T("%s"), tszTemp);
  if (tszTemp)
  {
    delete[] tszTemp;
    tszTemp = NULL;
  }
#else
  cStr.Format("%s", str);
#endif
  return cStr;
}

CString trans::TcharToCString(TCHAR* str)
{
  CString cStr;
  cStr.Format(_T("%s"), str);
  return cStr;
}

#endif




int trans::CharToLPTSTR(LPSTR str, LPTSTR ret_str, int length)
{
  if (strlen(str) == 0)
    return 0;
  if (_tcslen(ret_str) != 0)
    memset(ret_str, 0x00, length);   /* _tcslen(ret_str) * sizeof(TCHAR));*/
  int ret = 0;

#if defined(UNICODE) || defined(_UNICODE)
  //int nLen = (int)_tcslen(ret_str) * (int)sizeof(TCHAR);
  ret = MultiByteToWideChar(
    CP_ACP,
    MB_PRECOMPOSED,
    str,
    (int)strlen(str),
    ret_str,
    length
  );
#else
  int nLen = strlen(str) + 1;
  _tcscpy(ret_str, str);
  ret = nLen;
#endif
  return ret;
}

int trans::TcharToLPSTR(LPTSTR str, LPSTR ret_str, int length)
{
  int ret = 0;
  if (_tcslen(str) == 0)
    return 0;
  if (length != 0)
    memset(ret_str, 0x00, length * sizeof(CHAR));

#if defined(UNICODE) || defined(_UNICODE)
  ret = WideCharToMultiByte(
    CP_ACP,
    0,
    str,
    -1,
    ret_str,
    length + 1,
    NULL,
    NULL
  );
#else
  strcpy(ret_str, str);

#endif
  return ret;
}

bool compare::IsEqualStr(char* str_rf, char* str_cp, bool is_ignore)
{
  bool ret = true;
  char tmp_1, tmp_2 = 0x0;
  size_t size1 = strlen(str_rf);
  size_t size2 = strlen(str_cp);
  size_t idx = 0;
  if (size1 == size2) {
    while (size1) {
      tmp_1 = str_rf[idx];
      tmp_2 = str_rf[idx++];
      if (is_ignore) {
        if (toupper(tmp_1) != toupper(tmp_2)) {
          ret = false;
          break;
        }
      }
      else {
        if (tmp_1 != tmp_2) {
          ret = false;
          break;
        }
      }
      size1--;
    }
  }
  else
    ret = false;

  /*
  if (strcmp(str1, str2) == 0) {
    ret = true;
  }*/

  return ret;
}
