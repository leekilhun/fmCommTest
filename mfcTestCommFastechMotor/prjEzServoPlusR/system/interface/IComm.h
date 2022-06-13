#pragma once

#define ICOMM_CMD_MAX_DATA_LENGTH       256

class IComm :public virtual Icommon
{
public:
  virtual errno_t OpenPort() = 0;
  virtual void ClosePort() = 0;
  virtual bool IsConnected() = 0;
  virtual uint32_t SendReceiveData() = 0;

  virtual errno_t SendCmd(uint8_t cmd, uint8_t* p_data, uint8_t length) = 0;
  virtual errno_t SendResp(uint8_t cmd, uint8_t* p_data, uint8_t length) = 0;
  virtual errno_t SendCmdRxResp(uint8_t cmd, uint8_t* p_data, uint8_t length) = 0;

  virtual uint32_t ReadByte(uint8_t* resp, uint32_t len) = 0;
  virtual uint32_t WriteByte(uint8_t* send, uint32_t len) = 0;
  virtual void AttCallbackFunc(void* obj, evt_cb cb) = 0;


};