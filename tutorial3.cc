#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "Pythia8/Pythia.h"

int main()
{
  //defining a tree file(.root file).
  TFile *output = new TFile("tutorial3.root", "recreate");
  TTree *tree = new TTree("tree", "tree"); 
  
  int id, event, size, no;
  double m, px, py, pz;
  
  //defining branches of the tree file.
  tree->Branch("event", &event, "event/I");
  tree->Branch("size", &size, "size/I");
  tree->Branch("no", &no, "no/I");
  tree->Branch("id", &id, "id/I");
  tree->Branch("m", &m, "m/D");
  tree->Branch("px", &px, "px/D");
  tree->Branch("py", &py, "py/D");
  tree->Branch("pz", &pz, "pz/D");

  int nevents = 1e4;
  Pythia8::Pythia pythia;      //every event is a proton-proton collision. 
  pythia.readString("Beams:idA = 2212");
  pythia.readString("Beams:idB = 2212"); 
  pythia.readString("Beams:eCM = 14.e3");
  pythia.readString("SoftQCD:all = on");
  pythia.readString("HardQCD:all = on");
  
  pythia.init();

  for (int i = 0; i < nevents; i++)
  {
    if (!pythia.next()) continue;
 
    int entries = pythia.event.size(); //number of particles in an event.
    
    std::cout << "Event: " << i << std::endl;
    
    event = i;
    size = entries;

    for (int j = 0; j < entries; j++)
    {
      //a particle informations we want to get.
      no = j;
      id = pythia.event[j].id();
      m = pythia.event[j].m();
      px = pythia.event[j].px();
      py = pythia.event[j].py();
      pz = pythia.event[j].pz();

      tree->Fill();
      
    }  
  }  
  
  output->Write();
  output->Close();

  return 0;

}
