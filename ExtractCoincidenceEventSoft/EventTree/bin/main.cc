#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include "TROOT.h"
#include "TString.h"
#include "TVector3.h"

#include "ExtractCoincidenceEventTree.hh"

using namespace std;

void DescribeInputParameters();

int main(int argc, char** argv){
    if(argc < 4){
        DescribeInputParameters();
        return -1;
    }

    TString infilename_c1 = argv[1];
    TString infilename_c2 = argv[2];

    int timewindow = atoi(argv[3]);

    ExtractCoincidenceEventTree *extracter = new ExtractCoincidenceEventTree();
    
    extracter->SetPrintDivNum(10000);
    extracter->ReadFirstInputfile(infilename_c1);
    extracter->ReadSecondInputfile(infilename_c2);
    extracter->SetTimeWindow(timewindow);

    TString header = argv[3];
    header = "coin-width_" + header + "_"; 
    extracter->SetOutfileHeader(header);

    extracter->Initialize();
    extracter->Run();

    delete extracter;
}

void DescribeInputParameters(){
    cout << "1 : camera1 rootfilename (including eventtree)" << endl;
    cout << "2 : camera2 rootfilename (including eventtree)" << endl;
    cout << "3 : timewindow" << endl;
}

