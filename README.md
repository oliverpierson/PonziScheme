# PonziScheme
Scheme in C++

# TODO
* Change C style casting to C++/Boost ```<polymorphic_cast>```
* Change ```Cons::Eval``` to dispatch rather than one big ```if...else``` block
* Implement cyclic reference detection (one reference is http://researcher.watson.ibm.com/researcher/files/us-bacon/Bacon03Pure.pdf)
* Add ```const``` qualifiers wherever possible

# Building
From the base directory, do:
```
  make
  cd src/repl
  make clean reader
```

There are a few tests (more to come) in ```src/repl/tests.scm```:
```
  cd src/repl
  cat tests.scm | ./reader
```
