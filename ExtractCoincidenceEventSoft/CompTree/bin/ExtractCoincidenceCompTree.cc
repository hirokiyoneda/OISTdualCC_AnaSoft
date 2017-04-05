#include "ExtractCoincidenceCompTree.hh"

const Long64_t ExtractCoincidenceCompTree::MAXINTERNALCLK = 20000;

ExtractCoincidenceCompTree::ExtractCoincidenceCompTree(){
    header = "coin_";
    hist_difftimestamp = new TH1D("difftimestamp", "difftimestamp", 500, -250+0.5, 250+0.5);
    hist2d_difftimestamp = new TH2D("difftimestamp2d", "difftimestamp2d", 1000, 0, MAXINTERNALCLK, 500, -250+0.5, 250+0.5);

    print_div_num = 10000;
}

ExtractCoincidenceCompTree::~ExtractCoincidenceCompTree(){
    TFile *file_difftimestamp = new TFile("difftimestamp.root", "recreate");
    file_difftimestamp->cd("/");
    hist_difftimestamp->Write();
    hist2d_difftimestamp->Write();
    file_difftimestamp->Close();
}

void ExtractCoincidenceCompTree::ReadFirstInputfile(TString infilename){
    in_cetree1 = new CompTreeReader(infilename);
    infilename_c1 = infilename;
    totaleventnum1 = in_cetree1->GetEntries();
    currenteventnum1 = 0;
}

void ExtractCoincidenceCompTree::ReadSecondInputfile(TString infilename){
    in_cetree2 = new CompTreeReader(infilename);
    infilename_c2 = infilename;
    totaleventnum2 = in_cetree2->GetEntries();
    currenteventnum2 = 0;
}

void ExtractCoincidenceCompTree::SetTimeWindow(Long64_t timewindow){
    this->timewindow = timewindow;
}

void ExtractCoincidenceCompTree::SetOutfileHeader(TString header){
    this->header = header;
}

void ExtractCoincidenceCompTree::Initialize(){
    out_cetree1 = new CompTreeMaker();   
    out_cetree2 = new CompTreeMaker();   

//    out_cetree1->SetTreeName("cetree1");
//    out_cetree2->SetTreeName("cetree2");

    outfilename_c1 = header + infilename_c1;
    outfilename_c2 = header + infilename_c2;

    out_cetree1->Initialize(outfilename_c1);
    out_cetree2->Initialize(outfilename_c2);

    coincidence_number = 0;
}

void ExtractCoincidenceCompTree::Run(){
    Long64_t i_eventnum2_now = 0;

    for(Long64_t i_eventnum1 = 0; i_eventnum1 < totaleventnum1; ++i_eventnum1){ 
        if(i_eventnum1%print_div_num == 0){
            cout << i_eventnum1 << "/" << totaleventnum1 << "       " << "coincidence_number : " << coincidence_number << endl;
        }
        in_cetree1->SetEntry(i_eventnum1);
        for(Long64_t i_eventnum2 = i_eventnum2_now; i_eventnum2 < totaleventnum2; ++i_eventnum2){ 
            in_cetree2->SetEntry(i_eventnum2);
      
            int checker = CheckCoincidence();


            if(checker == 0){
                i_eventnum2_now = i_eventnum2;
                continue;
            }else if(checker == 1){
                break;
            }else if(checker == 2){

                hist_difftimestamp->Fill(GetDiffTimeStamp());
                hist2d_difftimestamp->Fill(in_cetree1->GetInternalClk(), GetDiffTimeStamp());

                i_eventnum2_now = i_eventnum2;

                WriteCoincidenceEvent();
                ++coincidence_number;
                break;
            }
        }
    }

    out_cetree1->SaveFile();
    out_cetree2->SaveFile();
}

int ExtractCoincidenceCompTree::CheckCoincidence(){
    extclk1 = in_cetree1->GetExternalClk(); 
    extclk2 = in_cetree2->GetExternalClk(); 

    intclk1 = in_cetree1->GetInternalClk();
    intclk2 = in_cetree2->GetInternalClk();

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

Long64_t ExtractCoincidenceCompTree::GetDiffTimeStamp(){
    extclk1 = in_cetree1->GetExternalClk(); 
    extclk2 = in_cetree2->GetExternalClk(); 

    intclk1 = in_cetree1->GetInternalClk();
    intclk2 = in_cetree2->GetInternalClk();

    if(extclk1 - extclk2 == 1){
        return intclk1 + MAXINTERNALCLK - intclk2;
    }

    if(extclk1 - extclk2 == -1){
        return intclk1 - MAXINTERNALCLK - intclk2;
    }

    return intclk1 - intclk2;
}

void ExtractCoincidenceCompTree::WriteCoincidenceEvent(){
    CopyValueFile(1);
    CopyValueFile(2);

    out_cetree1->eventid = coincidence_number;
    out_cetree2->eventid = coincidence_number;
    
    out_cetree1->FillNewEvent();
    out_cetree2->FillNewEvent();
}

void ExtractCoincidenceCompTree::CopyValueFile(int num){
    CompTreeReader *in_cetree;
    CompTreeMaker *out_cetree;

    if(num == 1){
        in_cetree = in_cetree1;
        out_cetree = out_cetree1;
    }else{
        in_cetree = in_cetree2;
        out_cetree = out_cetree2;
    }

	out_cetree->exptime = in_cetree->exptime;
	out_cetree->eventid = in_cetree->eventid;
	out_cetree->num_hits = in_cetree->num_hits;
	out_cetree->hit1_id = in_cetree->hit1_id;
	out_cetree->hit1_process = in_cetree->hit1_process;
	out_cetree->hit1_detector = in_cetree->hit1_detector;
	out_cetree->hit1_readout_module = in_cetree->hit1_readout_module;
	out_cetree->hit1_section = in_cetree->hit1_section;
	out_cetree->hit1_channel = in_cetree->hit1_channel;
	out_cetree->hit1_pixelx = in_cetree->hit1_pixelx;
	out_cetree->hit1_pixely = in_cetree->hit1_pixely;
	out_cetree->hit1_time = in_cetree->hit1_time;
	out_cetree->hit1_posx = in_cetree->hit1_posx;
	out_cetree->hit1_posy = in_cetree->hit1_posy;
	out_cetree->hit1_posz = in_cetree->hit1_posz;
	out_cetree->hit1_energy = in_cetree->hit1_energy;
	out_cetree->hit2_id = in_cetree->hit2_id;
	out_cetree->hit2_process = in_cetree->hit2_process;
	out_cetree->hit2_detector = in_cetree->hit2_detector;
	out_cetree->hit2_readout_module = in_cetree->hit2_readout_module;
	out_cetree->hit2_section = in_cetree->hit2_section;
	out_cetree->hit2_channel = in_cetree->hit2_channel;
	out_cetree->hit2_pixelx = in_cetree->hit2_pixelx;
	out_cetree->hit2_pixely = in_cetree->hit2_pixely;
	out_cetree->hit2_time = in_cetree->hit2_time;
	out_cetree->hit2_posx = in_cetree->hit2_posx;
	out_cetree->hit2_posy = in_cetree->hit2_posy;
	out_cetree->hit2_posz = in_cetree->hit2_posz;
	out_cetree->hit2_energy = in_cetree->hit2_energy;
	out_cetree->flags = in_cetree->flags;
	out_cetree->costheta = in_cetree->costheta;
	out_cetree->dtheta = in_cetree->dtheta;
	out_cetree->phi = in_cetree->phi;
	out_cetree->hitpattern = in_cetree->hitpattern;
	out_cetree->grade = in_cetree->grade;
	out_cetree->likelihood = in_cetree->likelihood;
	out_cetree->ini_energy = in_cetree->ini_energy;
	out_cetree->ini_dirx = in_cetree->ini_dirx;
	out_cetree->ini_diry = in_cetree->ini_diry;
	out_cetree->ini_dirz = in_cetree->ini_dirz;
	out_cetree->ini_time = in_cetree->ini_time;
	out_cetree->ini_posx = in_cetree->ini_posx;
	out_cetree->ini_posy = in_cetree->ini_posy;
	out_cetree->ini_posz = in_cetree->ini_posz;
	out_cetree->ini_polarx = in_cetree->ini_polarx;
	out_cetree->ini_polary = in_cetree->ini_polary;
	out_cetree->ini_polarz = in_cetree->ini_polarz;
	out_cetree->weight = in_cetree->weight;

}

void ExtractCoincidenceCompTree::SetPrintDivNum(Long64_t print_div_num){
    this->print_div_num = print_div_num;
}
