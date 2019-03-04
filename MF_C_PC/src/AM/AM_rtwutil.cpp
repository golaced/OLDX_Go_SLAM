//
// File: AM_rtwutil.cpp
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 29-Dec-2017 22:25:37
//

// Include Files
#include "rt_nonfinite.h"
#include "AM.h"
#include "AM_rtwutil.h"
#include "cosd.h"

// Function Definitions

//
// Arguments    : double u0
//                double u1
// Return Type  : double
//
double rt_remd_snf(double u0, double u1)
{
  double y;
  double b_u1;
  double tr;
  if (!((!rtIsNaN(u0)) && (!rtIsInf(u0)) && ((!rtIsNaN(u1)) && (!rtIsInf(u1)))))
  {
    y = rtNaN;
  } else {
    if (u1 < 0.0) {
      b_u1 = ceil(u1);
    } else {
      b_u1 = floor(u1);
    }

    if ((u1 != 0.0) && (u1 != b_u1)) {
      tr = u0 / u1;
      if (fabs(tr - rt_roundd_snf(tr)) <= DBL_EPSILON * fabs(tr)) {
        y = 0.0;
      } else {
        y = fmod(u0, u1);
      }
    } else {
      y = fmod(u0, u1);
    }
  }

  return y;
}

//
// Arguments    : float u0
//                float u1
// Return Type  : float
//
float rt_remf_snf(float u0, float u1)
{
  float y;
  float b_u1;
  float tr;
  if (!((!rtIsNaNF(u0)) && (!rtIsInfF(u0)) && ((!rtIsNaNF(u1)) && (!rtIsInfF(u1)))))
  {
    y = ((real32_T)rtNaN);
  } else {
    if (u1 < 0.0F) {
      b_u1 = (float)ceil((double)u1);
    } else {
      b_u1 = (float)floor((double)u1);
    }

    if ((u1 != 0.0F) && (u1 != b_u1)) {
      tr = u0 / u1;
      if ((float)fabs((double)(tr - rt_roundf_snf(tr))) <= FLT_EPSILON * (float)
          fabs((double)tr)) {
        y = 0.0F;
      } else {
        y = (float)fmod((double)u0, (double)u1);
      }
    } else {
      y = (float)fmod((double)u0, (double)u1);
    }
  }

  return y;
}

//
// Arguments    : double u
// Return Type  : double
//
double rt_roundd_snf(double u)
{
  double y;
  if (fabs(u) < 4.503599627370496E+15) {
    if (u >= 0.5) {
      y = floor(u + 0.5);
    } else if (u > -0.5) {
      y = u * 0.0;
    } else {
      y = ceil(u - 0.5);
    }
  } else {
    y = u;
  }

  return y;
}

//
// Arguments    : float u
// Return Type  : float
//
float rt_roundf_snf(float u)
{
  float y;
  if ((float)fabs((double)u) < 8.388608E+6F) {
    if (u >= 0.5F) {
      y = (float)floor((double)(u + 0.5F));
    } else if (u > -0.5F) {
      y = u * 0.0F;
    } else {
      y = (float)ceil((double)(u - 0.5F));
    }
  } else {
    y = u;
  }

  return y;
}

//
// File trailer for AM_rtwutil.cpp
//
// [EOF]
//
