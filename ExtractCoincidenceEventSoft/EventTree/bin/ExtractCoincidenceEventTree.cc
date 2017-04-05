#include "ExtractCoincidenceEventTree.hh"

const Long64_t ExtractCoincidenceEventTree::MAXINTERNALCLK = 20000;

ExtractCoincidenceEventTree::ExtractCoincidenceEventTree(){
    header = "coin_";
    hist_difftimestamp = new TH1D("difftimestamp", "difftimestamp", 500, -250+0.5, 250+0.5);
    hist2d_difftimestamp = new TH2D("difftimestamp2d", "difftimestamp2d", 1000, 0, MAXINTERNALCLK, 500, -250+0.5, 250+0.5);
    print_div_num = 10000;
    
}

ExtractCoincidenceEventTree::~ExtractCoincidenceEventTree(){
    TFile *file_difftimestamp = new TFile("difftimestamp.root", "recreate");
    file_difftimestamp->cd("/");
    hist_difftimestamp->Write();
    hist2d_difftimestamp->Write();
    file_difftimestamp->Close();
}

void ExtractCoincidenceEventTree::ReadFirstInputfile(TString infilename){
    in_eventtree1 = new EventTreeReader(infilename);
    infilename_c1 = infilename;
    totaleventnum1 = in_eventtree1->GetEntries();
    currenteventnum1 = 0;
}

void ExtractCoincidenceEventTree::ReadSecondInputfile(TString infilename){
    in_eventtree2 = new EventTreeReader(infilename);
    infilename_c2 = infilename;
    totaleventnum2 = in_eventtree2->GetEntries();
    currenteventnum2 = 0;
}

void ExtractCoincidenceEventTree::SetTimeWindow(Long64_t timewindow){
    this->timewindow = timewindow;
}

void ExtractCoincidenceEventTree::SetOutfileHeader(TString header){
    this->header = header;
}

void ExtractCoincidenceEventTree::Initialize(){
    out_eventtree1 = new EventTreeMaker();   
    out_eventtree2 = new EventTreeMaker();   

//    out_eventtree1->SetTreeName("eventtree1");
//    out_eventtree2->SetTreeName("eventtree2");

    outfilename_c1 = header + infilename_c1;
    outfilename_c2 = header + infilename_c2;

    out_eventtree1->Initialize(outfilename_c1);
    out_eventtree2->Initialize(outfilename_c2);

    coincidence_number = 0;
}

void ExtractCoincidenceEventTree::Run(){
    Long64_t i_eventnum2_now = 0;

    for(Long64_t i_eventnum1 = 0; i_eventnum1 < totaleventnum1; ++i_eventnum1){ 
        if(i_eventnum1%print_div_num == 0){
            cout << i_eventnum1 << "/" << totaleventnum1 << "       " << "coincidence_number : " << coincidence_number << endl;
        }
        in_eventtree1->SetEntry(i_eventnum1);
        for(Long64_t i_eventnum2 = i_eventnum2_now; i_eventnum2 < totaleventnum2; ++i_eventnum2){ 
            in_eventtree2->SetEntry(i_eventnum2);
      
            int checker = CheckCoincidence();


            if(checker == 0){
                i_eventnum2_now = i_eventnum2;
                continue;
            }else if(checker == 1){
                break;
            }else if(checker == 2){

                hist_difftimestamp->Fill(GetDiffTimeStamp());
                hist2d_difftimestamp->Fill(in_eventtree1->GetInternalClk(), GetDiffTimeStamp());

                i_eventnum2_now = i_eventnum2;

                WriteCoincidenceEvent();
                ++coincidence_number;
                break;
            }
        }
    }

    out_eventtree1->SaveFile();
    out_eventtree2->SaveFile();
}

int ExtractCoincidenceEventTree::CheckCoincidence(){
    extclk1 = in_eventtree1->GetExternalClk(); 
    extclk2 = in_eventtree2->GetExternalClk(); 

    intclk1 = in_eventtree1->GetInternalClk();
    intclk2 = in_eventtree2->GetInternalClk();

    if(extclk1 - extclk2 > 1){
        return 0;
    }

    if(extclk1 - extclk2 == 1){
        if(intclk1 + MAXINTERNALCLK - intclk2 <= timewindow){
            return 2;
        }
        return 0;
    }

    if(extclk1 - extclk2 == 0){
        if(intclk1 - intclk2 > timewindow){
            return 0;
        }else if(intclk1 - intclk2 <  -1*timewindow){
            return 1;
        }else{
            return 2;
        }
    }

    if(extclk1 - extclk2 == -1){
        if(intclk2 + MAXINTERNALCLK - intclk1 <= timewindow){
            return 2;
        }
        return 1;

    }

    if(extclk1 - extclk2 < -1){
        return 1;
    }

    return 0;
}

Long64_t ExtractCoincidenceEventTree::GetDiffTimeStamp(){
    extclk1 = in_eventtree1->GetExternalClk(); 
    extclk2 = in_eventtree2->GetExternalClk(); 

    intclk1 = in_eventtree1->GetInternalClk();
    intclk2 = in_eventtree2->GetInternalClk();

    if(extclk1 - extclk2 == 1){
        return intclk1 + MAXINTERNALCLK - intclk2;
    }

    if(extclk1 - extclk2 == -1){
        return intclk1 - MAXINTERNALCLK - intclk2;
    }

    return intclk1 - intclk2;
}

void ExtractCoincidenceEventTree::WriteCoincidenceEvent(){
    CopyValueFile(1);
    CopyValueFile(2);

//    out_eventtree1->eventid = coincidence_number;
//    out_eventtree2->eventid = coincidence_number;
    
    out_eventtree1->FillNewEvent();
    out_eventtree2->FillNewEvent();
}

void ExtractCoincidenceEventTree::CopyValueFile(int num){
    EventTreeReader *in_eventtree;
    EventTreeMaker *out_eventtree;

    if(num == 1){
        in_eventtree = in_eventtree1;
        out_eventtree = out_eventtree1;
    }else{
        in_eventtree = in_eventtree2;
        out_eventtree = out_eventtree2;
    }

    for(int i = 0;i<40;++i){
        for(int j = 0; j < 3; ++j){
            in_eventtree->chflag[i][j] = out_eventtree->chflag[i][j];
        }
        for(int j = 0; j < 64; ++j){
            in_eventtree->adc[i][j] = out_eventtree->adc[i][j];
            in_eventtree->index[i][j] = out_eventtree->index[i][j];
        }

        out_eventtree->cmn[i] = in_eventtree->cmn[i];
        out_eventtree->ref[i] = in_eventtree->ref[i];
        out_eventtree->hitnum[i] = in_eventtree->hitnum[i];
        out_eventtree->cmn_ex[i] = in_eventtree->cmn_ex[i];
    }

    out_eventtree->ti = in_eventtree->ti;
    out_eventtree->livetime = in_eventtree->livetime;
    out_eventtree->integral_livetime = in_eventtree->integral_livetime;
    out_eventtree->trighitpat = in_eventtree->trighitpat;
    out_eventtree->unixtime = in_eventtree->unixtime;
    out_eventtree->adcclkcnt = in_eventtree->adcclkcnt;
    out_eventtree->ext1pps = in_eventtree->ext1pps;
    out_eventtree->msec_counter = in_eventtree->msec_counter;
    out_eventtree->gpstime = in_eventtree->gpstime;
    out_eventtree->pctime = in_eventtree->pctime;
}

void ExtractCoincidenceEventTree::SetPrintDivNum(Long64_t print_div_num){
    this->print_div_num = print_div_num;
}
