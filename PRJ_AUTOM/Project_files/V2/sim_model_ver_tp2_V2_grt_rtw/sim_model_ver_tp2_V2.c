/*
 * sim_model_ver_tp2_V2.c
 *
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * Code generation for model "sim_model_ver_tp2_V2".
 *
 * Model version              : 1.267
 * Simulink Coder version : 9.3 (R2020a) 18-Nov-2019
 * C source code generated on : Mon Oct 19 20:29:05 2020
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "sim_model_ver_tp2_V2.h"
#include "sim_model_ver_tp2_V2_private.h"

/* Block signals (default storage) */
B_sim_model_ver_tp2_V2_T sim_model_ver_tp2_V2_B;

/* Continuous states */
X_sim_model_ver_tp2_V2_T sim_model_ver_tp2_V2_X;

/* Block states (default storage) */
DW_sim_model_ver_tp2_V2_T sim_model_ver_tp2_V2_DW;

/* Real-time model */
RT_MODEL_sim_model_ver_tp2_V2_T sim_model_ver_tp2_V2_M_;
RT_MODEL_sim_model_ver_tp2_V2_T *const sim_model_ver_tp2_V2_M =
  &sim_model_ver_tp2_V2_M_;

/*
 * This function updates continuous states using the ODE3 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  /* Solver Matrices */
  static const real_T rt_ODE3_A[3] = {
    1.0/2.0, 3.0/4.0, 1.0
  };

  static const real_T rt_ODE3_B[3][3] = {
    { 1.0/2.0, 0.0, 0.0 },

    { 0.0, 3.0/4.0, 0.0 },

    { 2.0/9.0, 1.0/3.0, 4.0/9.0 }
  };

  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE3_IntgData *id = (ODE3_IntgData *)rtsiGetSolverData(si);
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T hB[3];
  int_T i;
  int_T nXc = 3;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                (uint_T)nXc*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  sim_model_ver_tp2_V2_derivatives();

  /* f(:,2) = feval(odefile, t + hA(1), y + f*hB(:,1), args(:)(*)); */
  hB[0] = h * rt_ODE3_B[0][0];
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[0]);
  rtsiSetdX(si, f1);
  sim_model_ver_tp2_V2_step();
  sim_model_ver_tp2_V2_derivatives();

  /* f(:,3) = feval(odefile, t + hA(2), y + f*hB(:,2), args(:)(*)); */
  for (i = 0; i <= 1; i++) {
    hB[i] = h * rt_ODE3_B[1][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[1]);
  rtsiSetdX(si, f2);
  sim_model_ver_tp2_V2_step();
  sim_model_ver_tp2_V2_derivatives();

  /* tnew = t + hA(3);
     ynew = y + f*hB(:,3); */
  for (i = 0; i <= 2; i++) {
    hB[i] = h * rt_ODE3_B[2][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1] + f2[i]*hB[2]);
  }

  rtsiSetT(si, tnew);
  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

real_T rt_atan2d_snf(real_T u0, real_T u1)
{
  real_T y;
  int32_T u0_0;
  int32_T u1_0;
  if (rtIsNaN(u0) || rtIsNaN(u1)) {
    y = (rtNaN);
  } else if (rtIsInf(u0) && rtIsInf(u1)) {
    if (u0 > 0.0) {
      u0_0 = 1;
    } else {
      u0_0 = -1;
    }

    if (u1 > 0.0) {
      u1_0 = 1;
    } else {
      u1_0 = -1;
    }

    y = atan2(u0_0, u1_0);
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = atan2(u0, u1);
  }

  return y;
}

/* Model step function */
void sim_model_ver_tp2_V2_step(void)
{
  if (rtmIsMajorTimeStep(sim_model_ver_tp2_V2_M)) {
    /* set solver stop time */
    if (!(sim_model_ver_tp2_V2_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&sim_model_ver_tp2_V2_M->solverInfo,
                            ((sim_model_ver_tp2_V2_M->Timing.clockTickH0 + 1) *
        sim_model_ver_tp2_V2_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&sim_model_ver_tp2_V2_M->solverInfo,
                            ((sim_model_ver_tp2_V2_M->Timing.clockTick0 + 1) *
        sim_model_ver_tp2_V2_M->Timing.stepSize0 +
        sim_model_ver_tp2_V2_M->Timing.clockTickH0 *
        sim_model_ver_tp2_V2_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(sim_model_ver_tp2_V2_M)) {
    sim_model_ver_tp2_V2_M->Timing.t[0] = rtsiGetT
      (&sim_model_ver_tp2_V2_M->solverInfo);
  }

  {
    real_T *lastU;
    boolean_T is_on_part_2_flag;
    boolean_T is_on_part_3_flag;
    boolean_T is_on_part_4_flag;
    boolean_T rEQ0;
    real_T q;
    real_T b_r;
    real_T rtb_x_in;
    real_T rtb_y_in;
    real_T rtb_d_y_in;
    real_T rtb_d_x_in;
    if (rtmIsMajorTimeStep(sim_model_ver_tp2_V2_M)) {
      /* Delay: '<Root>/Initial condition X' */
      rtb_x_in = sim_model_ver_tp2_V2_DW.InitialconditionX_DSTATE;

      /* Delay: '<Root>/Initial condition Y' */
      rtb_y_in = sim_model_ver_tp2_V2_DW.InitialconditionY_DSTATE;

      /* MATLAB Function: '<Root>/Input block' incorporates:
       *  Constant: '<Root>/Speed'
       */
      rEQ0 = ((rtb_x_in == 0.0) && (rtb_y_in < 10.0));
      is_on_part_2_flag = ((rtb_x_in < 5.0) && (rtb_y_in >= 10.0));
      is_on_part_3_flag = ((rtb_x_in >= 5.0) && (rtb_y_in >= 10.0));
      is_on_part_4_flag = ((!rEQ0) && (!is_on_part_2_flag) &&
                           (!is_on_part_3_flag));
      rtb_d_x_in = rtb_x_in - 0.01 * sim_model_ver_tp2_V2_P.Speed_Value;
      rtb_x_in = (real_T)(rtb_d_x_in > 0.0) * rtb_d_x_in * (real_T)
        is_on_part_4_flag + ((0.01 * sim_model_ver_tp2_V2_P.Speed_Value +
        rtb_x_in) * (real_T)is_on_part_2_flag + (real_T)is_on_part_3_flag * 5.0);
      rtb_d_x_in = sin(3.1415926535897931 * rtb_x_in * 2.0 * 3.1415926535897931 /
                       35.9 / 5.0) * 2.5;
      sim_model_ver_tp2_V2_B.y = (((rtb_d_x_in + rtb_y_in) * (real_T)
        is_on_part_2_flag + (0.01 * sim_model_ver_tp2_V2_P.Speed_Value +
        rtb_y_in) * (real_T)rEQ0) + (rtb_y_in - 0.01 *
        sim_model_ver_tp2_V2_P.Speed_Value) * (real_T)is_on_part_3_flag) +
        (rtb_y_in - rtb_d_x_in) * (real_T)is_on_part_4_flag;
      sim_model_ver_tp2_V2_B.x = rtb_x_in;
    }

    /* Sum: '<S4>/Sum' incorporates:
     *  Integrator: '<S7>/Integrator'
     */
    rtb_y_in = sim_model_ver_tp2_V2_B.x -
      sim_model_ver_tp2_V2_X.Integrator_CSTATE;

    /* Sum: '<S4>/Sum1' incorporates:
     *  Integrator: '<S7>/Integrator1'
     */
    rtb_x_in = sim_model_ver_tp2_V2_B.y -
      sim_model_ver_tp2_V2_X.Integrator1_CSTATE;

    /* Derivative: '<S4>/Derivative' incorporates:
     *  Derivative: '<S4>/Derivative1'
     */
    rtb_d_x_in = sim_model_ver_tp2_V2_M->Timing.t[0];
    if ((sim_model_ver_tp2_V2_DW.TimeStampA >= rtb_d_x_in) &&
        (sim_model_ver_tp2_V2_DW.TimeStampB >= rtb_d_x_in)) {
      rtb_d_y_in = 0.0;
    } else {
      q = sim_model_ver_tp2_V2_DW.TimeStampA;
      lastU = &sim_model_ver_tp2_V2_DW.LastUAtTimeA;
      if (sim_model_ver_tp2_V2_DW.TimeStampA <
          sim_model_ver_tp2_V2_DW.TimeStampB) {
        if (sim_model_ver_tp2_V2_DW.TimeStampB < rtb_d_x_in) {
          q = sim_model_ver_tp2_V2_DW.TimeStampB;
          lastU = &sim_model_ver_tp2_V2_DW.LastUAtTimeB;
        }
      } else {
        if (sim_model_ver_tp2_V2_DW.TimeStampA >= rtb_d_x_in) {
          q = sim_model_ver_tp2_V2_DW.TimeStampB;
          lastU = &sim_model_ver_tp2_V2_DW.LastUAtTimeB;
        }
      }

      rtb_d_y_in = (sim_model_ver_tp2_V2_B.y - *lastU) / (rtb_d_x_in - q);
    }

    /* End of Derivative: '<S4>/Derivative' */

    /* Derivative: '<S4>/Derivative1' */
    if ((sim_model_ver_tp2_V2_DW.TimeStampA_i >= rtb_d_x_in) &&
        (sim_model_ver_tp2_V2_DW.TimeStampB_c >= rtb_d_x_in)) {
      rtb_d_x_in = 0.0;
    } else {
      q = sim_model_ver_tp2_V2_DW.TimeStampA_i;
      lastU = &sim_model_ver_tp2_V2_DW.LastUAtTimeA_g;
      if (sim_model_ver_tp2_V2_DW.TimeStampA_i <
          sim_model_ver_tp2_V2_DW.TimeStampB_c) {
        if (sim_model_ver_tp2_V2_DW.TimeStampB_c < rtb_d_x_in) {
          q = sim_model_ver_tp2_V2_DW.TimeStampB_c;
          lastU = &sim_model_ver_tp2_V2_DW.LastUAtTimeB_e;
        }
      } else {
        if (sim_model_ver_tp2_V2_DW.TimeStampA_i >= rtb_d_x_in) {
          q = sim_model_ver_tp2_V2_DW.TimeStampB_c;
          lastU = &sim_model_ver_tp2_V2_DW.LastUAtTimeB_e;
        }
      }

      rtb_d_x_in = (sim_model_ver_tp2_V2_B.x - *lastU) / (rtb_d_x_in - q);
    }

    /* MATLAB Function: '<S4>/MATLAB Function' */
    rtb_d_x_in = rt_atan2d_snf(rtb_d_y_in, rtb_d_x_in);
    if (rtIsNaN(rtb_d_x_in) || rtIsInf(rtb_d_x_in)) {
      rtb_d_y_in = (rtNaN);
    } else if (rtb_d_x_in == 0.0) {
      rtb_d_y_in = 0.0;
    } else {
      rtb_d_y_in = fmod(rtb_d_x_in, 6.2831853071795862);
      rEQ0 = (rtb_d_y_in == 0.0);
      if (!rEQ0) {
        q = fabs(rtb_d_x_in / 6.2831853071795862);
        rEQ0 = !(fabs(q - floor(q + 0.5)) > 2.2204460492503131E-16 * q);
      }

      if (rEQ0) {
        rtb_d_y_in = 0.0;
      } else {
        if (rtb_d_x_in < 0.0) {
          rtb_d_y_in += 6.2831853071795862;
        }
      }
    }

    /* End of MATLAB Function: '<S4>/MATLAB Function' */

    /* MATLAB Function: '<S5>/MATLAB Function' incorporates:
     *  Integrator: '<S7>/Integrator2'
     */
    rtb_d_x_in = rt_atan2d_snf(rtb_x_in, rtb_y_in);
    if (rtIsNaN(rtb_d_x_in) || rtIsInf(rtb_d_x_in)) {
      b_r = (rtNaN);
    } else if (rtb_d_x_in == 0.0) {
      b_r = 0.0;
    } else {
      b_r = fmod(rtb_d_x_in, 6.2831853071795862);
      rEQ0 = (b_r == 0.0);
      if (!rEQ0) {
        q = fabs(rtb_d_x_in / 6.2831853071795862);
        rEQ0 = !(fabs(q - floor(q + 0.5)) > 2.2204460492503131E-16 * q);
      }

      if (rEQ0) {
        b_r = 0.0;
      } else {
        if (rtb_d_x_in < 0.0) {
          b_r += 6.2831853071795862;
        }
      }
    }

    rtb_d_x_in = b_r - sim_model_ver_tp2_V2_X.Integrator2_CSTATE;

    /* MATLAB Function: '<S5>/MATLAB Function1' incorporates:
     *  MATLAB Function: '<S5>/MATLAB Function'
     */
    rtb_y_in = sqrt(rtb_y_in * rtb_y_in + rtb_x_in * rtb_x_in) * 5.5;

    /* MATLAB Function: '<S7>/MATLAB Function' incorporates:
     *  Integrator: '<S7>/Integrator2'
     *  MATLAB Function: '<S5>/MATLAB Function'
     *  MATLAB Function: '<S5>/MATLAB Function1'
     *  Sum: '<S4>/Sum2'
     */
    sim_model_ver_tp2_V2_B.d_x = rtb_y_in * cos
      (sim_model_ver_tp2_V2_X.Integrator2_CSTATE);
    sim_model_ver_tp2_V2_B.d_y = rtb_y_in * sin
      (sim_model_ver_tp2_V2_X.Integrator2_CSTATE);
    sim_model_ver_tp2_V2_B.d_theta = 220.0 * rtb_d_x_in - ((rtb_d_y_in -
      sim_model_ver_tp2_V2_X.Integrator2_CSTATE) - rtb_d_x_in) * 2.0;
  }

  if (rtmIsMajorTimeStep(sim_model_ver_tp2_V2_M)) {
    /* Matfile logging */
    rt_UpdateTXYLogVars(sim_model_ver_tp2_V2_M->rtwLogInfo,
                        (sim_model_ver_tp2_V2_M->Timing.t));
  }                                    /* end MajorTimeStep */

  if (rtmIsMajorTimeStep(sim_model_ver_tp2_V2_M)) {
    real_T *lastU;
    if (rtmIsMajorTimeStep(sim_model_ver_tp2_V2_M)) {
      /* Update for Delay: '<Root>/Initial condition X' */
      sim_model_ver_tp2_V2_DW.InitialconditionX_DSTATE =
        sim_model_ver_tp2_V2_B.x;

      /* Update for Delay: '<Root>/Initial condition Y' */
      sim_model_ver_tp2_V2_DW.InitialconditionY_DSTATE =
        sim_model_ver_tp2_V2_B.y;
    }

    /* Update for Derivative: '<S4>/Derivative' */
    if (sim_model_ver_tp2_V2_DW.TimeStampA == (rtInf)) {
      sim_model_ver_tp2_V2_DW.TimeStampA = sim_model_ver_tp2_V2_M->Timing.t[0];
      lastU = &sim_model_ver_tp2_V2_DW.LastUAtTimeA;
    } else if (sim_model_ver_tp2_V2_DW.TimeStampB == (rtInf)) {
      sim_model_ver_tp2_V2_DW.TimeStampB = sim_model_ver_tp2_V2_M->Timing.t[0];
      lastU = &sim_model_ver_tp2_V2_DW.LastUAtTimeB;
    } else if (sim_model_ver_tp2_V2_DW.TimeStampA <
               sim_model_ver_tp2_V2_DW.TimeStampB) {
      sim_model_ver_tp2_V2_DW.TimeStampA = sim_model_ver_tp2_V2_M->Timing.t[0];
      lastU = &sim_model_ver_tp2_V2_DW.LastUAtTimeA;
    } else {
      sim_model_ver_tp2_V2_DW.TimeStampB = sim_model_ver_tp2_V2_M->Timing.t[0];
      lastU = &sim_model_ver_tp2_V2_DW.LastUAtTimeB;
    }

    *lastU = sim_model_ver_tp2_V2_B.y;

    /* End of Update for Derivative: '<S4>/Derivative' */

    /* Update for Derivative: '<S4>/Derivative1' */
    if (sim_model_ver_tp2_V2_DW.TimeStampA_i == (rtInf)) {
      sim_model_ver_tp2_V2_DW.TimeStampA_i = sim_model_ver_tp2_V2_M->Timing.t[0];
      lastU = &sim_model_ver_tp2_V2_DW.LastUAtTimeA_g;
    } else if (sim_model_ver_tp2_V2_DW.TimeStampB_c == (rtInf)) {
      sim_model_ver_tp2_V2_DW.TimeStampB_c = sim_model_ver_tp2_V2_M->Timing.t[0];
      lastU = &sim_model_ver_tp2_V2_DW.LastUAtTimeB_e;
    } else if (sim_model_ver_tp2_V2_DW.TimeStampA_i <
               sim_model_ver_tp2_V2_DW.TimeStampB_c) {
      sim_model_ver_tp2_V2_DW.TimeStampA_i = sim_model_ver_tp2_V2_M->Timing.t[0];
      lastU = &sim_model_ver_tp2_V2_DW.LastUAtTimeA_g;
    } else {
      sim_model_ver_tp2_V2_DW.TimeStampB_c = sim_model_ver_tp2_V2_M->Timing.t[0];
      lastU = &sim_model_ver_tp2_V2_DW.LastUAtTimeB_e;
    }

    *lastU = sim_model_ver_tp2_V2_B.x;

    /* End of Update for Derivative: '<S4>/Derivative1' */
  }                                    /* end MajorTimeStep */

  if (rtmIsMajorTimeStep(sim_model_ver_tp2_V2_M)) {
    /* signal main to stop simulation */
    {                                  /* Sample time: [0.0s, 0.0s] */
      if ((rtmGetTFinal(sim_model_ver_tp2_V2_M)!=-1) &&
          !((rtmGetTFinal(sim_model_ver_tp2_V2_M)-
             (((sim_model_ver_tp2_V2_M->Timing.clockTick1+
                sim_model_ver_tp2_V2_M->Timing.clockTickH1* 4294967296.0)) *
              0.01)) > (((sim_model_ver_tp2_V2_M->Timing.clockTick1+
                          sim_model_ver_tp2_V2_M->Timing.clockTickH1*
                          4294967296.0)) * 0.01) * (DBL_EPSILON))) {
        rtmSetErrorStatus(sim_model_ver_tp2_V2_M, "Simulation finished");
      }
    }

    rt_ertODEUpdateContinuousStates(&sim_model_ver_tp2_V2_M->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick0 and the high bits
     * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++sim_model_ver_tp2_V2_M->Timing.clockTick0)) {
      ++sim_model_ver_tp2_V2_M->Timing.clockTickH0;
    }

    sim_model_ver_tp2_V2_M->Timing.t[0] = rtsiGetSolverStopTime
      (&sim_model_ver_tp2_V2_M->solverInfo);

    {
      /* Update absolute timer for sample time: [0.01s, 0.0s] */
      /* The "clockTick1" counts the number of times the code of this task has
       * been executed. The resolution of this integer timer is 0.01, which is the step size
       * of the task. Size of "clockTick1" ensures timer will not overflow during the
       * application lifespan selected.
       * Timer of this task consists of two 32 bit unsigned integers.
       * The two integers represent the low bits Timing.clockTick1 and the high bits
       * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
       */
      sim_model_ver_tp2_V2_M->Timing.clockTick1++;
      if (!sim_model_ver_tp2_V2_M->Timing.clockTick1) {
        sim_model_ver_tp2_V2_M->Timing.clockTickH1++;
      }
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void sim_model_ver_tp2_V2_derivatives(void)
{
  XDot_sim_model_ver_tp2_V2_T *_rtXdot;
  _rtXdot = ((XDot_sim_model_ver_tp2_V2_T *) sim_model_ver_tp2_V2_M->derivs);

  /* Derivatives for Integrator: '<S7>/Integrator' */
  _rtXdot->Integrator_CSTATE = sim_model_ver_tp2_V2_B.d_x;

  /* Derivatives for Integrator: '<S7>/Integrator1' */
  _rtXdot->Integrator1_CSTATE = sim_model_ver_tp2_V2_B.d_y;

  /* Derivatives for Integrator: '<S7>/Integrator2' */
  _rtXdot->Integrator2_CSTATE = sim_model_ver_tp2_V2_B.d_theta;
}

/* Model initialize function */
void sim_model_ver_tp2_V2_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)sim_model_ver_tp2_V2_M, 0,
                sizeof(RT_MODEL_sim_model_ver_tp2_V2_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&sim_model_ver_tp2_V2_M->solverInfo,
                          &sim_model_ver_tp2_V2_M->Timing.simTimeStep);
    rtsiSetTPtr(&sim_model_ver_tp2_V2_M->solverInfo, &rtmGetTPtr
                (sim_model_ver_tp2_V2_M));
    rtsiSetStepSizePtr(&sim_model_ver_tp2_V2_M->solverInfo,
                       &sim_model_ver_tp2_V2_M->Timing.stepSize0);
    rtsiSetdXPtr(&sim_model_ver_tp2_V2_M->solverInfo,
                 &sim_model_ver_tp2_V2_M->derivs);
    rtsiSetContStatesPtr(&sim_model_ver_tp2_V2_M->solverInfo, (real_T **)
                         &sim_model_ver_tp2_V2_M->contStates);
    rtsiSetNumContStatesPtr(&sim_model_ver_tp2_V2_M->solverInfo,
      &sim_model_ver_tp2_V2_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&sim_model_ver_tp2_V2_M->solverInfo,
      &sim_model_ver_tp2_V2_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&sim_model_ver_tp2_V2_M->solverInfo,
      &sim_model_ver_tp2_V2_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&sim_model_ver_tp2_V2_M->solverInfo,
      &sim_model_ver_tp2_V2_M->periodicContStateRanges);
    rtsiSetErrorStatusPtr(&sim_model_ver_tp2_V2_M->solverInfo,
                          (&rtmGetErrorStatus(sim_model_ver_tp2_V2_M)));
    rtsiSetRTModelPtr(&sim_model_ver_tp2_V2_M->solverInfo,
                      sim_model_ver_tp2_V2_M);
  }

  rtsiSetSimTimeStep(&sim_model_ver_tp2_V2_M->solverInfo, MAJOR_TIME_STEP);
  sim_model_ver_tp2_V2_M->intgData.y = sim_model_ver_tp2_V2_M->odeY;
  sim_model_ver_tp2_V2_M->intgData.f[0] = sim_model_ver_tp2_V2_M->odeF[0];
  sim_model_ver_tp2_V2_M->intgData.f[1] = sim_model_ver_tp2_V2_M->odeF[1];
  sim_model_ver_tp2_V2_M->intgData.f[2] = sim_model_ver_tp2_V2_M->odeF[2];
  sim_model_ver_tp2_V2_M->contStates = ((X_sim_model_ver_tp2_V2_T *)
    &sim_model_ver_tp2_V2_X);
  rtsiSetSolverData(&sim_model_ver_tp2_V2_M->solverInfo, (void *)
                    &sim_model_ver_tp2_V2_M->intgData);
  rtsiSetSolverName(&sim_model_ver_tp2_V2_M->solverInfo,"ode3");
  rtmSetTPtr(sim_model_ver_tp2_V2_M, &sim_model_ver_tp2_V2_M->Timing.tArray[0]);
  rtmSetTFinal(sim_model_ver_tp2_V2_M, 36.0);
  sim_model_ver_tp2_V2_M->Timing.stepSize0 = 0.01;

  /* Setup for data logging */
  {
    static RTWLogInfo rt_DataLoggingInfo;
    rt_DataLoggingInfo.loggingInterval = NULL;
    sim_model_ver_tp2_V2_M->rtwLogInfo = &rt_DataLoggingInfo;
  }

  /* Setup for data logging */
  {
    rtliSetLogXSignalInfo(sim_model_ver_tp2_V2_M->rtwLogInfo, (NULL));
    rtliSetLogXSignalPtrs(sim_model_ver_tp2_V2_M->rtwLogInfo, (NULL));
    rtliSetLogT(sim_model_ver_tp2_V2_M->rtwLogInfo, "tout");
    rtliSetLogX(sim_model_ver_tp2_V2_M->rtwLogInfo, "");
    rtliSetLogXFinal(sim_model_ver_tp2_V2_M->rtwLogInfo, "");
    rtliSetLogVarNameModifier(sim_model_ver_tp2_V2_M->rtwLogInfo, "rt_");
    rtliSetLogFormat(sim_model_ver_tp2_V2_M->rtwLogInfo, 4);
    rtliSetLogMaxRows(sim_model_ver_tp2_V2_M->rtwLogInfo, 0);
    rtliSetLogDecimation(sim_model_ver_tp2_V2_M->rtwLogInfo, 1);
    rtliSetLogY(sim_model_ver_tp2_V2_M->rtwLogInfo, "");
    rtliSetLogYSignalInfo(sim_model_ver_tp2_V2_M->rtwLogInfo, (NULL));
    rtliSetLogYSignalPtrs(sim_model_ver_tp2_V2_M->rtwLogInfo, (NULL));
  }

  /* block I/O */
  (void) memset(((void *) &sim_model_ver_tp2_V2_B), 0,
                sizeof(B_sim_model_ver_tp2_V2_T));

  /* states (continuous) */
  {
    (void) memset((void *)&sim_model_ver_tp2_V2_X, 0,
                  sizeof(X_sim_model_ver_tp2_V2_T));
  }

  /* states (dwork) */
  (void) memset((void *)&sim_model_ver_tp2_V2_DW, 0,
                sizeof(DW_sim_model_ver_tp2_V2_T));

  /* Matfile logging */
  rt_StartDataLoggingWithStartTime(sim_model_ver_tp2_V2_M->rtwLogInfo, 0.0,
    rtmGetTFinal(sim_model_ver_tp2_V2_M),
    sim_model_ver_tp2_V2_M->Timing.stepSize0, (&rtmGetErrorStatus
    (sim_model_ver_tp2_V2_M)));

  /* InitializeConditions for Delay: '<Root>/Initial condition X' */
  sim_model_ver_tp2_V2_DW.InitialconditionX_DSTATE =
    sim_model_ver_tp2_V2_P.InitialconditionX_InitialCondit;

  /* InitializeConditions for Delay: '<Root>/Initial condition Y' */
  sim_model_ver_tp2_V2_DW.InitialconditionY_DSTATE =
    sim_model_ver_tp2_V2_P.InitialconditionY_InitialCondit;

  /* InitializeConditions for Integrator: '<S7>/Integrator' */
  sim_model_ver_tp2_V2_X.Integrator_CSTATE =
    sim_model_ver_tp2_V2_P.Integrator_IC;

  /* InitializeConditions for Integrator: '<S7>/Integrator1' */
  sim_model_ver_tp2_V2_X.Integrator1_CSTATE =
    sim_model_ver_tp2_V2_P.Integrator1_IC;

  /* InitializeConditions for Integrator: '<S7>/Integrator2' */
  sim_model_ver_tp2_V2_X.Integrator2_CSTATE =
    sim_model_ver_tp2_V2_P.Integrator2_IC;

  /* InitializeConditions for Derivative: '<S4>/Derivative' */
  sim_model_ver_tp2_V2_DW.TimeStampA = (rtInf);
  sim_model_ver_tp2_V2_DW.TimeStampB = (rtInf);

  /* InitializeConditions for Derivative: '<S4>/Derivative1' */
  sim_model_ver_tp2_V2_DW.TimeStampA_i = (rtInf);
  sim_model_ver_tp2_V2_DW.TimeStampB_c = (rtInf);
}

/* Model terminate function */
void sim_model_ver_tp2_V2_terminate(void)
{
  /* (no terminate code required) */
}
