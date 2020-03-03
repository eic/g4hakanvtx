#include "G4HakanVtxSubsystem.h"

#include "G4HakanVtxDetector.h"
#include "G4HakanVtxDisplayAction.h"
#include "G4HakanVtxSteppingAction.h"

#include <phparameter/PHParameters.h>
#include <phparameter/PHParametersContainer.h>

#include <g4main/PHG4HitContainer.h>
#include <g4main/PHG4SteppingAction.h>  // for PHG4SteppingAction

#include <phool/PHCompositeNode.h>
#include <phool/PHIODataNode.h>    // for PHIODataNode
#include <phool/PHNode.h>          // for PHNode
#include <phool/PHNodeIterator.h>  // for PHNodeIterator
#include <phool/PHObject.h>        // for PHObject
#include <phool/getClass.h>

#include <cmath>                                // for isfinite

using namespace std;

//_______________________________________________________________________
G4HakanVtxSubsystem::G4HakanVtxSubsystem(const std::string &name)
  : PHG4DetectorGroupSubsystem(name)
  , m_Detector(nullptr)
  , m_SteppingAction(nullptr)
  , m_DisplayAction(nullptr)
{
  // call base class method which will set up parameter infrastructure
  // and call our SetDefaultParameters() method
  InitializeParameters();
}

//_______________________________________________________________________
G4HakanVtxSubsystem::~G4HakanVtxSubsystem()
{
  delete m_DisplayAction;
}

//_______________________________________________________________________
int G4HakanVtxSubsystem::InitRunSubsystem(PHCompositeNode *topNode)
{
  PHNodeIterator iter(topNode);
  PHCompositeNode *dstNode = dynamic_cast<PHCompositeNode *>(iter.findFirst("PHCompositeNode", "DST"));
  G4HakanVtxDisplayAction *disp_action = new G4HakanVtxDisplayAction(Name(), GetParams());
  if (isfinite(m_ColorArray[0]) &&
      isfinite(m_ColorArray[1]) &&
      isfinite(m_ColorArray[2]) &&
      isfinite(m_ColorArray[3]))
  {
    disp_action->SetColor(m_ColorArray[0], m_ColorArray[1], m_ColorArray[2], m_ColorArray[3]);
  }
  m_DisplayAction = disp_action;
  PHNodeIterator dstIter(dstNode);
  if (GetParamsContainer()->GetParameters(-1)->get_int_param("active"))
  {
    PHCompositeNode *DetNode = dynamic_cast<PHCompositeNode *>(dstIter.findFirst("PHCompositeNode", SuperDetector()));
    if (!DetNode)
    {
      DetNode = new PHCompositeNode(SuperDetector());
      dstNode->addNode(DetNode);
    }
    string g4hitnodename = "G4HIT_" + SuperDetector();
    PHG4HitContainer *g4_hits = findNode::getClass<PHG4HitContainer>(DetNode, g4hitnodename);
    if (!g4_hits)
    {
      g4_hits = new PHG4HitContainer(g4hitnodename);
      DetNode->addNode(new PHIODataNode<PHObject>(g4_hits, g4hitnodename, "PHObject"));
    }
  }
  // create detector
  m_Detector = new G4HakanVtxDetector(this, topNode, GetParamsContainer(), Name());
  m_Detector->OverlapCheck(CheckOverlap());
  // create stepping action if detector is active
  if (GetParamsContainer()->GetParameters(-1)->get_int_param("active"))
  {
    m_SteppingAction = new G4HakanVtxSteppingAction(m_Detector, GetParams());
  }
  return 0;
}

//_______________________________________________________________________
int G4HakanVtxSubsystem::process_event(PHCompositeNode *topNode)
{
  // pass top node to stepping action so that it gets
  // relevant nodes needed internally
  if (m_SteppingAction)
  {
    m_SteppingAction->SetInterfacePointers(topNode);
  }
  return 0;
}

void G4HakanVtxSubsystem::Print(const string &what) const
{
  if (m_Detector)
  {
    m_Detector->Print(what);
  }
  return;
}

//_______________________________________________________________________
PHG4Detector *G4HakanVtxSubsystem::GetDetector(void) const
{
  return m_Detector;
}

void G4HakanVtxSubsystem::SetDefaultParameters()
{
  // all units are in cm
  set_default_double_param(0, "Dx", 0.0281);
  set_default_double_param(0, "Dy", 1.5);
  set_default_double_param(0, "Dz", 27.);
  set_default_double_param(0, "Rin", 2.34);
  set_default_double_param(0, "PixelDx", 1.);  // dz/10. = 1.
  set_default_double_param(0, "PixelDy", 2. / 50.);   // dy/50

  set_default_double_param(1, "Dx", 0.0281);
  set_default_double_param(1, "Dy", 1.5);
  set_default_double_param(1, "Dz", 27.);
  set_default_double_param(1, "Rin", 4.68);
  set_default_double_param(1, "PixelDx", 11. / 10.);  // dz/10
  set_default_double_param(1, "PixelDy", 2. / 50.);   // dy/50

  set_default_double_param(2, "Dx", 0.0749);
  set_default_double_param(2, "Dy", 6.);
  set_default_double_param(2, "Dz", 84.);
  set_default_double_param(2, "Rin", 8.76);
  set_default_double_param(2, "PixelDx", 18. / 50.);  // dz/50
  set_default_double_param(2, "PixelDy", 4. / 10.);   // dy/10

  set_default_double_param(3, "Dx", 0.0749);
  set_default_double_param(3, "Dy", 6.);
  set_default_double_param(3, "Dz", 84.);
  set_default_double_param(3, "Rin", 13.38);
  set_default_double_param(3, "PixelDx", 24. / 50.);  // dz/50
  set_default_double_param(3, "PixelDy", 4. / 10.);   // dy/10

  set_default_double_param(4, "Dx", 2*0.0749);
  set_default_double_param(4, "Dy", 6.);
  set_default_double_param(4, "Dz", 84.);
  set_default_double_param(4, "Rin", 18.);
  set_default_double_param(4, "PixelDx", 24. / 50.);  // dz/50
  set_default_double_param(4, "PixelDy", 4. / 10.);   // dy/10

  // sizes are in cm
  // angles are in deg
  // units will be converted to G4 units when used
  set_default_double_param("place_x", 0.);
  set_default_double_param("place_y", 0.);
  set_default_double_param("place_z", 0.);
  set_default_double_param("rot_x", 0.);
  set_default_double_param("rot_y", 0.);
  set_default_double_param("rot_z", 0.);
  set_default_double_param("size_x", 20.);
  set_default_double_param("size_y", 20.);
  set_default_double_param("size_z", 20.);

  set_default_string_param("material", "G4_Galactic");
}
