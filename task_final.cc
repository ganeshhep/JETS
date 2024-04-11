#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "Pythia8/Pythia.h"

int main()
{
  //defining a tree file(.root file).
  TFile *output = new TFile("tutorial5.root", "recreate");
  TTree *tree = new TTree("tree", "tree"); 
  
  int id, event, size, event_num, n, prev;
  double charge, px, py, pz, e, pT;
  bool is_charged, is_neutral, may_decay, isLepton, isHadron, isbHadron;
  
  //defining branches of the tree file.
  tree->Branch("n", &n, "n/I");
  tree->Branch("charge", &charge, "charge/D");
  tree->Branch("is_charged", &is_charged, "is_charged/B");
  tree->Branch("is_neutral", &is_neutral, "is_neutral/B");
  tree->Branch("isbHadron", &isbHadron, "isbHadron/B");
  tree->Branch("px", &px, "px/D");
  tree->Branch("py", &py, "py/D");
  tree->Branch("pz", &pz, "pz/D");
  tree->Branch("e", &e, "e/D");

  int nevents = 5e4;
  Pythia8::Pythia pythia;      //every event is a proton-proton collision. 
  pythia.readString("Beams:idA = 2212");
  pythia.readString("Beams:idB = 2212"); 
  pythia.readString("Beams:eCM = 14.e3");
  pythia.readString("Top:gg2ttbar = on");
  pythia.readString("Top:qqbar2ttbar = on");
  pythia.readString("Top:ffbar2ttbar(s:gmZ) = on");
  pythia.readString("Top:gmgm2ttbar = on");
  pythia.readString("Top:ggm2ttbar = on");
  
  pythia.init();

  n = 0;
  prev = -1;
  for (int i = 0; i < nevents; i++)
  {
    if (!pythia.next()) continue;
 
    int entries = pythia.event.size(); //number of particles in an event.
    
    std::cout << "Event: " << i << std::endl;
    
    event = i;
    size = entries;
    for (int j = 0; j < entries; j++)
    {
      may_decay = pythia.event[j].mayDecay();
      isLepton = pythia.event[j].isLepton();
      isHadron = pythia.event[j].isHadron();
      pT = sqrt(pow(pythia.event[j].px(), 2) + pow(pythia.event[j].py(), 2));

      if (may_decay == 0 && isLepton == 0 && pT > 1){
          event_num = i;

          if (event_num != prev){
	    prev = event_num;
            n++;
          } else {
	        n = n;
	  }
	    
          id = pythia.event[j].id();
          charge = pythia.event[j].charge();
          is_charged = pythia.event[j].isCharged();
          is_neutral = pythia.event[j].isNeutral();
          isbHadron = 0;
          px = pythia.event[j].px();
          py = pythia.event[j].py();
          pz = pythia.event[j].pz();
          e = pythia.event[j].e();

	  if (isHadron == 1){
	      if (pythia.event[pythia.event[j].mother1()].id() == 5){
	          isbHadron = 1;
	      }
          }
          
          tree->Fill();
      }

    }
   
  }  
  
  output->Write();
  output->Close();

  return 0;

}
