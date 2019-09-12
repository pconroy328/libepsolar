/*
 */

/* 
 * File:   tracerseries.h
 * Author: pconroy
 *
 * Created on August 29, 2019, 10:04 AM
 */

#ifndef TRACERSERIES_H
#define TRACERSERIES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <modbus/modbus.h>
    
    

extern  float       getBatteryTemperature( modbus_t *ctx );
extern  float       getBatteryRealRatedVoltage( modbus_t *ctx );
extern  int         getBatteryStateOfCharge( modbus_t *ctx );

extern  float       getDeviceTemperature( modbus_t *ctx );

extern  float       getLoadPower( modbus_t *ctx );
extern  float       getLoadCurrent( modbus_t *ctx );
extern  float       getLoadVoltage( modbus_t *ctx );

extern  float       getPVArrayInputPower( modbus_t *ctx );
extern  float       getPVArrayInputCurrent( modbus_t *ctx );
extern  float       getPVArrayInputVoltage( modbus_t *ctx );

extern  int         isNightTime( modbus_t *ctx );

extern  float       getControllerInnerTemperatureUpperLimit( modbus_t * );
extern  float       getControllerInnerTemperatureUpperLimitRecover( modbus_t * );

extern  void        setLoadControllingMode( modbus_t *ctx, const int value );
extern  int         getLoadControllingMode( modbus_t *ctx );

extern  void        forceLoadOff( modbus_t *ctx );
extern  void        forceLoadOn( modbus_t *ctx );
extern  void        forceLoadOnOff( modbus_t *ctx, const int value );

extern  void        setLoadDeviceOff( modbus_t *ctx );
extern  void        setLoadDeviceOn( modbus_t *ctx );

extern  void        setEnableLoadTestMode( modbus_t *ctx, const int value );
extern  int         getEnableLoadTestMode( modbus_t *ctx );

extern  void        setChargingDeviceOff( modbus_t *ctx );
extern  void        setChargingDeviceOn( modbus_t *ctx );

extern  void        setDefaultLoadControlMode( modbus_t *ctx, const int value );
extern  int         getDefaultLoadControlMode( modbus_t *ctx );

extern  void        setManualLoadControlMode( modbus_t *ctx, const int value );
extern  int         getManualLoadControlMode( modbus_t *ctx );

extern  void        setOutputControlMode( modbus_t *ctx, const int value );
extern  int         getOutputControlMode( modbus_t *ctx );

extern  void        setChargingDeviceStatus( modbus_t *ctx, const int value );
extern  int         getChargingDeviceStatus( modbus_t *ctx );

extern  void        setManagementModesOfBatteryChargingAndDischarging( modbus_t *ctx, const int value );
extern  char        *getManagementModesOfBatteryChargingAndDischarging( modbus_t *ctx );

extern  void        setChargingPercentage( modbus_t *ctx,double value );
extern  void        setDischargingPercentage( modbus_t *ctx,float value );
extern  float       getDischargingPercentage( modbus_t *ctx );
extern  float       getChargingPercentage( modbus_t *ctx );

extern  void        setBoostDuration( modbus_t *ctx, const int value );
extern  void        setEqualizeDuration( modbus_t *ctx, const int value );
extern  int         getBoostDuration( modbus_t *ctx );
extern  int         getEqualizeDuration( modbus_t *ctx );

extern  void        setDefaultLoadOnOffInManualMode( modbus_t *ctx, const int value );

extern  char        *getBatteryRatedVoltageCode( modbus_t *ctx );
extern  void        setBatteryRatedVoltageCode( modbus_t *ctx, const int value );

extern  void        setDeviceConfigureOfMainPowerSupply( modbus_t *ctx, const int value );

extern  void        setLengthOfNight( modbus_t *ctx, const int hour, const int minute );
extern  void        getLengthOfNight( modbus_t *ctx, int *hour, int *minute );

extern  int         getBacklightTime( modbus_t *ctx );
extern  void        setBacklightTime( modbus_t *ctx, const int seconds );

extern  void        getTurnOnTiming1( modbus_t *ctx, int *hour, int *minute, int *second );
extern  void        setTurnOnTiming1( modbus_t *ctx, const int hour, const int minute, const int second );
extern  void        getTurnOffTiming1( modbus_t *ctx, int *hour, int *minute, int *second );
extern  void        setTurnOffTiming1( modbus_t *ctx, const int hour, const int minute, const int second );

extern  void        getTurnOnTiming2( modbus_t *ctx, int *hour, int *minute, int *second );
extern  void        setTurnOnTiming2( modbus_t *ctx, const int hour, const int minute, const int second );
extern  void        getTurnOffTiming2( modbus_t *ctx, int *hour, int *minute, int *second );
extern  void        setTurnOffTiming2( modbus_t *ctx, const int hour, const int minute, const int second );

extern  void        getWorkingTimeLength1( modbus_t *ctx, int *hour, int *minute );
extern  void        setWorkingTimeLength1( modbus_t *ctx, const int hour, const int minute );
extern  void        setWorkingTimeLength2( modbus_t *ctx, const int hour, const int minute );
extern  void        getWorkingTimeLength2( modbus_t *ctx, int *hour, int *minute );

extern  void        setLightSignalCloseDelayTime( modbus_t *ctx, const int value );
extern  void        setNightTimeThresholdVoltage( modbus_t *ctx,float value );
extern  void        setLightSignalStartupDelayTime( modbus_t *ctx, const int value );
extern  void        setDayTimeThresholdVoltage( modbus_t *ctx,double value );
extern  float       getDayTimeThresholdVoltage (modbus_t *ctx);
extern  float       getNightTimeThresholdVoltage (modbus_t *ctx);
extern  int         getLightSignalStartupDelayTime (modbus_t *ctx);
extern  int         getLightSignalCloseDelayTime (modbus_t *ctx);


extern  void        setControllerInnerTemperatureUpperLimitRecover( modbus_t *ctx, double value );
extern  void        setControllerInnerTemperatureUpperLimit( modbus_t *ctx, double value );

extern  void        setBatteryTemperatureWarningLowerLimit( modbus_t *ctx, float value );
extern  void        setBatteryTemperatureWarningUpperLimit( modbus_t *ctx, float value );
extern  float       getBatteryTemperatureWarningLowerLimit( modbus_t *ctx );
extern  float       getBatteryTemperatureWarningUpperLimit( modbus_t *ctx );

extern  void        setRealtimeClockToNow( modbus_t *ctx );
extern  void        setRealtimeClock( modbus_t *ctx, const int seconds, const int minutes, const int hour, const int day, const int month, const int year );

extern  char        *getRealtimeClockStr( modbus_t *ctx,char *buffer, const int buffSize );
extern  void        getRealtimeClock( modbus_t *ctx, int *seconds, int *minutes, int *hour, int *day, int *month, int *year );

extern  void        setDischargingLimitVoltage( modbus_t *ctx,double value );
extern  float       getDischargingLimitVoltage( modbus_t *ctx );

extern  void        setLowVoltageDisconnectVoltage( modbus_t *ctx,double value );
extern  float       getLowVoltageDisconnectVoltage( modbus_t *ctx );

extern  void        setUnderVoltageWarningVoltage( modbus_t *ctx,double value );
extern  float       getUnderVoltageWarningVoltage( modbus_t *ctx );

extern  void        setUnderVoltageWarningRecoverVoltage( modbus_t *ctx,double value );
extern  float       getUnderVoltageWarningRecoverVoltage( modbus_t *ctx );

extern  void        setLowVoltageReconnectVoltage( modbus_t *ctx,double value );
extern  float       getLowVoltageReconnectVoltage( modbus_t *ctx );

extern  void        setBoostReconnectVoltage( modbus_t *ctx,double value );
extern  float       getBoostReconnectVoltage( modbus_t *ctx );

extern  void        setFloatingVoltage( modbus_t *ctx,double value );
extern  float       getFloatingVoltage( modbus_t *ctx );

extern  void        setBoostingVoltage( modbus_t *ctx,double value );
extern  float       getBoostingVoltage( modbus_t *ctx );

extern  void        setEqualizationVoltage( modbus_t *ctx,double value );
extern  float       getEqualizationVoltage( modbus_t *ctx );

extern  void        setOverVoltageReconnect( modbus_t *ctx,double value );
extern  float       getOverVoltageReconnect( modbus_t *ctx );

extern  void        setChargingLimitVoltage( modbus_t *ctx,double value );
extern  float       getChargingLimitVoltage( modbus_t *ctx );

extern  void        setHighVoltageDisconnect( modbus_t *ctx,double value );
extern  float       getHighVoltageDisconnect( modbus_t *ctx );

extern  void        setTemperatureCompensationCoefficient( modbus_t *ctx, const int value );
extern  float       getTemperatureCompensationCoefficient( modbus_t *ctx );

extern  void        setBatteryCapacity( modbus_t *ctx, const int batteryCapacityAH );
extern  int         getBatteryCapacity( modbus_t *ctx );

extern  void        setBatteryType( modbus_t *ctx, const int batteryTypeCode );
extern  char        *getBatteryType( modbus_t *ctx );

extern  float       getRatedLoadCurrent( modbus_t *ctx );
extern  float       getRatedChargingCurrent( modbus_t *ctx );
extern  float       getBatteryCurrent( modbus_t *ctx );
extern  float       getBatteryVoltage( modbus_t *ctx );

extern  float       getGeneratedEnergyTotal( modbus_t *ctx );
extern  float       getGeneratedEnergyYear( modbus_t *ctx );
extern  float       getGeneratedEnergyMonth( modbus_t *ctx );
extern  float       getGeneratedEnergyToday( modbus_t *ctx );

extern  float       getConsumedEnergyTotal( modbus_t *ctx );
extern  float       getConsumedEnergyYear( modbus_t *ctx );
extern  float       getConsumedEnergyMonth( modbus_t *ctx );
extern  float       getConsumedEnergyToday( modbus_t *ctx );

extern  float       getMinimumBatteryVoltageToday( modbus_t *ctx );
extern  float       getMaximumBatteryVoltageToday( modbus_t *ctx );
extern  float       getMinimumPVVoltageToday( modbus_t *ctx );
extern  float       getMaximumPVVoltageToday( modbus_t *ctx );

extern  int         isdischargeStatusRunning( const uint16_t statusBits );
extern  int         isdischargeStatusNormal( const uint16_t statusBits );
extern  int         isdischargeStatusOutputOverVoltage( const uint16_t statusBits );
extern  int         isdischargeStatusBoostOverVoltage( const uint16_t statusBits );
extern  int         isdischargeStatusShortedInHighVoltage( const uint16_t statusBits );
extern  int         isdischargeStatusInputOverVoltage( const uint16_t statusBits );
extern  int         isdischargeStatusOutputVoltageAbnormal( const uint16_t statusBits );
extern  int         isdischargeStatusUnableToStopDischarge( const uint16_t statusBits );
extern  int         isdischargeStatusUnableToDischarge( const uint16_t statusBits );
extern  int         isdischargeStatusShorted( const uint16_t statusBits );
extern  char        *getDischargingStatusOutputPower( const uint16_t statusBits );
extern  char        *getDischargingStatusInputVoltageStatus( const uint16_t statusBits );
extern  uint16_t    getdisChargingEquipmentStatusBits( modbus_t *ctx );
extern  int         isChargingStatusRunning( const uint16_t statusBits );
extern  int         isChargingStatusNormal( const uint16_t statusBits );
extern  char        *getChargingStatus( const uint16_t statusBits );
extern  int         isPVInputShorted( const uint16_t statusBits );
extern  int         isDisequilibriumInThreeCircuits( const uint16_t statusBits );
extern  int         isLoadMOSFETShorted( const uint16_t statusBits );
extern  int         isLoadShorted( const uint16_t statusBits );
extern  int         isLoadOverCurrent( const uint16_t statusBits );
extern  int         isInputOverCurrent( const uint16_t statusBits );
extern  int         isAntiReverseMOSFETShort( const uint16_t statusBits );
extern  int         isChargingMOSFETOpen( const uint16_t statusBits );
extern  int         isChargingMOSFETShorted( const uint16_t statusBits );
extern  char        *getChargingEquipmentStatusInputVoltageStatus( const uint16_t statusBits );
extern  uint16_t    getChargingEquipmentStatusBits( modbus_t *ctx );

extern  uint16_t    getBatteryStatusBits( modbus_t *ctx );
extern  char        *getBatteryStatusIdentification( const uint16_t statusBits );
extern  char        *getBatteryStatusInnerResistance( const uint16_t statusBits );
extern  char        *getBatteryStatusTemperature( const uint16_t statusBits );
extern  char        *getBatteryStatusVoltage( const uint16_t statusBits );


extern  float       getConsumedEnergyToday( modbus_t *ctx );
extern  float       getConsumedEnergyMonth( modbus_t *ctx );
extern  float       getConsumedEnergyYear (modbus_t *ctx)  ;
extern  float       getConsumedEnergyTotal( modbus_t *ctx );

extern  float       getGeneratedEnergyToday( modbus_t *ctx );
extern  float       getGeneratedEnergyMonth( modbus_t *ctx );
extern  float       getGeneratedEnergyYear (modbus_t *ctx)  ;
extern  float       getGeneratedEnergyTotal( modbus_t *ctx );

extern  void        clearEnergyGeneratingStatistics( modbus_t *ctx );
extern  void        restoreSystemDefaults( modbus_t *ctx );


#ifdef __cplusplus
}
#endif

#endif /* TRACERSERIES_H */

