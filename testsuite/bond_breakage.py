
#
# Copyright (C) 2013,2014,2015,2016 The ESPResSo project
#
# This file is part of ESPResSo.
#
# ESPResSo is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# ESPResSo is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# Tests particle property setters/getters
import unittest as ut
import espressomd
import numpy as np
from espressomd.interactions import Tabulated,HarmonicBond
from tests_common import abspath


class BondBreakage(ut.TestCase):
    S = espressomd.System(box_l=[10,10,10])

    def test_aa_interface(self):
      self.S.bonded_inter.bond_breakage.add_handler("print_queue_entry")
      self.S.bonded_inter.bond_breakage.add_handler("break_simple_pair_bond")
      res=self.S.bonded_inter.bond_breakage.active_handlers()
      print res
      self.assertTrue(res==["print_queue_entry","break_simple_pair_bond"],"Active handlers list does not match added handlers.")
      self.S.bonded_inter.bond_breakage.clear_handlers()
      res=self.S.bonded_inter.bond_breakage.active_handlers()
      print res
      self.assertTrue(res==[],"List of bond breakage handlers wasn't cleared.")


    def run_for(self,bond_class,breakable_params):
      self.S.part.clear()
      self.S.box_l=(10,10,10)
      self.S.time_step=0.01
      self.S.bonded_inter.bond_breakage.clear_handlers()
      self.S.bonded_inter.bond_breakage.add_handler("print_queue_entry")
      self.S.bonded_inter.bond_breakage.add_handler("break_simple_pair_bond")

      bond=bond_class(**breakable_params)
      self.S.bonded_inter.add(bond)
      harm=HarmonicBond(k=1,r_0=1)
      self.S.bonded_inter.add(harm)
      self.S.part.add(pos=(0,0,0),id=0)
      self.S.part.add(pos=(1,0,0),id=1,bonds=((bond,0),))
      self.S.integrator.run(0)
      res=self.S.part[1].bonds
      expected=((bond,0),)
      self.assertTrue(res==expected,"Bond should not have been broken.")
      self.S.part[1].pos=(1.5,0,0)
      print self.S.part[0].pos_folded,self.S.part[1].pos_folded
      self.S.integrator.run(10,recalc_forces=True)
      print self.S.bonded_inter.bond_breakage.active_handlers(),self.S.part[1].bonds
      self.assertTrue(self.S.part[1].bonds==(),"Bond should have been broken.")

      self.S.part[1].bonds=((bond,0),)
      p=bond._params
      p.update(breakable=False)
      bond.params=p
      self.assertRaises(Exception,self.S.integrator.run(1),"Extending bond with breakge turned off shoudl raise runtime error")

      # Test breaking of bind_at_point_of_collision
      self.S.bonded_inter.bond_breakage.clear_handlers()
      self.S.bonded_inter.bond_breakage.add_handler("break_bind_at_point_of_collision")
      # SEtup
      # Non-virtual particles
      self.S.part[0].bonds=()
      self.S.part[0].pos=0,0,0
      self.S.part[1].bonds=((harm,0),)
      
      # Virtual sites
      self.S.part.add(id=2,pos=self.S.part[0].pos,virtual=1)
      self.S.part.add(id=3,pos=self.S.part[1].pos,virtual=1,bonds=((bond,2),))
      self.S.part[2].vs_auto_relate_to(0)
      self.S.part[3].vs_auto_relate_to(1)

      # Extr particles
      self.S.part.add(id=4,pos=(0,0,0))
      self.S.part[0].bonds=((harm,4),)
      p=bond._params
      p.update(breakable=True)
      bond.params=p
      self.S.integrator.run(0)
      # Check bonds
      print self.S.part[0].bonds
      expected=((harm,4),)
      print expected
      self.assertTrue(self.S.part[0].bonds==expected)
      self.assertTrue(self.S.part[1].bonds==())
      self.assertTrue(self.S.part.exists(2)==False)
      self.assertTrue(self.S.part.exists(3)==False)
      self.assertTrue(self.S.part[4].bonds==())

    def test_b_tab(self):
      self.run_for(HarmonicBond,{'k':1,"r_0":0,"r_cut":1.3, "breakable":True})
#      self.run_for(Tabulated, {"type":"distance","filename":abspath("data/lj1.tab"),"breakable":True})





if __name__ == "__main__":
    print("Features: ", espressomd.features())
    ut.main()
