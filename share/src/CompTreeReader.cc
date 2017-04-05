#include "CompTreeReader.hh"

/*
v1 -- 2017-02-19 by Hiroki Yoneda 
v2 -- Get_vecG を修正 2017-02-24 by Hiroki Yoneda 
v3 -- comptreeの変数をpublicに変更. exptimeを追加. 2017-03-22 by Hiroki Yoneda
v4 -- exptimeをinternal clk, external clkに分けて出力する関数を追加. 2017-03-22 by Hiroki Yoneda
*/

CompTreeReader::CompTreeReader(){

}

CompTreeReader::CompTreeReader(TString infilename){
    TFileReader::OpenInputFile(infilename);
    SetTreeName("cetree");
    Initialize();
}

CompTreeReader::CompTreeReader(TString infilename, TString treename){
    TFileReader::OpenInputFile(infilename);
    SetTreeName(treename);
    Initialize();
}

CompTreeReader::~CompTreeReader(){
}

void CompTreeReader::SetTreeName(TString treename){
	this->treename = treename;
}

void CompTreeReader::Initialize(){
	comptree = (TTree*)infile->Get(treename);
	totaleventnum = comptree->GetEntries();

	comptree->SetBranchAddress("exptime", &exptime);
	comptree->SetBranchAddress("eventid", &eventid);
	comptree->SetBranchAddress("num_hits", &num_hits);
	comptree->SetBranchAddress("hit1_id", &hit1_id);
	comptree->SetBranchAddress("hit1_process", &hit1_process);
	comptree->SetBranchAddress("hit1_detector", &hit1_detector);
	comptree->SetBranchAddress("hit1_readout_module", &hit1_readout_module);
	comptree->SetBranchAddress("hit1_section", &hit1_section);
	comptree->SetBranchAddress("hit1_channel", &hit1_channel);
	comptree->SetBranchAddress("hit1_pixelx", &hit1_pixelx);
	comptree->SetBranchAddress("hit1_pixely", &hit1_pixely);
	comptree->SetBranchAddress("hit1_time", &hit1_time);
	comptree->SetBranchAddress("hit1_posx", &hit1_posx);
	comptree->SetBranchAddress("hit1_posy", &hit1_posy);
	comptree->SetBranchAddress("hit1_posz", &hit1_posz);
	comptree->SetBranchAddress("hit1_energy", &hit1_energy);
	comptree->SetBranchAddress("hit2_id", &hit2_id);
	comptree->SetBranchAddress("hit2_process", &hit2_process);
	comptree->SetBranchAddress("hit2_detector", &hit2_detector);
	comptree->SetBranchAddress("hit2_readout_module", &hit2_readout_module);
	comptree->SetBranchAddress("hit2_section", &hit2_section);
	comptree->SetBranchAddress("hit2_channel", &hit2_channel);
	comptree->SetBranchAddress("hit2_pixelx", &hit2_pixelx);
	comptree->SetBranchAddress("hit2_pixely", &hit2_pixely);
	comptree->SetBranchAddress("hit2_time", &hit2_time);
	comptree->SetBranchAddress("hit2_posx", &hit2_posx);
	comptree->SetBranchAddress("hit2_posy", &hit2_posy);
	comptree->SetBranchAddress("hit2_posz", &hit2_posz);
	comptree->SetBranchAddress("hit2_energy", &hit2_energy);
	comptree->SetBranchAddress("flags", &flags);
	comptree->SetBranchAddress("costheta", &costheta);
	comptree->SetBranchAddress("dtheta", &dtheta);
	comptree->SetBranchAddress("phi", &phi);
	comptree->SetBranchAddress("hitpattern", &hitpattern);
	comptree->SetBranchAddress("grade", &grade);
	comptree->SetBranchAddress("likelihood", &likelihood);
	comptree->SetBranchAddress("ini_energy", &ini_energy);
	comptree->SetBranchAddress("ini_dirx", &ini_dirx);
	comptree->SetBranchAddress("ini_diry", &ini_diry);
	comptree->SetBranchAddress("ini_dirz", &ini_dirz);
	comptree->SetBranchAddress("ini_time", &ini_time);
	comptree->SetBranchAddress("ini_posx", &ini_posx);
	comptree->SetBranchAddress("ini_posy", &ini_posy);
	comptree->SetBranchAddress("ini_posz", &ini_posz);
	comptree->SetBranchAddress("ini_polarx", &ini_polarx);
	comptree->SetBranchAddress("ini_polary", &ini_polary);
	comptree->SetBranchAddress("ini_polarz", &ini_polarz);
	comptree->SetBranchAddress("weight", &weight);
}

void CompTreeReader::SetEntry(Long64_t i){
	comptree->GetEntry(i);
    currenteventnum = i;
}

Long64_t CompTreeReader::GetEntries(){
	return totaleventnum;	
}

Long64_t CompTreeReader::GetEntry(){
	return currenteventnum;	
}

Float_t CompTreeReader::GetTotalEnergy(){
	return hit1_energy + hit2_energy; 
}

ULong64_t CompTreeReader::Get_eventid(){
	return eventid;
}

Short_t CompTreeReader::Get_num_hits(){
	return num_hits;
}

Short_t CompTreeReader::Get_hit1_id(){
	return hit1_id;
}

UInt_t  CompTreeReader::Get_hit1_process(){
	return hit1_process;
}

Short_t CompTreeReader::Get_hit1_detector(){
	return hit1_detector;
}

vector<Short_t> CompTreeReader::Get_hit1_pixel(){
	vector<Short_t> pixel(hit1_pixelx, hit1_pixely);
	return pixel;
}

TVector3 CompTreeReader::Get_hit1_pos(){
	TVector3 pos(hit1_posx, hit1_posy, hit1_posz);
	return pos;
}

Float_t CompTreeReader::Get_hit1_energy(){
	return hit1_energy;
}

Short_t CompTreeReader::Get_hit2_id(){
	return hit2_id;
}

UInt_t  CompTreeReader::Get_hit2_process(){
	return hit2_process;
}

Short_t CompTreeReader::Get_hit2_detector(){
	return hit2_detector;
}

vector<Short_t> CompTreeReader::Get_hit2_pixel(){
	vector<Short_t> pixel(hit2_pixelx, hit2_pixely);
	return pixel;
}

TVector3 CompTreeReader::Get_hit2_pos(){
	TVector3 pos(hit2_posx, hit2_posy, hit2_posz);
	return pos;
}

Float_t CompTreeReader::Get_hit2_energy(){
	return hit2_energy;
}

Float_t CompTreeReader::Get_costheta(){
	return costheta;
}

Float_t CompTreeReader::Get_dtheta(){
	return dtheta;
}

Float_t CompTreeReader::Get_phi(){
	return phi;
}

ULong64_t  CompTreeReader::Get_hitpattern(){
	return hitpattern;
}

TVector3 CompTreeReader::Get_vecG(){
	TVector3 pos_first(hit1_posx, hit1_posy, hit1_posz);
	TVector3 pos_second(hit2_posx, hit2_posy, hit2_posz);

    TVector3 vecG = pos_first - pos_second;
    vecG = vecG.Unit();

    return vecG;
}

Long64_t CompTreeReader::GetExternalClk(){
    Long64_t externalclk = exptime >> 32;
    return externalclk;
}

Long64_t CompTreeReader::GetInternalClk(){
    Long64_t internalclk = exptime & 0xffffffff;
    return internalclk;
}
