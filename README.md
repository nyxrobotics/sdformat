# sdformat #

SDF is an XML file format that describes environments, objects, and robots
in a manner suitable for robotic applications. SDF is capable of representing
and describing different physic engines, lighting properties, terrain, static
or dynamic objects, and articulated robots with various sensors, and acutators.
The format of SDF is also described by XML, which facilitates updates and
allows conversion from previous versions. A parser is also contained within
this package that reads SDF files and returns a C++ interface.

Test coverage:

[![codecov](https://codecov.io/bb/osrf/sdformat/branch/default/graph/badge.svg)](https://codecov.io/bb/osrf/sdformat)


## Installation ##

Standard installation can be performed in UNIX systems using the following
steps:

 - mkdir build/
 - cd build/
 - cmake ..
 - sudo dpkg -r libsdformat6-dev libsdformat6 sdformat-sdf
 - sudo checkinstall -y -D --pkgname=sdformat-roki --pkgversion=6.2.0


sdformat supported cmake parameters at configuring time:
 - USE_INTERNAL_URDF (bool) [default False]
   Use an internal copy of urdfdom 1.0.0 instead of look for one
   installed in the system
 - USE_UPSTREAM_CFLAGS (bool) [default True]
   Use the sdformat team compilation flags instead of the common set defined
   by cmake.

## Uninstallation ##

To uninstall the software installed with the previous steps:
 - cd build/
 - sudo dpkg -r sdformat-roki
