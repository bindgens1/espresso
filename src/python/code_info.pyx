include "myconfig.pxi"


def features():
  """Returns list of compiled-in features"""
  f=[]
  IF ELECTROSTATICS == 1:
     f.append("ELECTROSTATICS")
  IF DIPOLES == 1:
     f.append("DIPOLES") 
  IF LB_GPU == 1:
      f.append("LB_GPU")
  IF ROTATION == 1:
    f.append("ROTATION")
  IF MASS == 1 : 
    f.append("MASS")
   

  return sorted(f)

