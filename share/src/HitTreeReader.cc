#include "HitTreeReader.hh"

/*
v1 -- 2017-03-24 by Hiroki Yoneda 
*/

HitTreeReader::HitTreeReader(){

}

HitTreeReader::HitTreeReader(TString infilename){
    TFileReader::OpenInputFile(infilename);
    SetTreeName("hittree");
    Initialize();
}

HitTreeReader::HitTreeReader(TString infilename, TString treename){
    TFileReader::OpenInputFile(infilename);
    SetTreeName(treename);
    Initialize();
}

HitTreeReader::~HitTreeReader(){
}

void HitTreeReader::SetTreeName(TString treename){
	this->treename = treename;
}

void HitTreeReader::Initialize(){
	hittree = (TTree*)infile->Get(treename);
	totaleventnum = hittree->GetEntries();

	hittree->SetBranchAddress("exptime", &exptime);
	hittree->SetBranchAddress("eventid", &eventid);
	hittree->SetBranchAddress("ihit", &ihit);
	hittree->SetBranchAddress("num_hits", &num_hits);
	hittree->SetBranchAddress("time", &time);
	hittree->SetBranchAddress("instrument", &instrument);
	hittree->SetBranchAddress("detector", &detector);
	hittree->SetBranchAddress("det_section", &det_section);
	hittree->SetBranchAddress("readout_module", &readout_module);
	hittree->SetBranchAddress("section", &section);
	hittree->SetBranchAddress("channel", &channel);
	hittree->SetBranchAddress("pixelx", &pixelx);
	hittree->SetBranchAddress("pixely", &pixely);
	hittree->SetBranchAddress("rawpha", &rawpha);
	hittree->SetBranchAddress("pha", &pha);
	hittree->SetBranchAddress("epi", &epi);
	hittree->SetBranchAddress("flag_data", &flag_data);
	hittree->SetBranchAddress("flags", &flags);
	hittree->SetBranchAddress("real_time", &real_time);
	hittree->SetBranchAddress("time_group", &time_group);
	hittree->SetBranchAddress("real_posx", &real_posx);
	hittree->SetBranchAddress("real_posy", &real_posy);
	hittree->SetBranchAddress("real_posz", &real_posz);
	hittree->SetBranchAddress("edep", &edep);
	hittree->SetBranchAddress("echarge", &echarge);
	hittree->SetBranchAddress("process", &process);
	hittree->SetBranchAddress("energy", &energy);
	hittree->SetBranchAddress("posx", &posx);
	hittree->SetBranchAddress("posy", &posy);
	hittree->SetBranchAddress("posz", &posz);
	hittree->SetBranchAddress("local_posx", &local_posx);
	hittree->SetBranchAddress("local_posy", &local_posy);
	hittree->SetBranchAddress("local_posz", &local_posz);
	hittree->SetBranchAddress("grade", &grade);
}

void HitTreeReader::SetEntry(Long64_t i){
	hittree->GetEntry(i);
    currenteventnum = i;
}

Long64_t HitTreeReader::GetEntries(){
	return totaleventnum;	
}

Long64_t HitTreeReader::GetEntry(){
	return currenteventnum;	
}
Long64_t HitTreeReader::GetExternalClk(){
    Long64_t externalclk = exptime >> 32;
    return externalclk;
}

Long64_t HitTreeReader::GetInternalClk(){
    Long64_t internalclk = exptime & 0xffffffff;
    return internalclk;
}
