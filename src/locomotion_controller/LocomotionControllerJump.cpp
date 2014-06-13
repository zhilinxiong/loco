/*!
* @file     LocomotionControllerJump.cpp
* @author   Christian Gehring
* @date     Feb, 2014
* @version  1.0
* @ingroup
* @brief
*/
#include "loco/locomotion_controller/LocomotionControllerJump.hpp"
//#include "Rotations.hpp"
#include "RobotModel.hpp"
namespace loco {

LocomotionControllerJump::LocomotionControllerJump(LegGroup* legs, TorsoBase* torso,
                                                                 TerrainPerceptionBase* terrainPerception,
                                                                 ContactDetectorBase* contactDetector,
                                                                 FootPlacementStrategyBase* footPlacementStrategy, TorsoControlBase* baseController,
                                                                 VirtualModelController* virtualModelController, ContactForceDistributionBase* contactForceDistribution,
                                                                 ParameterSet* parameterSet) :
    LocomotionControllerBase(),
    isInitialized_(false),
    legs_(legs),
    torso_(torso),
    terrainPerception_(terrainPerception),
    contactDetector_(contactDetector),
    footPlacementStrategy_(footPlacementStrategy),
    torsoController_(baseController),
    virtualModelController_(virtualModelController),
    contactForceDistribution_(contactForceDistribution),
    parameterSet_(parameterSet)
{

}

LocomotionControllerJump::~LocomotionControllerJump() {

}

bool LocomotionControllerJump::initialize(double dt)
{
  isInitialized_ = false;

  for (auto leg : *legs_) {
    if(!leg->initialize(dt)) {
      return false;
    }
    //  std::cout << *leg << std::endl;
  }
  if (!torso_->initialize(dt)) {
    return false;
  }



  TiXmlHandle hLoco(parameterSet_->getHandle().FirstChild("LocomotionController"));

  if (!terrainPerception_->initialize(dt)) {
    return false;
  }

  if (!contactDetector_->initialize(dt)) {
    return false;
  }

  if (!footPlacementStrategy_->loadParameters(hLoco)) {
    return false;
  }
  if (!footPlacementStrategy_->initialize(dt)) {
    return false;
  }

  if (!torsoController_->loadParameters(hLoco)) {
    return false;
  }
  if (!torsoController_->initialize(dt)) {
    return false;
  }

  if (!contactForceDistribution_->loadParameters(hLoco)) {
    return false;
  }

  if (!virtualModelController_->loadParameters(hLoco)) {
    return false;
  }

  isInitialized_ = true;
  return isInitialized_;
}

bool LocomotionControllerJump::advance(double dt) {
  if (!isInitialized_) {
//    std::cout << "locomotion controller is not initialized!\n" << std::endl;
    return false;
  }


  for (auto leg : *legs_) {
    leg->advance(dt);
//      std::cout << *leg << std::endl;
//    std::cout << "leg: " << leg->getName() << (leg->isGrounded() ? "is grounded" : "is NOT grounded") << std::endl;
  }

//  std::cout << "Torso:\n";
//  std::cout << *torso_ << std::endl;

  torso_->advance(dt);

  if (!contactDetector_->advance(dt)) {
    return false;
  }

  if (!terrainPerception_->advance(dt)) {
    return false;
  }

  for (auto leg : *legs_) {
    const double swingPhase = leg->getSwingPhase();
    if (leg->wasInStanceMode() && leg->isInSwingMode()) {
      // possible lift-off
      leg->getStateLiftOff()->setFootPositionInWorldFrame(leg->getWorldToFootPositionInWorldFrame()); // or base2foot?
      leg->getStateLiftOff()->setHipPositionInWorldFrame(leg->getWorldToHipPositionInWorldFrame());
      leg->getStateLiftOff()->setIsNow(true);
    } else {
      leg->getStateLiftOff()->setIsNow(false);
    }

    if (leg->wasInSwingMode() && leg->isInStanceMode()) {
      // possible touch-down
      leg->getStateTouchDown()->setIsNow(true);
    } else {
      leg->getStateTouchDown()->setIsNow(false);
    }

  }
  footPlacementStrategy_->advance(dt);
  torsoController_->advance(dt);
  if(!virtualModelController_->compute()) {
//    std::cout << "Error from virtual model controller" << std::endl;
    return false;
  }

  /* Set desired joint positions, torques and control mode */
  LegBase::JointControlModes desiredJointControlModes;
  int iLeg = 0;
  for (auto leg : *legs_) {
    if (leg->isAndShouldBeGrounded()) {
      desiredJointControlModes.setConstant(robotModel::AM_Torque);
    } else {
      desiredJointControlModes.setConstant(robotModel::AM_Position);
    }
    leg->setDesiredJointControlModes(desiredJointControlModes);
    iLeg++;
  }

  return true;
}

TorsoBase* LocomotionControllerJump::getTorso() {
  return torso_;
}
LegGroup* LocomotionControllerJump::getLegs() {
  return legs_;
}

FootPlacementStrategyBase* LocomotionControllerJump::getFootPlacementStrategy() {
  return footPlacementStrategy_;
}

VirtualModelController* LocomotionControllerJump::getVirtualModelController() {
  return virtualModelController_;
}
ContactForceDistributionBase* LocomotionControllerJump::getContactForceDistribution() {
  return contactForceDistribution_;
}

TerrainPerceptionBase* LocomotionControllerJump::getTerrainPerception() {
  return terrainPerception_;
}

bool LocomotionControllerJump::isInitialized() const {
  return isInitialized_;
}

} /* namespace loco */

