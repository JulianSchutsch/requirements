Building the requirements tools:

You need:

 cmake, at least version 3
 g++, at least version 4.9 (aquivalent clang may also work)
 libxml
 libgtkmm-3.0-dev (for the graphical tool)
 googletest (very recent version, best is from source)

Once all this is available, in the projects root folder type:

 mkdir -p build
 cd build
 cmake ..
 make

You can also limit building to certain targets:

 make req     Only build console requirement tool
 make greq    Only build graphical requirement tool
 make qreq    Only build new graphical requirement tool
 make t       Only build tests

All three are based on the requirements library, librequirements.

There is also a way to limit what CMAKE compiles into the makefile, hence
limiting the dependencies on libraries,

You can pass to cmake, additionally to the ".." for the CMakeLists.txt
folder,

-DDISABLE_REQ=1   Disable console requirements tool
-DDISABLE_GREQ=1  Disable graphical requirements tool
-DDISABLE_QREQ=1  Disable new graphical requirements tool
-DDISABLE_GTEST=1 Disable test binary
