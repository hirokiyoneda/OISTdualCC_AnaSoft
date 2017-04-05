#ifndef TFileMaker_HH 
#define TFileMaker_HH 

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

class TFileMaker{
    protected:
        TFile *outfile;
        TString outfilename;

    public:
        ~TFileMaker();
        TFileMaker();
        TFileMaker(TString outfilename);
	    
        void SetOutputFilename(TString outfilename);
        void MakeOutputFile();

        void Initialize(){};
        void Run(){};
};

#endif
