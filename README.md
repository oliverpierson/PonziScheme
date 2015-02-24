# PonziScheme
A toy scheme interpreter written in C++.

# What it's got
## Types
  * Integers
  * Symbols

## Special Forms
  * `if`
  * `lambda`
  * `define`
  * `quote` (the backtick \` is also supported)

## Builtin functions
  * `+`,`-`,`*`
  * `=`
  * `cons`
  * `null?`

# What's to come
  * Compilation to a intermediate form
  * tail call optimization
  * a garbage collector that detects cyclic references

# Requirements
  * Boost (only need for ```polymorphic_cast```)
  * GNU make
  * Bison 2.3
  * Flex

# Building
From the base directory, do:
```
  make
  cd src/repl
  make clean reader
```

There are a few tests (more to come) in ```test/```:
```
  cd test
  make
```

# Vagrant
The included `Vagrantfile` should provision everything correctly
for building PonziScheme.
