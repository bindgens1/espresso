from __future__ import print_function
import sys
import numpy as np
import unittest as ut
import espressomd
import espressomd.lb
from espressomd import *
from tests_common import abspath

@ut.skipIf(not espressomd.has_features(["LB_GPU","LENNARD_JONES"]) or espressomd.has_features("SHANCHEN"),
           "Features not available, skipping test!")
class TestLBGPU(ut.TestCase):
    box_l = 30.0
    system = espressomd.System(box_l=[box_l]*3)
    n_nodes = system.cell_system.get_state()["n_nodes"]
    system.seed = range(n_nodes)

    def _integrate(self, steps, split=True):
        if split:
            for i in range(steps):
                self.system.integrator.run(steps=1, recalc_forces=True)
        else:
            self.system.integrator.run(steps=steps)

    def test(self):
        #setup parameters
        system = self.system
        int_steps = 100
        int_times = 10
        time_step = 0.005
        tau = 0.02
        agrid = 1.0
        dens = 0.85
        viscosity = 30.0
        friction = 2.0
        temp = 0.0
        gamma = 1.0
        skin = 0.4
        mom_prec = 1.e-2

        system.periodicity = [1, 1, 1]
        system.time_step = time_step
        system.cell_system.skin = skin

        #import particle data
        data = np.genfromtxt(abspath("data/lb_system.data"))

        for particle in data:
            id = particle[0]
            typ = particle[1]
            pos = particle[3:6]
            f = particle[9:]
            v = particle[6:9]
            p=system.part.add(id=int(id), pos=pos, v=v, type=int(typ))
            if espressomd.has_features("ROTATION"):
                p.rotation=1,1,1

        system.time_step = time_step
        system.thermostat.set_langevin(kT=temp, gamma=gamma)
        system.integrator.run(100)
        system.thermostat.turn_off()

        lbf = lb.LBFluidGPU(visc=viscosity, dens=dens, agrid=agrid, tau=system.time_step, fric=friction)
        system.actors.add(lbf)
        system.thermostat.set_lb(kT=temp)
        #give particles a push
        for p in system.part:
            p.v=[0.1,0.0,0.0]

        tot_mom = np.array(system.analysis.analyze_linear_momentum())
        max_dm = 0.0

        #Integration
        for i in range(int_times):
          self._integrate(steps=int_steps)

          m_inst = np.array(system.analysis.analyze_linear_momentum())
          max_dm = max(max_dm, np.max(np.abs(m_inst - tot_mom)))

        self.assertLess(max_dm / tot_mom[0], 5.e-3)
        #print("maximal momentum deviation: {} ({} %)".format(max_dm, 100. * max_dm/tot_mom[0]))

if __name__ == "__main__":
    suite = ut.TestSuite()
    suite.addTests(ut.TestLoader().loadTestsFromTestCase(TestLBGPU))
    result = ut.TextTestRunner(verbosity=4).run(suite)
    sys.exit(not result.wasSuccessful())
