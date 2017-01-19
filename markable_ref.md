
## Concept `Mark_policy`

```c++
template <typename MP>
concept bool Mark_policy =
  requires
  {
    typename MP::mark_representation_type;
  } &&
  requires(const typename MP::mark_representation_type & v)
  {
    { MP::marked_value() }              -> typename MP::mark_representation_type;
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
    typename SP::storage_type;
    typename SP::reference_type;
  } &&
  requires(const typename SP::storage_type & s,
           const typename SP::value_type &   cv,
                 typename SP::value_type &&  rv)
  {    
    { SP::access_value(s) }             -> typename SP::reference_type;
    { SP::store_value(cv) }             -> typename SP::storage_type;
    { SP::store_value(std::move(rv)) }  -> typename SP::storage_type;
  };
```

## Concept `Mark_reinterpret_policy`

```c++
template <typename RP>
concept bool Mark_reinterpret_policy =
  requires
  {
    typename RP::mark_representation_type;
    typename SP::value_type;
    typename SP::storage_type;
    typename SP::reference_type;
  } &&
  requires(const typename SP::storage_type & s,
           const typename SP::value_type &   cv,
                 typename SP::value_type &&  rv)
  {    
    { SP::access_value(s) }             -> typename SP::reference_type;
    { SP::store_value(cv) }             -> typename SP::storage_type;
    { SP::store_value(std::move(rv)) }  -> typename SP::storage_type;
  };
```

## Concept `Storage`

```c++
template <typename S>
concept bool Storage = 
```
