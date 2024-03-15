#include "Pythia8/Pythia.h" // Include Pythia headers.
using namespace Pythia8; // Let Pythia8:: be implicit.
int main() { // Begin main program.
// Set up generation.
Pythia pythia; // Declare a Pythia object
pythia.readString("Top:gg2ttbar = on"); // Switch on process.
pythia.readString("Top:qqbar2ttbar = on");
pythia.readString("Beams:eCM = 8000."); // 8 TeV CM energy.
pythia.readString("Next:numberShowEvent = 5");
pythia.init(); // Initialize; incoming pp beams is default.
// Generate event(s).
Hist pT("top transverse momentum", 100, 0., 200.);
Hist eta("top pseudorapidity", 100, -5., 5.);
// Event loop
for (int iEvent = 0; iEvent < 5; ++iEvent) {
  pythia.next(); // Generate an(other) event. Fill event record.
  int iTop = 0;
  // Particle loop
  for (int i = 0; i < pythia.event.size(); ++i) {
     if (pythia.event[i].id() == 6) iTop = i;
     cout << "i = " << i << ", id = "
          << pythia.event[i].id() << endl;
  }
  pT.fill(pythia.event[iTop].pT());
  eta.fill(pythia.event[iTop].eta());
}
cout << pT << eta;
pythia.stat();
return 0;
}
