/*
 *      Copyright (C) 2014-2015 Team XBMC
 *      http://xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#include "input/joysticks/DriverPrimitive.h"
#include "input/joysticks/IJoystickDriverHandler.h"

#include <vector>

namespace JOYSTICK
{
  class IJoystickButtonMap;
  class IJoystickButtonMapper;

  /*
   * \brief Generic implementation of a class that provides button mapping by
   *        translating driver events to button mapping commands
   *
   * Button mapping commands are invoked instantly for buttons and hats.
   *
   * Button mapping commands are deferred for a short while after an axis is
   * activated, and only one command will be invoked per activation.
   */
  class CGenericJoystickButtonMapping : public IJoystickDriverHandler
  {
  public:
    /*
     * \brief Constructor for CGenericJoystickButtonMapping
     *
     * \param buttonMapper Carries out button-mapping commands using <buttonMap>
     * \param buttonMap The button map given to <buttonMapper> on each command
     */
    CGenericJoystickButtonMapping(IJoystickButtonMapper* buttonMapper, IJoystickButtonMap* buttonMap);

    virtual ~CGenericJoystickButtonMapping(void) { }

    // implementation of IJoystickDriverHandler
    virtual bool OnButtonMotion(unsigned int buttonIndex, bool bPressed) override;
    virtual bool OnHatMotion(unsigned int hatIndex, HAT_STATE state) override;
    virtual bool OnAxisMotion(unsigned int axisIndex, float position) override;
    virtual void ProcessAxisMotions(void) override;

  private:
    void Activate(const CDriverPrimitive& semiAxis);
    void Deactivate(const CDriverPrimitive& semiAxis);
    bool IsActive(const CDriverPrimitive& semiAxis);

    IJoystickButtonMapper* const m_buttonMapper;
    IJoystickButtonMap* const    m_buttonMap;

    struct ActivatedAxis
    {
      unsigned int     timestamp;
      CDriverPrimitive driverPrimitive;
      bool             bEmitted; // true if this axis has emited a button-mapping command
    };

    std::vector<ActivatedAxis> m_activatedAxes;
  };
}