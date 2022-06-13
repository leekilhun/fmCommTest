#pragma once

/*
 * system_def.h
 * 작성자 : 이길훈
 * 22.04.04 - system define 및 include
 */

/* 시스템 전역에 접근할 헤더를 include*/
/* 아래 순서대로 절차를 진행 */
/*

 def_prj.h   <-   system_def.h   
                       |
                  system_event.h
                       |
                  system_util.h
                       |
                  system_obj.h
                       |
                  interface_xx.h

*/





#include "def_prj.h"

#include "def_obj.h"

#include "def_err.h"

#include "system_event.h"

#include "system_util.h"

/* system object*/
#include "system_obj.h"
#include "system_timer.h"

/* data object*/
#include "system_dat.h"
#include "system_log.h"

/* interface object*/
// #include "interface/Icommon.h"  -> def_obj.h 헤더에서 include
#include "interface/IComm.h"
#include "interface/IIO.h"
#include "interface/IAxis.h"

 /* hal object*/
#include "_inc/motor_fastech.h"
#include "_inc/io_fastech.h"

#include "system_io.h"

struct prc_step_t
{
  uint8_t curr_step{};
  uint8_t pre_step{};
  uint8_t wait_step{};

  inline void SetStep(uint8_t step) {
    pre_step = curr_step;
    curr_step = step;
  }

  inline uint8_t GetStep() const {
    return curr_step;
  }

};
