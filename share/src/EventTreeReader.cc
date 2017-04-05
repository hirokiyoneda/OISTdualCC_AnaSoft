#include "EventTreeReader.hh"

/*
v1 -- 2017-03-22 by Hiroki Yoneda 
*/

EventTreeReader::EventTreeReader(){

}

EventTreeReader::EventTreeReader(TString infilename){
    TFileReader::OpenInputFile(infilename);
    SetTreeName("eventtree");
    Initialize();
}

EventTreeReader::EventTreeReader(TString infilename, TString treename){
    TFileReader::OpenInputFile(infilename);
    SetTreeName(treename);
    Initialize();
}

EventTreeReader::~EventTreeReader(){
}

void EventTreeReader::SetTreeName(TString treename){
	this->treename = treename;
}

void EventTreeReader::Initialize(){
	eventtree = (TTree*)infile->Get(treename);
	totaleventnum = eventtree->GetEntries();

    for(int i = 0;i<40;++i){
        char chflagname[30];
        char cmnname[30];
        char adcname[30];
        char indexname[30];
        char refname[30];
        char hitnumname[30];
        char cmn_exname[30];
        sprintf(chflagname,"chflag%d",i);
        sprintf(cmnname,"cmn%d",i);
        sprintf(adcname,"adc%d",i);
        sprintf(indexname,"index%d",i);
        sprintf(refname,"ref%d",i);
        sprintf(hitnumname,"hitnum%d",i);
        sprintf(cmn_exname,"cmn%d_ex",i);

        eventtree->SetBranchAddress(chflagname,chflag[i]);
        eventtree->SetBranchAddress(cmnname,&cmn[i]);
        eventtree->SetBranchAddress(adcname,adc[i]);
        eventtree->SetBranchAddress(indexname,index[i]);
        eventtree->SetBranchAddress(refname,&ref[i]);
        eventtree->SetBranchAddress(hitnumname,&hitnum[i]);
        eventtree->SetBranchAddress(cmn_exname,&cmn_ex[i]);
    }

    eventtree->SetBranchAddress("ti",&ti);
    eventtree->SetBranchAddress("livetime",&livetime);
    eventtree->SetBranchAddress("integral_livetime",&integral_livetime);
    eventtree->SetBranchAddress("trighitpat",&trighitpat);
    eventtree->SetBranchAddress("unixtime",&unixtime);
    eventtree->SetBranchAddress("adcclkcnt",&adcclkcnt);
    eventtree->SetBranchAddress("ext1pps",&ext1pps);
    eventtree->SetBranchAddress("msec_counter",&msec_counter);
    eventtree->SetBranchAddress("gpstime",&gpstime);
    eventtree->SetBranchAddress("pctime",&pctime);
}

void EventTreeReader::SetEntry(Long64_t i){
	eventtree->GetEntry(i);
    currenteventnum = i;
}

Long64_t EventTreeReader::GetEntries(){
	return totaleventnum;	
}

Long64_t EventTreeReader::GetEntry(){
	return currenteventnum;	
}

Long64_t EventTreeReader::GetExternalClk(){
    Long64_t externalclk = (Long64_t)ext1pps;
    return externalclk;
}

Long64_t EventTreeReader::GetInternalClk(){
    Long64_t internalclk = (Long64_t)msec_counter;
    return internalclk;
}
