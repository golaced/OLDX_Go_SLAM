//
// File: eml_rand_shr3cong_stateful.cpp
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 29-Dec-2017 22:25:37
//

// Include Files
#include "rt_nonfinite.h"
#include "AM.h"
#include "eml_rand_shr3cong_stateful.h"
#include "AM_data.h"

// Function Definitions

//
// Arguments    : void
// Return Type  : void
//
void eml_rand_shr3cong_stateful_init()
{
  int i4;
  for (i4 = 0; i4 < 2; i4++) {
    b_state[i4] = 362436069U + 158852560U * i4;
  }
}

//
// File trailer for eml_rand_shr3cong_stateful.cpp
//
// [EOF]
//
