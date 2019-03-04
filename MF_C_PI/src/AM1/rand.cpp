//
// File: rand.cpp
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 29-Dec-2017 22:25:37
//

// Include Files
#include "rt_nonfinite.h"
#include "AM.h"
#include "rand.h"
#include "AM_data.h"

// Variable Definitions
static unsigned int c_state[625];

// Function Declarations
static double eml_rand_mt19937ar(unsigned int d_state[625]);
static void twister_state_vector(unsigned int mt[625], double seed);

// Function Definitions

//
// Arguments    : unsigned int d_state[625]
// Return Type  : double
//
static double eml_rand_mt19937ar(unsigned int d_state[625])
{
  double r;
  int exitg1;
  int j;
  unsigned int u[2];
  unsigned int mti;
  int kk;
  unsigned int y;
  unsigned int b_y;
  unsigned int c_y;
  unsigned int d_y;
  boolean_T isvalid;
  boolean_T exitg2;

  // ========================= COPYRIGHT NOTICE ============================
  //  This is a uniform (0,1) pseudorandom number generator based on:
  //
  //  A C-program for MT19937, with initialization improved 2002/1/26.
  //  Coded by Takuji Nishimura and Makoto Matsumoto.
  //
  //  Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
  //  All rights reserved.
  //
  //  Redistribution and use in source and binary forms, with or without
  //  modification, are permitted provided that the following conditions
  //  are met:
  //
  //    1. Redistributions of source code must retain the above copyright
  //       notice, this list of conditions and the following disclaimer.
  //
  //    2. Redistributions in binary form must reproduce the above copyright
  //       notice, this list of conditions and the following disclaimer
  //       in the documentation and/or other materials provided with the
  //       distribution.
  //
  //    3. The names of its contributors may not be used to endorse or
  //       promote products derived from this software without specific
  //       prior written permission.
  //
  //  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  //  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  //  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  //  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
  //  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  //  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  //  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  //  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  //  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  //  (INCLUDING  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  //  OF THIS  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  //
  // =============================   END   =================================
  do {
    exitg1 = 0;
    for (j = 0; j < 2; j++) {
      u[j] = 0U;
      mti = d_state[624] + 1U;
      if (mti >= 625U) {
        for (kk = 0; kk < 227; kk++) {
          y = (d_state[kk] & 2147483648U) | (d_state[1 + kk] & 2147483647U);
          if ((int)(y & 1U) == 0) {
            b_y = y >> 1U;
          } else {
            b_y = y >> 1U ^ 2567483615U;
          }

          d_state[kk] = d_state[397 + kk] ^ b_y;
        }

        for (kk = 0; kk < 396; kk++) {
          y = (d_state[kk + 227] & 2147483648U) | (d_state[228 + kk] &
            2147483647U);
          if ((int)(y & 1U) == 0) {
            c_y = y >> 1U;
          } else {
            c_y = y >> 1U ^ 2567483615U;
          }

          d_state[kk + 227] = d_state[kk] ^ c_y;
        }

        y = (d_state[623] & 2147483648U) | (d_state[0] & 2147483647U);
        if ((int)(y & 1U) == 0) {
          d_y = y >> 1U;
        } else {
          d_y = y >> 1U ^ 2567483615U;
        }

        d_state[623] = d_state[396] ^ d_y;
        mti = 1U;
      }

      y = d_state[(int)mti - 1];
      d_state[624] = mti;
      y ^= y >> 11U;
      y ^= y << 7U & 2636928640U;
      y ^= y << 15U & 4022730752U;
      y ^= y >> 18U;
      u[j] = y;
    }

    r = 1.1102230246251565E-16 * ((double)(u[0] >> 5U) * 6.7108864E+7 + (double)
      (u[1] >> 6U));
    if (r == 0.0) {
      if ((d_state[624] >= 1U) && (d_state[624] < 625U)) {
        isvalid = true;
      } else {
        isvalid = false;
      }

      if (isvalid) {
        isvalid = false;
        j = 1;
        exitg2 = false;
        while ((!exitg2) && (j < 625)) {
          if (d_state[j - 1] == 0U) {
            j++;
          } else {
            isvalid = true;
            exitg2 = true;
          }
        }
      }

      if (!isvalid) {
        twister_state_vector(d_state, 5489.0);
      }
    } else {
      exitg1 = 1;
    }
  } while (exitg1 == 0);

  return r;
}

//
// Arguments    : unsigned int mt[625]
//                double seed
// Return Type  : void
//
static void twister_state_vector(unsigned int mt[625], double seed)
{
  unsigned int r;
  int mti;
  if (seed < 4.294967296E+9) {
    if (seed >= 0.0) {
      r = (unsigned int)seed;
    } else {
      r = 0U;
    }
  } else if (seed >= 4.294967296E+9) {
    r = MAX_uint32_T;
  } else {
    r = 0U;
  }

  mt[0] = r;
  for (mti = 0; mti < 623; mti++) {
    r = (r ^ r >> 30U) * 1812433253U + (1 + mti);
    mt[mti + 1] = r;
  }

  mt[624] = 624U;
}

//
// Arguments    : void
// Return Type  : double
//
double b_rand()
{
  double r;
  int hi;
  unsigned int test1;
  unsigned int test2;
  if (method == 4U) {
    hi = (int)(state / 127773U);
    test1 = 16807U * (state - hi * 127773U);
    test2 = 2836U * hi;
    if (test1 < test2) {
      state = (test1 - test2) + 2147483647U;
    } else {
      state = test1 - test2;
    }

    r = (double)state * 4.6566128752457969E-10;
  } else if (method == 5U) {
    test1 = 69069U * b_state[0] + 1234567U;
    test2 = b_state[1] ^ b_state[1] << 13;
    test2 ^= test2 >> 17;
    test2 ^= test2 << 5;
    b_state[0] = test1;
    b_state[1] = test2;
    r = (double)(test1 + test2) * 2.328306436538696E-10;
  } else {
    if (!state_not_empty) {
      memset(&c_state[0], 0, 625U * sizeof(unsigned int));
      twister_state_vector(c_state, 5489.0);
      state_not_empty = true;
    }

    r = eml_rand_mt19937ar(c_state);
  }

  return r;
}

//
// File trailer for rand.cpp
//
// [EOF]
//
