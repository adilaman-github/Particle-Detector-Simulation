#include "DetectorConstruction.hh"
#include "MagneticField.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4Mag_UsualEqRhs.hh"

#include "G4SDManager.hh"
#include "G4SDChargedFilter.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSTrackLength.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"



G4ThreadLocal MagneticField* DetectorConstruction::fMagneticField = 0;
G4ThreadLocal G4FieldManager* DetectorConstruction::fFieldMgr = 0;


DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(),
  fMagneticLogical(nullptr),
  fScoringVolume(0)
{ }


DetectorConstruction::~DetectorConstruction()
{ }


G4VPhysicalVolume* DetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();

  
  // Liquid argon material
  G4double a;  // mass of a mole;
  G4double z;  // z=mean number of protons;  
  G4double density; 
  new G4Material("liquidArgon", z=18., a= 39.95*g/mole, density= 1.390*g/cm3);
         // The argon by NIST Manager is a gas with a different density

  // Vacuum
  new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                  kStateGas, 2.73*kelvin, 3.e-18*pascal);

  // Print materials
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;

  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 160.0*cm;
  G4double world_sizeZ  = 160.0*cm;
  G4double calor_sizeXZ = 54.0*cm;
  G4double calor_sizeY  = 80.0*cm;
  G4Material* world_mat = nist->FindOrBuildMaterial("Galactic");
  G4Material* calor_mat = nist->FindOrBuildMaterial("liquidArgon");
  //G4Material* wall_mat = nist->FindOrBuildMaterial("G4_Pt");


  //
  //World
  //
  
  auto worldS = new G4Box("World", world_sizeXY,world_sizeXY,world_sizeZ);

  auto worldLV = new G4LogicalVolume(worldS,world_mat,"World");

  auto worldPV = new G4PVPlacement(0,
				   G4ThreeVector(),
				   worldLV,
				   "World",
				   0,
				   false,
				   0,
				   checkOverlaps);


  //
  //Cylinders with beam cap
  //
  
  G4VSolid* cylinder1
    = new G4Tubs("Cylinder1",0.,120.0*cm,150.0*cm,0.0*deg,360.0*rad);

   G4double gap = (120.0*cm-calor_sizeXZ)/2;
   
   G4VSolid* cylinder2
     = new G4Tubs("Cylinder2",0.,25.0*cm,gap,0.0*deg,360.0*rad);// radius of the beam_cap is 25 cm

  G4RotationMatrix* rotationMatrix = new G4RotationMatrix();
  rotationMatrix->rotateX(90.*deg);

  G4VSolid* cylinder
    = new G4SubtractionSolid("Cylinder1-Cylinder2", cylinder1, cylinder2,rotationMatrix,G4ThreeVector(0.,calor_sizeY+7.0*cm,0.));


   auto cylinderLV = new G4LogicalVolume(cylinder,calor_mat,"Cylinder");

   new G4PVPlacement(rotationMatrix,
		     G4ThreeVector(),
		     cylinderLV,
		     "Cylinder",
		     worldLV,
		     false,
		     0,
		     checkOverlaps);


   //
   //Calorimeter
   //
   
  auto calorimeterS
    = new G4Box("Calorimeter",    
                 calor_sizeXZ, calor_sizeXZ, calor_sizeY);

                         
  auto calorLV
    = new G4LogicalVolume(
                 calorimeterS,    // its solid
                 calor_mat, // its material
                 "Calorimeter");  // its name
                                   
  new G4PVPlacement(0,                // no rotation
                    G4ThreeVector(),  // at (0,0,0)
                    calorLV,          // its logical volume                         
                    "Calorimeter",     // its name
                    cylinderLV,          // its mother  volume
                    false,            // no boolean operation
                    0,              
                    checkOverlaps);


  //
  //magnetic field
  //
  
  auto magneticSolid 
    = new G4Tubs("magneticTubs",0.,50.0*cm,15.0*cm,0.,360.*deg);

  fMagneticLogical
    = new G4LogicalVolume(magneticSolid, world_mat, "magneticLogical");

 
  G4RotationMatrix* fieldRot = new G4RotationMatrix();
  fieldRot->rotateY(90.*deg);
  new G4PVPlacement(0,
		    G4ThreeVector(0.,0.,-calor_sizeY),
		    fMagneticLogical,
                    "magneticPhysical",
		    worldLV,
                    false,
		    0,
		    checkOverlaps);

  
  


  auto simpleCylinderVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  simpleCylinderVisAtt->SetVisibility(true);
  simpleCylinderVisAtt->SetForceWireframe(true);
  simpleCylinderVisAtt->SetForceAuxEdgeVisible(true);
  cylinderLV->SetVisAttributes(simpleCylinderVisAtt);

  auto simpleBoxVisAtt= new G4VisAttributes(G4Colour(0.0,1.0,1.0));
  simpleBoxVisAtt->SetVisibility(true);
  simpleBoxVisAtt->SetForceWireframe(true);
  simpleBoxVisAtt->SetForceAuxEdgeVisible(true);
  calorLV->SetVisAttributes(simpleBoxVisAtt);

  auto simpleMagVisAtt= new G4VisAttributes(G4Colour(1.0,0.0,1.0));
  simpleMagVisAtt->SetVisibility(true);
  simpleMagVisAtt->SetForceAuxEdgeVisible(true);
  fMagneticLogical->SetVisAttributes(simpleMagVisAtt);
  //calorLV->SetVisAttributes (G4VisAttributes::SetForceSolid());
  
 
  //
  fScoringVolume = calorLV;

  //
  //always return the physical World
  //
  return worldPV;
}

void DetectorConstruction::ConstructSDandField()
{
  G4SDManager::GetSDMpointer()->SetVerboseLevel(1);
  // 
  // Scorers
  //

  auto calorDetector = new G4MultiFunctionalDetector("Calorimeter");
  G4SDManager::GetSDMpointer()->AddNewDetector(calorDetector);
  G4VPrimitiveScorer* primitive;
  primitive = new G4PSEnergyDeposit("Edep");
  calorDetector->RegisterPrimitive(primitive);

  primitive = new G4PSTrackLength("TrackLength");
  //auto charged = new G4SDChargedFilter("chargedFilter");
  // primitive ->SetFilter(charged);
  calorDetector->RegisterPrimitive(primitive);  

  SetSensitiveDetector("Calorimeter",calorDetector);

  // 
  // Magnetic field
  //

  fMagneticField = new MagneticField();
  fFieldMgr = new G4FieldManager();
  fFieldMgr->SetDetectorField(fMagneticField);
  fFieldMgr->CreateChordFinder(fMagneticField);
  G4bool forceToAllDaughters = true;
  fMagneticLogical->SetFieldManager(fFieldMgr, forceToAllDaughters);

}




