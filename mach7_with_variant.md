


Working with Boost.Variant
--------------------------


```c++
#include <boost/variant.hpp>
#include <mach7/type_switchN-patterns-xtl.hpp>
#include <mach7/adapters/boost/adapt_boost_variant.hpp>
#include <mach7/patterns/constructor.hpp>                // for mch::C
#include <mach7/patterns/primitive.hpp>                  // for mch::var

struct TankA { int va = 0; };
struct TankB { int vb = 0; };
using  Tank = boost::variant<TankA, TankB>;

int read(Tank const& tank)
{
  using mch::C;
  mch::var<TankA const&> tA;
  mch::var<TankB const&> tB;

  Match(tank)
  {
  Case(C<TankA const>(tA))
    return tA->va;
  Case(C<TankB const>(tB))
    return tB->vb;
  Otherwise()
    return -1;
  }
  EndMatch
}
```
