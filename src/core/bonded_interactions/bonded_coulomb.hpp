/*
  Copyright (C) 2010-2018 The ESPResSo project
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
#ifndef _BONDED_COULOMB_HPP
#define _BONDED_COULOMB_HPP
/** \file
 *  Routines to calculate the bonded Coulomb potential between
 *  particle pairs.
 *
 *  Implementation in \ref bonded_coulomb.cpp
 */

/************************************************************/

#include "config.hpp"

#ifdef ELECTROSTATICS

#include "bonded_interaction_data.hpp"
#include "debug.hpp"
#include "particle_data.hpp"

/** Set the parameters for the bonded Coulomb potential
 *
 *  @retval ES_OK on success
 *  @retval ES_ERROR on error
 */
int bonded_coulomb_set_params(int bond_type, double prefactor);

/** Compute the bonded Coulomb pair force.
 *  @param[in]  p1        First particle.
 *  @param[in]  p2        Second particle.
 *  @param[in]  iaparams  Interaction parameters.
 *  @param[in]  dx        %Distance between the particles.
 *  @param[out] force     Force.
 *  @retval false
 */
inline bool calc_bonded_coulomb_pair_force(
    Particle const *const p1, Particle const *const p2,
    Bonded_ia_parameters const *const iaparams, Utils::Vector3d const &dx,
    Utils::Vector3d &force) {
  auto const dist2 = dx.norm2();
  auto const dist = std::sqrt(dist2);

  auto const fac =
      iaparams->p.bonded_coulomb.prefactor * p1->p.q * p2->p.q / (dist * dist2);
  force = fac * dx;

  return false;
}

/** Compute the bonded Coulomb pair energy.
 *  @param[in]  p1        First particle.
 *  @param[in]  p2        Second particle.
 *  @param[in]  iaparams  Interaction parameters.
 *  @param[in]  dx        %Distance between the particles.
 *  @param[out] _energy   Energy.
 *  @retval false
 */
inline bool
bonded_coulomb_pair_energy(Particle const *const p1, Particle const *const p2,
                           Bonded_ia_parameters const *const iaparams,
                           Utils::Vector3d const &dx, double *_energy) {
  auto const dist = dx.norm();

  *_energy = iaparams->p.bonded_coulomb.prefactor * p1->p.q * p2->p.q / dist;
  return false;
}

#endif

#endif
