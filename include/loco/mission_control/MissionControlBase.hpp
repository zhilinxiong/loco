/*
 * MissionControlBase.hpp
 *
 *  Created on: Mar 7, 2014
 *      Author: gech
 */

#ifndef LOCO_MISSIONCONTROLBASE_HPP_
#define LOCO_MISSIONCONTROLBASE_HPP_

#include "loco/common/TypeDefs.hpp"
#include "tinyxml.h"

namespace loco {

class MissionControlBase {
 public:
  MissionControlBase();
  virtual ~MissionControlBase();

  virtual const Twist& getDesiredBaseTwistInBaseFrame() const = 0;
  virtual bool initialize(double dt) = 0;
  virtual void advance(double dt) = 0;
  virtual bool loadParameters(const TiXmlHandle& handle) = 0;
};

} /* namespace loco */

#endif /* LOCO_MISSIONCONTROLBASE_HPP_ */