#include "ComptonEventReconstruction.hh"

void EventReconstruction(double &e, TVector3 &c_pos, double &c_costheta, double &c_dtheta, TVector3 &c_vecG,
Double_t e_first, Double_t e_second, TVector3 c_pos_first, TVector3 c_pos_second, Short_t c_detector_first, Short_t c_detector_second){
    if(e_first > GetMaxDepositEnergy(e)){
        SwapPosition(c_pos_first, c_pos_second);
        SwapEnergy(e_first, e_second);
        SwapDetector(c_detector_first, c_detector_second);
    }else if(e_second > GetMaxDepositEnergy(e)){
        //do nothing
    }else if(c_detector_first < 10 & c_detector_second >= 10){
        //do nothing
    }else if(c_detector_first >= 10 & c_detector_second < 10){
        SwapPosition(c_pos_first, c_pos_second);
        SwapEnergy(e_first, e_second);
        SwapDetector(c_detector_first, c_detector_second);
    }else if(e_first > e_second){
        SwapPosition(c_pos_first, c_pos_second);
        SwapEnergy(e_first, e_second);
        SwapDetector(c_detector_first, c_detector_second);
    }

    c_pos = c_pos_first;
    c_costheta = CalCosTheta(e, e_first);
    c_vecG = Get_vecG(c_pos_first, c_pos_second);
}

void SwapPosition(TVector3 &pos1, TVector3 &pos2){
    TVector3 tmp_pos = pos1;
    pos1 = pos2;
    pos2 = tmp_pos;
}

void SwapEnergy(double &e1, double &e2){
    double tmp_e = e1;
    e1 = e2;
    e2 = tmp_e;
}

void SwapDetector(Short_t &detector1, Short_t &detector2){
    Short_t tmp_detector = detector1;
    detector1 = detector2;
    detector2 = tmp_detector;
}

Double_t GetMaxDepositEnergy(double e){
    return e - e/(1 + e/PhysicalConstant::ElectronMass*2);
}

Double_t CalCosTheta(double e, double e_first){
    return 1 - PhysicalConstant::ElectronMass*(1.0/(e - e_first) - 1.0/e);
}

TVector3 Get_vecG(TVector3 pos1, TVector3 pos2){
    TVector3 vecG = pos1 - pos2;
    vecG = vecG.Unit();
    
    return vecG;
}
