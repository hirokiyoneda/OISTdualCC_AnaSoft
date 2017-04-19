#ifndef ComptonEventReconstruction_HH
#define ComptonEventReconstruction_HH

#include <iostream>
#include <vector>
#include "TROOT.h"
#include "TVector3.h"
#include "TMath.h"

#include "PhysicalConstant.hh"

void EventReconstruction(double &e, TVector3 &c_pos, double &c_costheta, double &c_dtheta, TVector3 &c_vecG,
Double_t e_first, Double_t e_second, TVector3 c_pos_first, TVector3 c_pos_second, Short_t c_detector_first, Short_t c_detector_second);

void SwapPosition(TVector3 &pos1, TVector3 &pos2);
void SwapEnergy(double &e1, double &e2);
void SwapDetector(Short_t &detector1, Short_t &detector2);

Double_t GetMaxDepositEnergy(double e);

Double_t CalCosTheta(double e, double e_first);
TVector3 Get_vecG(TVector3 pos1, TVector3 pos2);

#endif 
