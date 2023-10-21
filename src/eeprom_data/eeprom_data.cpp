#include "eeprom_data.h"
#include <FlashStorage_STM32.h>
#include "eeprom_metadata.h"
#include "default_profiles.h"
#include "legacy/eeprom_data_v4.h"
#include "legacy/eeprom_data_v5.h"
#include "legacy/eeprom_data_v6.h"
#include "legacy/eeprom_data_v7.h"
#include "legacy/eeprom_data_v8.h"
#include "legacy/eeprom_data_v9.h"
#include "legacy/eeprom_data_v10.h"
#include "legacy/eeprom_data_v11.h"
#include "../log.h"

namespace {

  struct eepromMetadata_t eepromMetadata;

  eepromValues_t getEepromDefaults(void) {
    eepromValues_t defaultData;

    // Profiles
    defaultData.activeProfile = 0;
    for (int i=0; i<MAX_PROFILES; i++) {
      snprintf(defaultData.profiles[i].name, PROFILE_NAME_LENGTH, "%s", defaultsProfile[i].name);
      // temp

      // PI
      defaultData.profiles[i].preinfusionState = defaultsProfile[i].preinfusionState;
      defaultData.profiles[i].preinfusionSec = defaultsProfile[i].preinfusionSec;
      defaultData.profiles[i].preinfusionBar = defaultsProfile[i].preinfusionBar;
      defaultData.profiles[i].preinfusionRamp = defaultsProfile[i].preinfusionRamp;
      defaultData.profiles[i].preinfusionFlowState = defaultsProfile[i].preinfusionFlowState;
      defaultData.profiles[i].preinfusionFlowVol = defaultsProfile[i].preinfusionFlowVol;
      defaultData.profiles[i].preinfusionFlowTime = defaultsProfile[i].preinfusionFlowTime;
      defaultData.profiles[i].preinfusionFlowPressureTarget = defaultsProfile[i].preinfusionFlowPressureTarget;
      defaultData.profiles[i].preinfusionPressureFlowTarget = defaultsProfile[i].preinfusionPressureFlowTarget;
      defaultData.profiles[i].preinfusionFilled = defaultsProfile[i].preinfusionFilled;
      defaultData.profiles[i].preinfusionPressureAbove = defaultsProfile[i].preinfusionPressureAbove;
      defaultData.profiles[i].preinfusionWeightAbove = defaultsProfile[i].preinfusionWeightAbove;
      // SOAK
      defaultData.profiles[i].soakState = defaultsProfile[i].soakState;
      defaultData.profiles[i].soakTimePressure = defaultsProfile[i].soakTimePressure;
      defaultData.profiles[i].soakTimeFlow = defaultsProfile[i].soakTimeFlow;
      defaultData.profiles[i].soakKeepPressure = defaultsProfile[i].soakKeepPressure;
      defaultData.profiles[i].soakKeepFlow = defaultsProfile[i].soakKeepFlow;
      defaultData.profiles[i].soakBelowPressure = defaultsProfile[i].soakBelowPressure;
      defaultData.profiles[i].soakAbovePressure = defaultsProfile[i].soakAbovePressure;
      defaultData.profiles[i].soakAboveWeight = defaultsProfile[i].soakAboveWeight;
      // PI -> TP/PF
      defaultData.profiles[i].preinfusionRamp = defaultsProfile[i].preinfusionRamp;
      defaultData.profiles[i].preinfusionRampSlope = defaultsProfile[i].preinfusionRampSlope;
      // Transition Profile - ramp&hold || advanced profiling
      defaultData.profiles[i].tpState = defaultsProfile[i].tpState;
      defaultData.profiles[i].tpType = defaultsProfile[i].tpType; // transtion profile type :pressure|flow:
      defaultData.profiles[i].tpProfilingStart = defaultsProfile[i].tpProfilingStart;
      defaultData.profiles[i].tpProfilingFinish = defaultsProfile[i].tpProfilingFinish;
      defaultData.profiles[i].tpProfilingHold = defaultsProfile[i].tpProfilingHold;
      defaultData.profiles[i].tpProfilingHoldLimit = defaultsProfile[i].tpProfilingHoldLimit;
      defaultData.profiles[i].tpProfilingSlope = defaultsProfile[i].tpProfilingSlope;
      defaultData.profiles[i].tpProfilingSlopeShape = defaultsProfile[i].tpProfilingSlopeShape;
      defaultData.profiles[i].tpProfilingFlowRestriction = defaultsProfile[i].tpProfilingFlowRestriction;
      defaultData.profiles[i].tfProfileStart = defaultsProfile[i].tfProfileStart;
      defaultData.profiles[i].tfProfileEnd = defaultsProfile[i].tfProfileEnd;
      defaultData.profiles[i].tfProfileHold = defaultsProfile[i].tfProfileHold;
      defaultData.profiles[i].tfProfileHoldLimit = defaultsProfile[i].tfProfileHoldLimit;
      defaultData.profiles[i].tfProfileSlope = defaultsProfile[i].tfProfileSlope;
      defaultData.profiles[i].tfProfileSlopeShape = defaultsProfile[i].tfProfileSlopeShape;
      defaultData.profiles[i].tfProfilingPressureRestriction = defaultsProfile[i].tfProfilingPressureRestriction;
      // Profiling
      defaultData.profiles[i].profilingState = defaultsProfile[i].profilingState;
      defaultData.profiles[i].mfProfileState = defaultsProfile[i].mfProfileState;
      defaultData.profiles[i].mpProfilingStart = defaultsProfile[i].mpProfilingStart;
      defaultData.profiles[i].mpProfilingFinish = defaultsProfile[i].mpProfilingFinish;
      defaultData.profiles[i].mpProfilingSlope = defaultsProfile[i].mpProfilingSlope;
      defaultData.profiles[i].mpProfilingSlopeShape = defaultsProfile[i].mpProfilingSlopeShape;
      defaultData.profiles[i].mpProfilingFlowRestriction = defaultsProfile[i].mpProfilingFlowRestriction;
      defaultData.profiles[i].mfProfileStart = defaultsProfile[i].mfProfileStart;
      defaultData.profiles[i].mfProfileEnd = defaultsProfile[i].mfProfileEnd;
      defaultData.profiles[i].mfProfileSlope = defaultsProfile[i].mfProfileSlope;
      defaultData.profiles[i].mfProfileSlopeShape = defaultsProfile[i].mfProfileSlopeShape;
      defaultData.profiles[i].mfProfilingPressureRestriction = defaultsProfile[i].mfProfilingPressureRestriction;
      /*-----------------------OTHER-----------------*/
      defaultData.profiles[i].setpoint = defaultsProfile[i].setpoint;
      // Dose settings
      defaultData.profiles[i].stopOnWeightState = defaultsProfile[i].stopOnWeightState;
      defaultData.profiles[i].shotDose = defaultsProfile[i].shotDose;
      defaultData.profiles[i].shotStopOnCustomWeight = defaultsProfile[i].shotStopOnCustomWeight;
      defaultData.profiles[i].shotPreset = defaultsProfile[i].shotPreset;
    }
    // General brew settings
    defaultData.homeOnShotFinish = false;
    defaultData.brewDeltaState = true;
    defaultData.basketPrefill = false;
    // System settings
    defaultData.steamSetPoint = 155;
    defaultData.offsetTemp = 7;
    defaultData.hpwr = 550;
    defaultData.mainDivider = 5;
    defaultData.brewDivider = 3;
    defaultData.powerLineFrequency = 50;
    defaultData.lcdSleep = 16;
    defaultData.warmupState = false;
    defaultData.scalesF1 = 4370;
    defaultData.scalesF2 = 4370;
    defaultData.pumpFlowAtZero = 0.2225f;
    defaultData.ledState  = true;
    defaultData.ledDisco  = true;
    defaultData.ledR = 9;
    defaultData.ledG = 0;
    defaultData.ledB = 9;

    return defaultData;
  }

  // kind of annoying, but allows reusing macro without messing up type safety
  template <typename T>
  bool copy_t(T& target, T& source) {
    target = source;
    return true;
  }

  bool loadCurrentEepromData EEPROM_METADATA_LOADER(EEPROM_DATA_VERSION, eepromMetadata_t, copy_t);

}


bool eepromWrite(eepromValues_t eepromValuesNew) {
  const char *errMsg = "Data out of range";
  /* Check various profile array values */
  for (int i=0; i<MAX_PROFILES; i++) {
    if ( eepromValuesNew.profiles[i].preinfusionFlowVol < 0.f
      || eepromValuesNew.profiles[i].mfProfileStart < 0.f
      || eepromValuesNew.profiles[i].mfProfileEnd < 0.f
      || eepromValuesNew.profiles[i].mpProfilingStart < 0.f
      || eepromValuesNew.profiles[i].mpProfilingFinish < 0.f
      || eepromValuesNew.profiles[i].setpoint < 1)
    {
      LOG_ERROR(errMsg);
      return false;
    }
  }
  /* Check various global values */
  if (eepromValuesNew.steamSetPoint < 1
  || eepromValuesNew.steamSetPoint > 165
  || eepromValuesNew.mainDivider < 1
  || eepromValuesNew.brewDivider < 1
  || eepromValuesNew.pumpFlowAtZero < 0.210f
  || eepromValuesNew.pumpFlowAtZero > 0.310f
  || eepromValuesNew.scalesF1 < -20000
  || eepromValuesNew.scalesF2 > 20000)
  {
    LOG_ERROR(errMsg);
    return false;
  }

  /* Saving the values struct + validation and versioning metadata */
  eepromMetadata.timestamp = millis();
  eepromMetadata.version = EEPROM_DATA_VERSION;
  eepromMetadata.values = eepromValuesNew;
  eepromMetadata.versionTimestampXOR = eepromMetadata.timestamp ^ eepromMetadata.version;
  EEPROM.put(0, eepromMetadata);

  return true;
}

void eepromInit(void) {
  // initialiaze defaults on memory
  eepromMetadata.values = getEepromDefaults();

  // read version
  uint16_t version;
  EEPROM.get(0, version);

  // load appropriate version (including current)
  bool readSuccess = false;

  if (version < EEPROM_DATA_VERSION && legacyEepromDataLoaders[version] != nullptr) {
    readSuccess = (*legacyEepromDataLoaders[version])(eepromMetadata.values);
  } else {
    readSuccess = loadCurrentEepromData(eepromMetadata.values);
  }

  if (!readSuccess) {
    LOG_ERROR("SECU_CHECK FAILED! Applying defaults! eepromMetadata.version=%d", version);
    eepromMetadata.values = getEepromDefaults();
  }

  if (!readSuccess || version != EEPROM_DATA_VERSION) {
    eepromWrite(eepromMetadata.values);
  }
}

struct eepromValues_t eepromGetCurrentValues(void) {
  return eepromMetadata.values;
}
struct eepromValues_t eepromGetDefaultValues(void) {
  return getEepromDefaults();
}
