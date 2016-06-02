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
   * `cmake .. -G "NMake Makefiles"
   * `nmake`

   OR
   
 * Start a console then:
   * `mkdir build`
   * `cd build`
   * `cmake .. -G "Visual Studio 12"
   * `cmake --build .`
      
Linux
-----

   * `mkdir build`
   * `cd build`
   * `cmake ..`
   * `make`

INtime (Visual Studio)
----------------------

 * Install INtime SDK and the INtime plug in for Visual Studio
 * Copy the file .\CMakeINtime\CompilerId\VS-10.vcxproj.in" to
   [CMake installation folder location]\share\cmake-X.Y\Modules\CompilerId
   (please see https://cmake.org/pipermail/cmake/2014-November/059177.html for details)

 * Start a Visual Studio command prompt OR a console then:
   * `mkdir build`
   * `cd build`
   * `cmake .. -G "Visual Studio 12" -DCOMPILATION_FOR_INTIME:BOOL=TRUE`
   * `cmake --build .`
   
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
