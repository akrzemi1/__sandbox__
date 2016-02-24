
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
#include <mach7/patterns/constructor.hpp>
#include <mach7/patterns/primitive.hpp>

struct TankA { int val; };
struct TankB { int val; };
using  Tank = boost::variant<TankA, TankB>;

int interact(Tank const& tank)
{
  using mch::C;
  mch::var<const TankA&> tankA;

  Match(tank)
  {
  Case(C<const TankA>(tankA))
     return 0;
  Otherwise()
    return -1;
  }
  EndMatch
}
```
