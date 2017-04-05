#include "TFileReader.hh"

/*
v1 -- 2017-02-19 by Hiroki Yoneda 
*/

TFileReader::TFileReader(){
}

TFileReader::TFileReader(TString infilename){
    OpenInputFile(infilename);
}

TFileReader::~TFileReader(){
    infile->Close();
}

void TFileReader::OpenInputFile(TString infilename){
    this->infilename = infilename;
    infile = new TFile(infilename, "r");
}
