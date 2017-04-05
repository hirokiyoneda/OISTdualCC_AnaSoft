#ifndef CompTreeMaker_HH 
#define CompTreeMaker_HH 

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

class CompTreeMaker : public TFileMaker{
    private:
        TTree *comptree;

        ULong64_t neweventid;

        TString treename;

    protected:

    public:
	  ULong64_t exptime;
	  ULong64_t eventid;
	  Short_t num_hits;
	  Short_t hit1_id;
	  UInt_t  hit1_process;
	  Short_t hit1_detector;
	  Short_t hit1_readout_module;
	  Short_t hit1_section;
	  Short_t hit1_channel;
	  Short_t hit1_pixelx;
	  Short_t hit1_pixely;
	  Float_t hit1_time;
	  Float_t hit1_posx;
	  Float_t hit1_posy;
	  Float_t hit1_posz;
	  Float_t hit1_energy;
	  Short_t hit2_id;
	  UInt_t  hit2_process;
	  Short_t hit2_detector;
	  Short_t hit2_readout_module;
	  Short_t hit2_section;
	  Short_t hit2_channel;
	  Short_t hit2_pixelx;
	  Short_t hit2_pixely;
	  Float_t hit2_time;
	  Float_t hit2_posx;
	  Float_t hit2_posy;
	  Float_t hit2_posz;
	  Float_t hit2_energy;
	  ULong64_t flags;
	  Float_t costheta;
	  Float_t dtheta;
	  Float_t phi;
	  ULong64_t  hitpattern;
	  Int_t   grade;
	  Float_t likelihood;
	  Float_t ini_energy;
	  Float_t ini_dirx;
	  Float_t ini_diry;
	  Float_t ini_dirz;
	  Float_t ini_time;
	  Float_t ini_posx;
	  Float_t ini_posy;
	  Float_t ini_posz;
	  Float_t ini_polarx;
	  Float_t ini_polary;
	  Float_t ini_polarz;
	  Float_t weight;

      CompTreeMaker();
      ~CompTreeMaker();
      void FillNewEvent();
      void Initialize(TString outfilename);
      void SaveFile();
      void SetTreeName(TString treename);
};

#endif
