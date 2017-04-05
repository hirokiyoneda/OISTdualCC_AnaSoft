#ifndef CompTreeReader_HH 
#define CompTreeReader_HH 

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
#include "TRandom.h"
#include "TGraph.h"
#include "TVector3.h"
#include "TF1.h"
#include "TSpline.h"
#include "TMath.h"

#include "TFileReader.hh"

using namespace std;

class CompTreeReader : public TFileReader{
    protected:
      TString treename;
      TTree* comptree;
      Long64_t totaleventnum;
      Long64_t currenteventnum;

    public :
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

   public :
      ~CompTreeReader();
      CompTreeReader();
      CompTreeReader(TString infilename);
	  CompTreeReader(TString infilename, TString treename);

	  void SetTreeName(TString treename);
	  void Initialize();

	  void SetEntry(Long64_t i);
	  Long64_t GetEntries();
	  Long64_t GetEntry();
	  Float_t GetTotalEnergy();

	  ULong64_t Get_eventid();
	  Short_t Get_num_hits();

	  Short_t Get_hit1_id();
	  UInt_t  Get_hit1_process();
	  Short_t Get_hit1_detector();
	  vector<Short_t> Get_hit1_pixel();
	  TVector3 Get_hit1_pos();
	  Float_t Get_hit1_energy();

	  Short_t Get_hit2_id();
	  UInt_t  Get_hit2_process();
	  Short_t Get_hit2_detector();
	  vector<Short_t> Get_hit2_pixel();
	  TVector3 Get_hit2_pos();
	  Float_t Get_hit2_energy();

	  Float_t Get_costheta();
	  Float_t Get_dtheta();
	  Float_t Get_phi();
	  ULong64_t  Get_hitpattern();

	  TVector3 Get_vecG();

	  Long64_t GetExternalClk();
	  Long64_t GetInternalClk();
};

#endif
