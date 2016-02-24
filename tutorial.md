
Tutorial
========

Running unit tests
------------------

Some tests test Boost Adapters and therefore require Boost libraries on your platform.
In order for these to compile you need to set up an environmental variable `BOOST_ROOT` to point to Boost ome directory,
so that `$BOOST_ROOT/boost` is an existent path that contains all Boost headers. 

### Using `make`

From Mach7 root directory:

```
cd code/test/unit
make
```

This builds a number of `*.exe` files, each representing a test to be run.


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
