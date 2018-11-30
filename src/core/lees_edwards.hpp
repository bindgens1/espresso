#ifndef _LEES_EDWARDS_H
#define _LEES_EDWARDS_H

/** \file lees_edwards.hpp
*
*/

#define LEES_EDWARDS_PROTOCOL_OFF 0
#define LEES_EDWARDS_PROTOCOL_STEP 2
#define LEES_EDWARDS_PROTOCOL_STEADY_SHEAR 4
#define LEES_EDWARDS_PROTOCOL_OSC_SHEAR 8

#include "config.hpp"

class LeesEdwards_parameters {
public:
  LeesEdwards_parameters()
      : type(LEES_EDWARDS_PROTOCOL_OFF), offset(0.), velocity(0.), amplitude(0.),
        frequency(0.), time0(0.), sheardir(0), shearplanenormal(1) {};

  int type;
  double offset;
  double velocity;
  double amplitude;
  double frequency;
  double time0;
  int sheardir;
  int shearplanenormal;
};

extern LeesEdwards_parameters LeesEdwards_params;

#ifdef LEES_EDWARDS

void setup_lees_edwards_protocol();
///** Calculation of current offset*/
double lees_edwards_get_offset(double time);
///** Calculation of current velocity*/
double lees_edwards_get_velocity(double time);

#endif
#endif
