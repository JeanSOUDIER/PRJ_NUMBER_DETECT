//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
//
// File: untitled.h
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
#ifndef RTW_HEADER_untitled_h_
#define RTW_HEADER_untitled_h_
#include <cmath>
#include "rtwtypes.h"
#include "untitled_types.h"
#include "rt_nonfinite.h"
#include "rt_defines.h"
#include "rtGetInf.h"

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif

// Class declaration for model untitled
class untitledModelClass {
  // public data and function members
 public:
  // Block states (default storage) for system '<Root>'
  typedef struct {
    real_T DiscreteTimeIntegrator_DSTATE;// '<S7>/Discrete-Time Integrator'
    real_T DiscreteTimeIntegrator1_DSTATE;// '<S7>/Discrete-Time Integrator1'
    real_T DiscreteTimeIntegrator2_DSTATE;// '<S7>/Discrete-Time Integrator2'
    real_T UD_DSTATE;                  // '<S4>/UD'
    real_T UD_DSTATE_i;                // '<S5>/UD'
  } DW_untitled_T;

  // External outputs (root outports fed by signals with default storage)
  typedef struct {
    real_T x;                          // '<Root>/x'
    real_T y;                          // '<Root>/y'
  } ExtY_untitled_T;

  // Real-time Model Data Structure
  struct RT_MODEL_untitled_T {
    const char_T * volatile errorStatus;
  };

  // model initialize function
  void initialize();

  // model step function
  void step();

  // model terminate function
  void terminate();

  // Constructor
  untitledModelClass();

  // Destructor
  ~untitledModelClass();

  // Root-level structure-based outputs get method

  // Root outports get method
  const untitledModelClass::ExtY_untitled_T & getExternalOutputs() const
  {
    return untitled_Y;
  }

  // Real-Time Model get method
  untitledModelClass::RT_MODEL_untitled_T * getRTM();

  // private data and function members
 private:
  // Block states
  DW_untitled_T untitled_DW;

  // External outputs
  ExtY_untitled_T untitled_Y;

  // Real-Time Model
  RT_MODEL_untitled_T untitled_M;
};

//-
//  These blocks were eliminated from the model due to optimizations:
//
//  Block '<S4>/Data Type Duplicate' : Unused code path elimination
//  Block '<S5>/Data Type Duplicate' : Unused code path elimination


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
//  '<Root>' : 'untitled'
//  '<S1>'   : 'untitled/Input block'
//  '<S2>'   : 'untitled/System'
//  '<S3>'   : 'untitled/System/Control block'
//  '<S4>'   : 'untitled/System/Discrete Derivative'
//  '<S5>'   : 'untitled/System/Discrete Derivative1'
//  '<S6>'   : 'untitled/System/MATLAB Function'
//  '<S7>'   : 'untitled/System/System block'
//  '<S8>'   : 'untitled/System/Control block/MATLAB Function'
//  '<S9>'   : 'untitled/System/Control block/MATLAB Function1'
//  '<S10>'  : 'untitled/System/System block/MATLAB Function'

#endif                                 // RTW_HEADER_untitled_h_

//
// File trailer for generated code.
//
// [EOF]
//
