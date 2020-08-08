
#include "MagneticField.hh"

#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"


MagneticField::MagneticField()
: G4MagneticField(),
  fBy(0*tesla)
{}


MagneticField::~MagneticField()
{}

void MagneticField::GetFieldValue(const G4double [4],double *bField) const
{
  bField[0] = fBy;
  bField[1] = 0.;
  bField[2] = 0.;
}


