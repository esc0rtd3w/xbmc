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

#include "addons/AddonDll.h"
#include "addons/DllPeripheral.h"
#include "addons/include/kodi_peripheral_types.h"
#include "addons/include/kodi_peripheral_utils.hpp"
#include "input/joysticks/JoystickTypes.h"
#include "peripherals/PeripheralTypes.h"

#include <map>
#include <memory>
#include <vector>

class IJoystickButtonMap;
class IJoystickDriverHandler;

namespace PERIPHERALS
{
  class CPeripheral;
  class CPeripheralJoystick;

  class CPeripheralAddon;
  typedef std::shared_ptr<CPeripheralAddon> PeripheralAddonPtr;
  typedef std::vector<PeripheralAddonPtr>   PeripheralAddonVector;

  typedef std::shared_ptr<ADDON::JoystickFeature>   JoystickFeaturePtr;
  typedef std::map<std::string, JoystickFeaturePtr> JoystickFeatureMap;

  class CPeripheralAddon : public ADDON::CAddonDll<DllPeripheral, PeripheralAddon, PERIPHERAL_PROPERTIES>
  {
  public:
    CPeripheralAddon(const ADDON::AddonProps& props);
    CPeripheralAddon(const cp_extension_t* ext);
    virtual ~CPeripheralAddon(void);

    // implementation of IAddon
    virtual ADDON::AddonPtr GetRunningInstance(void) const;

    /*!
     * @brief Initialise the instance of this add-on
     */
    ADDON_STATUS CreateAddon(void);

    bool         Register(unsigned int peripheralIndex, CPeripheral* peripheral);
    void         UnregisterRemovedDevices(const PeripheralScanResults &results, std::vector<CPeripheral*>& removedPeripherals);
    void         GetFeatures(std::vector<PeripheralFeature> &features) const;
    bool         HasFeature(const PeripheralFeature feature) const;
    CPeripheral* GetPeripheral(unsigned int index) const;
    CPeripheral* GetByPath(const std::string &strPath) const;
    int          GetPeripheralsWithFeature(std::vector<CPeripheral*> &results, const PeripheralFeature feature) const;
    size_t       GetNumberOfPeripherals(void) const;
    size_t       GetNumberOfPeripheralsWithId(const int iVendorId, const int iProductId) const;
    void         GetDirectory(const std::string &strPath, CFileItemList &items) const;

    /** @name Peripheral add-on methods */
    //@{
    bool PerformDeviceScan(PeripheralScanResults &results);
    bool ProcessEvents(void);
    //@}

    /** @name Joystick methods */
    //@{
    bool SetJoystickProperties(unsigned int index, CPeripheralJoystick& joystick);
    bool GetButtonMap(const CPeripheral* device, const std::string& strControllerId, JoystickFeatureMap& features);
    bool MapJoystickFeature(const CPeripheral* device, const std::string& strControllerId, const ADDON::JoystickFeature* feature);
    //@}

    void RegisterButtonMap(CPeripheral* device, IJoystickButtonMap* buttonMap);
    void UnregisterButtonMap(IJoystickButtonMap* buttonMap);
    void RefreshButtonMaps(const std::string& strDeviceName = "", const std::string& strControllerId = "");

    static const char* ToString(PERIPHERAL_ERROR error);

  protected:
    /*!
     * @brief Request the API version from the add-on, and check if it's compatible
     * @return True when compatible, false otherwise.
     * @remark Implementation of CAddonDll
     */
    virtual bool CheckAPIVersion(void);

  private:
    /*!
     * @brief Helper functions
     */
    static void GetPeripheralInfo(const CPeripheral* device, ADDON::Peripheral& peripheralInfo);
    //static void SetPeripheralInfo(CPeripheral* device, const ADDON::Peripheral& peripheralInfo); // TODO

    static void GetJoystickInfo(const CPeripheral* device, ADDON::Joystick& joystickInfo);
    static void SetJoystickInfo(CPeripheralJoystick& joystick, const ADDON::Joystick& joystickInfo);

    static HatDirection ToHatDirection(JOYSTICK_STATE_HAT state);

    /*!
     * @brief Reset all class members to their defaults. Called by the constructors
     */
    void ResetProperties(void);

    /*!
     * @brief Retrieve add-on properties from the add-on
     */
    bool GetAddonProperties(void);

    /*!
     * @brief Checks whether the provided API version is compatible with XBMC
     * @param minVersion The add-on's XBMC_PERIPHERAL_MIN_API_VERSION version
     * @param version The add-on's XBMC_PERIPHERAL_API_VERSION version
     * @return True when compatible, false otherwise
     */
    static bool IsCompatibleAPIVersion(const ADDON::AddonVersion &minVersion, const ADDON::AddonVersion &version);

    bool LogError(const PERIPHERAL_ERROR error, const char *strMethod) const;
    void LogException(const std::exception &e, const char *strFunctionName) const;

    /* @brief Cache for const char* members in PERIPHERAL_PROPERTIES */
    std::string         m_strUserPath;    /*!< @brief translated path to the user profile */
    std::string         m_strClientPath;  /*!< @brief translated path to this add-on */

    /* @brief Add-on properties */
    ADDON::AddonVersion m_apiVersion;
    bool                m_bProvidesJoysticks;

    /* @brief Map of peripherals belonging to the add-on */
    std::map<unsigned int, CPeripheral*>  m_peripherals;

    /* @brief Button map observers */
    std::vector<std::pair<CPeripheral*, IJoystickButtonMap*> > m_buttonMaps;

    /* @brief Thread synchronization */
    CCriticalSection    m_critSection;
  };
}