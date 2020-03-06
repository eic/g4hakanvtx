// Tell emacs that this is a C++ source
//  -*- C++ -*-.
#ifndef G4HAKANVTXDETECTOR_H
#define G4HAKANVTXDETECTOR_H

#include <g4main/PHG4Detector.h>

#include <set>
#include <string>  // for string

class G4HakanVtxDisplayAction;
class G4LogicalVolume;
class G4VPhysicalVolume;
class PHCompositeNode;
class PHG4Subsystem;
class PHParametersContainer;

class G4HakanVtxDetector : public PHG4Detector
{
 public:
  //! constructor
  G4HakanVtxDetector(PHG4Subsystem *subsys, PHCompositeNode *Node, PHParametersContainer *params_array, const std::string &dnam);

  //! destructor
  virtual ~G4HakanVtxDetector() {}

  //! construct
  virtual void ConstructMe(G4LogicalVolume *world);

  virtual void Print(const std::string &what = "ALL") const;

  //!@name volume accessors
  //@{
  int IsInDetector(G4VPhysicalVolume *) const;
  //@}

  void SuperDetector(const std::string &name) { m_SuperDetector = name; }
  const std::string SuperDetector() const { return m_SuperDetector; }

 private:
  void ConstructLaddersEndcaps(G4LogicalVolume *motherlogic);
  int m_IsActiveFlag;
  int m_IsAbsorberActiveFlag;
  int m_Layers;
  PHParametersContainer *m_ParamsContainer;

  G4HakanVtxDisplayAction *m_DisplayAction;

  // active volumes
  std::set<G4VPhysicalVolume *> m_PhysicalVolumesSet;

  std::string m_SuperDetector;
};

#endif  // G4HAKANVTXDETECTOR_H
