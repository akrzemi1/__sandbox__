
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
