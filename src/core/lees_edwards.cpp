/*
  Copyright (C) 2018 The ESPResSo project

  This file is part of ESPResSo.

  ESPResSo is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  ESPResSo is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/** \file lees_edwards.cpp
 */

#include <cmath>
#include "lees_edwards.hpp"
#include "integrate.hpp"

/* Functions to determine the current offset and shear rate with respect to the chosen protocol */

LeesEdwards_parameters LeesEdwards_params;

void setup_lees_edwards_protocol() {
  if (LeesEdwards_params.type == LEES_EDWARDS_PROTOCOL_OFF) {
    LeesEdwards_params.offset = 0.0;
    LeesEdwards_params.velocity = 0.0;
    LeesEdwards_params.amplitude = 0.0;
    LeesEdwards_params.frequency = 0.0;
  } 


  else if (LeesEdwards_params.type == LEES_EDWARDS_PROTOCOL_STEP) {
    LeesEdwards_params.velocity = 0.0;
    LeesEdwards_params.amplitude = 0.0;
    LeesEdwards_params.frequency = 0.0;
  } 

  else if (LeesEdwards_params.type == LEES_EDWARDS_PROTOCOL_STEADY_SHEAR) {
    LeesEdwards_params.offset = lees_edwards_get_offset(sim_time);
    LeesEdwards_params.amplitude = 0.0;
    LeesEdwards_params.frequency = 0.0;
  } 
  
  else if (LeesEdwards_params.type == LEES_EDWARDS_PROTOCOL_OSC_SHEAR) {
    LeesEdwards_params.offset = lees_edwards_get_offset(sim_time);
    LeesEdwards_params.velocity = lees_edwards_get_velocity(sim_time);
  }
  else {
    LeesEdwards_params.offset = 0.0;
    LeesEdwards_params.velocity = 0.0;
  }
}

double lees_edwards_get_offset(double time) {

  if (LeesEdwards_params.type == LEES_EDWARDS_PROTOCOL_OFF) {
    return 0.;
  } else if (LeesEdwards_params.type == LEES_EDWARDS_PROTOCOL_STEP) {
    return LeesEdwards_params.offset;
  } else if (LeesEdwards_params.type == LEES_EDWARDS_PROTOCOL_STEADY_SHEAR) {
    return LeesEdwards_params.velocity * (time-LeesEdwards_params.time0);
  } else if (LeesEdwards_params.type == LEES_EDWARDS_PROTOCOL_OSC_SHEAR) {
    return LeesEdwards_params.amplitude * std::sin(LeesEdwards_params.frequency*(time-LeesEdwards_params.time0));
  } else {
    return 0.0;
  }  
}

double lees_edwards_get_velocity(double time) {
  if (LeesEdwards_params.type == LEES_EDWARDS_PROTOCOL_OFF) {
    return 0.;
  } else if (LeesEdwards_params.type == LEES_EDWARDS_PROTOCOL_STEP) {
    return 0.;
  } else if (LeesEdwards_params.type == LEES_EDWARDS_PROTOCOL_STEADY_SHEAR) {
    return LeesEdwards_params.velocity;
  } else if (LeesEdwards_params.type == LEES_EDWARDS_PROTOCOL_OSC_SHEAR) {
    return LeesEdwards_params.frequency * LeesEdwards_params.amplitude * std::cos(LeesEdwards_params.frequency*(time-LeesEdwards_params.time0));
  } else {
    return 0.0;
  }  
}
