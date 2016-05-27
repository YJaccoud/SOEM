BUILDING
========

Prerequisites for all platforms
-------------------------------

 * CMake 2.8.0 or later


Windows (Visual Studio)
-----------------------

 * Start a Visual Studio command prompt then:
   * `mkdir build`
   * `cd build`
   * `cmake .. -G "NMake Makefiles" -DCOMPILATION_FOR_INTIME:BOOL=FALSE`
   *  OR
   * `cmake .. -G "Visual Studio 12" -DCOMPILATION_FOR_INTIME:BOOL=TRUE`
   * `nmake`

Linux
-----

   * `mkdir build`
   * `cd build`
   * `cmake ..`
   * `make`

Install
-------
 * `cd build`
 * `cmake --build . --target install`

Doc html
--------
 * `doxygen.exe  Doxyfile`

 Want to contribute to SOEM or SOES?
-----------------------------------

If you want to contribute to SOEM or SOES you will need to sign a Contributor
License Agreement and send it to us either by e-mail or by physical mail. More
information is available in the [PDF](http://openethercatsociety.github.io/cla/cla_soem_soes.pdf).
