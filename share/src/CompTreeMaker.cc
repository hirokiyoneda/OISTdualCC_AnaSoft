#include "CompTreeMaker.hh"

/*
v1 -- 2017-03-22 by Hiroki Yoneda 
v1.1.1 -- hit2_time を修正 2017-03-25 by Hiroki Yoneda
*/

CompTreeMaker::~CompTreeMaker(){
}

CompTreeMaker::CompTreeMaker(){
    treename = "cetree";
}

void CompTreeMaker::SaveFile(){
    outfile->cd("/");
    comptree->Write();
}

void CompTreeMaker::SetTreeName(TString treename){
    this->treename = treename;
}

void CompTreeMaker::Initialize(TString outfilename){
    TFileMaker::SetOutputFilename(outfilename);
    TFileMaker::MakeOutputFile();

	comptree = new TTree(treename, treename);
    
    neweventid = 0;

	comptree->Branch("exptime", &exptime, "exptime/l");
	comptree->Branch("eventid", &eventid, "eventid/l");
	comptree->Branch("num_hits", &num_hits, "num_hits/S");
	comptree->Branch("hit1_id", &hit1_id, "hit1_id/S");
	comptree->Branch("hit1_process", &hit1_process, "hit1_process/i");
	comptree->Branch("hit1_detector", &hit1_detector, "hit1_detector/S");
	comptree->Branch("hit1_readout_module", &hit1_readout_module, "hit1_readout_module/S");
	comptree->Branch("hit1_section", &hit1_section, "hit1_section/S");
	comptree->Branch("hit1_channel", &hit1_channel, "hit1_channel/S");
	comptree->Branch("hit1_pixelx", &hit1_pixelx, "hit1_pixelx/S");
	comptree->Branch("hit1_pixely", &hit1_pixely, "hit1_pixely/S");
	comptree->Branch("hit1_time", &hit1_time, "hit1_time/F");
	comptree->Branch("hit1_posx", &hit1_posx, "hit1_posx/F");
	comptree->Branch("hit1_posy", &hit1_posy, "hit1_posy/F");
	comptree->Branch("hit1_posz", &hit1_posz, "hit1_posz/F");
	comptree->Branch("hit1_energy", &hit1_energy, "hit1_energy/F");
	comptree->Branch("hit2_id", &hit2_id, "hit2_id/S");
	comptree->Branch("hit2_process", &hit2_process, "hit2_process/i");
	comptree->Branch("hit2_detector", &hit2_detector, "hit2_detector/S");
	comptree->Branch("hit2_readout_module", &hit2_readout_module, "hit2_readout_module/S");
	comptree->Branch("hit2_section", &hit2_section, "hit2_section/S");
	comptree->Branch("hit2_channel", &hit2_channel, "hit2_channel/S");
	comptree->Branch("hit2_pixelx", &hit2_pixelx, "hit2_pixelx/S");
	comptree->Branch("hit2_pixely", &hit2_pixely, "hit2_pixely/S");
	comptree->Branch("hit2_time", &hit2_time, "hit2_time/F");
	comptree->Branch("hit2_posx", &hit2_posx, "hit2_posx/F");
	comptree->Branch("hit2_posy", &hit2_posy, "hit2_posy/F");
	comptree->Branch("hit2_posz", &hit2_posz, "hit2_posz/F");
	comptree->Branch("hit2_energy", &hit2_energy, "hit2_energy/F");
	comptree->Branch("flags", &flags, "flags/l");
	comptree->Branch("costheta", &costheta, "costheta/F");
	comptree->Branch("dtheta", &dtheta, "dtheta/F");
	comptree->Branch("phi", &phi, "phi/F");
	comptree->Branch("hitpattern", &hitpattern, "hitpattern/l");
	comptree->Branch("grade", &grade, "grade/I");
	comptree->Branch("likelihood", &likelihood, "likelihood/F");
	comptree->Branch("ini_energy", &ini_energy, "ini_energy/F");
	comptree->Branch("ini_dirx", &ini_dirx, "ini_dirx/F");
	comptree->Branch("ini_diry", &ini_diry, "ini_diry/F");
	comptree->Branch("ini_dirz", &ini_dirz, "ini_dirz/F");
	comptree->Branch("ini_time", &ini_time, "ini_time/F");
	comptree->Branch("ini_posx", &ini_posx, "ini_posx/F");
	comptree->Branch("ini_posy", &ini_posy, "ini_posy/F");
	comptree->Branch("ini_posz", &ini_posz, "ini_posz/F");
	comptree->Branch("ini_polarx", &ini_polarx, "ini_polarx/F");
	comptree->Branch("ini_polary", &ini_polary, "ini_polary/F");
	comptree->Branch("ini_polarz", &ini_polarz, "ini_polarz/F");
	comptree->Branch("weight", &weight, "weight/F");
}

void CompTreeMaker::FillNewEvent(){
//    eventid = neweventid;

    comptree->Fill();
    ++neweventid;
}
