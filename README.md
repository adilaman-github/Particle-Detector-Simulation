# Steps to run the Simulation

- Download the muon file.
- Open terminal in Ubuntu and follow these steps:
  - cd muon/
  - export G4WORKDIR=~/muon/
  - make
  **This will create a /bin folder and a /tmp folder
  - cd /bin/Linux-g++/
  **Run the executable ./TPC**
- The ROOT files are found in the folder ~/muon/bin/Linux-g++.

# Information about the detector

This particle detector was programmed using Geant4 package (version 10.6). The basic model is inspired from ARIADNE, a liquid argon particle detector developed by a research group in University of Liverpool (wesbite: http://hep.ph.liv.ac.uk/ariadne/index.html). The initial conditions are set as muon - particles at an energy of 1* GeV. These conditions can be changed by modifying the PrimaryGeneratorAction.cc file in /src folder. The source files are in /src folder and the header files in /include folder. The steel rings in the original detector hasn't been simulated. The beam plud has an addition feature. The beam plug consist of a magnet. This magnet deflects the beam entering the main volume. This gives quite interesting results when tweaked around. The initial condition for the magnet is 0* tesla, i.e, no defelction of the initial beam. This can be changed by modifying the code of MagneticField.cc in /src. The basic structure of the simulation resembles the basic examples given in Geant4 simulation package. If you're new to Geant4, check out https://geant4.web.cern.ch/. Before installing Geant4, make sure you have ROOT https://root.cern/ installed in your system. 

# Contact
For any bugs or discussions regarding this project email me at adilamanwork@gmail.com .
