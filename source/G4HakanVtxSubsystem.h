// Tell emacs that this is a C++ source
//  -*- C++ -*-.
#ifndef G4HAKANVTXSUBSYSTEM_H
#define G4HAKANVTXSUBSYSTEM_H

#include <g4detectors/PHG4DetectorGroupSubsystem.h>

#if !defined(__CINT__) || defined(__CLING__)
#include <array>   // for array
#endif

#include <string>

class G4HakanVtxDetector;
class PHCompositeNode;
class PHG4Detector;
class PHG4DisplayAction;
class PHG4SteppingAction;

/**
   * \brief example Fun4All module
   *
   * The detector is constructed and registered via G4HakanVtxDetector
   *
   *
   * \see G4HakanVtxDetector
   * \see G4HakanVtxSubsystem
   *
   */
class G4HakanVtxSubsystem : public PHG4DetectorGroupSubsystem
{
 public:
  //! constructor
  G4HakanVtxSubsystem(const std::string& name = "HakanVtx");

  //! destructor
  virtual ~G4HakanVtxSubsystem();

  /*!
  creates the m_Detector object and place it on the node tree, under "DETECTORS" node (or whatever)
  reates the stepping action and place it on the node tree, under "ACTIONS" node
  creates relevant hit nodes that will be populated by the stepping action and stored in the output DST
  */
  virtual int InitRunSubsystem(PHCompositeNode*);

  //! event processing
  /*!
  get all relevant nodes from top nodes (namely hit list)
  and pass that to the stepping action
  */
  virtual int process_event(PHCompositeNode*);

  //! accessors (reimplemented)
  virtual PHG4Detector* GetDetector() const;

  virtual PHG4SteppingAction* GetSteppingAction() const { return m_SteppingAction; }
  //! Print info (from SubsysReco)
  virtual void Print(const std::string& what = "ALL") const;

  PHG4DisplayAction* GetDisplayAction() const { return m_DisplayAction; }

  void set_color(const double red, const double green, const double blue, const double alpha = 1.)
  {
    m_ColorArray[0] = red;
    m_ColorArray[1] = green;
    m_ColorArray[2] = blue;
    m_ColorArray[3] = alpha;
  }

 private:
  // \brief Set default parameter values
  void SetDefaultParameters();

  //! detector geometry
  /*! defives from PHG4Detector */
  G4HakanVtxDetector* m_Detector;

  PHG4SteppingAction* m_SteppingAction;

  //! particle tracking "stepping" action
  /*! derives from PHG4SteppingActions */
  //! display attribute setting
  /*! derives from PHG4DisplayAction */
  PHG4DisplayAction* m_DisplayAction;
  //! Color setting if we want to override the default
#if !defined(__CINT__) || defined(__CLING__)
  std::array<double, 4> m_ColorArray;
#else
  double m_ColorArray[4];
#endif
};

#endif  // G4HAKANVTXSUBSYSTEM_H
