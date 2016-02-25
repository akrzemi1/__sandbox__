


Working with Boost.Variant
--------------------------

### Conservative
This works today

```c++
#include <boost/variant.hpp>
#include <mach7/type_switchN-patterns-xtl.hpp>
#include <mach7/adapters/boost/adapt_boost_variant.hpp>
#include <mach7/patterns/constructor.hpp>                // for mch::C
#include <mach7/patterns/primitive.hpp>                  // for mch::var

struct TankA { int va = 0; };
struct TankB { int vb = 0; };
struct TankX { };
using  Tank = boost::variant<TankA, TankB, TankX>;

int read(Tank const& tank)
{
  using mch::C;

  Match(tank)
  {
    Case(C<TankA>())
      return match0.va;
    Case(C<TankB>())
      return match0.vb + 10;
    Case(C<TankX>())
      return -1;
  }
  EndMatch
}

int main()
{
  assert (read(TankB()) == 10);
}
```
### Nice
This works after my proposed changes.

```c++
#include <boost/variant.hpp>
#include <mach7/type_switchN-patterns-xtl.hpp>
#include <mach7/adapters/boost/adapt_boost_variant.hpp>
#include <mach7/patterns/constructor.hpp>                // for mch::C
#include <mach7/patterns/primitive.hpp>                  // for mch::var

struct TankA { int va = 0; };
struct TankB { int vb = 0; };
struct TankX { };
using  Tank = boost::variant<TankA, TankB, TankX>;

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
      return tB->vb + 10;
    Case(C<TankX const>())
      return -1;
  }
  EndMatch
}

int main()
{
  assert (read(TankB()) == 10);
}
```

TBD
---



```c++
struct TankA { int val; };
struct TankB { int val; };
struct TankX { };

using  Tank = boost::variant<TankA, TankB, TankX>;
```

With a hypothetical lanuage feature:
------------------------------------
```c++
int interact(Tank const& tank)
{
  match (tank)
  {
  case (TankA const& ta): // by reference to const
    return ta.val;
  case (TankB const& tb):
    return tb.val * 10;
  case (auto const&):     // any other type
    return -1;
  }
}
```

With Boost.Variant visitor:
---------------------------
```c++
int interact(Tank const& tank)
{
  struct v_interact : boost::static_visitor<int>
  {
    int operator()(TankA const& ta) const {
      return ta.val;
    }
    int operator()(TankB const& tb) const {
      return tb.val * 10;
    }
    template <typename T> int operator()(T const&) const {
      return -1;
    }    
  };
  
  return boost::apply_visitor(v_interact{}, tank);
}
```

With Mach7
==========

```c++
int inspect(Tank const& tank)
{
  using mch::C;

  Match(tank)
  {
    Case(C<TankA>())
      return match0.val;
    Case(C<TankB>())
      return match0.val * 10;
    Case(mch::_)               // does not compile
      return -1;
  }
  EndMatch
}
```
