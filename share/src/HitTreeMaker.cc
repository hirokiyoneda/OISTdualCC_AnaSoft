#include "HitTreeMaker.hh"

/*
v1 -- 2017-03-22 by Hiroki Yoneda 
*/

HitTreeMaker::~HitTreeMaker(){
}

HitTreeMaker::HitTreeMaker(){
    treename = "hittree";
}

void HitTreeMaker::SaveFile(){
    outfile->cd("/");
    hittree->Write();
}

void HitTreeMaker::SetTreeName(TString treename){
    this->treename = treename;
}

void HitTreeMaker::Initialize(TString outfilename){
    TFileMaker::SetOutputFilename(outfilename);
    TFileMaker::MakeOutputFile();

	hittree = new TTree(treename, treename);
    
    neweventid = 0;

    hittree->Branch("exptime", &exptime, "exptime/l");
    hittree->Branch("eventid", &eventid, "eventid/L");
    hittree->Branch("ihit", &ihit, "ihit/S");
    hittree->Branch("num_hits", &num_hits, "num_hits/I");
    hittree->Branch("time", &time, "time/L");
    hittree->Branch("instrument", &instrument, "instrument/S");
    hittree->Branch("detector", &detector, "detector/S");
    hittree->Branch("det_section", &det_section, "det_section/S");
    hittree->Branch("readout_module", &readout_module, "readout_module/S");
    hittree->Branch("section", &section, "section/S");
    hittree->Branch("channel", &channel, "channel/S");
    hittree->Branch("pixelx", &pixelx, "pixelx/S");
    hittree->Branch("pixely", &pixely, "pixely/S");
    hittree->Branch("rawpha", &rawpha, "rawpha/I");
    hittree->Branch("pha", &pha, "pha/F");
    hittree->Branch("epi", &epi, "epi/F");
    hittree->Branch("flag_data", &flag_data, "flag_data/l");
    hittree->Branch("flags", &flags, "flags/l");
    hittree->Branch("real_time", &real_time, "real_time/F");
    hittree->Branch("time_group", &time_group, "time_group/S");
    hittree->Branch("real_posx", &real_posx, "real_posx/F");
    hittree->Branch("real_posy", &real_posy, "real_posy/F");
    hittree->Branch("real_posz", &real_posz, "real_posz/F");
    hittree->Branch("edep", &edep, "edep/F");
    hittree->Branch("echarge", &echarge, "echarge/F");
    hittree->Branch("process", &process, "process/i");
    hittree->Branch("energy", &energy, "energy/F");
    hittree->Branch("posx", &posx, "posx/F");
    hittree->Branch("posy", &posy, "posy/F");
    hittree->Branch("posz", &posz, "posz/F");
    hittree->Branch("local_posx", &local_posx, "local_posx/F");
    hittree->Branch("local_posy", &local_posy, "local_posy/F");
    hittree->Branch("local_posz", &local_posz, "local_posz/F");
    hittree->Branch("grade", &grade, "grade/I");
}

void HitTreeMaker::FillNewEvent(){
//    eventid = neweventid;

    hittree->Fill();
    ++neweventid;
}
