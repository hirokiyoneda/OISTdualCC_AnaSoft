#ifndef EventTreeMaker_HH 
#define EventTreeMaker_HH 

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

#include "TFileMaker.hh"

using namespace std;

class EventTreeMaker : public TFileMaker{
    private:
        TTree *eventtree;

        ULong64_t neweventid;

        TString treename;

    protected:

    public:
      UInt_t chflag[40][3];
      UShort_t cmn[40];
      UShort_t adc[40][64];
      UShort_t index[40][64];
      UShort_t ref[40];
      UShort_t hitnum[40];
      Double_t cmn_ex[40];
      UInt_t ti;
      UInt_t livetime; 
      UInt_t integral_livetime;
      UShort_t trighitpat;
      UInt_t unixtime;
      UShort_t adcclkcnt;
      UInt_t ext1pps;
      UInt_t msec_counter;
      UInt_t gpstime;
      UInt_t pctime;

      EventTreeMaker();
      ~EventTreeMaker();
      void FillNewEvent();
      void Initialize(TString outfilename);
      void SaveFile();
      void SetTreeName(TString treename);
};

#endif
