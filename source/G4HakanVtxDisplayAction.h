// Tell emacs that this is a C++ source
//  -*- C++ -*-.
#ifndef G4HAKANVTXDISPLAYACTION_H
#define G4HAKANVTXDISPLAYACTION_H

#include <g4main/PHG4DisplayAction.h>

#include <string>  // for string

class G4Colour;
class G4VisAttributes;
class G4LogicalVolume;
class G4VPhysicalVolume;
class PHParameters;

class G4HakanVtxDisplayAction : public PHG4DisplayAction
{
 public:
  G4HakanVtxDisplayAction(const std::string &name);

  virtual ~G4HakanVtxDisplayAction();

  void ApplyDisplayAction(G4VPhysicalVolume *physvol);
  void SetMyVolume(G4LogicalVolume *vol) { m_MyVolume = vol; }
  void SetColor(const double red, const double green, const double blue, const double alpha = 1.);

 private:
  G4LogicalVolume *m_MyVolume;
  G4VisAttributes *m_VisAtt;
  G4Colour *m_Colour;
};

#endif  // G4DETECTORS_PHG4BLOCKDISPLAYACTION_H
