//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: sim_model_ver_tp2.h
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
#ifndef RTW_HEADER_sim_model_ver_tp2_h_
#define RTW_HEADER_sim_model_ver_tp2_h_
#include <cmath>
#include <cstring>
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "sim_model_ver_tp2_types.h"
#include "rtGetInf.h"
#include "rt_nonfinite.h"
#include "rt_defines.h"

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
#define rtmGetStopRequested(rtm)       ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
#define rtmSetStopRequested(rtm, val)  ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
#define rtmGetStopRequestedPtr(rtm)    (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
#define rtmGetT(rtm)                   (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmGetTPtr
#define rtmGetTPtr(rtm)                ((rtm)->Timing.t)
#endif

#ifndef ODE3_INTG
#define ODE3_INTG

// ODE3 Integration Data
typedef struct {
  real_T *y;                           // output
  real_T *f[3];                        // derivatives
} ODE3_IntgData;

#endif

// Class declaration for model sim_model_ver_tp2
class sim_model_ver_tp2ModelClass {
  // public data and function members
 public:
  // Block signals (default storage)
  typedef struct {
    real_T d_x;                        // '<S7>/MATLAB Function'
    real_T d_y;                        // '<S7>/MATLAB Function'
    real_T d_theta;                    // '<S7>/MATLAB Function'
    real_T x;                          // '<Root>/Input block'
    real_T y;                          // '<Root>/Input block'
  } B_sim_model_ver_tp2_T;

  // Block states (default storage) for system '<Root>'
  typedef struct {
    real_T TimeStampA;                 // '<S4>/Derivative'
    real_T LastUAtTimeA;               // '<S4>/Derivative'
    real_T TimeStampB;                 // '<S4>/Derivative'
    real_T LastUAtTimeB;               // '<S4>/Derivative'
    real_T TimeStampA_i;               // '<S4>/Derivative1'
    real_T LastUAtTimeA_g;             // '<S4>/Derivative1'
    real_T TimeStampB_c;               // '<S4>/Derivative1'
    real_T LastUAtTimeB_e;             // '<S4>/Derivative1'
  } DW_sim_model_ver_tp2_T;

  // Continuous states (default storage)
  typedef struct {
    real_T Integrator_CSTATE;          // '<S7>/Integrator'
    real_T Integrator1_CSTATE;         // '<S7>/Integrator1'
    real_T Integrator2_CSTATE;         // '<S7>/Integrator2'
  } X_sim_model_ver_tp2_T;

  // State derivatives (default storage)
  typedef struct {
    real_T Integrator_CSTATE;          // '<S7>/Integrator'
    real_T Integrator1_CSTATE;         // '<S7>/Integrator1'
    real_T Integrator2_CSTATE;         // '<S7>/Integrator2'
  } XDot_sim_model_ver_tp2_T;

  // State disabled
  typedef struct {
    boolean_T Integrator_CSTATE;       // '<S7>/Integrator'
    boolean_T Integrator1_CSTATE;      // '<S7>/Integrator1'
    boolean_T Integrator2_CSTATE;      // '<S7>/Integrator2'
  } XDis_sim_model_ver_tp2_T;

  // Real-time Model Data Structure
  struct RT_MODEL_sim_model_ver_tp2_T {
    const char_T *errorStatus;
    RTWSolverInfo solverInfo;
    X_sim_model_ver_tp2_T *contStates;
    int_T *periodicContStateIndices;
    real_T *periodicContStateRanges;
    real_T *derivs;
    boolean_T *contStateDisabled;
    boolean_T zCCacheNeedsReset;
    boolean_T derivCacheNeedsReset;
    boolean_T CTOutputIncnstWithState;
    real_T odeY[3];
    real_T odeF[3][3];
    ODE3_IntgData intgData;

    //
    //  Sizes:
    //  The following substructure contains sizes information
    //  for many of the model attributes such as inputs, outputs,
    //  dwork, sample times, etc.

    struct {
      int_T numContStates;
      int_T numPeriodicContStates;
      int_T numSampTimes;
    } Sizes;

    //
    //  Timing:
    //  The following substructure contains information regarding
    //  the timing information for the model.

    struct {
      uint32_T clockTick0;
      time_T stepSize0;
      uint32_T clockTick1;
      SimTimeStep simTimeStep;
      boolean_T stopRequestedFlag;
      time_T *t;
      time_T tArray[2];
    } Timing;
  };

  // model initialize function
  void initialize();

  // model step function
  void step();

  // model terminate function
  void terminate();

  // Constructor
  sim_model_ver_tp2ModelClass();

  // Destructor
  ~sim_model_ver_tp2ModelClass();

  // Real-Time Model get method
  sim_model_ver_tp2ModelClass::RT_MODEL_sim_model_ver_tp2_T * getRTM();

  // private data and function members
 private:
  // Block signals
  B_sim_model_ver_tp2_T sim_model_ver_tp2_B;

  // Block states
  DW_sim_model_ver_tp2_T sim_model_ver_tp2_DW;
  X_sim_model_ver_tp2_T sim_model_ver_tp2_X;// Block continuous states

  // Real-Time Model
  RT_MODEL_sim_model_ver_tp2_T sim_model_ver_tp2_M;

  // Continuous states update member function
  void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si );

  // Derivatives member function
  void sim_model_ver_tp2_derivatives();
};

//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'sim_model_ver_tp2'
//  '<S1>'   : 'sim_model_ver_tp2/Input'
//  '<S2>'   : 'sim_model_ver_tp2/Input block'
//  '<S3>'   : 'sim_model_ver_tp2/Output'
//  '<S4>'   : 'sim_model_ver_tp2/System'
//  '<S5>'   : 'sim_model_ver_tp2/System/Control block'
//  '<S6>'   : 'sim_model_ver_tp2/System/MATLAB Function'
//  '<S7>'   : 'sim_model_ver_tp2/System/System block'
//  '<S8>'   : 'sim_model_ver_tp2/System/Control block/MATLAB Function'
//  '<S9>'   : 'sim_model_ver_tp2/System/Control block/MATLAB Function1'
//  '<S10>'  : 'sim_model_ver_tp2/System/System block/MATLAB Function'

#endif                                 // RTW_HEADER_sim_model_ver_tp2_h_

//
// File trailer for generated code.
//
// [EOF]
//
