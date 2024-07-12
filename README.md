An example program that demonstrates our current problem with Kokkos::sort
on Aurora. This program will only fail when compiled with some specific
options for the compiler set with environment variables.

In addition to the program that demonstrates the problem
([sort-fail.cxx](sort-fail.cxx)), I have also included a build script
([build.bash](build.bash)) that sets up the build parameters where we see
the problem. (Compiling differently might not tickle the bug.)

This git repo contains a submodule pointing to the Kokkos source. To
compile properly, make sure the submodule is properly initialized and
updated.
