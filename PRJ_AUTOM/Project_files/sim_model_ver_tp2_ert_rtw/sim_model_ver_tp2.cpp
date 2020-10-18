//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: sim_model_ver_tp2.cpp
//
// Code generated for Simulink model 'sim_model_ver_tp2'.
//
// Model version                  : 1.202
// Simulink Coder version         : 9.4 (R2020b) 29-Jul-2020
// C/C++ source code generated on : Sun Oct 18 13:07:27 2020
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "sim_model_ver_tp2.h"
#include "sim_model_ver_tp2_private.h"

//
// This function updates continuous states using the ODE3 fixed-step
// solver algorithm
//
void sim_model_ver_tp2ModelClass::rt_ertODEUpdateContinuousStates(RTWSolverInfo *
  si )
{
  // Solver Matrices
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
  ODE3_IntgData *id = static_cast<ODE3_IntgData *>(rtsiGetSolverData(si));
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T hB[3];
  int_T i;
  int_T nXc = 3;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  // Save the state values at time t in y, we'll use x as ynew.
  (void) std::memcpy(y, x,
                     static_cast<uint_T>(nXc)*sizeof(real_T));

  // Assumes that rtsiSetT and ModelOutputs are up-to-date
  // f0 = f(t,y)
  rtsiSetdX(si, f0);
  sim_model_ver_tp2_derivatives();

  // f(:,2) = feval(odefile, t + hA(1), y + f*hB(:,1), args(:)(*));
  hB[0] = h * rt_ODE3_B[0][0];
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[0]);
  rtsiSetdX(si, f1);
  this->step();
  sim_model_ver_tp2_derivatives();

  // f(:,3) = feval(odefile, t + hA(2), y + f*hB(:,2), args(:)(*));
  for (i = 0; i <= 1; i++) {
    hB[i] = h * rt_ODE3_B[1][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[1]);
  rtsiSetdX(si, f2);
  this->step();
  sim_model_ver_tp2_derivatives();

  // tnew = t + hA(3);
  // ynew = y + f*hB(:,3);
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

    y = std::atan2(static_cast<real_T>(u0_0), static_cast<real_T>(u1_0));
  } else if (u1 == 0.0) {
    if (u0 > 0.0) {
      y = RT_PI / 2.0;
    } else if (u0 < 0.0) {
      y = -(RT_PI / 2.0);
    } else {
      y = 0.0;
    }
  } else {
    y = std::atan2(u0, u1);
  }

  return y;
}

// Model step function
void sim_model_ver_tp2ModelClass::step()
{
  if (rtmIsMajorTimeStep((&sim_model_ver_tp2_M))) {
    // set solver stop time
    rtsiSetSolverStopTime(&(&sim_model_ver_tp2_M)->solverInfo,
                          (((&sim_model_ver_tp2_M)->Timing.clockTick0+1)*
      (&sim_model_ver_tp2_M)->Timing.stepSize0));
  }                                    // end MajorTimeStep

  // Update absolute time of base rate at minor time step
  if (rtmIsMinorTimeStep((&sim_model_ver_tp2_M))) {
    (&sim_model_ver_tp2_M)->Timing.t[0] = rtsiGetT(&(&sim_model_ver_tp2_M)
      ->solverInfo);
  }

  {
    real_T b_r;
    real_T q;
    real_T rtb_Derivative;
    real_T rtb_Derivative1;
    real_T rtb_Sum1;
    real_T timestamp;
    real_T *lastU;
    boolean_T is_on_part_1_flag;
    boolean_T is_on_part_2_flag;
    boolean_T is_on_part_3_flag;
    boolean_T is_on_part_4_flag;

    // Clock: '<Root>/Time' incorporates:
    //   Derivative: '<S4>/Derivative'
    //   Derivative: '<S4>/Derivative1'

    rtb_Derivative1 = (&sim_model_ver_tp2_M)->Timing.t[0];

    // MATLAB Function: '<Root>/Input block' incorporates:
    //   Clock: '<Root>/Time'

    if (rtIsNaN(rtb_Derivative1)) {
      timestamp = (rtNaN);
    } else if (rtIsInf(rtb_Derivative1)) {
      timestamp = (rtNaN);
    } else if (rtb_Derivative1 == 0.0) {
      timestamp = 0.0;
    } else {
      timestamp = std::fmod(rtb_Derivative1, 30.0);
      if (timestamp == 0.0) {
        timestamp = 0.0;
      } else {
        if (rtb_Derivative1 < 0.0) {
          timestamp += 30.0;
        }
      }
    }

    is_on_part_1_flag = (timestamp <= 10.0);
    if (timestamp > 10.0) {
      if (timestamp <= 15.0) {
        is_on_part_2_flag = true;
        is_on_part_3_flag = false;
      } else {
        is_on_part_2_flag = false;
        is_on_part_3_flag = (timestamp <= 25.0);
      }
    } else {
      is_on_part_2_flag = false;
      is_on_part_3_flag = false;
    }

    is_on_part_4_flag = ((!is_on_part_1_flag) && (!is_on_part_2_flag) &&
                         (!is_on_part_3_flag));
    timestamp = (((timestamp - 10.0) * static_cast<real_T>(is_on_part_2_flag) +
                  static_cast<real_T>(is_on_part_1_flag) * timestamp) +
                 ((timestamp - 10.0) - 5.0) * static_cast<real_T>
                 (is_on_part_3_flag)) + (timestamp - 25.0) * static_cast<real_T>
      (is_on_part_4_flag);
    rtb_Sum1 = std::sin(3.1415926535897931 * timestamp / 5.0);
    sim_model_ver_tp2_B.y = (((2.5 * rtb_Sum1 + 10.0) * static_cast<real_T>
      (is_on_part_2_flag) + static_cast<real_T>(is_on_part_1_flag) * timestamp)
      + (10.0 - timestamp) * static_cast<real_T>(is_on_part_3_flag)) + -2.5 *
      rtb_Sum1 * static_cast<real_T>(is_on_part_4_flag);
    sim_model_ver_tp2_B.x = static_cast<real_T>(5.0 - timestamp > 0.0) * (5.0 -
      timestamp) * static_cast<real_T>(is_on_part_4_flag) + (static_cast<real_T>
      (is_on_part_2_flag) * timestamp + static_cast<real_T>(is_on_part_3_flag) *
      5.0);

    // End of MATLAB Function: '<Root>/Input block'

    // Sum: '<S4>/Sum' incorporates:
    //   Integrator: '<S7>/Integrator'

    timestamp = sim_model_ver_tp2_B.x - sim_model_ver_tp2_X.Integrator_CSTATE;

    // Sum: '<S4>/Sum1' incorporates:
    //   Integrator: '<S7>/Integrator1'

    rtb_Sum1 = sim_model_ver_tp2_B.y - sim_model_ver_tp2_X.Integrator1_CSTATE;

    // Derivative: '<S4>/Derivative'
    if ((sim_model_ver_tp2_DW.TimeStampA >= rtb_Derivative1) &&
        (sim_model_ver_tp2_DW.TimeStampB >= rtb_Derivative1)) {
      rtb_Derivative = 0.0;
    } else {
      q = sim_model_ver_tp2_DW.TimeStampA;
      lastU = &sim_model_ver_tp2_DW.LastUAtTimeA;
      if (sim_model_ver_tp2_DW.TimeStampA < sim_model_ver_tp2_DW.TimeStampB) {
        if (sim_model_ver_tp2_DW.TimeStampB < rtb_Derivative1) {
          q = sim_model_ver_tp2_DW.TimeStampB;
          lastU = &sim_model_ver_tp2_DW.LastUAtTimeB;
        }
      } else {
        if (sim_model_ver_tp2_DW.TimeStampA >= rtb_Derivative1) {
          q = sim_model_ver_tp2_DW.TimeStampB;
          lastU = &sim_model_ver_tp2_DW.LastUAtTimeB;
        }
      }

      rtb_Derivative = (sim_model_ver_tp2_B.y - *lastU) / (rtb_Derivative1 - q);
    }

    // Derivative: '<S4>/Derivative1'
    if ((sim_model_ver_tp2_DW.TimeStampA_i >= rtb_Derivative1) &&
        (sim_model_ver_tp2_DW.TimeStampB_c >= rtb_Derivative1)) {
      rtb_Derivative1 = 0.0;
    } else {
      q = sim_model_ver_tp2_DW.TimeStampA_i;
      lastU = &sim_model_ver_tp2_DW.LastUAtTimeA_g;
      if (sim_model_ver_tp2_DW.TimeStampA_i < sim_model_ver_tp2_DW.TimeStampB_c)
      {
        if (sim_model_ver_tp2_DW.TimeStampB_c < rtb_Derivative1) {
          q = sim_model_ver_tp2_DW.TimeStampB_c;
          lastU = &sim_model_ver_tp2_DW.LastUAtTimeB_e;
        }
      } else {
        if (sim_model_ver_tp2_DW.TimeStampA_i >= rtb_Derivative1) {
          q = sim_model_ver_tp2_DW.TimeStampB_c;
          lastU = &sim_model_ver_tp2_DW.LastUAtTimeB_e;
        }
      }

      rtb_Derivative1 = (sim_model_ver_tp2_B.x - *lastU) / (rtb_Derivative1 - q);
    }

    // MATLAB Function: '<S4>/MATLAB Function'
    rtb_Derivative = rt_atan2d_snf(rtb_Derivative, rtb_Derivative1);
    if (rtIsNaN(rtb_Derivative)) {
      rtb_Derivative1 = (rtNaN);
    } else if (rtIsInf(rtb_Derivative)) {
      rtb_Derivative1 = (rtNaN);
    } else if (rtb_Derivative == 0.0) {
      rtb_Derivative1 = 0.0;
    } else {
      rtb_Derivative1 = std::fmod(rtb_Derivative, 6.2831853071795862);
      is_on_part_1_flag = (rtb_Derivative1 == 0.0);
      if (!is_on_part_1_flag) {
        q = std::abs(rtb_Derivative / 6.2831853071795862);
        is_on_part_1_flag = !(std::abs(q - std::floor(q + 0.5)) >
                              2.2204460492503131E-16 * q);
      }

      if (is_on_part_1_flag) {
        rtb_Derivative1 = 0.0;
      } else {
        if (rtb_Derivative < 0.0) {
          rtb_Derivative1 += 6.2831853071795862;
        }
      }
    }

    // End of MATLAB Function: '<S4>/MATLAB Function'

    // MATLAB Function: '<S5>/MATLAB Function' incorporates:
    //   Integrator: '<S7>/Integrator2'

    rtb_Derivative = rt_atan2d_snf(rtb_Sum1, timestamp);
    if (rtIsNaN(rtb_Derivative)) {
      b_r = (rtNaN);
    } else if (rtIsInf(rtb_Derivative)) {
      b_r = (rtNaN);
    } else if (rtb_Derivative == 0.0) {
      b_r = 0.0;
    } else {
      b_r = std::fmod(rtb_Derivative, 6.2831853071795862);
      is_on_part_1_flag = (b_r == 0.0);
      if (!is_on_part_1_flag) {
        q = std::abs(rtb_Derivative / 6.2831853071795862);
        is_on_part_1_flag = !(std::abs(q - std::floor(q + 0.5)) >
                              2.2204460492503131E-16 * q);
      }

      if (is_on_part_1_flag) {
        b_r = 0.0;
      } else {
        if (rtb_Derivative < 0.0) {
          b_r += 6.2831853071795862;
        }
      }
    }

    rtb_Derivative = b_r - sim_model_ver_tp2_X.Integrator2_CSTATE;

    // MATLAB Function: '<S5>/MATLAB Function1' incorporates:
    //   MATLAB Function: '<S5>/MATLAB Function'

    timestamp = std::sqrt(timestamp * timestamp + rtb_Sum1 * rtb_Sum1) * 5.5;

    // MATLAB Function: '<S7>/MATLAB Function' incorporates:
    //   Integrator: '<S7>/Integrator2'
    //   MATLAB Function: '<S5>/MATLAB Function'
    //   MATLAB Function: '<S5>/MATLAB Function1'
    //   Sum: '<S4>/Sum2'

    sim_model_ver_tp2_B.d_x = timestamp * std::cos
      (sim_model_ver_tp2_X.Integrator2_CSTATE);
    sim_model_ver_tp2_B.d_y = timestamp * std::sin
      (sim_model_ver_tp2_X.Integrator2_CSTATE);
    sim_model_ver_tp2_B.d_theta = 220.0 * rtb_Derivative - ((rtb_Derivative1 -
      sim_model_ver_tp2_X.Integrator2_CSTATE) - rtb_Derivative) * 2.0;
  }

  if (rtmIsMajorTimeStep((&sim_model_ver_tp2_M))) {
    real_T *lastU;

    // Update for Derivative: '<S4>/Derivative'
    if (sim_model_ver_tp2_DW.TimeStampA == (rtInf)) {
      sim_model_ver_tp2_DW.TimeStampA = (&sim_model_ver_tp2_M)->Timing.t[0];
      lastU = &sim_model_ver_tp2_DW.LastUAtTimeA;
    } else if (sim_model_ver_tp2_DW.TimeStampB == (rtInf)) {
      sim_model_ver_tp2_DW.TimeStampB = (&sim_model_ver_tp2_M)->Timing.t[0];
      lastU = &sim_model_ver_tp2_DW.LastUAtTimeB;
    } else if (sim_model_ver_tp2_DW.TimeStampA < sim_model_ver_tp2_DW.TimeStampB)
    {
      sim_model_ver_tp2_DW.TimeStampA = (&sim_model_ver_tp2_M)->Timing.t[0];
      lastU = &sim_model_ver_tp2_DW.LastUAtTimeA;
    } else {
      sim_model_ver_tp2_DW.TimeStampB = (&sim_model_ver_tp2_M)->Timing.t[0];
      lastU = &sim_model_ver_tp2_DW.LastUAtTimeB;
    }

    *lastU = sim_model_ver_tp2_B.y;

    // End of Update for Derivative: '<S4>/Derivative'

    // Update for Derivative: '<S4>/Derivative1'
    if (sim_model_ver_tp2_DW.TimeStampA_i == (rtInf)) {
      sim_model_ver_tp2_DW.TimeStampA_i = (&sim_model_ver_tp2_M)->Timing.t[0];
      lastU = &sim_model_ver_tp2_DW.LastUAtTimeA_g;
    } else if (sim_model_ver_tp2_DW.TimeStampB_c == (rtInf)) {
      sim_model_ver_tp2_DW.TimeStampB_c = (&sim_model_ver_tp2_M)->Timing.t[0];
      lastU = &sim_model_ver_tp2_DW.LastUAtTimeB_e;
    } else if (sim_model_ver_tp2_DW.TimeStampA_i <
               sim_model_ver_tp2_DW.TimeStampB_c) {
      sim_model_ver_tp2_DW.TimeStampA_i = (&sim_model_ver_tp2_M)->Timing.t[0];
      lastU = &sim_model_ver_tp2_DW.LastUAtTimeA_g;
    } else {
      sim_model_ver_tp2_DW.TimeStampB_c = (&sim_model_ver_tp2_M)->Timing.t[0];
      lastU = &sim_model_ver_tp2_DW.LastUAtTimeB_e;
    }

    *lastU = sim_model_ver_tp2_B.x;

    // End of Update for Derivative: '<S4>/Derivative1'
  }                                    // end MajorTimeStep

  if (rtmIsMajorTimeStep((&sim_model_ver_tp2_M))) {
    rt_ertODEUpdateContinuousStates(&(&sim_model_ver_tp2_M)->solverInfo);

    // Update absolute time for base rate
    // The "clockTick0" counts the number of times the code of this task has
    //  been executed. The absolute time is the multiplication of "clockTick0"
    //  and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
    //  overflow during the application lifespan selected.

    ++(&sim_model_ver_tp2_M)->Timing.clockTick0;
    (&sim_model_ver_tp2_M)->Timing.t[0] = rtsiGetSolverStopTime
      (&(&sim_model_ver_tp2_M)->solverInfo);

    {
      // Update absolute timer for sample time: [0.01s, 0.0s]
      // The "clockTick1" counts the number of times the code of this task has
      //  been executed. The resolution of this integer timer is 0.01, which is the step size
      //  of the task. Size of "clockTick1" ensures timer will not overflow during the
      //  application lifespan selected.

      (&sim_model_ver_tp2_M)->Timing.clockTick1++;
    }
  }                                    // end MajorTimeStep
}

// Derivatives for root system: '<Root>'
void sim_model_ver_tp2ModelClass::sim_model_ver_tp2_derivatives()
{
  sim_model_ver_tp2ModelClass::XDot_sim_model_ver_tp2_T *_rtXdot;
  _rtXdot = ((XDot_sim_model_ver_tp2_T *) (&sim_model_ver_tp2_M)->derivs);

  // Derivatives for Integrator: '<S7>/Integrator'
  _rtXdot->Integrator_CSTATE = sim_model_ver_tp2_B.d_x;

  // Derivatives for Integrator: '<S7>/Integrator1'
  _rtXdot->Integrator1_CSTATE = sim_model_ver_tp2_B.d_y;

  // Derivatives for Integrator: '<S7>/Integrator2'
  _rtXdot->Integrator2_CSTATE = sim_model_ver_tp2_B.d_theta;
}

// Model initialize function
void sim_model_ver_tp2ModelClass::initialize()
{
  // Registration code

  // initialize non-finites
  rt_InitInfAndNaN(sizeof(real_T));

  {
    // Setup solver object
    rtsiSetSimTimeStepPtr(&(&sim_model_ver_tp2_M)->solverInfo,
                          &(&sim_model_ver_tp2_M)->Timing.simTimeStep);
    rtsiSetTPtr(&(&sim_model_ver_tp2_M)->solverInfo, &rtmGetTPtr
                ((&sim_model_ver_tp2_M)));
    rtsiSetStepSizePtr(&(&sim_model_ver_tp2_M)->solverInfo,
                       &(&sim_model_ver_tp2_M)->Timing.stepSize0);
    rtsiSetdXPtr(&(&sim_model_ver_tp2_M)->solverInfo, &(&sim_model_ver_tp2_M)
                 ->derivs);
    rtsiSetContStatesPtr(&(&sim_model_ver_tp2_M)->solverInfo, (real_T **)
                         &(&sim_model_ver_tp2_M)->contStates);
    rtsiSetNumContStatesPtr(&(&sim_model_ver_tp2_M)->solverInfo,
      &(&sim_model_ver_tp2_M)->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&(&sim_model_ver_tp2_M)->solverInfo,
      &(&sim_model_ver_tp2_M)->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&(&sim_model_ver_tp2_M)->solverInfo,
      &(&sim_model_ver_tp2_M)->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&(&sim_model_ver_tp2_M)->solverInfo,
      &(&sim_model_ver_tp2_M)->periodicContStateRanges);
    rtsiSetErrorStatusPtr(&(&sim_model_ver_tp2_M)->solverInfo,
                          (&rtmGetErrorStatus((&sim_model_ver_tp2_M))));
    rtsiSetRTModelPtr(&(&sim_model_ver_tp2_M)->solverInfo, (&sim_model_ver_tp2_M));
  }

  rtsiSetSimTimeStep(&(&sim_model_ver_tp2_M)->solverInfo, MAJOR_TIME_STEP);
  (&sim_model_ver_tp2_M)->intgData.y = (&sim_model_ver_tp2_M)->odeY;
  (&sim_model_ver_tp2_M)->intgData.f[0] = (&sim_model_ver_tp2_M)->odeF[0];
  (&sim_model_ver_tp2_M)->intgData.f[1] = (&sim_model_ver_tp2_M)->odeF[1];
  (&sim_model_ver_tp2_M)->intgData.f[2] = (&sim_model_ver_tp2_M)->odeF[2];
  (&sim_model_ver_tp2_M)->contStates = ((X_sim_model_ver_tp2_T *)
    &sim_model_ver_tp2_X);
  rtsiSetSolverData(&(&sim_model_ver_tp2_M)->solverInfo, static_cast<void *>
                    (&(&sim_model_ver_tp2_M)->intgData));
  rtsiSetSolverName(&(&sim_model_ver_tp2_M)->solverInfo,"ode3");
  rtmSetTPtr((&sim_model_ver_tp2_M), &(&sim_model_ver_tp2_M)->Timing.tArray[0]);
  (&sim_model_ver_tp2_M)->Timing.stepSize0 = 0.01;

  // InitializeConditions for Integrator: '<S7>/Integrator'
  sim_model_ver_tp2_X.Integrator_CSTATE = 0.0;

  // InitializeConditions for Integrator: '<S7>/Integrator1'
  sim_model_ver_tp2_X.Integrator1_CSTATE = 0.0;

  // InitializeConditions for Integrator: '<S7>/Integrator2'
  sim_model_ver_tp2_X.Integrator2_CSTATE = 0.0;

  // InitializeConditions for Derivative: '<S4>/Derivative'
  sim_model_ver_tp2_DW.TimeStampA = (rtInf);
  sim_model_ver_tp2_DW.TimeStampB = (rtInf);

  // InitializeConditions for Derivative: '<S4>/Derivative1'
  sim_model_ver_tp2_DW.TimeStampA_i = (rtInf);
  sim_model_ver_tp2_DW.TimeStampB_c = (rtInf);
}

// Model terminate function
void sim_model_ver_tp2ModelClass::terminate()
{
  // (no terminate code required)
}

// Constructor
sim_model_ver_tp2ModelClass::sim_model_ver_tp2ModelClass() :
  sim_model_ver_tp2_B(),
  sim_model_ver_tp2_DW(),
  sim_model_ver_tp2_X(),
  sim_model_ver_tp2_M()
{
  // Currently there is no constructor body generated.
}

// Destructor
sim_model_ver_tp2ModelClass::~sim_model_ver_tp2ModelClass()
{
  // Currently there is no destructor body generated.
}

// Real-Time Model get method
sim_model_ver_tp2ModelClass::RT_MODEL_sim_model_ver_tp2_T
  * sim_model_ver_tp2ModelClass::getRTM()
{
  return (&sim_model_ver_tp2_M);
}

//
// File trailer for generated code.
//
// [EOF]
//
