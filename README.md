This is Analysis Soft for Dual Compton Cameras Experiment @ OIST.
--------
v1.0.0 written by Hiroki Yoneda -- 2017 04 05
--------
- add parameter "gauss_FWHM" in parameter file
v1.0.1 written by Hiroki Yoneda -- 2017 04 05
--------
- renormazalition function is added. (option)
- gauss_FWHM depends on photon energy/
v1.1.1_dev written by Hiroki Yoneda -- 2017 04 06
--------

====== install ======

- install share library
1. cd share
2. mkdir build
3. cmake .. (if it does not work, add "--DROOT_AUTOSEARCH")

- install imaging soft
1. cd ImagingSoft
2. mkdir build
3. cmake .. (if it does not work, add "--DROOT_AUTOSEARCH")

======================
