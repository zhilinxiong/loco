/*
 * LegStateBase.cpp
 *
 *  Created on: Feb 26, 2014
 *      Author: gech
 */

#include "loco/common/LegStateBase.hpp"

namespace loco {

  LegStateBase::LegStateBase() :
        isNow_(false),
        lastStateWasEarly_(false),
        lastStateWasLate_(false),
        contactChangedAtTime_(0.0)
  {


  }


  LegStateBase::~LegStateBase() {

  }


  void LegStateBase::setIsNow(bool isNow) { isNow_= isNow; }
  bool LegStateBase::isNow() const { return isNow_; }

  void LegStateBase::setLastStateWasEarly(bool wasEarly) { lastStateWasEarly_ = wasEarly; }
  bool LegStateBase::lastStateWasEarly() const { return lastStateWasEarly_; }

  void LegStateBase::setLastStateWasLate(bool wasLate) { lastStateWasLate_ = wasLate; }
  bool LegStateBase::lastStateWasLate() const { return lastStateWasLate_; }

  void LegStateBase::setContactChangedAtTime(double time) { contactChangedAtTime_ = time; }
  double LegStateBase::contactChangedAtTime() const { return contactChangedAtTime_; }

} /* namespace loco */
