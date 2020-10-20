/*
 * sim_model_ver_tp2_V2.h
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

#ifndef RTW_HEADER_sim_model_ver_tp2_V2_h_
#define RTW_HEADER_sim_model_ver_tp2_V2_h_
#include <math.h>
#include <float.h>
#include <string.h>
#include <stddef.h>
#ifndef sim_model_ver_tp2_V2_COMMON_INCLUDES_
# define sim_model_ver_tp2_V2_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rtw_continuous.h"
#include "rtw_solver.h"
#include "rt_logging.h"
#endif                               /* sim_model_ver_tp2_V2_COMMON_INCLUDES_ */

#include "sim_model_ver_tp2_V2_types.h"

/* Shared type includes */
#include "multiword_types.h"
#include "rtGetInf.h"
#include "rt_nonfinite.h"
#include "rt_defines.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetContStateDisabled
# define rtmGetContStateDisabled(rtm)  ((rtm)->contStateDisabled)
#endif

#ifndef rtmSetContStateDisabled
# define rtmSetContStateDisabled(rtm, val) ((rtm)->contStateDisabled = (val))
#endif

#ifndef rtmGetContStates
# define rtmGetContStates(rtm)         ((rtm)->contStates)
#endif

#ifndef rtmSetContStates
# define rtmSetContStates(rtm, val)    ((rtm)->contStates = (val))
#endif

#ifndef rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag
# define rtmGetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm) ((rtm)->CTOutputIncnstWithState)
#endif

#ifndef rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag
# define rtmSetContTimeOutputInconsistentWithStateAtMajorStepFlag(rtm, val) ((rtm)->CTOutputIncnstWithState = (val))
#endif

#ifndef rtmGetDerivCacheNeedsReset
# define rtmGetDerivCacheNeedsReset(rtm) ((rtm)->derivCacheNeedsReset)
#endif

#ifndef rtmSetDerivCacheNeedsReset
# define rtmSetDerivCacheNeedsReset(rtm, val) ((rtm)->derivCacheNeedsReset = (val))
#endif

#ifndef rtmGetFinalTime
# define rtmGetFinalTime(rtm)          ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetIntgData
# define rtmGetIntgData(rtm)           ((rtm)->intgData)
#endif

#ifndef rtmSetIntgData
# define rtmSetIntgData(rtm, val)      ((rtm)->intgData = (val))
#endif

#ifndef rtmGetOdeF
# define rtmGetOdeF(rtm)               ((rtm)->odeF)
#endif

#ifndef rtmSetOdeF
# define rtmSetOdeF(rtm, val)          ((rtm)->odeF = (val))
#endif

#ifndef rtmGetOdeY
# define rtmGetOdeY(rtm)               ((rtm)->odeY)
#endif

#ifndef rtmSetOdeY
# define rtmSetOdeY(rtm, val)          ((rtm)->odeY = (val))
#endif

#ifndef rtmGetPeriodicContStateIndices
# define rtmGetPeriodicContStateIndices(rtm) ((rtm)->periodicContStateIndices)
#endif

#ifndef rtmSetPeriodicContStateIndices
# define rtmSetPeriodicContStateIndices(rtm, val) ((rtm)->periodicContStateIndices = (val))
#endif

#ifndef rtmGetPeriodicContStateRanges
# define rtmGetPeriodicContStateRanges(rtm) ((rtm)->periodicContStateRanges)
#endif

#ifndef rtmSetPeriodicContStateRanges
# define rtmSetPeriodicContStateRanges(rtm, val) ((rtm)->periodicContStateRanges = (val))
#endif

#ifndef rtmGetRTWLogInfo
# define rtmGetRTWLogInfo(rtm)         ((rtm)->rtwLogInfo)
#endif

#ifndef rtmGetZCCacheNeedsReset
# define rtmGetZCCacheNeedsReset(rtm)  ((rtm)->zCCacheNeedsReset)
#endif

#ifndef rtmSetZCCacheNeedsReset
# define rtmSetZCCacheNeedsReset(rtm, val) ((rtm)->zCCacheNeedsReset = (val))
#endif

#ifndef rtmGetdX
# define rtmGetdX(rtm)                 ((rtm)->derivs)
#endif

#ifndef rtmSetdX
# define rtmSetdX(rtm, val)            ((rtm)->derivs = (val))
#endif

#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

#ifndef rtmGetStopRequested
# define rtmGetStopRequested(rtm)      ((rtm)->Timing.stopRequestedFlag)
#endif

#ifndef rtmSetStopRequested
# define rtmSetStopRequested(rtm, val) ((rtm)->Timing.stopRequestedFlag = (val))
#endif

#ifndef rtmGetStopRequestedPtr
# define rtmGetStopRequestedPtr(rtm)   (&((rtm)->Timing.stopRequestedFlag))
#endif

#ifndef rtmGetT
# define rtmGetT(rtm)                  (rtmGetTPtr((rtm))[0])
#endif

#ifndef rtmGetTFinal
# define rtmGetTFinal(rtm)             ((rtm)->Timing.tFinal)
#endif

#ifndef rtmGetTPtr
# define rtmGetTPtr(rtm)               ((rtm)->Timing.t)
#endif

/* Block signals (default storage) */
typedef struct {
  real_T d_x;                          /* '<S7>/MATLAB Function' */
  real_T d_y;                          /* '<S7>/MATLAB Function' */
  real_T d_theta;                      /* '<S7>/MATLAB Function' */
  real_T x;                            /* '<Root>/Input block' */
  real_T y;                            /* '<Root>/Input block' */
} B_sim_model_ver_tp2_V2_T;

/* Block states (default storage) for system '<Root>' */
typedef struct {
  real_T InitialconditionX_DSTATE;     /* '<Root>/Initial condition X' */
  real_T InitialconditionY_DSTATE;     /* '<Root>/Initial condition Y' */
  real_T TimeStampA;                   /* '<S4>/Derivative' */
  real_T LastUAtTimeA;                 /* '<S4>/Derivative' */
  real_T TimeStampB;                   /* '<S4>/Derivative' */
  real_T LastUAtTimeB;                 /* '<S4>/Derivative' */
  real_T TimeStampA_i;                 /* '<S4>/Derivative1' */
  real_T LastUAtTimeA_g;               /* '<S4>/Derivative1' */
  real_T TimeStampB_c;                 /* '<S4>/Derivative1' */
  real_T LastUAtTimeB_e;               /* '<S4>/Derivative1' */
} DW_sim_model_ver_tp2_V2_T;

/* Continuous states (default storage) */
typedef struct {
  real_T Integrator_CSTATE;            /* '<S7>/Integrator' */
  real_T Integrator1_CSTATE;           /* '<S7>/Integrator1' */
  real_T Integrator2_CSTATE;           /* '<S7>/Integrator2' */
} X_sim_model_ver_tp2_V2_T;

/* State derivatives (default storage) */
typedef struct {
  real_T Integrator_CSTATE;            /* '<S7>/Integrator' */
  real_T Integrator1_CSTATE;           /* '<S7>/Integrator1' */
  real_T Integrator2_CSTATE;           /* '<S7>/Integrator2' */
} XDot_sim_model_ver_tp2_V2_T;

/* State disabled  */
typedef struct {
  boolean_T Integrator_CSTATE;         /* '<S7>/Integrator' */
  boolean_T Integrator1_CSTATE;        /* '<S7>/Integrator1' */
  boolean_T Integrator2_CSTATE;        /* '<S7>/Integrator2' */
} XDis_sim_model_ver_tp2_V2_T;

#ifndef ODE3_INTG
#define ODE3_INTG

/* ODE3 Integration Data */
typedef struct {
  real_T *y;                           /* output */
  real_T *f[3];                        /* derivatives */
} ODE3_IntgData;

#endif

/* Parameters (default storage) */
struct P_sim_model_ver_tp2_V2_T_ {
  real_T InitialconditionX_InitialCondit;/* Expression: 0.0
                                          * Referenced by: '<Root>/Initial condition X'
                                          */
  real_T Speed_Value;                  /* Expression: 1
                                        * Referenced by: '<Root>/Speed'
                                        */
  real_T InitialconditionY_InitialCondit;/* Expression: 0.0
                                          * Referenced by: '<Root>/Initial condition Y'
                                          */
  real_T Integrator_IC;                /* Expression: 0
                                        * Referenced by: '<S7>/Integrator'
                                        */
  real_T Integrator1_IC;               /* Expression: 0
                                        * Referenced by: '<S7>/Integrator1'
                                        */
  real_T Integrator2_IC;               /* Expression: 0
                                        * Referenced by: '<S7>/Integrator2'
                                        */
};

/* Real-time Model Data Structure */
struct tag_RTM_sim_model_ver_tp2_V2_T {
  const char_T *errorStatus;
  RTWLogInfo *rtwLogInfo;
  RTWSolverInfo solverInfo;
  X_sim_model_ver_tp2_V2_T *contStates;
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

  /*
   * Sizes:
   * The following substructure contains sizes information
   * for many of the model attributes such as inputs, outputs,
   * dwork, sample times, etc.
   */
  struct {
    int_T numContStates;
    int_T numPeriodicContStates;
    int_T numSampTimes;
  } Sizes;

  /*
   * Timing:
   * The following substructure contains information regarding
   * the timing information for the model.
   */
  struct {
    uint32_T clockTick0;
    uint32_T clockTickH0;
    time_T stepSize0;
    uint32_T clockTick1;
    uint32_T clockTickH1;
    time_T tFinal;
    SimTimeStep simTimeStep;
    boolean_T stopRequestedFlag;
    time_T *t;
    time_T tArray[2];
  } Timing;
};

/* Block parameters (default storage) */
extern P_sim_model_ver_tp2_V2_T sim_model_ver_tp2_V2_P;

/* Block signals (default storage) */
extern B_sim_model_ver_tp2_V2_T sim_model_ver_tp2_V2_B;

/* Continuous states (default storage) */
extern X_sim_model_ver_tp2_V2_T sim_model_ver_tp2_V2_X;

/* Block states (default storage) */
extern DW_sim_model_ver_tp2_V2_T sim_model_ver_tp2_V2_DW;

/* Model entry point functions */
extern void sim_model_ver_tp2_V2_initialize(void);
extern void sim_model_ver_tp2_V2_step(void);
extern void sim_model_ver_tp2_V2_terminate(void);

/* Real-time Model object */
extern RT_MODEL_sim_model_ver_tp2_V2_T *const sim_model_ver_tp2_V2_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'sim_model_ver_tp2_V2'
 * '<S1>'   : 'sim_model_ver_tp2_V2/Input'
 * '<S2>'   : 'sim_model_ver_tp2_V2/Input block'
 * '<S3>'   : 'sim_model_ver_tp2_V2/Output'
 * '<S4>'   : 'sim_model_ver_tp2_V2/System'
 * '<S5>'   : 'sim_model_ver_tp2_V2/System/Control block'
 * '<S6>'   : 'sim_model_ver_tp2_V2/System/MATLAB Function'
 * '<S7>'   : 'sim_model_ver_tp2_V2/System/System block'
 * '<S8>'   : 'sim_model_ver_tp2_V2/System/Control block/MATLAB Function'
 * '<S9>'   : 'sim_model_ver_tp2_V2/System/Control block/MATLAB Function1'
 * '<S10>'  : 'sim_model_ver_tp2_V2/System/System block/MATLAB Function'
 */
#endif                                 /* RTW_HEADER_sim_model_ver_tp2_V2_h_ */
