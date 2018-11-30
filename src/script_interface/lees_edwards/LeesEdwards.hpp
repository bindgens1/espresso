


#include "../ScriptInterfaceBase.hpp"
#include "core/lees_edwards.hpp"

#ifdef LEES_EDWARDS

namespace ScriptInterface {
namespace LeesEdwards {

class LeesEdwards : public AutoParameters<LeesEdwards> {
public:
  LeesEdwards() {
    add_parameters(
	{{"type", LeesEdwards_params.type},
	 {"time0", LeesEdwards_params.time0},
     {"offset", LeesEdwards_params.offset},
     {"velocity", LeesEdwards_params.velocity},
     {"amplitude", LeesEdwards_params.amplitude},
     {"frequency", LeesEdwards_params.frequency},
     {"sheardir", LeesEdwards_params.sheardir},
     {"shearplanenormal", LeesEdwards_params.shearplanenormal}});
  };

}; // Class LeesEdwards

} // namespace LeesEdwards
} // namespace ScriptInterface

#endif
