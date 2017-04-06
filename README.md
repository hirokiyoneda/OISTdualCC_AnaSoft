# Analysis Soft for Dual Compton Cameras Experiment @ OIST.
-----
### current status
###### v1.0.0 by Hiroki Yoneda -- 2017 04 05
###### v1.0.1 by Hiroki Yoneda -- 2017 04 05
- add parameter "gauss_FWHM" in parameter file
###### v1.0.2 by Hiroki Yoneda -- 2017 04 05
- gauss_FWHM depends on photon energy
###### v1.1.0_dev by Hiroki Yoneda -- 2017 04 06
- renormazalition function is added (optional).
--------
### install
##### install share library
1.`cd share`  
2.`mkdir build`  
3.`cmake ..` (if it does not work, add `--DROOT_AUTOSEARCH`)  
##### install imaging soft
1.`cd ImagingSoft`  
2.`mkdir build`  
3.`cmake ..` (if it does not work, add `--DROOT_AUTOSEARCH`)  
