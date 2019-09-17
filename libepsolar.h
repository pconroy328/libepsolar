/*
 */

/* 
 * File:   libepsolar.h
 * Author: pconroy
 *
 * Created on August 29, 2019, 4:49 PM
 */

#ifndef LIBEPSOLAR_H
#define LIBEPSOLAR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <modbus/modbus.h>
#include <epsolar/tracerseries.h>



extern  char        *epsolarGetVersion( void );
extern  int         epsolarModbusConnect( const char *portName, const int slaveNumber );
extern  int         epsolarModbusDisconnect( void );
extern  modbus_t    *epsolarModbusGetContext( void );

//
// Defines to make Modbus Context-Free calls easier
#define     eps_getRatedLoadCurrent()               getRatedLoadCurrent( epsolarModbusGetContext() )
#define     eps_getRatedChargingCurrent()           getRatedChargingCurrent( epsolarModbusGetContext() )

#define     eps_getBatteryVoltage()                 getBatteryVoltage( epsolarModbusGetContext() )
#define     eps_getBatteryCurrent()                 getBatteryCurrent( epsolarModbusGetContext() )
#define     eps_getBatteryTemperature()             getBatteryTemperature( epsolarModbusGetContext() )
#define     eps_getBatteryRealRatedVoltage()        getBatteryRealRatedVoltage( epsolarModbusGetContext() )
#define     eps_getBatteryStateOfCharge()           getBatteryStateOfCharge( epsolarModbusGetContext() )

#define     eps_getBatteryType()                    getBatteryType( epsolarModbusGetContext() )
#define     eps_getBatteryCapacity()                getBatteryCapacity( epsolarModbusGetContext() )
#define     eps_setBatteryType(V)                   setBatteryType( epsolarModbusGetContext(),(V) )
#define     eps_setBatteryCapacity(V)               setBatteryCapacity( epsolarModbusGetContext(),(V) )


#define     eps_getDeviceTemperature()              getDeviceTemperature( epsolarModbusGetContext() )

#define     eps_getLoadPower()                      getLoadPower( epsolarModbusGetContext() )
#define     eps_getLoadCurrent()                    getLoadCurrent( epsolarModbusGetContext() )
#define     eps_getLoadVoltage()                    getLoadVoltage( epsolarModbusGetContext() )

#define     eps_getPVArrayInputPower()              getPVArrayInputPower( epsolarModbusGetContext() )
#define     eps_getPVArrayInputCurrent()            getPVArrayInputCurrent( epsolarModbusGetContext() )
#define     eps_getPVArrayInputVoltage()            getPVArrayInputVoltage( epsolarModbusGetContext() )

#define     eps_isNightTime()                       isNightTime( epsolarModbusGetContext() )

#define     eps_getControllerInnerTemperatureUpperLimit()           getControllerInnerTemperatureUpperLimit( epsolarModbusGetContext() )
#define     eps_setControllerInnerTemperatureUpperLimit(V)          setControllerInnerTemperatureUpperLimit( epsolarModbusGetContext(),(V) )

#define     eps_getControllerInnerTemperatureUpperLimitRecover()    getControllerInnerTemperatureUpperLimitRecover( epsolarModbusGetContext() )
#define     eps_setControllerInnerTemperatureUpperLimitRecover(V)   setControllerInnerTemperatureUpperLimitRecover( epsolarModbusGetContext(),(V) )

#define     eps_setLoadControllingMode(V)           setLoadControllingMode( epsolarModbusGetContext(),(V) )
#define     eps_getLoadControllingMode()            getLoadControllingMode( epsolarModbusGetContext() )

#define     eps_forceLoadOff()                      forceLoadOff( epsolarModbusGetContext() )
#define     eps_forceLoadOn()                       forceLoadOn( epsolarModbusGetContext() )
#define     eps_forceLoadOnOff(V)                   forceLoadOnOff( epsolarModbusGetContext(),(V) )

#define     eps_setLoadDeviceOff()                  setLoadDeviceOff( epsolarModbusGetContext() )
#define     eps_setLoadDeviceOn()                   setLoadDeviceOn( epsolarModbusGetContext() )

#define     eps_setEnableLoadTestMode(V)            setEnableLoadTestMode( epsolarModbusGetContext(),(V) )
#define     eps_getEnableLoadTestMode()             getEnableLoadTestMode( epsolarModbusGetContext() )

#define     eps_setChargingDeviceOff()              setChargingDeviceOff( epsolarModbusGetContext() )
#define     eps_setChargingDeviceOn()               setChargingDeviceOn( epsolarModbusGetContext() )

#define     eps_setDefaultLoadControlMode(V)        setDefaultLoadControlMode( epsolarModbusGetContext(),(V) )
#define     eps_getDefaultLoadControlMode()         getDefaultLoadControlMode( epsolarModbusGetContext() )

#define     eps_setManualLoadControlMode(V)         setManualLoadControlMode( epsolarModbusGetContext(),(V) )
#define     eps_getManualLoadControlMode()          getManualLoadControlMode( epsolarModbusGetContext() )

#define     eps_setOutputControlMode(V)             setOutputControlMode( epsolarModbusGetContext(),(V) )
#define     eps_getOutputControlMode()              getOutputControlMode( epsolarModbusGetContext() )

#define     eps_getMinimumBatteryVoltageToday()     getMinimumBatteryVoltageToday( epsolarModbusGetContext() )
#define     eps_getMaximumBatteryVoltageToday()     getMaximumBatteryVoltageToday( epsolarModbusGetContext() )

#define     eps_getChargingEquipmentStatusBits()    getChargingEquipmentStatusBits( epsolarModbusGetContext() )
#define     eps_getChargingStatus(V)                getChargingStatus(V)
#define     eps_getChargingEquipmentStatusInputVoltageStatus(V)     getChargingEquipmentStatusInputVoltageStatus(V)

#define     eps_getBatteryStatusBits()              getBatteryStatusBits( epsolarModbusGetContext() )
#define     eps_getBatteryStatusVoltage(V)          getBatteryStatusVoltage(V)
#define     eps_getBatteryStatusIdentification(V)   getBatteryStatusIdentification(V)
#define     eps_getBatteryStatusInnerResistance(V)  getBatteryStatusInnerResistance(V)
#define     eps_getBatteryStatusTemperature(V)      getBatteryStatusTemperature(V)

#define     eps_getManagementModesOfBatteryChargingAndDischarging() getManagementModesOfBatteryChargingAndDischarging( epsolarModbusGetContext() )
#define     eps_setManagementModesOfBatteryChargingAndDischarging(V)    setManagementModesOfBatteryChargingAndDischarging( epsolarModbusGetContext(),(V) )

#define     eps_getTemperatureCompensationCoefficient() getTemperatureCompensationCoefficient( epsolarModbusGetContext() )

#define     eps_getdisChargingEquipmentStatusBits() getdisChargingEquipmentStatusBits( epsolarModbusGetContext() )
#define     eps_isdischargeStatusRunning(V)         isdischargeStatusRunning(V)     

#define     eps_getConsumedEnergyToday()            getConsumedEnergyToday( epsolarModbusGetContext() )
#define     eps_getConsumedEnergyMonth()            getConsumedEnergyMonth( epsolarModbusGetContext() )
#define     eps_getConsumedEnergyYear()             getConsumedEnergyYear( epsolarModbusGetContext() )
#define     eps_getConsumedEnergyTotal()            getConsumedEnergyTotal( epsolarModbusGetContext() )
#define     eps_getGeneratedEnergyToday()           getGeneratedEnergyToday( epsolarModbusGetContext() )
#define     eps_getGeneratedEnergyMonth()           getGeneratedEnergyMonth( epsolarModbusGetContext() )
#define     eps_getGeneratedEnergyYear()            getGeneratedEnergyYear( epsolarModbusGetContext() )
#define     eps_getGeneratedEnergyTotal()           getGeneratedEnergyTotal( epsolarModbusGetContext() )

#define     eps_getHighVoltageDisconnect()          getHighVoltageDisconnect( epsolarModbusGetContext() )           // * rename
#define     eps_getOverVoltageReconnect()           getOverVoltageReconnect( epsolarModbusGetContext() )

#define     eps_getBoostingVoltage()                getBoostingVoltage( epsolarModbusGetContext() )
#define     eps_getEqualizationVoltage()            getEqualizationVoltage( epsolarModbusGetContext() )
#define     eps_getFloatingVoltage()                getFloatingVoltage( epsolarModbusGetContext() )
#define     eps_getBoostReconnectVoltage()          getBoostReconnectVoltage( epsolarModbusGetContext() )
#define     eps_getBatteryRatedVoltageCode()        getBatteryRatedVoltageCode( epsolarModbusGetContext() )

#define     eps_getBoostDuration()                  getBoostDuration( epsolarModbusGetContext() )
#define     eps_getEqualizeDuration()               getEqualizeDuration( epsolarModbusGetContext() )
#define     eps_getChargingLimitVoltage()           getChargingLimitVoltage( epsolarModbusGetContext() )
#define     eps_getDischargingLimitVoltage()        getDischargingLimitVoltage( epsolarModbusGetContext() )
#define     eps_getLowVoltageDisconnectVoltage()    getLowVoltageDisconnectVoltage( epsolarModbusGetContext() )
#define     eps_getLowVoltageReconnectVoltage()     getLowVoltageReconnectVoltage( epsolarModbusGetContext() )
#define     eps_getUnderVoltageWarningVoltage()     getUnderVoltageWarningVoltage( epsolarModbusGetContext() )
#define     eps_getUnderVoltageWarningRecoverVoltage()  getUnderVoltageWarningRecoverVoltage( epsolarModbusGetContext() )


#define     eps_getChargingPercentage()             getChargingPercentage( epsolarModbusGetContext() )
#define     eps_getDischargingPercentage()          getDischargingPercentage( epsolarModbusGetContext() )

#define     eps_getLoadControllingMode()            getLoadControllingMode( epsolarModbusGetContext() )
#define     eps_getNightTimeThresholdVoltage()      getNightTimeThresholdVoltage( epsolarModbusGetContext() )
#define     eps_getLightSignalStartupDelayTime()    getLightSignalStartupDelayTime( epsolarModbusGetContext() )
#define     eps_getDayTimeThresholdVoltage()        getDayTimeThresholdVoltage( epsolarModbusGetContext() )
#define     eps_getLightSignalCloseDelayTime()      getLightSignalCloseDelayTime( epsolarModbusGetContext() )


#define     eps_getLengthOfNight( X,Y)              getLengthOfNight( epsolarModbusGetContext(),(X),(Y))
#define     eps_getBacklightTime()                  getBacklightTime( epsolarModbusGetContext() )

#define     eps_getTurnOnTiming1( X,Y,Z)            getTurnOnTiming1( epsolarModbusGetContext(),(X),(Y),(Z))
#define     eps_getTurnOffTiming1( X,Y,Z)           getTurnOffTiming1( epsolarModbusGetContext(),(X),(Y),(Z))
#define     eps_getTurnOnTiming2( X,Y,Z)            getTurnOnTiming2( epsolarModbusGetContext(),(X),(Y),(Z))
#define     eps_getTurnOffTiming2( X,Y,Z)           getTurnOffTiming2( epsolarModbusGetContext(),(X),(Y),(Z))

#define     eps_getWorkingTimeLength1( X,Y)         getWorkingTimeLength1( epsolarModbusGetContext(),(X),(Y))
#define     eps_getWorkingTimeLength2( X,Y)         getWorkingTimeLength2( epsolarModbusGetContext(),(X),(Y))

#define     eps_getControllerInnerTemperatureUpperLimit()           getControllerInnerTemperatureUpperLimit( epsolarModbusGetContext() )
#define     eps_getControllerInnerTemperatureUpperLimitRecover()    getControllerInnerTemperatureUpperLimitRecover( epsolarModbusGetContext() )
#define     eps_getBatteryTemperatureWarningUpperLimit()            getBatteryTemperatureWarningUpperLimit( epsolarModbusGetContext() )
#define     eps_getBatteryTemperatureWarningLowerLimit()            getBatteryTemperatureWarningLowerLimit( epsolarModbusGetContext() )

#define     eps_setBatteryTemperatureWarningUpperLimit(V)           setBatteryTemperatureWarningUpperLimit( epsolarModbusGetContext(),(V) )
#define     eps_setBatteryTemperatureWarningLowerLimit(V)           setBatteryTemperatureWarningLowerLimit( epsolarModbusGetContext(),(V) )

#define     eps_getRealtimeClockStr(X,Y)            getRealtimeClockStr(epsolarModbusGetContext(),(X),(Y))
#define     eps_setRealtimeClockToNow()             setRealtimeClockToNow(epsolarModbusGetContext())



#ifdef __cplusplus
}
#endif

#endif /* LIBEPSOLAR_H */

