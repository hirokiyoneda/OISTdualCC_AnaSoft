#ifndef ExtractCoincidenceEventTree_HH 
#define ExtractCoincidenceEventTree_HH 

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

#include "EventTreeReader.hh"
#include "EventTreeMaker.hh"

using namespace std;

class ExtractCoincidenceEventTree{
    private:
        EventTreeReader *in_eventtree1, *in_eventtree2;
        TString infilename_c1, infilename_c2;
        TString outfilename_c1, outfilename_c2;

        TH1D *hist_difftimestamp;
        TH2D *hist2d_difftimestamp;

        EventTreeMaker *out_eventtree1, *out_eventtree2;

        Long64_t totaleventnum1, currenteventnum1;

        Long64_t totaleventnum2, currenteventnum2;

        Long64_t print_div_num;

        Long64_t coincidence_number;

        Long64_t extclk1, extclk2;
        Long64_t intclk1, intclk2;

        static const Long64_t MAXINTERNALCLK;

        TString header; 

        Long64_t timewindow;

    protected:
        int CheckCoincidence();
        Long64_t GetDiffTimeStamp();
        void WriteCoincidenceEvent();
        void CopyValueFile(int num);

    public:
        ExtractCoincidenceEventTree();
        ~ExtractCoincidenceEventTree();
        void ReadFirstInputfile(TString infilename);
        void ReadSecondInputfile(TString infilename);
        void SetTimeWindow(Long64_t timewindow);
        void SetOutfileHeader(TString header);
        void SetPrintDivNum(Long64_t print_div_num);
        void Initialize();
        void Run();
};

#endif
