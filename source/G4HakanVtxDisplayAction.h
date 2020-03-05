// Tell emacs that this is a C++ source
//  -*- C++ -*-.
#ifndef G4HAKANVTXDISPLAYACTION_H
#define G4HAKANVTXDISPLAYACTION_H

#include <g4main/PHG4DisplayAction.h>

#include <map>
#include <string>  // for string
#include <vector>

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
  void AddVolume(G4LogicalVolume *vol, int index) { m_VolMap.insert(std::make_pair(vol,index));}

 private:
  std::vector<G4VisAttributes *> m_VisAttVec;
  std::map<G4LogicalVolume *, int> m_VolMap;
};

#endif  // G4DETECTORS_PHG4BLOCKDISPLAYACTION_H
