

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4RotationMatrix.hh"
#include "G4FieldManager.hh"

#include <vector>

class MagneticField;

class G4VPhysicalVolume;
class G4Material;
class G4VSensitiveDetector;
class G4VisAttributes;


class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();


  public:
     virtual G4VPhysicalVolume* Construct();
     virtual void ConstructSDandField();


  private:   
      static G4ThreadLocal MagneticField*  fMagneticField; 
      static G4ThreadLocal G4FieldManager* fFieldMgr;


      G4LogicalVolume* fMagneticLogical;
    
  public:
     G4LogicalVolume* GetScoringVolume() const { return fScoringVolume; }

  protected:
    G4LogicalVolume*  fScoringVolume;
};


#endif

