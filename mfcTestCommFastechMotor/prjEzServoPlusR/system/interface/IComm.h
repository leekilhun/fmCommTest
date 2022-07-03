#pragma once

#define ICOMM_CMD_MAX_DATA_LENGTH       256

class IComm :public virtual Icommon
{
public:
  virtual errno_t OpenPort() = 0;
  virtual void ClosePort() = 0;
  virtual bool IsOpened() = 0;
  virtual void Recovery() = 0;
  virtual void AttCallbackFunc(void* obj, evt_cb cb) = 0;


};