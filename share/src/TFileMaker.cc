#include "TFileMaker.hh"

/*
v1 -- 2017-02-19 by Hiroki Yoneda 
*/

TFileMaker::~TFileMaker(){
    outfile->Close();
}

TFileMaker::TFileMaker(){

}

TFileMaker::TFileMaker(TString outfilename){
    SetOutputFilename(outfilename);
    MakeOutputFile();
//    Initialize();
}

void TFileMaker::SetOutputFilename(TString outfilename){
    this->outfilename = outfilename;
}

void TFileMaker::MakeOutputFile(){
    outfile = new TFile(outfilename, "recreate");
}
