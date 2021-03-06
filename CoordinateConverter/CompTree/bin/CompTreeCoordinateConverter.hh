#ifndef CompTreeCoordinateConverter_HH 
#define CompTreeCoordinateConverter_HH 

#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <map>
#include <string.h>
#include <vector>
#include "TROOT.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TRandom.h"
#include "TGraph.h"
#include "TVector3.h"
#include "TF1.h"
#include "TSpline.h"
#include "TMath.h"

#include "CompTreeReader.hh"
#include "CompTreeMaker.hh"
#include "In111Constant.hh"
#include "AnalysisParameter.hh"

using namespace std;

class CompTreeCoordinateConverter{
    private:
        CompTreeReader *in_cetree;
        CompTreeMaker *out_cetree;
        TString infilename;
        TString outfilename;

        Long64_t totaleventnum;
        Long64_t currenteventnum;
        
        Long64_t print_div_num;

        TString header;

        TVector3 pt_vector;
        double rotate_angle;

        bool delete_fluo;

	    Int_t num_hits;
        Int_t n_deleled_hit;

	    Float_t pt_hit1_posx;
	    Float_t pt_hit1_posy;
	    Float_t pt_hit1_posz;

	    Float_t pt_hit2_posx;
	    Float_t pt_hit2_posy;
	    Float_t pt_hit2_posz;

    protected:
        void CopyValueFile();
        void WriteEvent();
        void ConvertCoordinate();
        void LoadPosition();
        void ParallelTranslation(TVector3 vector);
        void RotateY(double angle);
        void SetPosition();
        bool DeleteFluo();

    public:
        CompTreeCoordinateConverter();
        ~CompTreeCoordinateConverter();

        void ReadInputfile(TString infilename);
        void SetOutfileHeader(TString header);
        void SetParallelTranslationVector(TVector3 pt_vector);
        void SetRotateAngle(double rotate_angle);
        void SetDeleteFluo(bool delete_fluo);

        void SetPrintDivNum(Long64_t print_div_num);

        void Initialize();
        void Run();
};

#endif
