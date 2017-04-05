#include "EventTreeMaker.hh"

/*
v1 -- 2017-03-22 by Hiroki Yoneda 
*/

EventTreeMaker::~EventTreeMaker(){
}

EventTreeMaker::EventTreeMaker(){
    treename = "eventtree";
}

void EventTreeMaker::SaveFile(){
    outfile->cd("/");
    eventtree->Write();
}

void EventTreeMaker::SetTreeName(TString treename){
    this->treename = treename;
}

void EventTreeMaker::Initialize(TString outfilename){
    TFileMaker::SetOutputFilename(outfilename);
    TFileMaker::MakeOutputFile();

	eventtree = new TTree(treename, treename);
    
    neweventid = 0;

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

        char chflagname2[30];
        char cmnname2[30];
        char adcname2[30];
        char indexname2[30];
        char refname2[30];
        char hitnumname2[30];
        char cmn_exname2[30];
        sprintf(chflagname2,"chflag%d[3]/i",i);
        sprintf(cmnname2,"cmn%d/s",i);
        sprintf(adcname2,"adc%d[64]/s",i);
        sprintf(indexname2,"index%d[64]/s",i);
        sprintf(refname2,"ref%d/s",i);
        sprintf(hitnumname2,"hitnum%d/s",i);
        sprintf(cmn_exname2,"cmn%d_ex/D",i);

        eventtree->Branch(chflagname,chflag[i],chflagname2);
        eventtree->Branch(cmnname,&cmn[i],cmnname2);
        eventtree->Branch(adcname,adc[i],adcname2);
        eventtree->Branch(indexname,index[i],indexname2);
        eventtree->Branch(refname,&ref[i],refname2);
        eventtree->Branch(hitnumname,&hitnum[i],hitnumname2);
        eventtree->Branch(cmn_exname,&cmn_ex[i],cmn_exname2);
    }

    eventtree->Branch("ti",&ti,"ti/i");
    eventtree->Branch("livetime",&livetime,"livetime/i");
    eventtree->Branch("integral_livetime",&integral_livetime,"integral_livetime/i");
    eventtree->Branch("trighitpat",&trighitpat,"trighitpat/s");
    eventtree->Branch("unixtime",&unixtime,"unixtime/i");
    eventtree->Branch("adcclkcnt",&adcclkcnt,"adcclkcnt/s");
    eventtree->Branch("ext1pps",&ext1pps,"ext1pps/i");
    eventtree->Branch("msec_counter",&msec_counter,"msec_counter/i");
    eventtree->Branch("gpstime",&gpstime,"gps1pps/i");
    eventtree->Branch("pctime",&pctime,"pctime/i");
}

void EventTreeMaker::FillNewEvent(){
    eventtree->Fill();
    ++neweventid;
}
