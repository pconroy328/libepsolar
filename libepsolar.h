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


typedef struct  epsolarRealTimeData {
    double  pvVoltage;                      //  Solar Panel Data
    double  pvCurrent;
    double  pvPower;
    char    *pvStatus;
   
    double  batteryVoltage;                 // Battery Data
    double  batteryCurrent;
    double  batteryStateOfCharge;
    char    *batteryStatus;
    double  batteryMaxVoltage;
    double  batteryMinVoltage;
    char    *batteryChargingStatus;
    double  batteryTemperature;
    
    double  loadVoltage;                    // Load Data
    double  loadCurrent;
    double  loadPower;
    int     loadIsOn;
    char    *loadControlMode;
    
    double      controllerTemp;             // Controller (aka Device) Data
    char        *controllerStatus;
    uint16_t    controllerStatusBits;
    
    double  energyGeneratedToday;           // Energy Generated
    double  energyGeneratedMonth;
    double  energyGeneratedYear;
    double  energyGeneratedTotal;
    
    double  energyConsumedToday;            // Energy Consumed
    double  energyConsumedMonth;
    double  energyConsumedYear;
    double  energyConsumedTotal;
    
    int     isNightTime;                    
    char    controllerClock[ 20 ];           // dd/mm/yy hh:mm:ss    18 chars w/ NULL
} epsolarRealTimeData_t;


typedef struct epsolarBatteryData {
    
} epsolarBatteryData_t;


extern  char        *epsolarGetVersion( void );
extern  int         epsolarModbusConnect( const char *portName, const int slaveNumber );
extern  int         epsolarModbusDisconnect( void );
extern  modbus_t    *epsolarModbusGetContext( void );
extern  const char  *epsolarGetDefaultPortName (void);
extern  void        epsolarSetDefaultPortName( const char *newName );
extern  const int   epsolarGetDefaultBaudRate( void );
extern  void        epsolarSetDefaultBaudRate( const int newRate );
extern  const char  epsolarGetDefaultParity( void );
extern  void        epsolarSetDefaultParity (const char newParity );
extern  const int   epsolarGetDataBits( void );
extern  void        epsolarSetDefaultDataBits( const int newBits );
extern  const int   epsolarGetStopBits( void );
extern  void        epsolarSetDefaultStopBits( const int newBits );
extern  void        epsolarGetRealTimeData( epsolarRealTimeData_t *rtData );


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
#define     eps_getBatteryStatusVoltage(B)          getBatteryStatusVoltage(B)
#define     eps_getBatteryStatusIdentification(B)   getBatteryStatusIdentification(B)
#define     eps_getBatteryStatusInnerResistance(B)  getBatteryStatusInnerResistance(B)
#define     eps_getBatteryStatusTemperature(B)      getBatteryStatusTemperature(B)

#define     eps_getManagementModesOfBatteryChargingAndDischarging()     getManagementModesOfBatteryChargingAndDischarging( epsolarModbusGetContext() )
#define     eps_setManagementModesOfBatteryChargingAndDischarging(V)    setManagementModesOfBatteryChargingAndDischarging( epsolarModbusGetContext(),(V) )

#define     eps_getTemperatureCompensationCoefficient()     getTemperatureCompensationCoefficient( epsolarModbusGetContext() )
#define     eps_setTemperatureCompensationCoefficient(V)    setTemperatureCompensationCoefficient( epsolarModbusGetContext(),(V) )

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
#define     eps_setHighVoltageDisconnect(V)         setHighVoltageDisconnect( epsolarModbusGetContext(),(V) )           // * rename
#define     eps_setOverVoltageReconnect(V)          setOverVoltageReconnect( epsolarModbusGetContext(),(V) )

#define     eps_getBoostingVoltage()                getBoostingVoltage( epsolarModbusGetContext() )
#define     eps_getEqualizationVoltage()            getEqualizationVoltage( epsolarModbusGetContext() )
#define     eps_getFloatingVoltage()                getFloatingVoltage( epsolarModbusGetContext() )

#define     eps_setBoostingVoltage(V)               setBoostingVoltage( epsolarModbusGetContext(),(V) )
#define     eps_setEqualizationVoltage(V)           setEqualizationVoltage( epsolarModbusGetContext(),(V) )
#define     eps_setFloatingVoltage(V)               setFloatingVoltage( epsolarModbusGetContext(),(V) )

#define     eps_getBoostReconnectVoltage()          getBoostReconnectVoltage( epsolarModbusGetContext() )
#define     eps_setBoostReconnectVoltage(V)         setBoostReconnectVoltage( epsolarModbusGetContext(),(V) )
#define     eps_getBatteryRatedVoltageCode()        getBatteryRatedVoltageCode( epsolarModbusGetContext() )

#define     eps_getBoostDuration()                  getBoostDuration( epsolarModbusGetContext() )
#define     eps_getEqualizeDuration()               getEqualizeDuration( epsolarModbusGetContext() )
#define     eps_setBoostDuration(V)                 setBoostDuration( epsolarModbusGetContext(),(V) )
#define     eps_setEqualizeDuration(V)              setEqualizeDuration( epsolarModbusGetContext(),(V) )


#define     eps_getChargingLimitVoltage()           getChargingLimitVoltage( epsolarModbusGetContext() )
#define     eps_getDischargingLimitVoltage()        getDischargingLimitVoltage( epsolarModbusGetContext() )
#define     eps_getLowVoltageDisconnectVoltage()    getLowVoltageDisconnectVoltage( epsolarModbusGetContext() )
#define     eps_getLowVoltageReconnectVoltage()     getLowVoltageReconnectVoltage( epsolarModbusGetContext() )
#define     eps_getUnderVoltageWarningVoltage()     getUnderVoltageWarningVoltage( epsolarModbusGetContext() )
#define     eps_getUnderVoltageWarningRecoverVoltage()  getUnderVoltageWarningRecoverVoltage( epsolarModbusGetContext() )

#define     eps_setChargingLimitVoltage(V)              setChargingLimitVoltage( epsolarModbusGetContext(),(V) )
#define     eps_setDischargingLimitVoltage(V)           setDischargingLimitVoltage( epsolarModbusGetContext(),(V) )
#define     eps_setLowVoltageDisconnectVoltage(V)       setLowVoltageDisconnectVoltage( epsolarModbusGetContext(),(V) )
#define     eps_setLowVoltageReconnectVoltage(V)        setLowVoltageReconnectVoltage( epsolarModbusGetContext(),(V) )
#define     eps_setLowVoltageWarningVoltage(V)          setUnderVoltageWarningVoltage( epsolarModbusGetContext(),(V) )
#define     eps_setLowVoltageWarningRecoverVoltage(V)   setUnderVoltageWarningRecoverVoltage( epsolarModbusGetContext(),(V) )


#define     eps_getChargingPercentage()             getChargingPercentage( epsolarModbusGetContext() )
#define     eps_getDischargingPercentage()          getDischargingPercentage( epsolarModbusGetContext() )
#define     eps_setDepthOfCharge(V)                 setChargingPercentage( epsolarModbusGetContext(),(V) )
#define     eps_setDepthOfDischarge(V)              setDischargingPercentage( epsolarModbusGetContext(),(V) )

#define     eps_getNightTimeThresholdVoltage()      getNightTimeThresholdVoltage( epsolarModbusGetContext() )
#define     eps_getLightSignalStartupDelayTime()    getLightSignalStartupDelayTime( epsolarModbusGetContext() )
#define     eps_setNightTimeThresholdVoltage(V)     setNightTimeThresholdVoltage( epsolarModbusGetContext(),(V) )

#define     eps_getDayTimeThresholdVoltage()        getDayTimeThresholdVoltage( epsolarModbusGetContext() )
#define     eps_getLightSignalCloseDelayTime()      getLightSignalCloseDelayTime( epsolarModbusGetContext() )
#define     eps_setDayTimeThresholdVoltage(V)       setDayTimeThresholdVoltage( epsolarModbusGetContext(),(V) )


#define     eps_getLengthOfNight(X,Y)               getLengthOfNight( epsolarModbusGetContext(),(X),(Y))
#define     eps_setLengthOfNight(X,Y)               setLengthOfNight( epsolarModbusGetContext(),(X),(Y))
#define     eps_getBacklightTime()                  getBacklightTime( epsolarModbusGetContext() )

#define     eps_getTurnOnTiming1(X,Y,Z)             getTurnOnTiming1( epsolarModbusGetContext(),(X),(Y),(Z))
#define     eps_getTurnOffTiming1(X,Y,Z)            getTurnOffTiming1( epsolarModbusGetContext(),(X),(Y),(Z))
#define     eps_getTurnOnTiming2(X,Y,Z)             getTurnOnTiming2( epsolarModbusGetContext(),(X),(Y),(Z))
#define     eps_getTurnOffTiming2(X,Y,Z)            getTurnOffTiming2( epsolarModbusGetContext(),(X),(Y),(Z))

#define     eps_setTurnOnTiming1(X,Y,Z)             setTurnOnTiming1( epsolarModbusGetContext(),(X),(Y),(Z))
#define     eps_setTurnOffTiming1(X,Y,Z)            setTurnOffTiming1( epsolarModbusGetContext(),(X),(Y),(Z))
#define     eps_setTurnOnTiming2(X,Y,Z)             setTurnOnTiming2( epsolarModbusGetContext(),(X),(Y),(Z))
#define     eps_setTurnOffTiming2(X,Y,Z)            setTurnOffTiming2( epsolarModbusGetContext(),(X),(Y),(Z))

#define     eps_getWorkingTimeLength1(X,Y)          getWorkingTimeLength1( epsolarModbusGetContext(),(X),(Y))
#define     eps_getWorkingTimeLength2(X,Y)          getWorkingTimeLength2( epsolarModbusGetContext(),(X),(Y))
#define     eps_setWorkingTimeLength1(X,Y)          setWorkingTimeLength1( epsolarModbusGetContext(),(X),(Y))
#define     eps_setWorkingTimeLength2(X,Y)          setWorkingTimeLength2( epsolarModbusGetContext(),(X),(Y))

#define     eps_getControllerInnerTemperatureUpperLimit()           getControllerInnerTemperatureUpperLimit( epsolarModbusGetContext() )
#define     eps_getControllerInnerTemperatureUpperLimitRecover()    getControllerInnerTemperatureUpperLimitRecover( epsolarModbusGetContext() )
#define     eps_getBatteryTemperatureWarningUpperLimit()            getBatteryTemperatureWarningUpperLimit( epsolarModbusGetContext() )
#define     eps_getBatteryTemperatureWarningLowerLimit()            getBatteryTemperatureWarningLowerLimit( epsolarModbusGetContext() )

#define     eps_setBatteryTemperatureWarningUpperLimit(V)           setBatteryTemperatureWarningUpperLimit( epsolarModbusGetContext(),(V) )
#define     eps_setBatteryTemperatureWarningLowerLimit(V)           setBatteryTemperatureWarningLowerLimit( epsolarModbusGetContext(),(V) )

#define     eps_getRealtimeClockStr(X,Y)            getRealtimeClockStr( epsolarModbusGetContext(),(X),(Y) )
#define     eps_setRealtimeClockToNow()             setRealtimeClockToNow( epsolarModbusGetContext() )

#define     eps_clearEnergyGeneratingStatistics()   clearEnergyGeneratingStatistics( epsolarModbusGetContext() )
#define     eps_restoreSystemDefaults()             restoreSystemDefaults( epsolarModbusGetContext() )


#ifdef __cplusplus
}
#endif

#endif /* LIBEPSOLAR_H */

