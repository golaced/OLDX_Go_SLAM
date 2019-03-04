//
// File: sind.cpp
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 29-Dec-2017 22:25:37
//

// Include Files
#include "rt_nonfinite.h"
#include "AM.h"
#include "sind.h"
#include "cosd.h"
#include "AM_rtwutil.h"

// Function Definitions

//
// Arguments    : double *x
// Return Type  : void
//
void b_sind(double *x)
{
  double absx;
  signed char n;
  if (!((!rtIsInf(*x)) && (!rtIsNaN(*x)))) {
    *x = rtNaN;
  } else {
    *x = rt_remd_snf(*x, 360.0);
    absx = fabs(*x);
    if (absx > 180.0) {
      if (*x > 0.0) {
        *x -= 360.0;
      } else {
        *x += 360.0;
      }

      absx = fabs(*x);
    }

    if (absx <= 45.0) {
      *x *= 0.017453292519943295;
      n = 0;
    } else if (absx <= 135.0) {
      if (*x > 0.0) {
        *x = 0.017453292519943295 * (*x - 90.0);
        n = 1;
      } else {
        *x = 0.017453292519943295 * (*x + 90.0);
        n = -1;
      }
    } else if (*x > 0.0) {
      *x = 0.017453292519943295 * (*x - 180.0);
      n = 2;
    } else {
      *x = 0.017453292519943295 * (*x + 180.0);
      n = -2;
    }

    if (n == 0) {
      *x = sin(*x);
    } else if (n == 1) {
      *x = cos(*x);
    } else if (n == -1) {
      *x = -cos(*x);
    } else {
      *x = -sin(*x);
    }
  }
}

//
// Arguments    : float *x
// Return Type  : void
//
void sind(float *x)
{
  float absx;
  signed char n;
  if (!((!rtIsInfF(*x)) && (!rtIsNaNF(*x)))) {
    *x = ((real32_T)rtNaN);
  } else {
    *x = rt_remf_snf(*x, 360.0F);
    absx = (float)fabs((double)*x);
    if (absx > 180.0F) {
      if (*x > 0.0F) {
        *x -= 360.0F;
      } else {
        *x += 360.0F;
      }

      absx = (float)fabs((double)*x);
    }

    if (absx <= 45.0F) {
      *x *= 0.0174532924F;
      n = 0;
    } else if (absx <= 135.0F) {
      if (*x > 0.0F) {
        *x = 0.0174532924F * (*x - 90.0F);
        n = 1;
      } else {
        *x = 0.0174532924F * (*x + 90.0F);
        n = -1;
      }
    } else if (*x > 0.0F) {
      *x = 0.0174532924F * (*x - 180.0F);
      n = 2;
    } else {
      *x = 0.0174532924F * (*x + 180.0F);
      n = -2;
    }

    if (n == 0) {
      *x = (float)sin((double)*x);
    } else if (n == 1) {
      *x = (float)cos((double)*x);
    } else if (n == -1) {
      *x = -(float)cos((double)*x);
    } else {
      *x = -(float)sin((double)*x);
    }
  }
}

//
// File trailer for sind.cpp
//
// [EOF]
//
