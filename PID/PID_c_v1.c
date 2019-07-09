/**********************************************************************************************
 * Arduino PID Library - Version 1.2.1
 * by Brett Beauregard <br3ttb@gmail.com> brettbeauregard.com
 *
 * This Library is licensed under the MIT License
 **********************************************************************************************/

#include "PID_c_v1.h"

struct {

    //  public:

    double dispKp; // * we'll hold on to the tuning parameters in user-entered
    double dispKi; //   format for display purposes
    double dispKd; //

    double kp; // * (P)roportional Tuning Parameter
    double ki; // * (I)ntegral Tuning Parameter
    double kd; // * (D)erivative Tuning Parameter

    int controllerDirection;
    int pOn;

    double* myInput;    // * Pointers to the Input, Output, and Setpoint variables
    double* myOutput;   //   This creates a hard link between the variables and the
    double* mySetpoint; //   PID, freeing the user from having to constantly tell us
                        //   what these values are.  with pointers we'll just know.

    unsigned long lastTime;
    double        outputSum, lastInput;

    unsigned long SampleTime;
    double        outMin, outMax;
    bool          inAuto, pOnE;
} PID;
/*Constructor (...)*********************************************************
 *    The parameters specified here are those for for which we can't set up
 *    reliable defaults, so we need to have the user set them.
 ***************************************************************************/
void PID_construct(double* Input, double* Output, double* Setpoint,
                   double Kp, double Ki, double Kd, int POn, int ControllerDirection)
{
    PID.myOutput   = Output;
    PID.myInput    = Input;
    PID.mySetpoint = Setpoint;
    PID.inAuto     = false;

    PID_SetOutputLimits(0, 255); //default output limit corresponds to
                                 //the arduino pwm limits

    PID.SampleTime = 100; //default Controller Sample Time is 0.1 seconds

    PID_SetControllerDirection(ControllerDirection);
    PID_SetTunings_pp(Kp, Ki, Kd, POn);

    // todo: change millis to real time
    PID.lastTime = millis() - PID.SampleTime;
}

/* Compute() **********************************************************************
 *     This, as they say, is where the magic happens.  this function should be called
 *   every time "void loop()" executes.  the function will decide for itself whether a new
 *   pid Output needs to be computed.  returns true when the output is computed,
 *   false when nothing has been done.
 **********************************************************************************/
bool PID_Compute()
{
    unsigned long now;
    unsigned long timeChange;

    double input, error, dInput, output;

    if (!PID.inAuto) {
        return false;
    }
    now        = millis();
    timeChange = (now - PID.lastTime);

    if (timeChange >= PID.SampleTime) {
        /*Compute all the working error variables*/
        input  = *PID.myInput;
        error  = *PID.mySetpoint - input;
        dInput = (input - PID.lastInput);
        PID.outputSum += (PID.ki * error);

        /*Add Proportional on Measurement, if P_ON_M is specified*/
        if (!PID.pOnE)
            PID.outputSum -= PID.kp * dInput;

        if (PID.outputSum > PID.outMax)
            PID.outputSum = PID.outMax;
        else if (PID.outputSum < PID.outMin)
            PID.outputSum = PID.outMin;

        /*Add Proportional on Error, if P_ON_E is specified*/
        if (PID.pOnE)
            output = PID.kp * error;
        else
            output = 0;

        /*Compute Rest of PID Output*/
        output += PID.outputSum - PID.kd * dInput;

        if (output > PID.outMax)
            output = PID.outMax;
        else if (output < PID.outMin)
            output = PID.outMin;
        *PID.myOutput = output;

        /*Remember some variables for next time*/
        PID.lastInput = input;
        PID.lastTime  = now;
        return true;
    } else
        return false;
}

/* SetTunings(...)*************************************************************
 * This function allows the controller's dynamic performance to be adjusted.
 * it's called automatically from the constructor, but tunings can also
 * be adjusted on the fly during normal operation
 ******************************************************************************/
void PID_SetTunings_pp(double Kp, double Ki, double Kd, int POn)
{
    double SampleTimeInSec;
    if (Kp < 0 || Ki < 0 || Kd < 0)
        return;

    PID.pOn  = POn;
    PID.pOnE = POn == P_ON_E;

    PID.dispKp = Kp;
    PID.dispKi = Ki;
    PID.dispKd = Kd;

    SampleTimeInSec = ((double)PID.SampleTime) / 1000;
    PID.kp          = Kp;
    PID.ki          = Ki * SampleTimeInSec;
    PID.kd          = Kd / SampleTimeInSec;

    if (PID.controllerDirection == REVERSE) {
        PID.kp = (0 - PID.kp);
        PID.ki = (0 - PID.ki);
        PID.kd = (0 - PID.kd);
    }
}

/* SetTunings(...)*************************************************************
 * Set Tunings using the last-rembered POn setting
 ******************************************************************************/
void PID_SetTunings(double Kp, double Ki, double Kd)
{
    PID_SetTunings_pp(Kp, Ki, Kd, PID.pOn);
}

/* SetSampleTime(...) *********************************************************
 * sets the period, in Milliseconds, at which the calculation is performed
 ******************************************************************************/
void PID_SetSampleTime(int NewSampleTime)
{
    if (NewSampleTime > 0) {
        double ratio = (double)NewSampleTime
                       / (double)PID.SampleTime;
        PID.ki *= ratio;
        PID.kd /= ratio;
        PID.SampleTime = (unsigned long)NewSampleTime;
    }
}

/* SetOutputLimits(...)****************************************************
 *     This function will be used far more often than SetInputLimits.  while
 *  the input to the controller will generally be in the 0-1023 range (which is
 *  the default already,)  the output will be a little different.  maybe they'll
 *  be doing a time window and will need 0-8000 or something.  or maybe they'll
 *  want to clamp it from 0-125.  who knows.  at any rate, that can all be done
 *  here.
 **************************************************************************/
void PID_SetOutputLimits(double Min, double Max)
{
    if (Min >= Max)
        return;
    PID.outMin = Min;
    PID.outMax = Max;

    if (PID.inAuto) {
        if (*PID.myOutput > PID.outMax)
            *PID.myOutput = PID.outMax;
        else if (*PID.myOutput < PID.outMin)
            *PID.myOutput = PID.outMin;

        if (PID.outputSum > PID.outMax)
            PID.outputSum = PID.outMax;
        else if (PID.outputSum < PID.outMin)
            PID.outputSum = PID.outMin;
    }
}

/* SetMode(...)****************************************************************
 * Allows the controller Mode to be set to manual (0) or Automatic (non-zero)
 * when the transition from manual to auto occurs, the controller is
 * automatically initialized
 ******************************************************************************/
void PID_SetMode(int Mode)
{
    bool newAuto = (Mode == AUTOMATIC);
    if (newAuto && !PID.inAuto) { /*we just went from manual to auto*/
        PID_Initialize();
    }
    PID.inAuto = newAuto;
}

/* Initialize()****************************************************************
 *	does all the things that need to happen to ensure a bumpless transfer
 *  from manual to automatic mode.
 ******************************************************************************/
void PID_Initialize()
{
    PID.outputSum = *PID.myOutput;
    PID.lastInput = *PID.myInput;
    if (PID.outputSum > PID.outMax)
        PID.outputSum = PID.outMax;
    else if (PID.outputSum < PID.outMin)
        PID.outputSum = PID.outMin;
}

/* SetControllerDirection(...)*************************************************
 * The PID will either be connected to a DIRECT acting process (+Output leads
 * to +Input) or a REVERSE acting process(+Output leads to -Input.)  we need to
 * know which one, because otherwise we may increase the output when we should
 * be decreasing.  This is called from the constructor.
 ******************************************************************************/
void PID_SetControllerDirection(int Direction)
{
    if (PID.inAuto && Direction != PID.controllerDirection) {
        PID.kp = (0 - PID.kp);
        PID.ki = (0 - PID.ki);
        PID.kd = (0 - PID.kd);
    }
    PID.controllerDirection = Direction;
}

/* Status Funcions*************************************************************
 * Just because you set the Kp=-1 doesn't mean it actually happened.  these
 * functions query the internal state of the PID.  they're here for display
 * purposes.  this are the functions the PID Front-end uses for example
 ******************************************************************************/
double PID_GetKp() { return PID.dispKp; }
double PID_GetKi() { return PID.dispKi; }
double PID_GetKd() { return PID.dispKd; }
int    PID_GetMode() { return PID.inAuto ? AUTOMATIC : MANUAL; }
int    PID_GetDirection() { return PID.controllerDirection; }

