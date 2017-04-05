#ifndef HitTreeMaker_HH 
#define HitTreeMaker_HH 

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

class HitTreeMaker : public TFileMaker{
    private:
        TTree *hittree;

        ULong64_t neweventid;

        TString treename;

    protected:

    public:
	  ULong64_t exptime;
	  Long64_t eventid;
	  Short_t ihit;
	  Int_t num_hits;
	  Long64_t time;
	  Short_t instrument;
	  Short_t detector;
	  Short_t det_section;
	  Short_t readout_module;
	  Short_t section;
	  Short_t channel;
	  Short_t pixelx;
	  Short_t pixely;
	  Int_t rawpha;
	  Float_t pha;
	  Float_t epi;
	  ULong64_t flag_data;
	  ULong64_t flags;
	  Float_t real_time;
	  Short_t time_group;
	  Float_t real_posx;
	  Float_t real_posy;
	  Float_t real_posz;
	  Float_t edep;
	  Float_t echarge;
	  UInt_t process;
	  Float_t energy;
	  Float_t posx;
	  Float_t posy;
	  Float_t posz;
	  Float_t local_posx;
	  Float_t local_posy;
	  Float_t local_posz;
	  Int_t grade;

      HitTreeMaker();
      ~HitTreeMaker();
      void FillNewEvent();
      void Initialize(TString outfilename);
      void SaveFile();
      void SetTreeName(TString treename);
};

#endif
