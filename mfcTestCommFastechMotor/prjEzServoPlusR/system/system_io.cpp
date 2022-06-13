#include "pch.h"
#include "system_def.h"
#include "system_io.h"
#include <vector>
#include "_inc/io_fastech.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


systemIO::systemIO(common::Attribute_t common_data, systemIO::cfg_t& cfg)
  :Icommon(common_data)
{
}

systemIO::~systemIO()
{
}

bool systemIO::IsOn(uint32_t addr)
{
  return false;
}

bool systemIO::IsOff(uint32_t addr)
{
  return false;
}

errno_t systemIO::OutputOn(uint32_t addr)
{
  return errno_t();
}

errno_t systemIO::OutputOff(uint32_t addr)
{
  return errno_t();
}

errno_t systemIO::OutputToggle(uint32_t addr)
{
  return errno_t();
}
