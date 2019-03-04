//
// File: kmeans.cpp
//
// MATLAB Coder version            : 3.0
// C/C++ source code generated on  : 29-Dec-2017 22:25:37
//

// Include Files
#include "rt_nonfinite.h"
#include "AM.h"
#include "kmeans.h"
#include "rand.h"
#include "bsearch.h"
#include "isfinite.h"
#include "randi.h"

// Function Declarations
static void b_distfun(double D[200], const double X[200], const double C[4], int
                      crows, int ncrows);
static void b_gcentroids(double C[4], int counts[2], const double X[200], const
  int idx[100], int clusters, int nclusters);
static void batchUpdate(const double X[200], int idx[100], double C[4], double
  D[200], int counts[2], boolean_T *converged, int *iter, double *totsumD);
static int countEmpty(int empties[2], const int counts[2], const int changed[2],
                      int nchanged);
static void distfun(double D[200], const double X[200], const double C[4], const
                    int crows[2], int ncrows);
static int findchanged(int changed[2], const int idx[100], const int previdx[100],
  const int moved[100], int nmoved);
static void gcentroids(double C[4], int counts[2], const double X[200], const
  int idx[100], const int clusters[2], int nclusters);
static void local_kmeans(const double X[200], int idxbest[100], double Cbest[4],
  double varargout_1[2], double varargout_2[200]);
static void loopBody(const double X[200], double *totsumD, int idx[100], double
                     C[4], double sumD[2], double D[200]);
static void mindim2(const double D[200], double d[100], int idx[100]);
static boolean_T onlineUpdate(const double X[200], int idx[100], double C[4],
  int counts[2], int *iter, double *totsumD);
static void simpleRandperm(int idx[100]);

// Function Definitions

//
// Arguments    : double D[200]
//                const double X[200]
//                const double C[4]
//                int crows
//                int ncrows
// Return Type  : void
//
static void b_distfun(double D[200], const double X[200], const double C[4], int
                      crows, int ncrows)
{
  int i;
  double a;
  i = 1;
  while (i <= ncrows) {
    for (i = 0; i < 100; i++) {
      a = X[i] - C[crows - 1];
      D[i + 100 * (crows - 1)] = a * a;
      a = X[100 + i] - C[crows + 1];
      D[i + 100 * (crows - 1)] += a * a;
    }

    i = 2;
  }
}

//
// Arguments    : double C[4]
//                int counts[2]
//                const double X[200]
//                const int idx[100]
//                int clusters
//                int nclusters
// Return Type  : void
//
static void b_gcentroids(double C[4], int counts[2], const double X[200], const
  int idx[100], int clusters, int nclusters)
{
  int cc;
  int j;
  int i;
  cc = 1;
  while (cc <= nclusters) {
    counts[clusters - 1] = 0;
    for (j = 0; j < 2; j++) {
      C[(clusters + (j << 1)) - 1] = rtNaN;
    }

    cc = 2;
  }

  cc = 1;
  while (cc <= nclusters) {
    cc = 0;
    for (j = 0; j < 2; j++) {
      C[(clusters + (j << 1)) - 1] = 0.0;
    }

    for (i = 0; i < 100; i++) {
      if (idx[i] == clusters) {
        cc++;
        for (j = 0; j < 2; j++) {
          C[(clusters + (j << 1)) - 1] += X[i + 100 * j];
        }
      }
    }

    counts[clusters - 1] = cc;
    for (j = 0; j < 2; j++) {
      C[(clusters + (j << 1)) - 1] /= (double)cc;
    }

    cc = 2;
  }
}

//
// Arguments    : const double X[200]
//                int idx[100]
//                double C[4]
//                double D[200]
//                int counts[2]
//                boolean_T *converged
//                int *iter
//                double *totsumD
// Return Type  : void
//
static void batchUpdate(const double X[200], int idx[100], double C[4], double
  D[200], int counts[2], boolean_T *converged, int *iter, double *totsumD)
{
  int empties[2];
  int i;
  int previdx[100];
  int moved[100];
  int changed[2];
  int nMoved;
  int nchanged;
  double prevtotsumD;
  int exitg1;
  int nempty;
  double maxd;
  int lonely;
  double d;
  int from;
  boolean_T exitg3;
  boolean_T exitg2;
  int nidx[100];
  double b_d[100];
  for (i = 0; i < 2; i++) {
    empties[i] = 0;
  }

  for (i = 0; i < 100; i++) {
    previdx[i] = 0;
    moved[i] = 0;
  }

  for (nMoved = 0; nMoved < 2; nMoved++) {
    changed[nMoved] = nMoved + 1;
  }

  nchanged = 2;
  prevtotsumD = rtInf;
  *iter = 0;
  *converged = false;
  do {
    exitg1 = 0;
    (*iter)++;
    gcentroids(C, counts, X, idx, changed, nchanged);
    distfun(D, X, C, changed, nchanged);
    nempty = countEmpty(empties, counts, changed, nchanged);
    if (nempty > 0) {
      for (i = 0; i + 1 <= nempty; i++) {
        maxd = D[100 * (idx[0] - 1)];
        lonely = 0;
        for (nMoved = 0; nMoved < 100; nMoved++) {
          d = D[nMoved + 100 * (idx[nMoved] - 1)];
          if (d > maxd) {
            maxd = d;
            lonely = nMoved;
          }
        }

        from = idx[lonely];
        if (counts[idx[lonely] - 1] < 2) {
          from = 1;
          while (!(counts[from - 1] > 1)) {
            from++;
          }

          nMoved = 0;
          exitg3 = false;
          while ((!exitg3) && (nMoved + 1 < 101)) {
            if (idx[nMoved] == from) {
              lonely = nMoved;
              exitg3 = true;
            } else {
              nMoved++;
            }
          }
        }

        for (nMoved = 0; nMoved < 2; nMoved++) {
          C[(empties[i] + (nMoved << 1)) - 1] = X[lonely + 100 * nMoved];
        }

        counts[empties[i] - 1] = 1;
        idx[lonely] = empties[i];
        b_distfun(D, X, C, empties[i], 1);
        b_gcentroids(C, counts, X, idx, from, 1);
        b_distfun(D, X, C, from, 1);
        if (nchanged < 2) {
          nMoved = 1;
          exitg2 = false;
          while ((!exitg2) && ((nMoved <= nchanged) && (!(from == changed[0]))))
          {
            if (from > changed[0]) {
              changed[1] = changed[0];
              changed[0] = from;
              nchanged = 2;
              exitg2 = true;
            } else {
              nMoved = 2;
            }
          }
        }
      }
    }

    *totsumD = 0.0;
    for (i = 0; i < 100; i++) {
      *totsumD += D[i + 100 * (idx[i] - 1)];
    }

    if (prevtotsumD <= *totsumD) {
      memcpy(&idx[0], &previdx[0], 100U * sizeof(int));
      gcentroids(C, counts, X, previdx, changed, nchanged);
      (*iter)--;
      exitg1 = 1;
    } else if (*iter >= 100) {
      exitg1 = 1;
    } else {
      memcpy(&previdx[0], &idx[0], 100U * sizeof(int));
      prevtotsumD = *totsumD;
      mindim2(D, b_d, nidx);
      nMoved = 0;
      for (i = 0; i < 100; i++) {
        if ((nidx[i] != previdx[i]) && (D[i + 100 * (previdx[i] - 1)] > b_d[i]))
        {
          nMoved++;
          moved[nMoved - 1] = i + 1;
          idx[i] = nidx[i];
        }
      }

      if (nMoved == 0) {
        *converged = true;
        exitg1 = 1;
      } else {
        nchanged = findchanged(changed, idx, previdx, moved, nMoved);
      }
    }
  } while (exitg1 == 0);
}

//
// Arguments    : int empties[2]
//                const int counts[2]
//                const int changed[2]
//                int nchanged
// Return Type  : int
//
static int countEmpty(int empties[2], const int counts[2], const int changed[2],
                      int nchanged)
{
  int nempty;
  int j;
  nempty = 0;
  for (j = 0; j + 1 <= nchanged; j++) {
    if (counts[changed[j] - 1] == 0) {
      nempty++;
      empties[nempty - 1] = changed[j];
    }
  }

  return nempty;
}

//
// Arguments    : double D[200]
//                const double X[200]
//                const double C[4]
//                const int crows[2]
//                int ncrows
// Return Type  : void
//
static void distfun(double D[200], const double X[200], const double C[4], const
                    int crows[2], int ncrows)
{
  int i;
  int cr;
  int r;
  double a;
  for (i = 1; i <= ncrows; i++) {
    cr = crows[i - 1] - 1;
    for (r = 0; r < 100; r++) {
      a = X[r] - C[cr];
      D[r + 100 * cr] = a * a;
    }

    for (r = 0; r < 100; r++) {
      a = X[100 + r] - C[2 + cr];
      D[r + 100 * cr] += a * a;
    }
  }
}

//
// Arguments    : int changed[2]
//                const int idx[100]
//                const int previdx[100]
//                const int moved[100]
//                int nmoved
// Return Type  : int
//
static int findchanged(int changed[2], const int idx[100], const int previdx[100],
  const int moved[100], int nmoved)
{
  int nchanged;
  boolean_T b[100];
  int j;
  memset(&b[0], 0, 100U * sizeof(boolean_T));
  for (j = 0; j + 1 <= nmoved; j++) {
    b[idx[moved[j] - 1] - 1] = true;
    b[previdx[moved[j] - 1] - 1] = true;
  }

  nchanged = 0;
  for (j = 0; j < 100; j++) {
    if (b[j]) {
      nchanged++;
      changed[nchanged - 1] = j + 1;
    }
  }

  return nchanged;
}

//
// Arguments    : double C[4]
//                int counts[2]
//                const double X[200]
//                const int idx[100]
//                const int clusters[2]
//                int nclusters
// Return Type  : void
//
static void gcentroids(double C[4], int counts[2], const double X[200], const
  int idx[100], const int clusters[2], int nclusters)
{
  int ic;
  int j;
  int clic;
  int cc;
  int i;
  for (ic = 0; ic + 1 <= nclusters; ic++) {
    counts[clusters[ic] - 1] = 0;
    for (j = 0; j < 2; j++) {
      C[(clusters[ic] + (j << 1)) - 1] = rtNaN;
    }
  }

  for (ic = 0; ic + 1 <= nclusters; ic++) {
    clic = clusters[ic] - 1;
    cc = 0;
    for (j = 0; j < 2; j++) {
      C[clic + (j << 1)] = 0.0;
    }

    for (i = 0; i < 100; i++) {
      if (idx[i] == clic + 1) {
        cc++;
        for (j = 0; j < 2; j++) {
          C[clic + (j << 1)] += X[i + 100 * j];
        }
      }
    }

    counts[clusters[ic] - 1] = cc;
    for (j = 0; j < 2; j++) {
      C[clic + (j << 1)] /= (double)cc;
    }
  }
}

//
// Arguments    : const double X[200]
//                int idxbest[100]
//                double Cbest[4]
//                double varargout_1[2]
//                double varargout_2[200]
// Return Type  : void
//
static void local_kmeans(const double X[200], int idxbest[100], double Cbest[4],
  double varargout_1[2], double varargout_2[200])
{
  double totsumDbest;
  int rep;
  double D[200];
  double sumD[2];
  double C[4];
  int idx[100];
  double totsumD;
  int i;
  loopBody(X, &totsumDbest, idxbest, Cbest, varargout_1, varargout_2);
  for (rep = 0; rep < 2; rep++) {
    loopBody(X, &totsumD, idx, C, sumD, D);
    if (totsumD < totsumDbest) {
      totsumDbest = totsumD;
      memcpy(&idxbest[0], &idx[0], 100U * sizeof(int));
      for (i = 0; i < 4; i++) {
        Cbest[i] = C[i];
      }

      for (i = 0; i < 2; i++) {
        varargout_1[i] = sumD[i];
      }

      memcpy(&varargout_2[0], &D[0], 200U * sizeof(double));
    }
  }
}

//
// Arguments    : const double X[200]
//                double *totsumD
//                int idx[100]
//                double C[4]
//                double sumD[2]
//                double D[200]
// Return Type  : void
//
static void loopBody(const double X[200], double *totsumD, int idx[100], double
                     C[4], double sumD[2], double D[200])
{
  int crows[2];
  int c;
  double b_index;
  double sampleDist[101];
  int pidx;
  double d[100];
  int nonEmpties[2];
  boolean_T converged;
  for (c = 0; c < 2; c++) {
    crows[c] = c + 1;
  }

  b_index = randi();
  for (c = 0; c < 4; c++) {
    C[c] = 0.0;
  }

  for (c = 0; c < 2; c++) {
    C[c << 1] = X[((int)b_index + 100 * c) - 1];
  }

  memset(&sampleDist[0], 0, 101U * sizeof(double));
  memset(&D[0], 0, 200U * sizeof(double));
  distfun(D, X, C, crows, 1);
  b_index = 0.0;
  sampleDist[0] = 0.0;
  for (c = 0; c < 100; c++) {
    sampleDist[c + 1] = sampleDist[c] + D[c];
    b_index += D[c];
  }

  if ((b_index == 0.0) || (!b_isfinite(b_index))) {
    simpleRandperm(idx);
    for (c = 0; c < 2; c++) {
      C[1 + (c << 1)] = X[(idx[0] + 100 * c) - 1];
    }
  } else {
    for (c = 0; c < 101; c++) {
      sampleDist[c] /= b_index;
    }

    pidx = b_bsearch(sampleDist, b_rand());
    b_index = sampleDist[pidx - 1];
    if (sampleDist[pidx - 1] < 1.0) {
      while ((pidx <= 100) && (sampleDist[pidx] <= b_index)) {
        pidx++;
      }
    } else {
      while ((pidx >= 2) && (sampleDist[pidx - 2] >= b_index)) {
        pidx--;
      }
    }

    for (c = 0; c < 2; c++) {
      C[1 + (c << 1)] = X[(pidx + 100 * c) - 1];
    }
  }

  distfun(D, X, C, crows, 2);
  mindim2(D, d, idx);
  for (c = 0; c < 2; c++) {
    crows[c] = 0;
  }

  for (c = 0; c < 100; c++) {
    crows[idx[c] - 1]++;
  }

  for (c = 0; c < 2; c++) {
    nonEmpties[c] = 0;
  }

  batchUpdate(X, idx, C, D, crows, &converged, &c, totsumD);
  onlineUpdate(X, idx, C, crows, &c, totsumD);
  pidx = 0;
  for (c = 0; c < 2; c++) {
    if (crows[c] > 0) {
      pidx++;
      nonEmpties[pidx - 1] = c + 1;
    }
  }

  distfun(D, X, C, nonEmpties, pidx);
  for (c = 0; c < 100; c++) {
    d[c] = D[c + 100 * (idx[c] - 1)];
  }

  for (c = 0; c < 2; c++) {
    sumD[c] = 0.0;
  }

  for (c = 0; c < 100; c++) {
    sumD[idx[c] - 1] += d[c];
  }

  *totsumD = 0.0;
  for (c = 1; c <= pidx; c++) {
    *totsumD += sumD[nonEmpties[c - 1] - 1];
  }
}

//
// Arguments    : const double D[200]
//                double d[100]
//                int idx[100]
// Return Type  : void
//
static void mindim2(const double D[200], double d[100], int idx[100])
{
  int i;
  int j;
  double b_d;
  for (i = 0; i < 100; i++) {
    d[i] = rtInf;
    idx[i] = 1;
  }

  for (j = 0; j < 2; j++) {
    for (i = 0; i < 100; i++) {
      b_d = d[i];
      if (D[i + 100 * j] < d[i]) {
        idx[i] = j + 1;
        b_d = D[i + 100 * j];
      }

      d[i] = b_d;
    }
  }
}

//
// Arguments    : const double X[200]
//                int idx[100]
//                double C[4]
//                int counts[2]
//                int *iter
//                double *totsumD
// Return Type  : boolean_T
//
static boolean_T onlineUpdate(const double X[200], int idx[100], double C[4],
  int counts[2], int *iter, double *totsumD)
{
  boolean_T converged;
  int changed[2];
  int i;
  int nchanged;
  int lastmoved;
  int nummoved;
  int iter1;
  double Del[200];
  int nmoved;
  boolean_T exitg1;
  int modmin;
  int j;
  double aMember;
  double aNonMember;
  int nidx[100];
  double minDel[100];
  signed char moved[100];
  int mvScalar;
  boolean_T guard1 = false;
  for (i = 0; i < 2; i++) {
    changed[i] = 0;
  }

  nchanged = 0;
  for (i = 0; i < 2; i++) {
    if (counts[1] > 0) {
      nchanged++;
      changed[nchanged - 1] = i + 1;
    }
  }

  lastmoved = 0;
  nummoved = 0;
  iter1 = *iter;
  converged = false;
  for (nmoved = 0; nmoved < 200; nmoved++) {
    Del[nmoved] = rtNaN;
  }

  exitg1 = false;
  while ((!exitg1) && (*iter < 100)) {
    for (nmoved = 0; nmoved + 1 <= nchanged; nmoved++) {
      modmin = changed[nmoved] - 1;
      memset(&Del[100 * (changed[nmoved] - 1)], 0, 100U * sizeof(double));
      for (j = 0; j < 2; j++) {
        for (i = 0; i < 100; i++) {
          aMember = X[i + 100 * j] - C[modmin + (j << 1)];
          Del[i + 100 * modmin] += aMember * aMember;
        }
      }

      if (counts[changed[nmoved] - 1] == 1) {
        aMember = 0.0;
      } else {
        aMember = (double)counts[changed[nmoved] - 1] / ((double)
          counts[changed[nmoved] - 1] - 1.0);
      }

      aNonMember = (double)counts[changed[nmoved] - 1] / ((double)
        counts[changed[nmoved] - 1] + 1.0);
      for (i = 0; i < 100; i++) {
        if (idx[i] == modmin + 1) {
          Del[i + 100 * modmin] *= aMember;
        } else {
          Del[i + 100 * modmin] *= aNonMember;
        }
      }
    }

    mindim2(Del, minDel, nidx);
    nmoved = 0;
    memset(&moved[0], 0, 100U * sizeof(signed char));
    for (i = 0; i < 100; i++) {
      if ((idx[i] != nidx[i]) && (Del[i + 100 * (idx[i] - 1)] > minDel[i])) {
        nmoved++;
        moved[nmoved - 1] = (signed char)(i + 1);
      }
    }

    if (nmoved == 0) {
      if ((*iter == iter1) || (nummoved > 0)) {
        (*iter)++;
      }

      converged = true;
      exitg1 = true;
    } else {
      modmin = 100;
      for (i = 0; i + 1 <= nmoved; i++) {
        if (moved[i] < lastmoved + 1) {
          mvScalar = 99 - (lastmoved - moved[i]);
          mvScalar -= mvScalar / 100 * 100;
        } else {
          mvScalar = (moved[i] - lastmoved) - 1;
          mvScalar -= mvScalar / 100 * 100;
        }

        if (mvScalar < modmin) {
          modmin = mvScalar;
        }
      }

      mvScalar = modmin + lastmoved;
      nmoved = mvScalar / 100 * 100;
      i = mvScalar - nmoved;
      mvScalar -= nmoved;
      guard1 = false;
      if (i + 1 <= lastmoved) {
        (*iter)++;
        if (*iter >= 100) {
          exitg1 = true;
        } else {
          nummoved = 0;
          guard1 = true;
        }
      } else {
        guard1 = true;
      }

      if (guard1) {
        nummoved++;
        lastmoved = i + 1;
        nmoved = idx[i] - 1;
        modmin = nidx[i] - 1;
        *totsumD = (*totsumD + Del[i + 100 * (nidx[mvScalar] - 1)]) - Del[i +
          100 * (idx[mvScalar] - 1)];
        idx[i] = nidx[mvScalar];
        counts[nidx[mvScalar] - 1]++;
        counts[nmoved]--;
        if (nidx[mvScalar] < nmoved + 1) {
          changed[0] = nidx[mvScalar];
          changed[1] = nmoved + 1;
        } else {
          changed[0] = nmoved + 1;
          changed[1] = nidx[mvScalar];
        }

        nchanged = 2;
        for (j = 0; j < 2; j++) {
          C[modmin + (j << 1)] += (X[i + 100 * j] - C[modmin + (j << 1)]) /
            (double)counts[nidx[mvScalar] - 1];
          C[nmoved + (j << 1)] -= (X[i + 100 * j] - C[nmoved + (j << 1)]) /
            (double)counts[nmoved];
        }
      }
    }
  }

  return converged;
}

//
// Arguments    : int idx[100]
// Return Type  : void
//
static void simpleRandperm(int idx[100])
{
  int t;
  double denom;
  double pt;
  double u;
  t = 1;
  memset(&idx[0], 0, 100U * sizeof(int));
  denom = 100.0;
  pt = 0.01;
  u = b_rand();
  while (u > pt) {
    t++;
    denom--;
    pt += (1.0 - pt) * (1.0 / denom);
  }

  denom = b_rand();
  idx[0] = 0;
  idx[(int)floor(denom)] = t;
}

//
// Arguments    : const double X[200]
//                double idxbest[100]
//                double Cbest[4]
//                double varargout_1[2]
//                double varargout_2[200]
// Return Type  : void
//
void kmeans(const double X[200], double idxbest[100], double Cbest[4], double
            varargout_1[2], double varargout_2[200])
{
  int idx[100];
  int i;
  local_kmeans(X, idx, Cbest, varargout_1, varargout_2);
  for (i = 0; i < 100; i++) {
    idxbest[i] = idx[i];
  }
}

//
// File trailer for kmeans.cpp
//
// [EOF]
//
