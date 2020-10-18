//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: untitled.cpp
//
// Code generated for Simulink model 'untitled'.
//
// Model version                  : 1.7
// Simulink Coder version         : 9.4 (R2020b) 29-Jul-2020
// C/C++ source code generated on : Sun Oct 18 13:35:58 2020
//
// Target selection: ert.tlc
// Embedded hardware selection: Intel->x86-64 (Windows64)
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "untitled.h"
#include "untitled_private.h"

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
void untitledModelClass::step()
{
  real_T b_r;
  real_T q;
  real_T r;
  real_T rtb_theta_r;
  boolean_T rEQ0;

  // Outport: '<Root>/x' incorporates:
  //   DiscreteIntegrator: '<S7>/Discrete-Time Integrator'

  untitled_Y.x = untitled_DW.DiscreteTimeIntegrator_DSTATE;

  // MATLAB Function: '<S2>/MATLAB Function' incorporates:
  //   Sum: '<S4>/Diff'
  //   Sum: '<S5>/Diff'
  //   UnitDelay: '<S4>/UD'
  //   UnitDelay: '<S5>/UD'
  //
  //  Block description for '<S4>/Diff':
  //
  //   Add in CPU
  //
  //  Block description for '<S5>/Diff':
  //
  //   Add in CPU
  //
  //  Block description for '<S4>/UD':
  //
  //   Store in Global RAM
  //
  //  Block description for '<S5>/UD':
  //
  //   Store in Global RAM

  r = rt_atan2d_snf(5.0 - untitled_DW.UD_DSTATE, 0.0 - untitled_DW.UD_DSTATE_i);
  if (rtIsNaN(r)) {
    rtb_theta_r = (rtNaN);
  } else if (rtIsInf(r)) {
    rtb_theta_r = (rtNaN);
  } else if (r == 0.0) {
    rtb_theta_r = 0.0;
  } else {
    rtb_theta_r = std::fmod(r, 6.2831853071795862);
    rEQ0 = (rtb_theta_r == 0.0);
    if (!rEQ0) {
      q = std::abs(r / 6.2831853071795862);
      rEQ0 = !(std::abs(q - std::floor(q + 0.5)) > 2.2204460492503131E-16 * q);
    }

    if (rEQ0) {
      rtb_theta_r = 0.0;
    } else {
      if (r < 0.0) {
        rtb_theta_r += 6.2831853071795862;
      }
    }
  }

  // End of MATLAB Function: '<S2>/MATLAB Function'

  // MATLAB Function: '<S3>/MATLAB Function' incorporates:
  //   DiscreteIntegrator: '<S7>/Discrete-Time Integrator'
  //   DiscreteIntegrator: '<S7>/Discrete-Time Integrator1'
  //   DiscreteIntegrator: '<S7>/Discrete-Time Integrator2'
  //   Sum: '<S2>/Sum'
  //   Sum: '<S2>/Sum1'

  r = rt_atan2d_snf(1.0 - untitled_DW.DiscreteTimeIntegrator1_DSTATE, 0.0 -
                    untitled_DW.DiscreteTimeIntegrator_DSTATE);
  if (rtIsNaN(r)) {
    b_r = (rtNaN);
  } else if (rtIsInf(r)) {
    b_r = (rtNaN);
  } else if (r == 0.0) {
    b_r = 0.0;
  } else {
    b_r = std::fmod(r, 6.2831853071795862);
    rEQ0 = (b_r == 0.0);
    if (!rEQ0) {
      q = std::abs(r / 6.2831853071795862);
      rEQ0 = !(std::abs(q - std::floor(q + 0.5)) > 2.2204460492503131E-16 * q);
    }

    if (rEQ0) {
      b_r = 0.0;
    } else {
      if (r < 0.0) {
        b_r += 6.2831853071795862;
      }
    }
  }

  r = b_r - untitled_DW.DiscreteTimeIntegrator2_DSTATE;

  // MATLAB Function: '<S3>/MATLAB Function1' incorporates:
  //   DiscreteIntegrator: '<S7>/Discrete-Time Integrator'
  //   DiscreteIntegrator: '<S7>/Discrete-Time Integrator1'
  //   MATLAB Function: '<S3>/MATLAB Function'
  //   Sum: '<S2>/Sum'
  //   Sum: '<S2>/Sum1'

  q = std::sqrt((0.0 - untitled_DW.DiscreteTimeIntegrator_DSTATE) * (0.0 -
    untitled_DW.DiscreteTimeIntegrator_DSTATE) + (1.0 -
    untitled_DW.DiscreteTimeIntegrator1_DSTATE) * (1.0 -
    untitled_DW.DiscreteTimeIntegrator1_DSTATE)) * 5.5;

  // Outport: '<Root>/y' incorporates:
  //   DiscreteIntegrator: '<S7>/Discrete-Time Integrator1'

  untitled_Y.y = untitled_DW.DiscreteTimeIntegrator1_DSTATE;

  // Update for DiscreteIntegrator: '<S7>/Discrete-Time Integrator' incorporates:
  //   DiscreteIntegrator: '<S7>/Discrete-Time Integrator2'
  //   MATLAB Function: '<S7>/MATLAB Function'

  untitled_DW.DiscreteTimeIntegrator_DSTATE += q * std::cos
    (untitled_DW.DiscreteTimeIntegrator2_DSTATE) * 0.2;

  // Update for DiscreteIntegrator: '<S7>/Discrete-Time Integrator1' incorporates:
  //   DiscreteIntegrator: '<S7>/Discrete-Time Integrator2'
  //   MATLAB Function: '<S7>/MATLAB Function'

  untitled_DW.DiscreteTimeIntegrator1_DSTATE += q * std::sin
    (untitled_DW.DiscreteTimeIntegrator2_DSTATE) * 0.2;

  // Update for DiscreteIntegrator: '<S7>/Discrete-Time Integrator2' incorporates:
  //   MATLAB Function: '<S3>/MATLAB Function'
  //   MATLAB Function: '<S3>/MATLAB Function1'
  //   Sum: '<S2>/Sum2'

  untitled_DW.DiscreteTimeIntegrator2_DSTATE += (220.0 * r - ((rtb_theta_r -
    untitled_DW.DiscreteTimeIntegrator2_DSTATE) - r) * 2.0) * 0.2;

  // Update for UnitDelay: '<S4>/UD'
  //
  //  Block description for '<S4>/UD':
  //
  //   Store in Global RAM

  untitled_DW.UD_DSTATE = 5.0;

  // Update for UnitDelay: '<S5>/UD'
  //
  //  Block description for '<S5>/UD':
  //
  //   Store in Global RAM

  untitled_DW.UD_DSTATE_i = 0.0;
}

// Model initialize function
void untitledModelClass::initialize()
{
  // Registration code

  // initialize non-finites
  rt_InitInfAndNaN(sizeof(real_T));
}

// Model terminate function
void untitledModelClass::terminate()
{
  // (no terminate code required)
}

// Constructor
untitledModelClass::untitledModelClass() :
  untitled_DW(),
  untitled_Y(),
  untitled_M()
{
  // Currently there is no constructor body generated.
}

// Destructor
untitledModelClass::~untitledModelClass()
{
  // Currently there is no destructor body generated.
}

// Real-Time Model get method
untitledModelClass::RT_MODEL_untitled_T * untitledModelClass::getRTM()
{
  return (&untitled_M);
}

//
// File trailer for generated code.
//
// [EOF]
//
