//
// File: AM.cpp
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 29-Dec-2017 22:25:37
//

// Include Files
#include "rt_nonfinite.h"
#include "AM.h"
#include "kmeans.h"
#include "cosd.h"
#include "sind.h"
#include "AM_rtwutil.h"
#include "ukey.h"
#include <iostream>
#include <sstream>
using namespace std;
static int key_right=0;
static char u_key[20];
void  set_key(char key[20])
{
   for(int i=0;i<20;i++)
     u_key[i]=key[i];
   key_right= set_lisence(u_key);
}

// Variable Definitions
static double mark_pos_map_buf[40000];
static boolean_T mark_pos_map_buf_not_empty;
static double mark_buf_cnt[100];
static boolean_T mark_buf_cnt_not_empty;
static double mark_pos_lock_cnt[100];
static boolean_T mark_pos_lock_cnt_not_empty;

// Function Declarations
static float To_180_degrees(float x);
static void cal_center_from_est(const float pos1[4], const double camera1[4],
  const double camera2[4], double mark_center_o[4]);
static void cal_pos_from_map(const float pos[4], const double camera[4], double
  camera_pos_o[4]);

// Function Definitions

//
// Arguments    : float x
// Return Type  : float
//
static float To_180_degrees(float x)
{
  float yaw;
  if (x > 180.0F) {
    yaw = x - 360.0F;
  } else if (x < -180.0F) {
    yaw = x + 360.0F;
  } else {
    yaw = x;
  }

  return yaw;
}

//
// Arguments    : const float pos1[4]
//                const double camera1[4]
//                const double camera2[4]
//                double mark_center_o[4]
// Return Type  : void
//
static void cal_center_from_est(const float pos1[4], const double camera1[4],
  const double camera2[4], double mark_center_o[4])
{
  float x;
  float f0;
  float f1;
  float f2;
  float f3;
  double d0;
  double d1;
  double d2;
  double d3;
  float fv0[4];
  float b_camera1[2];
  float fv1[2];
  float b_pos1[2];
  double dv0[4];
  double b_camera2[2];
  float pos_temp[2];
  int i0;
  int i1;
  x = (pos1[3] + (float)camera1[3]) - (float)camera2[3];
  if (x > 180.0F) {
    x -= 360.0F;
  } else {
    if (x < -180.0F) {
      x += 360.0F;
    }
  }

  mark_center_o[3] = x;
  f0 = pos1[3];
  cosd(&f0);
  f1 = pos1[3];
  sind(&f1);
  f2 = pos1[3];
  sind(&f2);
  f3 = pos1[3];
  cosd(&f3);
  d0 = x;
  b_cosd(&d0);
  d1 = x;
  b_sind(&d1);
  d2 = x;
  b_sind(&d2);
  d3 = x;
  b_cosd(&d3);
  fv0[0] = f0;
  fv0[1] = -f1;
  fv0[2] = f2;
  fv0[3] = f3;
  b_camera1[0] = (float)camera1[0];
  b_camera1[1] = (float)camera1[1];
  b_pos1[0] = pos1[0];
  b_pos1[1] = pos1[1];
  dv0[0] = d0;
  dv0[1] = -d1;
  dv0[2] = d2;
  dv0[3] = d3;
  b_camera2[0] = camera2[0];
  b_camera2[1] = camera2[1];
  for (i0 = 0; i0 < 2; i0++) {
    fv1[i0] = 0.0F;
    d0 = 0.0;
    for (i1 = 0; i1 < 2; i1++) {
      d0 += dv0[i0 + (i1 << 1)] * b_camera2[i1];
      fv1[i0] += fv0[i0 + (i1 << 1)] * b_camera1[i1];
    }

    pos_temp[i0] = (b_pos1[i0] + fv1[i0]) - (float)d0;
  }

  mark_center_o[0] = pos_temp[0];
  mark_center_o[1] = pos_temp[1];
  mark_center_o[2] = camera1[2] / 2.0 + camera2[2] / 2.0;
}

//
// Arguments    : const float pos[4]
//                const double camera[4]
//                double camera_pos_o[4]
// Return Type  : void
//
static void cal_pos_from_map(const float pos[4], const double camera[4], double
  camera_pos_o[4])
{
  float x;
  float f4;
  float f5;
  float f6;
  float fv2[4];
  float b_camera[2];
  float fv3[2];
  float b_pos[2];
  float pos_temp[2];
  int i2;
  int i3;
  x = pos[3] + (float)camera[3];
  if (x > 180.0F) {
    camera_pos_o[3] = x - 360.0F;
  } else if (x < -180.0F) {
    camera_pos_o[3] = x + 360.0F;
  } else {
    camera_pos_o[3] = x;
  }

  x = pos[3];
  cosd(&x);
  f4 = pos[3];
  sind(&f4);
  f5 = pos[3];
  sind(&f5);
  f6 = pos[3];
  cosd(&f6);
  fv2[0] = x;
  fv2[1] = -f4;
  fv2[2] = f5;
  fv2[3] = f6;
  b_camera[0] = (float)camera[0];
  b_camera[1] = (float)camera[1];
  b_pos[0] = pos[0];
  b_pos[1] = pos[1];
  for (i2 = 0; i2 < 2; i2++) {
    fv3[i2] = 0.0F;
    for (i3 = 0; i3 < 2; i3++) {
      fv3[i2] += fv2[i2 + (i3 << 1)] * b_camera[i3];
    }

    pos_temp[i2] = b_pos[i2] + fv3[i2];
  }

  camera_pos_o[0] = pos_temp[0];
  camera_pos_o[1] = pos_temp[1];
  camera_pos_o[2] = camera[2];

  // z
}



//
// input���� x,y,z,yaw,id  ���Ƴߴ� ��ά���̶�����  �̶��жϴ���  ��������
// Arguments    : float map[500]
//                float camera[4]
//                short *first_check_id
//                const float c1[5]
//                const float c2[5]
//                const float c3[5]
//                const float c4[5]
//                const float c5[5]
//                const float c6[5]
//                short cloud_size
//                float mark_pos_ero_check
//                short lock_cnt
//                float center_fix_flt
//                short *init
//                short cal_sel
//                short en_map_fix
//                const short *has_map
// Return Type  : void
//
void AM(float map[500], float camera[4], short *first_check_id, const float c1[5],
        const float c2[5], const float c3[5], const float c4[5], const float c5
        [5], const float c6[5], short cloud_size, float mark_pos_ero_check,
        short lock_cnt, float center_fix_flt, short *init, short cal_sel, short
        en_map_fix, const short *has_map)
{
  int check_num;
  double check_id[6];
  int i;
  double mark[400];
  float a;
  short i5;
  int j;
  float b_map[4];
  double b_mark[4];
  double c_mark[4];
  double camera_sum[4];
  int b_i;
  short p;
  double d4;
  double X[200];
  double unusedU1[200];
  double SumD[2];
  double Ctrs[4];
  double unusedU0[100];
  double temp;
  int k;
  double w;
  float center_fix_flt_temp;
  double mark_pos_map_buf_temp[400];
  int cnt_temp;
  short l;
  float b_a;
  int b_l;
  float c_map[4];
  double d_mark[4];
  double camera_temp[4];
  static int init1;
  if(!key_right){
    if(init1==0){
    init1=1;
    std::cout<<"Lisence Error: Need Ukey from OLDX!!!"<<std::endl;
    }
    goto lise;
  }
  // ������ά���������г���
  // ���Ƴߴ�
  if (cloud_size > 100) {
    cloud_size = 100;
  }

  if (cloud_size == 0) {
    cloud_size = 1;
  }

  // map ����buf
  if (*init != 0) {
    memset(&mark_pos_map_buf[0], 0, 40000U * sizeof(double));
    mark_pos_map_buf_not_empty = true;
  } else {
    if (!mark_pos_map_buf_not_empty) {
      memset(&mark_pos_map_buf[0], 0, 40000U * sizeof(double));
      mark_pos_map_buf_not_empty = true;
    }
  }

  // map ���Ƽ�����
  if (*init != 0) {
    memset(&mark_buf_cnt[0], 0, 100U * sizeof(double));
    mark_buf_cnt_not_empty = true;
  } else {
    if (!mark_buf_cnt_not_empty) {
      memset(&mark_buf_cnt[0], 0, 100U * sizeof(double));
      mark_buf_cnt_not_empty = true;
    }
  }

  // map �̶�������������������
  if (*init != 0) {
    memset(&mark_pos_lock_cnt[0], 0, 100U * sizeof(double));
    mark_pos_lock_cnt_not_empty = true;
    *init = 0;
  } else {
    if (!mark_pos_lock_cnt_not_empty) {
      memset(&mark_pos_lock_cnt[0], 0, 100U * sizeof(double));
      mark_pos_lock_cnt_not_empty = true;
    }
  }

  check_num = -1;
  for (i = 0; i < 6; i++) {
    check_id[i] = 0.0;
  }

  memset(&mark[0], 0, 400U * sizeof(double));

  // x y z yaw
  if (*has_map == 0) {
    if ((c1[4] > 0.0F) && (*first_check_id == 0)) {
      map[(int)c1[4] + 399] = 3.0F;
      a = rt_roundf_snf(c1[4]);
      if (a < 32768.0F) {
        i5 = (short)a;
      } else {
        i5 = MAX_int16_T;
      }

      *first_check_id = i5;
      map[*first_check_id - 1] = 1.0E-6F;
      map[*first_check_id + 99] = 1.0E-6F;
    } else if ((c2[4] > 0.0F) && (*first_check_id == 0)) {
      map[(int)c2[4] + 399] = 3.0F;
      a = rt_roundf_snf(c2[4]);
      if (a < 32768.0F) {
        i5 = (short)a;
      } else {
        i5 = MAX_int16_T;
      }

      *first_check_id = i5;
      map[*first_check_id - 1] = 1.0E-6F;
      map[*first_check_id + 99] = 1.0E-6F;
    } else if ((c3[4] > 0.0F) && (*first_check_id == 0)) {
      map[(int)c3[4] + 399] = 3.0F;
      a = rt_roundf_snf(c3[4]);
      if (a < 32768.0F) {
        i5 = (short)a;
      } else {
        i5 = MAX_int16_T;
      }

      *first_check_id = i5;
      map[*first_check_id - 1] = 1.0E-6F;
      map[*first_check_id + 99] = 1.0E-6F;
    } else if ((c4[4] > 0.0F) && (*first_check_id == 0)) {
      map[(int)c4[4] + 399] = 3.0F;
      a = rt_roundf_snf(c4[4]);
      if (a < 32768.0F) {
        i5 = (short)a;
      } else {
        i5 = MAX_int16_T;
      }

      *first_check_id = i5;
      map[*first_check_id - 1] = 1.0E-6F;
      map[*first_check_id + 99] = 1.0E-6F;
    } else if ((c5[4] > 0.0F) && (*first_check_id == 0)) {
      map[(int)c5[4] + 399] = 3.0F;
      a = rt_roundf_snf(c5[4]);
      if (a < 32768.0F) {
        i5 = (short)a;
      } else {
        i5 = MAX_int16_T;
      }

      *first_check_id = i5;
      map[*first_check_id - 1] = 1.0E-6F;
      map[*first_check_id + 99] = 1.0E-6F;
    } else {
      if ((c6[4] > 0.0F) && (*first_check_id == 0)) {
        map[(int)c6[4] + 399] = 3.0F;
        a = rt_roundf_snf(c6[4]);
        if (a < 32768.0F) {
          i5 = (short)a;
        } else {
          i5 = MAX_int16_T;
        }

        *first_check_id = i5;
        map[*first_check_id - 1] = 1.0E-6F;
        map[*first_check_id + 99] = 1.0E-6F;
      }
    }
  }

  // sample
  if (c1[4] > 0.0F) {
    check_num = 0;
    check_id[0] = c1[4];
    mark[(int)c1[4] - 1] = c1[0];
    mark[(int)c1[4] + 99] = c1[1];
    mark[(int)c1[4] + 199] = c1[2];
    mark[(int)c1[4] + 299] = To_180_degrees(c1[3]);
  }

  if (c2[4] > 0.0F) {
    check_num++;
    check_id[check_num] = c2[4];
    mark[(int)c2[4] - 1] = c2[0];
    mark[(int)c2[4] + 99] = c2[1];
    mark[(int)c2[4] + 199] = c2[2];
    mark[(int)c2[4] + 299] = To_180_degrees(c2[3]);
  }

  if (c3[4] > 0.0F) {
    check_num++;
    check_id[check_num] = c3[4];
    mark[(int)c3[4] - 1] = c3[0];
    mark[(int)c3[4] + 99] = c3[1];
    mark[(int)c3[4] + 199] = c3[2];
    mark[(int)c3[4] + 299] = To_180_degrees(c3[3]);
  }

  if (c4[4] > 0.0F) {
    check_num++;
    check_id[check_num] = c4[4];
    mark[(int)c4[4] - 1] = c4[0];
    mark[(int)c4[4] + 99] = c4[1];
    mark[(int)c4[4] + 199] = c4[2];
    mark[(int)c4[4] + 299] = To_180_degrees(c4[3]);
  }

  if (c5[4] > 0.0F) {
    check_num++;
    check_id[check_num] = c5[4];
    mark[(int)c5[4] - 1] = c5[0];
    mark[(int)c5[4] + 99] = c5[1];
    mark[(int)c5[4] + 199] = c5[2];
    mark[(int)c5[4] + 299] = To_180_degrees(c5[3]);
  }

  if (c6[4] > 0.0F) {
    check_num++;
    check_id[check_num] = c6[4];
    mark[(int)c6[4] - 1] = c6[0];
    mark[(int)c6[4] + 99] = c6[1];
    mark[(int)c6[4] + 199] = c6[2];
    mark[(int)c6[4] + 299] = To_180_degrees(c6[3]);
  }

  //  start
  if ((check_num + 1 > 1) && (en_map_fix != 0)) {
    for (i = 0; i <= check_num; i++) {
      for (j = 0; j <= check_num; j++) {
    //   if ((1 + i != 1 + j) && (check_id[i] != *first_check_id) && (map[(int)check_id[j] + 399] >= 1.0F)) {
       if ((1 + i != 1 + j) && (map[(int)check_id[j] + 399] >= 1.0F)) {
	      	 //use j to correct i
          b_map[0] = map[(int)check_id[j] - 1];
          b_map[1] = map[(int)check_id[j] + 99];
          b_map[2] = map[(int)check_id[j] + 199];
          b_map[3] = map[(int)check_id[j] + 299];
          b_mark[0] = mark[(int)check_id[j] - 1];
          b_mark[1] = mark[(int)check_id[j] + 99];
          b_mark[2] = mark[(int)check_id[j] + 199];
          b_mark[3] = mark[(int)check_id[j] + 299];
          c_mark[0] = mark[(int)check_id[i] - 1];
          c_mark[1] = mark[(int)check_id[i] + 99];
          c_mark[2] = mark[(int)check_id[i] + 199];
          c_mark[3] = mark[(int)check_id[i] + 299];
          cal_center_from_est(b_map, b_mark, c_mark, camera_sum);
          if (map[(int)check_id[i] + 399] == 0.0F) {
            // added new marker
            // fprintf('marker add : %d\n',check_id(i));
            map[(int)check_id[i] + 399] = 1.0F;
            map[(int)check_id[i] - 1] = (float)camera_sum[0];
            map[(int)check_id[i] + 99] = (float)camera_sum[1];
            map[(int)check_id[i] + 199] = (float)camera_sum[2];
            map[(int)check_id[i] + 299] = (float)camera_sum[3];
          } else {
           // if (((map[(int)check_id[i] + 399] >= 1.0F) && (cal_sel == 3)) ||
            //    (map[(int)check_id[i] + 399] == 1.0F)) {
	    if ((map[(int)check_id[i] + 399] >= 1.0F) && (cal_sel == 3)){
              // correct marker
              mark_buf_cnt[(int)check_id[i] - 1]++;

              mark_pos_map_buf[((int)check_id[i] + 400 * ((int)mark_buf_cnt[(int)
                check_id[i] - 1] - 1)) - 1] = camera_sum[0];
              mark_pos_map_buf[((int)check_id[i] + 400 * ((int)mark_buf_cnt[(int)
                check_id[i] - 1] - 1)) + 99] = camera_sum[1];
              mark_pos_map_buf[((int)check_id[i] + 400 * ((int)mark_buf_cnt[(int)
                check_id[i] - 1] - 1)) + 199] = camera_sum[2];
              mark_pos_map_buf[((int)check_id[i] + 400 * ((int)mark_buf_cnt[(int)
                check_id[i] - 1] - 1)) + 299] = camera_sum[3];
	     int cloud_size_temp;
	     if(map[(int)check_id[i] + 399] >= 2.0F)
	     cloud_size_temp=cloud_size*1.618;
	     else
	     cloud_size_temp=cloud_size;   
              if (mark_buf_cnt[(int)check_id[i] - 1] > cloud_size_temp) {
                //cloud full 
                for (b_i = 0; b_i < 4; b_i++) {
                  camera_sum[b_i] = 0.0;
                }
                
                if ((cal_sel == 0) || (cal_sel == 3)) {
                  for (p = 1; p <= cloud_size_temp; p++) {
                    camera_sum[0] += mark_pos_map_buf[((int)check_id[i] + 400 *
                      (p - 1)) - 1];
                    camera_sum[1] += mark_pos_map_buf[((int)check_id[i] + 400 *
                      (p - 1)) + 99];
                    camera_sum[2] += mark_pos_map_buf[((int)check_id[i] + 400 *
                      (p - 1)) + 199];
                    camera_sum[3] += mark_pos_map_buf[((int)check_id[i] + 400 *
                      (p - 1)) + 299];
                  }

                  d4 = rt_roundd_snf(camera_sum[0] / (double)cloud_size_temp);
                  if (d4 < 32768.0) {
                    if (d4 >= -32768.0) {
                      i5 = (short)d4;
                    } else {
                      i5 = MIN_int16_T;
                    }
                  } else if (d4 >= 32768.0) {
                    i5 = MAX_int16_T;
                  } else {
                    i5 = 0;
                  }

                  camera_sum[0] = i5;
                  d4 = rt_roundd_snf(camera_sum[1] / (double)cloud_size_temp);
                  if (d4 < 32768.0) {
                    if (d4 >= -32768.0) {
                      i5 = (short)d4;
                    } else {
                      i5 = MIN_int16_T;
                    }
                  } else if (d4 >= 32768.0) {
                    i5 = MAX_int16_T;
                  } else {
                    i5 = 0;
                  }

                  camera_sum[1] = i5;
                  d4 = rt_roundd_snf(camera_sum[2] / (double)cloud_size_temp);
                  if (d4 < 32768.0) {
                    if (d4 >= -32768.0) {
                      i5 = (short)d4;
                    } else {
                      i5 = MIN_int16_T;
                    }
                  } else if (d4 >= 32768.0) {
                    i5 = MAX_int16_T;
                  } else {
                    i5 = 0;
                  }

                  camera_sum[2] = i5;
                  d4 = rt_roundd_snf(camera_sum[3] / (double)cloud_size_temp);
                  if (d4 < 32768.0) {
                    if (d4 >= -32768.0) {
                      i5 = (short)d4;
                    } else {
                      i5 = MIN_int16_T;
                    }
                  } else if (d4 >= 32768.0) {
                    i5 = MAX_int16_T;
                  } else {
                    i5 = 0;
                  }

                  camera_sum[3] = i5;
                } else {
                  if (cal_sel == 1) {
                    // Grubbs+K-means
                    memset(&X[0], 0, 200U * sizeof(double));
                    for (p = 1; p <= cloud_size_temp; p++) {
                      X[p - 1] = mark_pos_map_buf[((int)check_id[i] + 400 * (p -
                        1)) - 1];
                      X[p + 99] = mark_pos_map_buf[((int)check_id[i] + 400 * (p
                        - 1)) + 99];
                      camera_sum[2] += mark_pos_map_buf[((int)check_id[i] + 400 *
                        (p - 1)) + 199];
                      camera_sum[3] += mark_pos_map_buf[((int)check_id[i] + 400 *
                        (p - 1)) + 299];
                    }

                    kmeans(X, unusedU0, Ctrs, SumD, unusedU1);
                    temp = 0.0;
                    for (k = 0; k < 2; k++) {
                      temp += SumD[k];
                    }

                    for (k = 0; k < 2; k++) {
                      w = SumD[k] / temp;
                      camera_sum[0] += Ctrs[k] * w;
                      camera_sum[1] += Ctrs[2 + k] * w;
                    }

                    d4 = rt_roundd_snf(camera_sum[2] / (double)cloud_size_temp);
                    if (d4 < 32768.0) {
                      if (d4 >= -32768.0) {
                        i5 = (short)d4;
                      } else {
                        i5 = MIN_int16_T;
                      }
                    } else if (d4 >= 32768.0) {
                      i5 = MAX_int16_T;
                    } else {
                      i5 = 0;
                    }

                    camera_sum[2] = i5;
                    d4 = rt_roundd_snf(camera_sum[3] / (double)cloud_size_temp);
                    if (d4 < 32768.0) {
                      if (d4 >= -32768.0) {
                        i5 = (short)d4;
                      } else {
                        i5 = MIN_int16_T;
                      }
                    } else if (d4 >= 32768.0) {
                      i5 = MAX_int16_T;
                    } else {
                      i5 = 0;
                    }

                    camera_sum[3] = i5;
                  }
                }

                // fixed check
                if (((float)fabs((double)((float)camera_sum[0] - map[(int)
                       check_id[i] - 1])) < mark_pos_ero_check) && ((float)fabs
                     ((double)((float)camera_sum[1] - map[(int)check_id[i] + 99]))
                     < mark_pos_ero_check) && (map[(int)check_id[j] + 399] ==
                     1.0F)) {
                  mark_pos_lock_cnt[(int)check_id[i] - 1]++;
                }

                if (mark_pos_lock_cnt[(int)check_id[i] - 1] > lock_cnt) {
                  map[(int)check_id[i] + 399] = 2.0F;
                }

                center_fix_flt_temp = center_fix_flt;
                if (map[(int)check_id[i] + 399] == 2.0F) {
                  if ((cal_sel == 3) && (en_map_fix != 0)) {
                    center_fix_flt_temp = center_fix_flt * 0.816;//0.618F;
                  } else {
                    center_fix_flt_temp = center_fix_flt * 0.0F;
                  }
                }

                map[(int)check_id[i] - 1] = map[(int)check_id[i] - 1] * (1.0F -
                  center_fix_flt_temp) + center_fix_flt_temp * (float)
                  camera_sum[0];
                map[(int)check_id[i] + 99] = map[(int)check_id[i] + 99] * (1.0F
                  - center_fix_flt_temp) + center_fix_flt_temp * (float)
                  camera_sum[1];
                map[(int)check_id[i] + 199] = map[(int)check_id[i] + 199] *
                  (1.0F - center_fix_flt_temp) + center_fix_flt_temp * (float)
                  camera_sum[2];
                map[(int)check_id[i] + 299] = map[(int)check_id[i] + 299] *
                  (1.0F - center_fix_flt_temp) + center_fix_flt_temp * (float)
                  camera_sum[3];

		//resize cloud
                if ((map[(int)check_id[i] + 399] == 2.0F) && (cal_sel == 3)&&1) {
                  mark_buf_cnt[(int)check_id[i] - 1] = 0.0;

                  // ��λ���Ƽ�����
                } else {
                  // ��������
                  memset(&mark_pos_map_buf_temp[0], 0, 400U * sizeof(double));
                  cnt_temp = -1;
                  for (l = 1; l <= cloud_size_temp; l++) {
                    a = map[(int)check_id[i] - 1] - (float)mark_pos_map_buf
                      [((int)check_id[i] + 400 * (l - 1)) - 1];
                    b_a = map[(int)check_id[i] + 99] - (float)mark_pos_map_buf
                      [((int)check_id[i] + 400 * (l - 1)) + 99];
                    if ((float)sqrt((double)(a * a + b_a * b_a)) <
                        mark_pos_ero_check * 1.618F) {
                      cnt_temp++;
                      mark_pos_map_buf_temp[cnt_temp] = mark_pos_map_buf[((int)
                        check_id[i] + 400 * (l - 1)) - 1];
                      mark_pos_map_buf_temp[100 + cnt_temp] = mark_pos_map_buf
                        [((int)check_id[i] + 400 * (l - 1)) + 99];
                      mark_pos_map_buf_temp[200 + cnt_temp] = mark_pos_map_buf
                        [((int)check_id[i] + 400 * (l - 1)) + 199];
                      mark_pos_map_buf_temp[300 + cnt_temp] = mark_pos_map_buf
                        [((int)check_id[i] + 400 * (l - 1)) + 299];
                    }
                  }
		  //resize
                  mark_buf_cnt[(int)check_id[i] - 1] = cnt_temp + 1;
                  for (b_l = 0; b_l <= cnt_temp; b_l++) {
                    mark_pos_map_buf[((int)check_id[i] + 400 * b_l) - 1] =
                      mark_pos_map_buf_temp[b_l];
                    mark_pos_map_buf[((int)check_id[i] + 400 * b_l) + 99] =
                      mark_pos_map_buf_temp[100 + b_l];
                    mark_pos_map_buf[((int)check_id[i] + 400 * b_l) + 199] =
                      mark_pos_map_buf_temp[200 + b_l];
                    mark_pos_map_buf[((int)check_id[i] + 400 * b_l) + 299] =
                      mark_pos_map_buf_temp[300 + b_l];
                  }
                }

                // end correct
              }
            }
          }
        }
      }
    }
  }

  // cal camera pos
  for (i = 0; i < 4; i++) {
    camera_sum[i] = 0.0;
  }

  if (check_num + 1 > 0) {
    for (i = 0; i <= check_num; i++) {
      if (map[(int)check_id[i] + 399] >= 1.0F) {
        // �����ڵ�ͼ��
        c_map[0] = map[(int)check_id[i] - 1];
        c_map[1] = map[(int)check_id[i] + 99];
        c_map[2] = map[(int)check_id[i] + 199];
        c_map[3] = map[(int)check_id[i] + 299];
        d_mark[0] = mark[(int)check_id[i] - 1];
        d_mark[1] = mark[(int)check_id[i] + 99];
        d_mark[2] = mark[(int)check_id[i] + 199];
        d_mark[3] = mark[(int)check_id[i] + 299];
        cal_pos_from_map(c_map, d_mark, camera_temp);
        camera_sum[0] += camera_temp[0];
        camera_sum[1] += camera_temp[1];
        camera_sum[2] += camera_temp[2];
        camera_sum[3] += camera_temp[3];
      }
    }

    camera[0] = (float)(camera_sum[0] / ((double)check_num + 1.0));
    camera[1] = (float)(camera_sum[1] / ((double)check_num + 1.0));
    camera[2] = (float)(camera_sum[2] / ((double)check_num + 1.0));
    camera[3] = (float)(camera_sum[3] / ((double)check_num + 1.0));
  }
   lise:;
}

//
// Arguments    : void
// Return Type  : void
//
void c_mark_pos_lock_cnt_not_empty_i()
{
  mark_pos_lock_cnt_not_empty = false;
}

//
// Arguments    : void
// Return Type  : void
//
void mark_buf_cnt_not_empty_init()
{
  mark_buf_cnt_not_empty = false;
}

//
// Arguments    : void
// Return Type  : void
//
void mark_pos_map_buf_not_empty_init()
{
  mark_pos_map_buf_not_empty = false;
}

//
// File trailer for AM.cpp
//
// [EOF]
//
