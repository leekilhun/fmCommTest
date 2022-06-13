#pragma once
class ioFastech:public IIO
{
 /****************************************************
  *	data
  ****************************************************/
public:
  enum class mode_e 
  {
    RS_485,
    ETHERNET,
  };

  enum class err_e
  {
    SUCCESS,
    TIMEOUT = 1,
  };

  struct cfg_t
  {
    ioFastech::mode_e mode{};
    //netSocket::cfg_t net_cfg{};
    //SerialComm::cfg_t serial_cfg{};
    void* p_cfg{ new void* () };
    inline void operator = (const cfg_t* p_cfg) {
      this->mode = p_cfg->mode;
      //this->net_cfg = p_cfg->net_cfg;
      //this->serial_cfg = p_cfg->serial_cfg;
    }

    ~cfg_t() {
      if (p_cfg)
      {
        delete p_cfg;
      }
      p_cfg = nullptr;
    }
  };





  /****************************************************
   *	Constructor
   ****************************************************/
  public:
    ioFastech(ioFastech::cfg_t* cfg);
    virtual ~ioFastech();

  /****************************************************
   *	func
   ****************************************************/



    bool IsOn(uint32_t addr) {
      return false;
    }
    bool IsOff(uint32_t addr) {
      return false;
    }
    errno_t OutputOn(uint32_t addr) {
      return errno_t();
    }
    errno_t OutputOff(uint32_t addr) {
      return errno_t();
    }
    errno_t OutputToggle(uint32_t addr) {
      return errno_t();
    }



};

