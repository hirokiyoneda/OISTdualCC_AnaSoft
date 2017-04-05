#ifndef TFileReader_HH 
#define TFileReader_HH 

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

using namespace std;

class TFileReader{
   protected:
      TFile* infile;
      TString infilename;

   public :
      ~TFileReader();
      TFileReader();
      TFileReader(TString infilename);
      void OpenInputFile(TString infilename);
};

#endif
