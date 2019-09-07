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

//
// Real Time Status and Settings
extern  float   getBatteryRealRatedVoltage( modbus_t *ctx );
extern  int     getBatteryStateOfCharge( modbus_t *ctx );
extern  float   getDeviceTemperature( modbus_t *ctx );
extern  float   getBatteryTemperature( modbus_t *ctx );
extern  float   getLoadPower( modbus_t *ctx );
extern  float   getLoadCurrent( modbus_t *ctx );
extern  float   getLoadVoltage( modbus_t *ctx );
extern  float   getPVArrayInputPower( modbus_t *ctx );
extern  float   getPVArrayInputCurrent( modbus_t *ctx );
extern  float   getPVArrayInputVoltage( modbus_t *ctx );
extern  int     isNightTime( modbus_t *ctx );
extern  int     getOverTemperatureInsideDevice( modbus_t *ctx );
    
//
// Load Related Commands    
extern  void        forceLoadOff( modbus_t *ctx );
extern  void        forceLoadOn( modbus_t *ctx );
extern  void        setLoadDeviceOff( modbus_t *ctx );
extern  void        setLoadDeviceOn( modbus_t *ctx );
extern  void        forceLoadOnOff( modbus_t *ctx, const int value );
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

extern  void    setChargingDeviceStatus( modbus_t *ctx, const int value );
extern  int     getChargingDeviceStatus( modbus_t *ctx );

extern  void    setManagementModesOfBatteryChargingAndDischarging( modbus_t *ctx, const int value );

extern  void    setChargingPercentage( modbus_t *ctx,double value );
extern  void    setDischargingPercentage( modbus_t *ctx,float value );
extern  void    setBoostDuration( modbus_t *ctx, const int value );
extern  void    setEqualizeDuration( modbus_t *ctx, const int value );
extern  void    setDefaultLoadOnOffInManualMode( modbus_t *ctx, const int value );
extern  char    *getBatteryRatedVoltageCode( modbus_t *ctx );
extern  void    setBatteryRatedVoltageCode( modbus_t *ctx, const int value );
extern  void    setDeviceConfigureOfMainPowerSupply( modbus_t *ctx, const int value );
extern  void    setLengthOfNight( modbus_t *ctx, const int hour, const int minute );
extern  void    setBacklightTime( modbus_t *ctx, const int seconds );
extern  void    setTurnOffTiming2( modbus_t *ctx, const int hour, const int minute, const int second );
extern  void    setTurnOnTiming2( modbus_t *ctx, const int hour, const int minute, const int second );
extern  void    setTurnOffTiming1( modbus_t *ctx, const int hour, const int minute, const int second );
extern  void    setTurnOnTiming1( modbus_t *ctx, const int hour, const int minute, const int second );
extern  void    setWorkingTimeLength2( modbus_t *ctx, const int hour, const int minute );
extern  void    setWorkingTimeLength1( modbus_t *ctx, const int hour, const int minute );
extern  void    setLoadControllingModes( modbus_t *ctx, const int value );
extern  void    setLightSignalCloseDelayTime( modbus_t *ctx, const int value );
extern  void    setNightTimeThresholdVoltage( modbus_t *ctx,float value );
extern  void    setLightSignalStartupDelayTime( modbus_t *ctx, const int value );
extern  void    setDayTimeThresholdVoltage( modbus_t *ctx,double value );
extern  void    setControllerInnerTemperatureUpperLimitRecover( modbus_t *ctx,double value );
extern  void    setControllerInnerTemperatureUpperLimit( modbus_t *ctx,double value );
extern  void    setBatteryTemperatureWarningLowerLimit( modbus_t *ctx,float value );
extern  void    setBatteryTemperatureWarningUpperLimit( modbus_t *ctx,float value );
extern  void    setRealtimeClockToNow( modbus_t *ctx );
extern  void    setRealtimeClock( modbus_t *ctx, const int seconds, const int minutes, const int hour, const int day, const int month, const int year );
extern  char    *getRealtimeClockStr( modbus_t *ctx,char *buffer, const int buffSize );
extern  void    getRealtimeClock( modbus_t *ctx, int *seconds, int *minutes, int *hour, int *day, int *month, int *year );
extern  void    setDischargingLimitVoltage( modbus_t *ctx,double value );
extern  float    getDischargingLimitVoltage( modbus_t *ctx );
extern  void    setLowVoltageDisconnectVoltage( modbus_t *ctx,double value );
extern  float   getLowVoltageDisconnectVoltage( modbus_t *ctx );
extern  void    setUnderVoltageWarningVoltage( modbus_t *ctx,double value );
extern  float    getUnderVoltageWarningVoltage( modbus_t *ctx );
extern  void    etUnderVoltageWarningRecoverVoltage( modbus_t *ctx,double value );
extern  float    getUnderVoltageWarningRecoverVoltage( modbus_t *ctx );
extern  void    setLowVoltageReconnectVoltage( modbus_t *ctx,double value );
extern  float     getLowVoltageReconnectVoltage( modbus_t *ctx );
extern  void    setBoostReconnectVoltage( modbus_t *ctx,double value );
extern  float     getBoostReconnectVoltage( modbus_t *ctx );
extern  void    setFloatingVoltage( modbus_t *ctx,double value );
extern  float     getFloatingVoltage( modbus_t *ctx );
extern  void    setBoostingVoltage( modbus_t *ctx,double value );
extern  float     getBoostingVoltage( modbus_t *ctx );
extern  void    setEqualizationVoltage( modbus_t *ctx,double value );
extern  float     getEqualizationVoltage( modbus_t *ctx );
extern  void    setOverVoltageReconnect( modbus_t *ctx,double value );
extern  float     getOverVoltageReconnect( modbus_t *ctx );
extern  void    setChargingLimitVoltage( modbus_t *ctx,double value );
extern  float     getChargingLimitVoltage( modbus_t *ctx );
extern  void    setHighVoltageDisconnect( modbus_t *ctx,double value );
extern  float     getHighVoltageDisconnect( modbus_t *ctx );
extern  void    setTempertureCompensationCoefficient( modbus_t *ctx, const int value );
extern  float     getTempertureCompensationCoefficient( modbus_t *ctx );
extern  void    setBatteryCapacity( modbus_t *ctx, const int batteryCapacityAH );
extern  int     getBatteryCapacity( modbus_t *ctx );
extern  void    setBatteryType( modbus_t *ctx, const int batteryTypeCode );
extern    char     *getBatteryType( modbus_t *ctx );
extern  float     getRatedLoadCurrent( modbus_t *ctx );
extern  float     getRatedChargingCurrent( modbus_t *ctx );
extern  float     getBatteryCurrent( modbus_t *ctx );
extern  float     getBatteryVoltage( modbus_t *ctx );
extern  float     getGeneratedEnergyTotal( modbus_t *ctx );
extern  float     getGeneratedEnergyYear( modbus_t *ctx );
extern  float     getGeneratedEnergyMonth( modbus_t *ctx );
extern  float     getGeneratedEnergyToday( modbus_t *ctx );
extern  float     getConsumedEnergyTotal( modbus_t *ctx );
extern  float     getConsumedEnergyYear( modbus_t *ctx );
extern  float     getConsumedEnergyMonth( modbus_t *ctx );
extern  float     getConsumedEnergyToday( modbus_t *ctx );
extern  float     getMinimumBatteryVoltageToday( modbus_t *ctx );
extern  float     getMaximumBatteryVoltageToday( modbus_t *ctx );
extern  float     getMinimumPVVoltageToday( modbus_t *ctx );
extern  float     getMaximumPVVoltageToday( modbus_t *ctx );
extern  int     isdischargeStatusRunning(const uint16_t statusBits );
extern  int     isdischargeStatusNormal(const uint16_t statusBits );
extern  int     isdischargeStatusOutputOverVoltage(const uint16_t statusBits );
extern  int     isdischargeStatusBoostOverVoltage(const uint16_t statusBits );
extern  int     isdischargeStatusShortedInHighVoltage(const uint16_t statusBits );
extern  int     isdischargeStatusInputOverVoltage(const uint16_t statusBits );
extern  int     isdischargeStatusOutputVoltageAbnormal(const uint16_t statusBits );
extern  int     isdischargeStatusUnableToStopDischarge(const uint16_t statusBits );
extern  int     isdischargeStatusUnableToDischarge(const uint16_t statusBits );
extern  int     isdischargeStatusShorted(const uint16_t statusBits );
extern    char     *getDischargingStatusOutputPower(const uint16_t statusBits );
extern    char     *getDischargingStatusInputVoltageStatus(const uint16_t statusBits );
extern  uint16_t getdisChargingEquipmentStatusBits( modbus_t *ctx );
extern  int     isChargingStatusRunning(const uint16_t statusBits );
extern  int     isChargingStatusNormal(const uint16_t statusBits );
extern    char     *getChargingStatus(const uint16_t statusBits );
extern  int     isPVInputShorted(const uint16_t statusBits );
extern  int     isDisequilibriumInThreeCircuits(const uint16_t statusBits );
extern  int     isLoadMOSFETShorted(const uint16_t statusBits );
extern  int     isLoadShorted(const uint16_t statusBits );
extern  int     isLoadOverCurrent(const uint16_t statusBits );
extern  int     isInputOverCurrent(const uint16_t statusBits );
extern  int     isAntiReverseMOSFETShort(const uint16_t statusBits );
extern  int     isChargingMOSFETOpen(const uint16_t statusBits );
extern  int     isChargingMOSFETShorted(const uint16_t statusBits );
extern    char     *getChargingEquipmentStatusInputVoltageStatus(const uint16_t statusBits );
extern  uint16_t getChargingEquipmentStatusBits( modbus_t *ctx );

extern  uint16_t getBatteryStatusBits( modbus_t *ctx );
extern    char     *getBatteryStatusIdentification(const uint16_t statusBits );
extern    char     *getBatteryStatusInnerResistance(const uint16_t statusBits );
extern    char     *getBatteryStatusTemperature(const uint16_t statusBits );
extern    char     *getBatteryStatusVoltage(const uint16_t statusBits );


extern  float   getConsumedEnergyToday( modbus_t *ctx );
extern  float   getConsumedEnergyMonth( modbus_t *ctx );
extern  float   getConsumedEnergyYear (modbus_t *ctx)  ;
extern  float   getConsumedEnergyTotal( modbus_t *ctx );

// -----------------------------------------------------------------------------
extern  float   getGeneratedEnergyToday( modbus_t *ctx );
extern  float   getGeneratedEnergyMonth( modbus_t *ctx );
extern  float   getGeneratedEnergyYear (modbus_t *ctx)  ;
extern  float   getGeneratedEnergyTotal( modbus_t *ctx );

extern  void        clearEnergyGeneratingStatistics( modbus_t *ctx );
extern  void        restoreSystemDefaults( modbus_t *ctx );

//
//  Data Structures below we patterned after the EPSolar V2.3 Specification. 
//  Things have changed a bit with the V2.5 spec, but I'll stick with the 2.3 structures
//  for now.    
//    
//  Note 1: I'm using the terms "RatedData", "RealTime Data" as they are used in the 
//  documentation I have from EPSolar.
//
//  Note 2: I'm using an EPSolar document noted as "B_Series MODBUS Specification EPEVER Corporation V2.3 June 12, 2015"
//  Other, older versions of this document are out on the net
//
//  About the types I chose:
//      float, if the spec has a value in the Times column 
//      int, if the value appears to be an int
//      uint_16, if the value is a collection of bit values
//
//                                                  Modbus
typedef struct  RealTimeData {                      // address  func        notes
    int     isOverTempInsideDevice;                 // 2000     02(read)    1 = yes
    int     isNightTime;                            // 200C     02(read)    1 = yes
    float   pvArrayVoltage;                         // 3100     04(read)
    float   pvArrayCurrent;                         // 3101     04(read)
    float   pvArrayPower;                           // 3102,3   04(read)
    float   loadVoltage;                            // 310C     04(read)
    float   loadCurrent;                            // 310D     04(read)
    float   loadPower;                              // 310E,F   04(read)
    float   batteryTemperature;                     // 3110     04(read)
    float   deviceTemperature;                      // 3111     04(read)
    int     batterySOC;                             // 311A     04(read)
    float   batteryRealRatedVoltage;                // 311D     04(read)

    uint16_t    batteryStatus;                      // 3200     04(read)    each bit means something
    uint16_t    chargingEquipmentStatus;            // 3201     04(read)    each bit means something
    uint16_t    dischargingEquipmentStatus;         // 3202     04(read)    each bit means something

    float   maximumBatteryVoltageToday;             //  3302    04(read)    resets at 00:00
    float   minimumBatteryVoltageToday;             //  3303    04(read)    resets at 00:00
    float   consumedEnergyToday;                    //  3304,5  04(read)    resets at 00:00
    float   consumedEnergyMonth;                    //  3306,7  04(read)    resets at 00:00
    float   consumedEnergyYear;                     //  3308,9  04(read)    resets at Jan 1, 00:00
    float   totalConsumedEnergy;                    //  330A,B  04(read)
    float   generatedEnergyToday;                   //  330C,D  04(read)    resets at 00:00
    float   generatedEnergyMonth;                   //  330E,F  04(read)    resets at 00:00
    float   generatedEnergyYear;                    //  3310,1  04(read)    resets at Jan 1, 00:00
    float   totalGeneratedEnergy;                   //  3312,3  04(read)

    float   batteryVoltage;                         //  331A    04(read)
    float   batteryCurrent;                         //  331B,C  04(read)

    //
    //  These are NOT in the V2.5 spec! But were in the V2.3!
    //float   maximumInputVoltageToday;               //  3300  04(read)    resets at 00:00
    //float   minimumInputVoltageToday;               //  3301
} RealTimeData_t;


typedef struct      BatteryData {
    float   ratedBatteryCurrent;                    // 3005     04(read)
    float   ratedLoadCurrent;                       // 300E     04(read)    
    float   batteryRealRatedVoltage;                // 311D     04(read)    Current system rated voltage. 1200=12V,2400=24V etc

    int     batteryType;                            // 9000     03(read), 10(write) 
    int     batteryCapacity;                        // 9001     03(read), 10(write) 
    float   temperatureCompensationCoefficient;     // 9002     03(read), 10(write) 
    float   overVoltageDisconnect;                  // 9003     03(read), 10(write) 
    float   chargingLimitVoltage;                   // 9004     03(read), 10(write) 
    float   overVoltageReconnect;                   // 9005     03(read), 10(write) 
    float   equalizationVoltage;                    //  9006    03(read), 10(write) 
    float   boostChargingVoltage;                   //  9007    03(read), 10(write) 
    float   floatChargingVoltage;                   //  9008    03(read), 10(write) 
    float   boostReconnectChargingVoltage;          //  9009    03(read), 10(write) 
    float   lowVoltageReconnectVoltage;             //  900A    03(read), 10(write) 
    float   underVoltageWarningRecoverVoltage;      //  900B    03(read), 10(write) 
    float   underVoltageWarningVoltage;             //  900C    03(read), 10(write)  
    float   lowVoltageDisconnectVoltage;            //  900D    03(read), 10(write) 
    float   dischargingLimitVoltage;                //  900E    03(read), 10(write) 
    
    int     batteryRatedVoltageLevel;               //  9067    03(read), 10(write) 
    int     defaultLoadOnOffInManualMode;           //  906A    03(read), 10(write)    0-off, 1-on
    
    int     equalizeDuration;                       //  906B    03(read), 10(write)
    int     boostDuration;                          //  906C    03(read), 10(write)
    float   batteryDischarge;                       //  906D    03(read), 10(write)
    float   batteryCharge;                          //  906E    03(read), 10(write)
    
    int     chargingMode;                           //  9070    03(read), 10(write)
} BatteryData_t;

typedef struct  LoadParameters {
    // manual control the load
    float   nightTimeThresholdVoltage;          //  901E    03(read), 10(write)
    int     lightSignalStartupDelayTime;        //  901F    03(read), 10(write)
    float   dayTimeThresholdVoltage;            //  9020    03(read), 10(write)
    int     lightSignalCloseDelayTime;          //  9021    03(read), 10(write)
    
    int     loadControllingModes;               //  903D    03(read), 10(write)
    int     workingTimeLength1;                 //  903E    03(read), 10(write)
    int     workingTimeLength2;                 //  903F    03(read), 10(write)
    
    int     turnOnTiming1_seconds;              //  9042    03(read), 10(write)
    int     turnOnTiming1_minutes;              //  9043    03(read), 10(write)
    int     turnOnTiming1_hours;                //  9044    03(read), 10(write)

    int     turnOffTiming1_seconds;             //  9045    03(read), 10(write)
    int     turnOffTiming1_minutes;             //  9046    03(read), 10(write)
    int     turnOffTiming1_hours;               //  9047    03(read), 10(write)
    
    int     turnOnTiming2_seconds;              //  9048    03(read), 10(write)
    int     turnOnTiming2_minutes;              //  9049    03(read), 10(write)
    int     turnOnTiming2_hours;                //  904A    03(read), 10(write)

    int     turnOffTiming2_seconds;             //  904B    03(read), 10(write)
    int     turnOffTiming2_minutes;             //  904C    03(read), 10(write)
    int     turnOffTiming2_hours;               //  904D    03(read), 10(write)
    

    int     lengthOfNight;                      // 9065     03(read), 10(write)
    int     timingControl;                      // 9069     03(read), 10(write)
    int     defaultLoadOnOffManualMode;         // 906A     03(read), 10(write)
} LoadParameters_t;


typedef  struct  RealTimeStatus {
    char    *batteryStatusVoltage;              //  D3..D0
    char    *batteryStatusTemperature;          //  D7..D4
    char    *batteryInnerResistance;            //  D8
    char    *batteryCorrectIdentification;      //  D15
    
    char    *chargingInputVoltageStatus;        //  D15..D14
    int     chargingMOSFETShort;                //  D13
    int     someMOSFETShort;                    //  D12
    int     antiReverseMOSFETShort;             //  D11
    int     inputIsOverCurrent;                 //  D10
    int     loadIsOverCurrent;                  //  D9
    int     loadIsShort;                        //  D8
    int     loadMOSFETIsShort;                  //  D7
    int     pvInputIsShort;                     //  D4
    char    *chargingStatusString;              //  D3..D2
    int     chargingStatusNormal;               //  D1
    int     chargingStatusRunning;              //  D0
    
    char    *dischargingInputVoltageStatus;     //  D15..D14
    char    *dischargingOutputPower;            //  D13..D12
    int     dischargingShortCircuit;            //  D11
    int     unableToDischarge;                  //  D10
    int     unableToStopDischarging;            //  D9
    int     outputVoltageAbnormal;              //  D8
    int     inputOverpressure;                  //  D7
    int     highVoltageSideShort;               //  D6
    int     boostOverpressure;                  //  D5
    int     outputOverpressure;                 //  D4
    int     dischargingStatusNormal;            //  D1
    int     dischargingStatusRunning;           //  D0
} RealTimeStatus_t;

typedef struct RatedData {                          // address, func,       notes
    float   pvArrayRatedVoltage;                    // 
    float   pvArrayRatedCurrent;                    // 
    float   pvArrayRatedPower;                      // , 
    float   batteryRatedPower;
    char    *chargingMode;
    float   ratedCurrentOfLoad;
} RatedData_t;

typedef struct  Settings {
    char    *batteryType;                       //  9000
    int     batteryCapacity;                    //  9001
    float   tempCompensationCoeff;              //  9002
    float   highVoltageDisconnect;              //  9003
    float   chargingLimitVoltage;               //  9004
    float   overVoltageReconnect;               //  9005    
    float   equalizationVoltage;                //  9006
    float   boostVoltage;                       //  9007
    float   floatVoltage;                       //  9008   
    float   boostReconnectVoltage;              //  9009
    float   lowVoltageReconnect;                //  900A
    float   underVoltageRecover;                //  900B
    float   underVoltageWarning;                //  900C
    float   lowVoltageDisconnect;               //  900D
    float   dischargingLimitVoltage;            //  900E
    //int     equalizationChargingCycle;
    char    *realtimeClock;                     //  9013
    float   batteryTempWarningUpperLimit;       //  9017
    float   batteryTempWarningLowerLimit;       //  9018
    
    float   controllerInnerTempUpperLimit;      //  9019
    float   controllerInnerTempUpperLimitRecover;   //  901A
    //float   powerComponentTempUpperLimit;
    //float   powerComponentTempUpperLimitRecover;
    //float   lineImpedence;
    
    float   daytimeThresholdVoltage;            //  901E
    int     lightSignalStartupDelayTime;        //  901F
    float   lightTimeThresholdVoltage;          //  9020
    int     lightSignalCloseDelayTime;          //  9021
    
    int     loadControllingModes;               //  903D
    int     workingTimeLength1;                 //  903E
    int     workingTimeLength2;                 //  903F
    
    int     turnOnTiming1_seconds;              //  9042
    int     turnOnTiming1_minutes;              //  9043
    int     turnOnTiming1_hours;                //  9044

    int     turnOffTiming1_seconds;             //  9045
    int     turnOffTiming1_minutes;             //  9046
    int     turnOffTiming1_hours;               //  9047
    
    int     turnOnTiming2_seconds;              //  9048
    int     turnOnTiming2_minutes;              //  9049
    int     turnOnTiming2_hours;                //  904A

    int     turnOffTiming2_seconds;             //  904B
    int     turnOffTiming2_minutes;             //  904C
    int     turnOffTiming2_hours;               //  904D

    int     backlightTime;                      //  9063

    int     lengthOfNight;                      //  9065
    int     deviceConfigureMainPower;           //  9066
    int     batteryRatedVoltageCode;            //  9067
    //int     loadTimingControlSelection;
    float   equalizeDuration;                   //  906B
    float   boostDuration;                      //  906C
    int     dischargingPercentage;              //  906D
    int     chargingPercentage;                 //  906E
    int     batteryManagementMode;              //  9070
} Settings_t;

typedef struct  StatisticalParameters {
} StatisticalParameters_t;




/* Modbus
 * Function Code    Register Type
    1               Read Coil
    2               Read Discrete Input
    3               Read Holding Registers
    4               Read Input Registers
    5               Write Single Coil
    6               Write Single Holding Register
    15              Write Multiple Coils
    16              Write Multiple Holding Registers
 */
#define MODBUS_NO_FUNC                  0x00
#define MODBUS_READ_COIL                0x01
#define MODBUS_READ_INPUT               0x02
#define MODBUS_READ_HOLDING_REGISTERS   0x03
#define MODBUS_READ_INPUT_REGISTERS     0x04
#define MODBUS_WRITE_COIL               0x05
#define MODBUS_WRITE_HOLDING_REGISTER   0x06
#define MODBUS_WRITE_COILS              0x0F
#define MODBUS_WRITE_HOLDING_REGISTERS  0x10

/*
typedef struct  epsolar_tracer_series {
    uint16_t    v25SpecCode;
    uint16_t    address1;
    uint16_t    address2;
    uint16_t    readFunction;
    uint16_t    writeFunction;
    
    void        *result;
    void        *(rf)( );
    void        *(wf)( );
    char        *description;
} epsolar_t = {
    { 0xA1, 0x2000, 0x00,   MODBUS_READ_INPUT,   MODBUS_NO_FUNC,            NULL,   NULL, NULL,     "Device is Over Temperature" },
    { 0xA2, 0x2000, 0x00,   MODBUS_READ_INPUT,   MODBUS_NO_FUNC,            NULL,   NULL, NULL,     "Is Night" },
    { 0xA3, 0x3100, 0x00,   MODBUS_READ_INPUT_REGISTERS, MODBUS_NO_FUNC,    NULL,   NULL, NULL,     "PC Array Voltage" },
    {}
};
*/

#ifdef __cplusplus
}
#endif

#endif /* TRACERSERIES_H */

