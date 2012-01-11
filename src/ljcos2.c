/*
  Copyright (C) 2010,2011,2012 The ESPResSo project
  Copyright (C) 2002,2003,2004,2005,2006,2007,2008,2009,2010 
    Max-Planck-Institute for Polymer Research, Theory Group
  
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

/** \file ljcos2.h
 *
 *  Routines to calculate the lennard-jones with cosine tail energy and/or  force 
 *  for a particle pair.  Cosine tail is different from that in ljcos.h
 *  Used for attractive tail/tail interactions in lipid bilayer calculations
 *  \ref forces.c
*/

#include "utils.h"

#ifdef LJCOS2
#include <math.h>

#include "ljcos2.h"
#include "parser.h"
#include "communication.h"

int ljcos2_set_params(int part_type_a, int part_type_b,
				      double eps, double sig, double offset,
				      double w)
{
  IA_parameters *data, *data_sym;

  make_particle_type_exist(part_type_a);
  make_particle_type_exist(part_type_b);
    
  data     = get_ia_param(part_type_a, part_type_b);
  data_sym = get_ia_param(part_type_b, part_type_a);

  if (!data || !data_sym) {
    return TCL_ERROR;
  }

  /* lj-cos2 should be symmetrically */
  data->LJCOS2_eps    = data_sym->LJCOS2_eps    = eps;
  data->LJCOS2_sig    = data_sym->LJCOS2_sig    = sig;
  data->LJCOS2_offset = data_sym->LJCOS2_offset = offset;
  data->LJCOS2_w      = data_sym->LJCOS2_w      = w;

  /* calculate dependent parameters */
  data->LJCOS2_rchange = data_sym->LJCOS2_rchange = pow(2,1/6.)*sig;
  data->LJCOS2_cut     = data_sym->LJCOS2_cut     = w + data_sym->LJCOS2_rchange;

  /* broadcast interaction parameters */
  mpi_bcast_ia_params(part_type_a, part_type_b);
  mpi_bcast_ia_params(part_type_b, part_type_a);

  if (lj_force_cap != -1.0)
    mpi_lj_cap_forces(lj_force_cap);

  return TCL_OK;
}



/** calculate ljcos2_capradius from ljcos2_force_cap */
void calc_ljcos2_cap_radii(double force_cap)
{
  int i,j,cnt=0;
  IA_parameters *params;
  double force=0.0, rad=0.0, step, frac2, frac6;

  for(i=0; i<n_particle_types; i++) {
    for(j=0; j<n_particle_types; j++) {
      params = get_ia_param(i,j);
      if(force_cap > 0.0 && params->LJCOS2_eps > 0.0) {
	/* I think we have to solve this numerically... and very crude as well */
	cnt=0;
	rad = params->LJCOS2_sig;
	step = -0.1 * params->LJCOS2_sig;
	force=0.0;
	
	while(step != 0) {
	  frac2 = SQR(params->LJCOS2_sig/rad);
	  frac6 = frac2*frac2*frac2;
          if (rad < params->LJCOS2_rchange) {
            force = 48.0 * params->LJCOS2_eps * frac6*(frac6 - 0.5)/rad;
          }
          else {
	    force = 0;
	  }
	  if((step < 0 && force_cap < force) || (step > 0 && force_cap > force)) {
	    step = - (step/2.0); 
	  }
	  if(fabs(force-force_cap) < 1.0e-6) step=0;
	  rad += step; cnt++;
	} 
      	params->LJCOS2_capradius = rad;
      }
      else {
	params->LJCOS2_capradius = 0.0; 
      }
      FORCE_TRACE(fprintf(stderr,"%d: Ptypes %d-%d have cap_radius %f and cap_force %f (iterations: %d)\n",
			  this_node,i,j,rad,force,cnt));
    }
  }
}

#endif /* ifdef LJCOS2 */
