
## Concept `Mark_policy`

```c++
template <typename MP>
concept bool Mark_policy =
  requires
  {
    typename MP::representation_type;
  } &&
  requires(const typename MP::representation_type & v)
  {
    { MP::marked_value() }              -> typename MP::representation_type;
    { MP::is_marked_value(s) } noexcept -> bool;
  };
```


## Concept `Storage_policy`

```c++
template <typename SP>
concept bool Storage_policy =
  requires
  {
    typename SP::value_type;
    typename SP::representation_type;
    typename SP::reference_type;
  } &&
  requires(const typename SP::representation_type & s,
           const typename SP::value_type &          cv,
                 typename SP::value_type &&         rv)
  {    
    { SP::access_value(s) }             -> typename SP::reference_type;
    { SP::store_value(cv) }             -> typename SP::representation_type;
    { SP::store_value(std::move(rv)) }  -> typename SP::representation_type;
  };
```

## Concept `Lifetime_manager`

```c++
template <typename LM>
concept bool Lifetime_manager =
  Semiregular<LM>() &&
  requires(const LM &  cv, LM &  mv)
  {
    typename LM::representation_type;
    typename LM::value_type;
    
    { cv.representation() } noexcept -> const typename LM::representation_type &;
    { mv.representation() } noexcept ->       typename LM::representation_type &;
  } &&
  requires(const typename LM::value_type &   cv,
                 typename LM::value_type &&  rv)
  {    
    LM {cv};
    LM {std::move(rv)};
  };
```

## Concept `Storage`

```c++
template <typename S>
concept bool Storage = 
```
