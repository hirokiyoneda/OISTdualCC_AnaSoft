#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <string.h>
#include "TROOT.h"
#include "TString.h"
#include "TVector3.h"

#include "In_3D_ComptonImager.hh"

using namespace std;

void DescribeInputParameters();

int main(int argc, char** argv){
    if(argc < 5){
        DescribeInputParameters();
        return -1;
    }
    TString infilename_c1 = argv[1];
    TString infilename_c2 = argv[2];
    TString outfilename = argv[3];
    string parfilename = argv[4];
    
    In_3D_ComptonImager *imager = new In_3D_ComptonImager(infilename_c1, infilename_c2, outfilename);

    imager->LoadParameterFile(parfilename);
    imager->SetPrintDivNum(100);

    imager->Initialize();
    imager->Run();
    delete imager;
}

void DescribeInputParameters(){
    cout << "################### In_3D_ComptonImager ###################" << endl;
    cout << " 1 : camera 1 rootfilename (including cetree)" << endl;
    cout << " 2 : camera 2 rootfilename (including cetree)" << endl;
    cout << " 3 : output file name" << endl;
    cout << " 4 : parameter file name" << endl;
    cout << "###########################################################" << endl;
}
