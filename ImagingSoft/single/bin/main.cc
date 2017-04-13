#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include "TROOT.h"
#include "TString.h"
#include "TVector3.h"

#include "In_3D_SingleCam_ComptonImager.hh"

using namespace std;

void DescribeInputParameters();

int main(int argc, char** argv){
    if(argc < 4){
        DescribeInputParameters();
        return -1;
    }
    TString infilename = argv[1];
    TString outfilename = argv[2];
    string parfilename = argv[3];
    
    In_3D_SingleCam_ComptonImager *imager = new In_3D_SingleCam_ComptonImager(infilename, outfilename);

    imager->LoadParameterFile(parfilename);
    imager->SetPrintDivNum(100);

    imager->Initialize();
    imager->Run();
    delete imager;
}

void DescribeInputParameters(){
    cout << "################### In_3D_SingleCam_ComptonImager ###################" << endl;
    cout << " 1 : camera rootfilename (including cetree)" << endl;
    cout << " 2 : output file name" << endl;
    cout << " 3 : parameter file name" << endl;
    cout << "###########################################################" << endl;
}
