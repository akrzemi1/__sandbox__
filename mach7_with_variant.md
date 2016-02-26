


Working with Boost.Variant
==========================

Challenge 1 -- accessing by const reference
-----------------------------------------

Given the following Variant:

```c++
struct TankA { int vol; };
struct TankB { int area; int height; };
struct TankX { };

using  Tank = boost::variant<TankA, TankB, TankX>;
```

### With a hypothetical lanuage feature:

Suppose there was a built-in statement in C++ to do a type switch, I would like to do this:
```c++
int volume(Tank const& tank)
{
  match (tank)
  {
  case (TankA const& ta):       // by reference to const
    return ta.vol;              // sub-type reference has a name
  case (TankB const& tb):
    return tb.area * tb.height;
  case (auto const&):           // any other type
    return 0;
  }
}
```

### With Boost.Variant visitor:

I can do it today with Boost.Variant static visitor:

```c++
int volume(Tank const& tank)
{
  struct v_volume : boost::static_visitor<int>
  {
    int operator()(TankA const& ta) const {
      return ta.vol;
    }
    int operator()(TankB const& tb) const {
      return tb.area * tb.height;
    }
    template <typename T> int operator()(T const&) const {
      return 0;
    }    
  };
  
  return boost::apply_visitor(v_volume{}, tank);
}
```

### With Mach7 (basic)

```c++
int volume(Tank const& tank)
{
  Match(tank)
  {
    Case(mch::C<TankA>())
      return match0.vol;                  // match0 - a name out of thin air
    Case(mch::C<TankB>())
      return match0.area * match0.height;
    Case(mch::_)                          // does not compile
      return 0;
  }
  EndMatch
}
```
Objection 1: I cannot give names to sub-typed objects. I am forced to use magical hardcoded names `match0`.

Objection 2: bind to `mch::_` does not work.

### With Mach7 (nested binding)

```c++
namespace mch
{
  template <> struct bindings<TankA> { Members(TankA::vol); };
  template <> struct bindings<TankB> { Members(TankB::area, TankB::height); };
}

int volume(Tank const& tank)
{
  mch::var<const int&> vol;
  mch::var<const int&> area;
  mch::var<const int&> height;
  
  Match(tank)
  {
    Case(mch::C<TankA>(vol))
      return vol;
    Case(mch::C<TankB>(area, height))
      return area * height;
    Case(mch::_)                          // does not compile
      return 0;
  }
  EndMatch
}
```

Objection 1: It is a bit too tricky to my developer's sense. I want to see that I am accessing members.

Objection 2: I have to put extra customization for `mch::bindings` for every variant type I use.

Objection 3: It will not work if I kave private members exposed by getter and setter functions.


Challenge 2 -- variant data members are private
-----------------------------------------

Given the following Variant:

```c++
class TankA
{
  int vol_;
public:
  explicit TankA(int v) : vol_(v) {}
  int vol() const { return vol_; }
};

struct TankB
{
  int area_;
  int height_;
public:
  explicit TankB(int a, int h) : area_(a), height_(h) {}
  int area() const { return area_; }
  int height() const { return height_; }
};

class TankX { };

using  Tank = boost::variant<TankA, TankB, TankX>;
```

### With a hypothetical lanuage feature:

```c++
int volume(Tank const& tank)
{
  match (tank)
  {
  case (TankA const& ta):           // by reference to const
    return ta.vol();                // sub-type reference has a name
  case (TankB const& tb):
    return tb.area() * tb.height();
  case (auto const&):               // any other type
    return 0;
  }
}
```

### With Boost.Variant visitor:


```c++
int volume(Tank const& tank)
{
  struct v_volume : boost::static_visitor<int>
  {
    int operator()(TankA const& ta) const {
      return ta.vol();
    }
    int operator()(TankB const& tb) const {
      return tb.area() * tb.height();
    }
    template <typename T> int operator()(T const&) const {
      return 0;
    }    
  };
  
  return boost::apply_visitor(v_volume{}, tank);
}
```

### With Mach7 (basic)

```c++
int volume(Tank const& tank)
{
  Match(tank)
  {
    Case(mch::C<TankA>())
      return match0.vol();                    // match0 - a name out of thin air
    Case(mch::C<TankB>())
      return match0.area() * match0.height();
    Case(mch::_)                              // does not compile
      return 0;
  }
  EndMatch
}
```
