/*
 * Library of routines to communicate with the EPSolar EPEVER
 * LS-B、VS-B、Tracer-B、Tracer-A、iTracer、eTracer Series 
 * of Solar Charge Controllers
 * 
 * Information pulled from vendor specifications:
 *  V2.5 LS-B、VS-B、Tracer-B、Tracer-A、iTracer、eTracer Series Controller 
 *  Communication Instruction
 * 
 * 29Aug2019    patrick conroy      patrick@conroy-family.net
 * 
 */
#include <assert.h>
#include <pthread.h>
#include <log4c.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <modbus/modbus.h>

#include "tracerseries.h"

//
// Functions that drop down to the MODBUS level
static  int     get_coil_value( modbus_t *ctx, const int coilNum, const char *description );
static  void    set_coil_value( modbus_t *ctx, const int coilNum, const int value, const char *description );
static  int     int_read_input_register ( modbus_t *ctx, const int registerAddress, const int numBytes, const char *description, const int badReadValue );
static  float   float_read_input_register ( modbus_t *ctx, const int registerAddress,const int numBytes, const char *description, const float badReadValue );
static  int     int_read_register ( modbus_t *ctx, const int registerAddress, const int numBytes, const char *description, const int badReadValue );
static  float   float_read_register ( modbus_t *ctx, const int registerAddress, const int numBytes, const char *description, const float badReadValue );
static  int     int_read_coil( modbus_t *ctx, const int coilNum, const char *description );
static  void    int_set_coil( modbus_t *ctx, const int coilNum, const int value, const char *description );
static  void    float_write_registers( modbus_t *ctx, const int registerAddress, const float floatValue );
static  void    int_write_registers( modbus_t *ctx, const int registerAddress, const int intValue );

//
// I want my temperatures to default to Farhenheit
static  float   C2F( const float tempC );
static  float   F2C( const float tempF );



//
// If we're in a multithreaded world, let's have a Mutex
static  pthread_mutex_t aMutex = PTHREAD_MUTEX_INITIALIZER;



// -----------------------------------------------------------------------------
static
float   C2F (const float tempC)
{
    // T(°F) = T(°C) × 9/5 + 32
    return ((tempC * 9.0 / 5.0) + 32.0);
}

static  
float   F2C (const float tempF)
{
    return ((tempF - 32.0) * 5.0 / 9.0);
}

// -----------------------------------------------------------------------------
//
//  Get and Set Calls
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
int deviceIsTooHot (modbus_t *ctx)
{
    int         registerAddress = 0x2000;
    uint8_t     value = 0;

    pthread_mutex_lock( &aMutex );
    
    //
    //  Modbus fuction code 0x02    
    if (modbus_read_input_bits( ctx, registerAddress, 1, &value ) == -1) {
        Logger_LogError( "read_input_bits on register %X failed: %s\n", registerAddress, modbus_strerror( errno ));
    }
    pthread_mutex_unlock( &aMutex );    
        
    //
    // Mask off the top 7 just in case
    return ( value & 0b00000001 );    
}

// -----------------------------------------------------------------------------
int isNightTime (modbus_t *ctx)
{
    int         registerAddress = 0x200C;
    uint8_t     value = 0;

    pthread_mutex_lock( &aMutex );
    //
    //  Modbus fuction code 0x02
    if (modbus_read_input_bits( ctx, registerAddress, 1, &value ) == -1) {
        Logger_LogError( "read_input_bits on register %X failed: %s\n", registerAddress, modbus_strerror( errno ));
    }
    pthread_mutex_unlock( &aMutex );    
        
    //
    // Mask off the top 7 just in case
    return ( (value & 0b00000001) == 1 );    
}

// -----------------------------------------------------------------------------
float   getPVArrayInputVoltage  (modbus_t *ctx)
{
    return float_read_input_register( ctx, 0x3100, 1, "PV Array Input Voltage", -1.0 );
}

// -----------------------------------------------------------------------------
float   getPVArrayInputCurrent  (modbus_t *ctx)
{
    return float_read_input_register( ctx, 0x3101, 1, "PV Array Input Current", -1.0 );
}

// -----------------------------------------------------------------------------
float   getPVArrayInputPower  (modbus_t *ctx)
{
    return float_read_input_register( ctx, 0x3102, 2, "PV Array Input Power", -1.0 );
}

// -----------------------------------------------------------------------------
float   getLoadVoltage (modbus_t *ctx)
{
    return float_read_input_register( ctx, 0x310C, 1, "Load Voltage", -1.0 );
}

// -----------------------------------------------------------------------------
float   getLoadCurrent (modbus_t *ctx)
{
    return float_read_input_register( ctx, 0x310D, 1, "Load Current", -1.0 );
}

// -----------------------------------------------------------------------------
float   getLoadPower (modbus_t *ctx)
{
    return float_read_input_register( ctx, 0x310E, 2, "Load Power", -1.0 );
}

// -----------------------------------------------------------------------------
float   getBatteryTemperature(modbus_t *ctx)
{
    return C2F( float_read_input_register( ctx, 0x3110, 1, "Battery Temp", -100.0 ) );
}

// -----------------------------------------------------------------------------
float   getDeviceTemperature (modbus_t *ctx)
{
    return C2F( float_read_input_register( ctx, 0x3111, 1, "Device Temp", -100.0 ) );
}

// -----------------------------------------------------------------------------
int     getBatteryStateOfCharge (modbus_t *ctx)
{
    return (int) (float_read_input_register( ctx, 0x311A, 1, "Battery SoC", -1.0 ) * 100.0 );
}

// -----------------------------------------------------------------------------
float   getBatteryRealRatedVoltage (modbus_t *ctx)
{
    //
    // Current system rated voltage. 12.00, 24.00, 36.00, 48.00 
    //
    // Changing back to a float and removing / 100.
    float   value = float_read_input_register( ctx, 0x311D, 1, "Battery Real Rated Voltage", -1.0 );
    return value;
}


// -----------------------------------------------------------------------------
char    *getBatteryRatedVoltageCode (modbus_t *ctx)
{
    /* 0, auto recognize. 1-12V,
        2-24V ,3-36V, 4-48V, 5-60V, 6-110V,
        7-120V,8-220V,9-240V */
    int value = -9;
    value = int_read_input_register( ctx, 0x9067, 1, "Battery Rated Voltage Code", -1 );
    
    switch (value) {
            case 0: return "Auto";     break;
            case 1: return "12V";      break;
            case 2: return "24V";      break;
            case 3: return "36V";      break;
            case 4: return "48V";      break;
            case 5: return "60V";      break;
            case 6: return "110V";     break;
            case 7: return "120V";     break;
            case 8: return "220V";     break;
            case 9: return "240V";     break;
            default:    return "???";   break;
    }
    return "???";   
}

// -----------------------------------------------------------------------------
uint16_t    getBatteryStatusBits (modbus_t *ctx)
{
    /* from the V2.5 Spec
        D15: 1-Wrong identification for rated voltage
        D8: Battery inner resistance   abnormal 1, normal 0
        D7-D4: 00H Normal, 01H Over Temp.(Higher than the warning settings), 
               02H Low Temp.(Lower than the warning settings),
        D3-D0: 00H Normal ,01H Over Voltage. , 02H Under Voltage, 
               03H Over discharge, 04H Fault
     * */
    return int_read_input_register( ctx, 0x3200, 1, "Battery Status", 0xFFFF );
}

// -----------------------------------------------------------------------------
char    *getBatteryStatusVoltage (const uint16_t statusBits)
{
    // Bits 0..3
    switch (statusBits & 0b0000000000001111) {
        case    0x00:   return "Normal";            break;
        case    0x01:   return "Over";      break;
        case    0x02:   return "Under";     break;
        case    0x03:   return "Over Discharge";    break;
        case    0x04:   return "Fault";             break;
    }
    return "???";
}

// -----------------------------------------------------------------------------
char    *getBatteryStatusTemperature (const uint16_t statusBits)
{
    // Bits 4..7
    //                      fedcba9876543210
    switch ((statusBits & 0b0000000011110000) >> 4) {
        case    0x00:   return "Normal";            break;
        case    0x01:   return "High";  break;
        case    0x02:   return "Low";   break;
    }
    return "???";
}

// -----------------------------------------------------------------------------
char    *getBatteryStatusInnerResistance (const uint16_t statusBits)
{
    // Bit 8
    //                      fedcba9876543210
    return ((statusBits & 0b0000000100000000) ? "Abnormal" : "Normal");
}

// -----------------------------------------------------------------------------
char    *getBatteryStatusIdentification (const uint16_t statusBits)
{   
    //  Bit 15
    //                      fedcba9876543210
    return ((statusBits & 0b1000000000000000) ? "Incorrect" : "Correct");
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
uint16_t    getChargingEquipmentStatusBits (modbus_t *ctx)
{
    /*
     * From the V2.5 Spec
     * D15-D14: Input voltage status. 
     *      00H normal, 01H No input power connected, 
     *      02H Higher input voltage , 03H Input voltage error.
     * D13: Charging MOSFET is short circuit.
     * D12: Charging or Anti-reverse MOSFET is open circuit.
     * D11: Anti-reverse MOSFET is short circuit.
     * D10: Input is over current.
     * D9: The load is over current.
     * D8: The load is short circuit.
     * D7: Load MOSFET is short circuit.
     * D6:Disequilibrium in three circuits.
     * D4: PV input is short circuit.
     * D3-D2: Charging status. 00H No charging,01H Float,02H Boost, 03H Equalization.
     * D1: 0 Normal, 1 Fault.
     * D0: 1 ing, 0 Standby
     */
    return int_read_input_register( ctx, 0x3201, 1, "Charging Equipment Status", 0xFFFF );
}


// -----------------------------------------------------------------------------
char    *getChargingEquipmentStatusInputVoltageStatus (const uint16_t statusBits)
{
    // Bits 14..15
    //                      fedcba9876543210
    switch ((statusBits & 0b1100000000000000) >> 14) {
        case    0x00:   return "Normal";                    break;
        case    0x01:   return "No Input Power Connected";  break;
        case    0x02:   return "Higher Input Voltage";      break;
        case    0x03:   return "Input Volt Error";          break;
    }
    
    return "???";  
}

// -----------------------------------------------------------------------------
int isChargingMOSFETShorted (const uint16_t statusBits)
{   
    // Bits 13
    //                      fedcba9876543210
    return ((statusBits & 0b0010000000000000) ? TRUE : FALSE);
}

// -----------------------------------------------------------------------------
int isChargingMOSFETOpen (const uint16_t statusBits)
{
    // Bits 12
    //                      fedcba9876543210
    return ((statusBits & 0b0001000000000000) ? TRUE : FALSE);
}

// -----------------------------------------------------------------------------
int isAntiReverseMOSFETShort (const uint16_t statusBits)
{
    // Bits 11
    //                      fedcba9876543210
    return ((statusBits & 0b0000100000000000) ? TRUE : FALSE);
}

// -----------------------------------------------------------------------------
int isInputOverCurrent (const uint16_t statusBits)
{
    // Bits 10
    //                     fedcba9876543210
   return ((statusBits & 0b0000010000000000) ? TRUE : FALSE);
}

// -----------------------------------------------------------------------------
int isLoadOverCurrent (const uint16_t statusBits)
{   // Bit 9
    //                      fedcba9876543210
    return ((statusBits & 0b0000001000000000) ? TRUE : FALSE);
}

// -----------------------------------------------------------------------------
int isLoadShorted (const uint16_t statusBits)
{   // Bit 8                fedcba9876543210
    return ((statusBits & 0b0000000100000000) ? TRUE : FALSE);
}

// -----------------------------------------------------------------------------
int isLoadMOSFETShorted (const uint16_t statusBits)
{   // Bit 7                fedcba9876543210
    return ((statusBits & 0b0000000010000000) ? TRUE : FALSE);
}

// -----------------------------------------------------------------------------
int isDisequilibriumInThreeCircuits (const uint16_t statusBits)
{   // Bit 6                fedcba9876543210
    return ((statusBits & 0b0000000001000000) ? TRUE : FALSE);
}

// -----------------------------------------------------------------------------
int isPVInputShorted (const uint16_t statusBits)
{   // Bit 4                fedcba9876543210
    return ((statusBits & 0b0000000000010000) ? TRUE : FALSE);
}

// -----------------------------------------------------------------------------
char    *getChargingStatus (const uint16_t statusBits)
{   // Bits 2,3              fedcba9876543210
    switch ( (statusBits & 0b0000000000001100) >> 2) {
        case    0x00:   return "Not Charging";  break;
        case    0x01:   return "Floating";      break;
        case    0x02:   return "Boosting";      break;
        case    0x03:   return "Equalizing";    break;
    }
    
    return "???";
}

// -----------------------------------------------------------------------------
int isChargingStatusNormal (const uint16_t statusBits)
{   // Bit 1                fedcba9876543210
    return ((statusBits & 0b0000000000000010) ? FALSE : TRUE );
}

// -----------------------------------------------------------------------------
int isChargingStatusRunning (const uint16_t statusBits)
{   // Bit 0               fedcba9876543210
    return ((statusBits & 0b0000000000000001) ? TRUE : FALSE);
}



// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
uint16_t    getdisChargingEquipmentStatusBits (modbus_t *ctx)
{
    /* From V2.5 Spec:
     * D15-D14: 00H Input voltage normal, 01H Input voltage low, 
     *          02H Input voltage high, 03H no access.
     * D13-D12: Output power. 00H Light load, 01H Moderate, 02H rated, 03H overload
     * D11: Short circuit
     * D10: Unable to discharge
     * D9: Unable to stop discharging
     * D8: Output voltage abnormal
     * D7: Input over voltage
     * D6: Short circuit in high voltage side
     * D5: Boost over voltage
     * D4: Output over voltage
     * D1: 0 Normal, 1 Fault.
     * D0: 1 Running, 0 Standby
     */
    return int_read_input_register( ctx, 0x3202, 1, "Discharging Equipment Status", 0xFFFF );
}


// -----------------------------------------------------------------------------
char    *getDischargingStatusInputVoltageStatus (const uint16_t statusBits)
{
    // Bits 14,15           fedcba9876543210
    switch ((statusBits & 0b1100000000000000) >> 14) {
        case    0x00:   return "Normal";             break;
        case    0x01:   return "Low";  break;
        case    0x02:   return "High"; break;
        case    0x03:   return "No Access - Input Volt Error";  break;
    }
    
    return "???"; 
}

// -----------------------------------------------------------------------------
char    *getDischargingStatusOutputPower (const uint16_t statusBits)
{
   // Bits 12,13           fedcba9876543210
   switch ((statusBits & 0b0011000000000000) >> 12) {
        case    0x00:   return "Light";        break;
        case    0x01:   return "Moderate";     break;
        case    0x02:   return "Rated";        break;
        case    0x03:   return "Overload";          break;
    }
    
    return "???"; 
}

// -----------------------------------------------------------------------------                     fedcba9876543210
int isdischargeStatusShorted (const uint16_t statusBits) {                  return ( (statusBits & 0b0000100000000000) ? TRUE : FALSE ); }      //cBit 11
int isdischargeStatusUnableToDischarge (const uint16_t statusBits) {        return ( (statusBits & 0b0000010000000000) ? TRUE : FALSE ); }      // Bit 10
int isdischargeStatusUnableToStopDischarge (const uint16_t statusBits) {    return ( (statusBits & 0b0000001000000000) ? TRUE : FALSE ); }      // Bit 9
int isdischargeStatusOutputVoltageAbnormal (const uint16_t statusBits) {    return ( (statusBits & 0b0000000100000000) ? TRUE : FALSE ); }      // Bit 8
int isdischargeStatusInputOverVoltage (const uint16_t statusBits) {         return ( (statusBits & 0b0000000010000000) ? TRUE : FALSE ); }      // Bit 7
int isdischargeStatusShortedInHighVoltage(const uint16_t statusBits) {      return ( (statusBits & 0b0000000001000000) ? TRUE : FALSE ); }      // Bit 6
int isdischargeStatusBoostOverVoltage(const uint16_t statusBits) {          return ( (statusBits & 0b0000000000100000) ? TRUE : FALSE ); }      // Bit 5
int isdischargeStatusOutputOverVoltage(const uint16_t statusBits) {         return ( (statusBits & 0b0000000000010000) ? TRUE : FALSE ); }      // Bit 4
int isdischargeStatusNormal(const uint16_t statusBits) {                    return ( (statusBits & 0b0000000000000010) ? FALSE : TRUE ); }      // Bit 1
int isdischargeStatusRunning(const uint16_t statusBits) {                   return ( (statusBits & 0b0000000000000001) ? TRUE : FALSE ); }      // Bit 0


// -----------------------------------------------------------------------------
float   getMaximumPVVoltageToday (modbus_t *ctx) { return float_read_input_register( ctx, 0x3300, 1, "Maximum PV Voltage Today", -1.0 ); }
float   getMinimumPVVoltageToday (modbus_t *ctx) { return float_read_input_register( ctx, 0x3301, 1, "Minimum PV Voltage Today", -1.0 ); }

// -----------------------------------------------------------------------------
float   getMaximumBatteryVoltageToday (modbus_t *ctx) { return float_read_input_register( ctx, 0x3302, 1, "Maximum Battery Voltage Today", -1.0 ); }
float   getMinimumBatteryVoltageToday (modbus_t *ctx) { return float_read_input_register( ctx, 0x3303, 1, "Minimum Battery Voltage Today", -1.0 ); }


// -----------------------------------------------------------------------------
float   getConsumedEnergyToday (modbus_t *ctx) { return float_read_input_register( ctx, 0x3304, 2, "Consumed Energy Today", -1.0 ); }
float   getConsumedEnergyMonth (modbus_t *ctx) { return float_read_input_register( ctx, 0x3306, 2, "Consumed Energy This Month", -1.0 ); }
float   getConsumedEnergyYear (modbus_t *ctx)  { return float_read_input_register( ctx, 0x3308, 2, "Consumed Energy This Year", -1.0 ); }
float   getConsumedEnergyTotal (modbus_t *ctx) { return float_read_input_register( ctx, 0x330A, 2, "Total Consumed Energyr", -1.0 ); }

// -----------------------------------------------------------------------------
float   getGeneratedEnergyToday (modbus_t *ctx) { return float_read_input_register( ctx, 0x330C, 2, "Generated Energy Today", -1.0 ); }
float   getGeneratedEnergyMonth (modbus_t *ctx) { return float_read_input_register( ctx, 0x330E, 2, "Generated Energy This Month", -1.0 ); }
float   getGeneratedEnergyYear (modbus_t *ctx)  { return float_read_input_register( ctx, 0x3310, 2, "Generated Energy This Year", -1.0 ); }
float   getGeneratedEnergyTotal (modbus_t *ctx) { return float_read_input_register( ctx, 0x3312, 2, "Total Generated Energyr", -1.0 ); }


// -----------------------------------------------------------------------------
float   getBatteryVoltage (modbus_t *ctx) { return float_read_input_register( ctx, 0x331A, 1, "Battery Voltage", -1.0 ); }
float   getBatteryCurrent (modbus_t *ctx) { return float_read_input_register( ctx, 0x331B, 2, "Battery Current", -1.0 ); }


// -----------------------------------------------------------------------------
float   getRatedChargingCurrent (modbus_t *ctx) { return float_read_input_register( ctx, 0x3005, 1, "Rated Current to Battery", -1.0 ); }
float   getRatedLoadCurrent (modbus_t *ctx) { return float_read_input_register( ctx, 0x300E, 1, "Rated Current to Load", -1.0 ); }

// -----------------------------------------------------------------------------
int     getBoostDuration (modbus_t *ctx) { return int_read_register( ctx, 0x906C, 1, "Boost Duration", -1 ); }
int     getEqualizeDuration (modbus_t *ctx) { return int_read_register( ctx, 0x906B, 1, "Equalize Duration", -1 ); }



// -----------------------------------------------------------------------------
char    *getBatteryType (modbus_t *ctx)
{
    int bt = int_read_register( ctx, 0x9000, 1, "Battery Type", -1 );
    switch (bt) {
        case    0:  return "User Defined";      break;
        case    1:  return "Sealed";            break;
        case    2:  return "Gel";               break;
        case    3:  return "Flooded";           break;
    }
    return "???";
}

//------------------------------------------------------------------------------
void    setBatteryType (modbus_t *ctx, int batteryTypeCode)
{
    assert( batteryTypeCode >= 0x00 && batteryTypeCode <= 0x03 );
    int_write_registers( ctx, 0x9000, batteryTypeCode );
}    
    
//------------------------------------------------------------------------------
int     getBatteryCapacity (modbus_t *ctx) { return int_read_register( ctx, 0x9001, 1, "Battery Capacity", -1 ); }

//------------------------------------------------------------------------------
void    setBatteryCapacity (modbus_t *ctx, int batteryCapacityAH)
{
    assert( batteryCapacityAH >= 0x00 );
    int_write_registers( ctx, 0x9001, batteryCapacityAH );
}

// ------------------------------------------------------------------------------
/******* why is one float and the other int? */
float   getTempertureCompensationCoefficient (modbus_t *ctx) { return float_read_register( ctx, 0x9002, 1, "Temperature Compensation Coefficient", -1.0 ); }
void    setTempertureCompensationCoefficient (modbus_t *ctx, int value) { assert( value >= 0x00 && value <= 0x09); int_write_registers( ctx, 0x9002, value ); }

float   getHighVoltageDisconnect (modbus_t *ctx) { return float_read_register( ctx, 0x9003, 1, "High Voltage Disconnect", -1.0 ); }
void    setHighVoltageDisconnect (modbus_t *ctx, double value) { assert( value >= 9.0 && value <= 17.0 ); float_write_registers( ctx, 0x9003, (float) value ); }

float   getChargingLimitVoltage (modbus_t *ctx) { return float_read_register( ctx, 0x9004, 1, "Charging Limit Voltage", -1.0 ); }
void    setChargingLimitVoltage (modbus_t *ctx, double value) { assert( value >= 9.0 && value <= 17.0 ); float_write_registers( ctx, 0x9004, (float) value ); }

float   getOverVoltageReconnect (modbus_t *ctx) { return float_read_register( ctx, 0x9005, 1, "Over Voltage Reconnect", -1.0 ); }
void    setOverVoltageReconnect (modbus_t *ctx, double value) { assert( value >= 9.0 && value <= 17.0 ); float_write_registers( ctx, 0x9005, (float) value ); }

float   getEqualizationVoltage (modbus_t *ctx) { return float_read_register( ctx, 0x9006, 1, "Equalization Voltage", -1.0 ); }
void    setEqualizationVoltage (modbus_t *ctx, double value) { assert( value >= 9.0 && value <= 17.0 ); float_write_registers( ctx, 0x9006, (float) value ); }

float   getBoostingVoltage (modbus_t *ctx) { return float_read_register( ctx, 0x9007, 1, "Boosting Voltage", -1.0 ); }
void    setBoostingVoltage (modbus_t *ctx, double value) { assert( value >= 9.0 && value <= 17.0 ); float_write_registers( ctx, 0x9007, (float) value ); }

float   getFloatingVoltage (modbus_t *ctx) { return float_read_register( ctx, 0x9008, 1, "Floating Voltage", -1.0 ); }
void    setFloatingVoltage (modbus_t *ctx, double value) { assert( value >= 9.0 && value <= 17.0 ); float_write_registers( ctx, 0x9008, (float) value ); }

float   getBoostReconnectVoltage (modbus_t *ctx) { return float_read_register( ctx, 0x9009, 1, "Boost Reconnect Voltage", -1.0 ); }
void    setBoostReconnectVoltage (modbus_t *ctx, double value) { assert( value >= 9.0 && value <= 17.0 ); float_write_registers( ctx, 0x9009, (float) value ); }



float   getLowVoltageReconnectVoltage (modbus_t *ctx) { return float_read_register( ctx, 0x900A, 1, "Low Voltage Reconnect Voltage", -1.0 ); }
void    setLowVoltageReconnectVoltage (modbus_t *ctx, double value) { assert( value >= 9.0 && value <= 17.0 ); float_write_registers( ctx, 0x900A, (float) value ); }

float   getUnderVoltageWarningRecoverVoltage (modbus_t *ctx) { return float_read_register( ctx, 0x900B, 1, "Under Voltage Warning Recover Voltage", -1.0 ); }
void    setUnderVoltageWarningRecoverVoltage (modbus_t *ctx, double value) { assert( value >= 9.0 && value <= 17.0 ); float_write_registers( ctx, 0x900B, (float) value ); }

float   getUnderVoltageWarningVoltage (modbus_t *ctx) { return float_read_register( ctx, 0x900C, 1, "Under Voltage Warning Voltage", -1.0 ); }
void    setUnderVoltageWarningVoltage (modbus_t *ctx, double value) { float_write_registers( ctx, 0x900C, (float) value ); }

float   getLowVoltageDisconnectVoltage (modbus_t *ctx) { return float_read_register( ctx, 0x900D, 1, "Low Voltage Disconnect Voltage", -1.0 ); }
void    setLowVoltageDisconnectVoltage (modbus_t *ctx, double value) { float_write_registers( ctx, 0x900D, (float) value ); }

float   getDischargingLimitVoltage (modbus_t *ctx) { return float_read_register( ctx, 0x900E, 1, "Discharging Limit Voltage", -1.0 ); }
void    setDischargingLimitVoltage (modbus_t *ctx, double value) { float_write_registers( ctx, 0x900E, (float) value ); }

//------------------------------------------------------------------------------
float   getDischargingPercentage( modbus_t *ctx) { return (float_read_register( ctx, 0x906D, 1, "Discharging Percentage", -1.0 ) * 100.0); }
float   getChargingPercentage( modbus_t *ctx )  { return  (float_read_register( ctx, 0x906E, 1, "Charging Percentage", -1.0 ) * 100.0); }



// -----------------------------------------------------------------------------
void    getRealtimeClock (modbus_t *ctx, int *seconds, int *minutes, int *hour, int *day, int *month, int *year)
{
    int         registerAddress = 0x9013;
    int         numBytes = 0x03;
    uint16_t    buffer[ 32 ];

    memset( buffer, '\0', sizeof buffer );
    
    pthread_mutex_lock( &aMutex );
    
    //
    //  Modbus Function 0x03
    if (modbus_read_registers( ctx, registerAddress, numBytes, buffer ) == -1) {
        Logger_LogError( "getRealtimeClock() - Read of 3 at 0x9013 failed: %s\n", modbus_strerror( errno ));
    }
    pthread_mutex_unlock( &aMutex );
    
    //
    // Failed read sets these all to zero - which is ok
    *seconds = (buffer[ 0 ] & 0x00FF);
    *minutes = ((buffer[ 0 ] & 0xFF00) >> 8);
    *hour = (buffer[ 1 ] & 0x00FF);
    *day = ((buffer[ 1 ] & 0xFF00) >> 8);
    *month = (buffer[ 2 ] & 0x00FF);
    *year = ((buffer[ 2 ] & 0xFF00) >> 8);
}


// -----------------------------------------------------------------------------
char    *getRealtimeClockStr (modbus_t *ctx, char *buffer, const int buffSize)
{
    int seconds, minutes, hour, day, month, year;

    getRealtimeClock( ctx, &seconds, &minutes, &hour, &day, &month, &year );
    snprintf( buffer, buffSize, "%02d/%02d/%02d %02d:%02d:%02d", month, day, year, hour, minutes, seconds );
    return buffer;
}


// -----------------------------------------------------------------------------
void    setRealtimeClock (modbus_t *ctx, const int seconds, const int minutes, const int hour, const int day, const int month, const int year)
{
    assert( seconds >= 0 && seconds <= 59 );
    assert( minutes >= 0 && minutes <= 59 );
    assert( hour >= 0 && hour <= 23 );
    assert( day >= 1 && day <= 31 );
    assert( month >= 1 && month <= 12 );
    assert( year >= 1 && year <= 99 );
    
    uint16_t    buffer[ 32 ];
    
    memset( buffer, '\0', sizeof buffer );
    buffer[ 0 ] = (minutes << 8) | seconds;
    buffer[ 1 ] = (day << 8) | hour;
    buffer[ 2 ] = (year << 8) | month;
    
    int se = (buffer[ 0 ] & 0x00FF);
    int mi = ((buffer[ 0 ] & 0xFF00) >> 8);
    int hr = (buffer[ 1 ] & 0x00FF);
    int dy = ((buffer[ 1 ] & 0xFF00) >> 8);
    int mn = (buffer[ 2 ] & 0x00FF);
    int yr = ((buffer[ 2 ] & 0xFF00) >> 8);
    
    assert( se == seconds );
    assert( mi == minutes );
    assert( hr == hour );
    assert( dy == day );
    assert( mn == month );
    assert( yr == year );

    int         registerAddress = 0x9013;
    int         numBytes = 0x03;
    if (modbus_write_registers( ctx, registerAddress, numBytes, buffer ) == -1) {
        Logger_LogError( "setRealTimeClock() - write failed: %s\n", modbus_strerror( errno ));
    }
}

// -----------------------------------------------------------------------------
void    setRealtimeClockToNow (modbus_t *ctx)
{
    struct tm   *timeInfo;
    time_t      now;

    time( &now );
    timeInfo = localtime( &now );
    int  seconds = timeInfo->tm_sec;
    int  minutes = timeInfo->tm_min;
    int  hour = timeInfo->tm_hour;
    int  day = timeInfo->tm_mday;
    int  month = timeInfo->tm_mon + 1;
    int  year = timeInfo->tm_year % 100;
    
    setRealtimeClock( ctx, seconds, minutes, hour, day, month, year );
}



//------------------------------------------------------------------------------
void    setBatteryTemperatureWarningUpperLimit (modbus_t *ctx, float value)
{
    value = F2C( value );
    float_write_registers( ctx, 0x9017, value );
}

//------------------------------------------------------------------------------
float   getBatteryTemperatureWarningUpperLimit (modbus_t *ctx)
{
    return C2F( float_read_register( ctx, 0x9017, 1, "Battery Temp Upper Limit", -1.0 ) );
}

//------------------------------------------------------------------------------
void    setBatteryTemperatureWarningLowerLimit (modbus_t *ctx, float value)
{
    value = F2C( value );
    float_write_registers( ctx, 0x9018, value );
}

//------------------------------------------------------------------------------
float   getBatteryTemperatureWarningLowerLimit (modbus_t *ctx)
{
    return C2F( float_read_register( ctx, 0x9018, 1, "Battery Temp Lower Limit", -1.0 ) );
}

//------------------------------------------------------------------------------
void    setControllerInnerTemperatureUpperLimit (modbus_t *ctx, double value)
{
    value = F2C( value );
    float_write_registers( ctx, 0x9019, (float) value );
}

//------------------------------------------------------------------------------
float   getControllerInnerTemperatureUpperLimit (modbus_t *ctx)
{
    return C2F( float_read_register( ctx, 0x9019, 1, "Controller Temp Upper Limit", -1.0 ) );
}

//------------------------------------------------------------------------------
void    setControllerInnerTemperatureUpperLimitRecover (modbus_t *ctx, double value)
{
    value = F2C( value );
    float_write_registers( ctx, 0x901A, (float) value );
}

//------------------------------------------------------------------------------
float   getControllerInnerTemperatureUpperLimitRecover (modbus_t *ctx)
{
    return C2F( float_read_register( ctx, 0x901A, 1, "Controller Temp Upper Limit Recovery", -1.0 ) );
}

//------------------------------------------------------------------------------
void    setDayTimeThresholdVoltage (modbus_t *ctx, double value)
{
    float_write_registers( ctx, 0x901E, (float) value );
}

// -----------------------------------------------------------------------------
float   getDayTimeThresholdVoltage (modbus_t *ctx)
{
  return float_read_register( ctx, 0x901E, 1, "Daytime Threshold Voltage", -1.0 );
}

//------------------------------------------------------------------------------
void    setLightSignalStartupDelayTime (modbus_t *ctx, const int value)
{
    int_write_registers( ctx, 0x901F, value );
}

//------------------------------------------------------------------------------
int getLightSignalStartupDelayTime (modbus_t *ctx)
{
    return int_read_register( ctx, 0x901F, 1, "Light Signal Startup Delay", -1 );
}

//------------------------------------------------------------------------------
void    setNightTimeThresholdVoltage (modbus_t *ctx, float value)
{
    float_write_registers( ctx, 0x9020, value );
}

// -----------------------------------------------------------------------------
float   getNightTimeThresholdVoltage (modbus_t *ctx)
{
  return float_read_register( ctx, 0x9020, 1, "Nighttime Threshold Voltage", -1.0 );
}

//------------------------------------------------------------------------------
void    setLightSignalCloseDelayTime (modbus_t *ctx, const int value)
{
    int_write_registers( ctx, 0x9021, value );
}

//------------------------------------------------------------------------------
int getLightSignalCloseDelayTime (modbus_t *ctx)
{
    return int_read_register( ctx, 0x9021, 1, "Light Signal Close Delay", -1 );
}

//------------------------------------------------------------------------------
void    setLoadControllingMode (modbus_t *ctx, const int value)
{
    assert( value >= 0x00 && value <= 0x03 );
    int_write_registers( ctx, 0x903D, value );
}

//------------------------------------------------------------------------------
int     getLoadControllingMode (modbus_t *ctx)
{
    return int_read_register( ctx, 0x903D, 1, "Load Controlling Mode", -1 );
}


//------------------------------------------------------------------------------
void    setWorkingTimeLength1 (modbus_t *ctx, const int hour, const int minute)
{
    assert( hour >= 0 && hour <= 23 );
    assert( minute >= 0 && minute <= 59 );
    int_write_registers( ctx, 0x903E, ((hour << 8) | minute ) );
}

//------------------------------------------------------------------------------
void    setWorkingTimeLength2 (modbus_t *ctx, const int hour, const int minute)
{
    assert( hour >= 0 && hour <= 23 );
    assert( minute >= 0 && minute <= 59 );
    int_write_registers( ctx, 0x903F, ((hour << 8) | minute ) );
}

//------------------------------------------------------------------------------
void    setTurnOnTiming1 (modbus_t *ctx, const int hour, const int minute, const int second)
{
    assert( hour >= 0 && hour <= 23 );
    assert( minute >= 0 && minute <= 59 );
    assert( second >= 0 && second <= 59 );
    int_write_registers( ctx, 0x9042, second );
    int_write_registers( ctx, 0x9043, minute );
    int_write_registers( ctx, 0x9044, hour );
}

//------------------------------------------------------------------------------
void    setTurnOffTiming1 (modbus_t *ctx, const int hour, const int minute, const int second)
{
    assert( hour >= 0 && hour <= 23 );
    assert( minute >= 0 && minute <= 59 );
    assert( second >= 0 && second <= 59 );
    int_write_registers( ctx, 0x9045, second );
    int_write_registers( ctx, 0x9046, minute );
    int_write_registers( ctx, 0x9047, hour );
}

//------------------------------------------------------------------------------
void    setTurnOnTiming2 (modbus_t *ctx, const int hour, const int minute, const int second)
{
    assert( hour >= 0 && hour <= 23 );
    assert( minute >= 0 && minute <= 59 );
    assert( second >= 0 && second <= 59 );
    int_write_registers( ctx, 0x9048, second );
    int_write_registers( ctx, 0x9049, minute );
    int_write_registers( ctx, 0x904A, hour );
}

//------------------------------------------------------------------------------
void    setTurnOffTiming2 (modbus_t *ctx, const int hour, const int minute, const int second)
{
    assert( hour >= 0 && hour <= 23 );
    assert( minute >= 0 && minute <= 59 );
    assert( second >= 0 && second <= 59 );
    int_write_registers( ctx, 0x904B, second );
    int_write_registers( ctx, 0x904C, minute );
    int_write_registers( ctx, 0x904D, hour );
}

//------------------------------------------------------------------------------
void    setBacklightTime (modbus_t *ctx, const int seconds)
{
    int_write_registers( ctx, 0x9063, seconds );
}

//------------------------------------------------------------------------------
void    setLengthOfNight (modbus_t *ctx, const int hour, const int minute)
{
    assert( hour >= 0 && hour <= 23 );
    assert( minute >= 0 && minute <= 59 );
    int_write_registers( ctx, 0x9065, ((hour << 8) | minute ) );
}

// -----------------------------------------------------------------------------
void    getLengthOfNight (modbus_t *ctx, int *hour, int *minute)
{
    int value = 0xFFFF;
    value = int_read_register( ctx, 0x9065, 1, "Length of Night", 0xFFFF );
    
    *minute = (value & 0x0F);
    *hour = (value >> 8);
    Logger_LogDebug( "Length of night returned [%d][%x]  Hour = %d, Min = %d\n", value, value, *hour, *minute );
}

int getBacklightTime (modbus_t *ctx)
{
    return int_read_register( ctx, 0x9063, 1, "Backlight time", -1 );
}

void    getTurnOnTiming2( modbus_t *ctx, int *hour, int *minute, int *second ) 
{ 
    *hour = 1; *minute = 2; *second = 3;
    *second = int_read_register( ctx, 0x9048, 1, "Turn On T2 - Second", -1 );
    *minute = int_read_register( ctx, 0x9049, 1, "Turn On T2 - Minute", -1 );
    *hour   = int_read_register( ctx, 0x904A, 1, "Turn On T2 - Hour", -1 );
}


void    getTurnOffTiming2( modbus_t *ctx, int *hour, int *minute, int *second )
{ 
    *hour = 3; *minute = 4; *second = 5;
    *second = int_read_register( ctx, 0x904B, 1, "Turn Off T2 - Second", -1 );
    *minute = int_read_register( ctx, 0x904C, 1, "Turn Off T2 - Minute", -1 );
    *hour   = int_read_register( ctx, 0x904D, 1, "Turn Off T2 - Hour", -1 );
}



void    getTurnOnTiming1( modbus_t *ctx, int *hour, int *minute, int *second ) 
{ 
    *hour = 11; *minute = 12; *second = 13;
    *second = int_read_register( ctx, 0x9042, 1, "Turn On T1 - Second", -1 );
    *minute = int_read_register( ctx, 0x9043, 1, "Turn On T1 - Minute", -1 );
    *hour   = int_read_register( ctx, 0x9044, 1, "Turn On T1 - Hour", -1 );
}

void    getTurnOffTiming1( modbus_t *ctx, int *hour, int *minute, int *second ) 
{ 
    *hour = 13; *minute = 14; *second = 15;
    *second = int_read_register( ctx, 0x9045, 1, "Turn Off T1 - Second", -1 );
    *minute = int_read_register( ctx, 0x9046, 1, "Turn Off T1 - Minute", -1 );
    *hour   = int_read_register( ctx, 0x9047, 1, "Turn Off T1 - Hour", -1 );
}

void    getWorkingTimeLength1( modbus_t *ctx, int *hour, int *minute ) 
{ 
    *hour = 20; *minute = 21;
    int value = int_read_register( ctx, 0x903E, 1, "Working Time 1 Length", 0xFFFF );
    
    *minute = (value & 0x0F);
    *hour = (value >> 8);
    Logger_LogDebug( "Working Time 1 Length returned [%d][%x]  Hour = %d, Min = %d\n", value, value, *hour, *minute );
}
    
void    getWorkingTimeLength2( modbus_t *ctx, int *hour, int *minute )
{
    *hour = 22; *minute = 23; 
    int value = int_read_register( ctx, 0x903F, 1, "Working Time 1 Length", 0xFFFF );
    
    *minute = (value & 0x0F);
    *hour = (value >> 8);
    Logger_LogDebug( "Working Time 2 Length returned [%d][%x]  Hour = %d, Min = %d\n", value, value, *hour, *minute );
}


//------------------------------------------------------------------------------
void    setDeviceConfigureOfMainPowerSupply (modbus_t *ctx, const int value)
{
    assert( value >= 0x01 && value <= 0x02 );
    int_write_registers( ctx, 0x9066, value );
}

//------------------------------------------------------------------------------
void    setBatteryRatedVoltageCode (modbus_t *ctx, const int value)
{
    assert( value >= 0x00 && value <= 0x09 );
    int_write_registers( ctx, 0x9067, value );
}

//------------------------------------------------------------------------------
void    setDefaultLoadOnOffInManualMode (modbus_t *ctx, const int value)
{
    assert( value >= 0x00 && value <= 0x01 );
    int_write_registers( ctx, 0x906A, value );
}

//------------------------------------------------------------------------------
void    setEqualizeDuration (modbus_t *ctx, const int value)
{
    assert( value >= 0 && value <= 180 );
    int_write_registers( ctx, 0x906B, value );
}

//------------------------------------------------------------------------------
void    setBoostDuration (modbus_t *ctx, const int value)
{
    assert( value >= 10 && value <= 180 );
    int_write_registers( ctx, 0x906C, value );
}

//------------------------------------------------------------------------------
void    setDischargingPercentage (modbus_t *ctx, float value)
{
    assert( value >= 20.0 && value <= 100.0 );
    float_write_registers( ctx, 0x906D, value );
}

//------------------------------------------------------------------------------
void    setChargingPercentage (modbus_t *ctx, double value)
{
    assert( value >= 0.0 && value <= 100.0 );
    float_write_registers( ctx, 0x906E, (double) value );
}

//------------------------------------------------------------------------------
void    setManagementModesOfBatteryChargingAndDischarging (modbus_t *ctx, const int value)
{
    assert( value >= 0 && value <= 1 );
    int_write_registers( ctx, 0x9070, value );
}

//------------------------------------------------------------------------------
char    *getManagementModesOfBatteryChargingAndDischarging (modbus_t *ctx)
{
    int value = int_read_register( ctx, 0x9070, 1, "Charging Mode", -1 );
    if (value == 0)
        return "Volt Comp";
    else if (value == 1)
        return "SoC";
    else
        return "???";
}


// -----------------------------------------------------------------------------
int getChargingDeviceStatus (modbus_t *ctx)
{
    int     coilNum = 0;
    return (get_coil_value( ctx, coilNum, "Charging Device Status (Coil 0)" ));
}

// -----------------------------------------------------------------------------
void    setChargingDeviceStatus (modbus_t *ctx, const int value)
{
    int     coilNum = 0;
    set_coil_value( ctx, coilNum, value, "Charging Device Status (Coil 0)" );
}

// -----------------------------------------------------------------------------
int getOutputControlMode (modbus_t *ctx)
{
    int     coilNum = 1;
    return (get_coil_value( ctx, coilNum, "Output Control Mode (Coil 1)" ));
}

// -----------------------------------------------------------------------------
void    setOutputControlMode (modbus_t *ctx, const int value)
{
    int     coilNum = 1;
    set_coil_value( ctx, coilNum, value, "Output Control Mode (Coil 1)" );
}

// -----------------------------------------------------------------------------
int getManualLoadControlMode (modbus_t *ctx)
{
    int     coilNum = 2;
    return (get_coil_value( ctx, coilNum, "Manual Load Control Mode (Coil 2)" ));
}

// -----------------------------------------------------------------------------
void    setManualLoadControlMode (modbus_t *ctx, const int value)
{
    int     coilNum = 2;
    set_coil_value( ctx, coilNum, value, "Manual Load Control Mode (Coil 2)" );
}

// -----------------------------------------------------------------------------
int getDefaultLoadControlMode (modbus_t *ctx)
{
    int     coilNum = 3;
    return (get_coil_value( ctx, coilNum, "Default Load Control Mode (Coil 3)" ));
}

// -----------------------------------------------------------------------------
void    setDefaultLoadControlMode (modbus_t *ctx, const int value)
{
    int     coilNum = 3;
    set_coil_value( ctx, coilNum, value, "Default Load Control Mode (Coil 3)" );
}

// -----------------------------------------------------------------------------
int getEnableLoadTestMode (modbus_t *ctx)
{
    int     coilNum = 5;
    return (get_coil_value( ctx, coilNum, "Enable Load Test Mode (Coil 5)" ));
}

// -----------------------------------------------------------------------------
void    setEnableLoadTestMode (modbus_t *ctx, const int value)
{
    int     coilNum = 5;
    set_coil_value( ctx, coilNum, value, "Enable Load Test Mode (Coil 5)" );
}

// -----------------------------------------------------------------------------
void    forceLoadOnOff (modbus_t *ctx, const int value)
{
    int     coilNum = 6;
    set_coil_value( ctx, coilNum, value, "Force Load (Coil 6)" );
}

// -----------------------------------------------------------------------------
void    restoreSystemDefaults (modbus_t *ctx)
{
    int     coilNum = 0x13;
    set_coil_value( ctx, coilNum, 1, "Restore System Defaults (Coil 0x13)" );
}

// -----------------------------------------------------------------------------
void    clearEnergyGeneratingStatistics (modbus_t *ctx)
{
    int     coilNum = 0x14;
    set_coil_value( ctx, coilNum, 1, "Clear Energy Gen Stats Load (Coil 0x14)" );
}

//------------------------------------------------------------------------------
void    setChargingDeviceOn (modbus_t *ctx)
{
    int     coilNum = 0x00;
    set_coil_value( ctx, coilNum, 1, "Control Charging Device - Set On (Coil 0x00)" );    
}

//------------------------------------------------------------------------------
void    setChargingDeviceOff (modbus_t *ctx)
{
    int     coilNum = 0x00;
    set_coil_value( ctx, coilNum, 0, "Control Charging Device - Set Off (Coil 0x00)" );  
}

//------------------------------------------------------------------------------
void    setLoadDeviceOn (modbus_t *ctx)
{
    //
    //  LoadControllingMode has to be zero. Mode 1, 2 or 3 then this doesn't work      
    // The turn it on
    set_coil_value( ctx, 0x02, 1, "Setting Load Control to On (Coil 0x02)" );    
}

//------------------------------------------------------------------------------
void    setLoadDeviceOff (modbus_t *ctx)
{
    // LoadControllingMode has to be zero. Mode 1, 2 or 3 then this doesn't work
    // The turn it off
    set_coil_value( ctx, 0x02, 0, "Setting Load Control to Off (Coil 0x02)" );    
}

//------------------------------------------------------------------------------
static
char    *chargingModeToString (uint16_t mode)
{
    switch (mode) {
        case    0x00:   return "Connect/Disconnect";    break;
        case    0x01:   return "PWM";                   break;
        case    0x02:   return "MPPT";                  break;
        default:        return "Unknown";
    }
}


// *****************************************************************************
// **
// ** Little bit of libmodbus doc
//
//  Modbus Library Call         Function    Description         Used in EPSolar?
//  modbus_read_bits()          0x01        read coil status    Yes
//  modbus_read_input_bits      0x02        read input status   Yes
//  modbus_read_registers       0x03        read holding regs   Yes
//  modbus_read_input_registers 0x04        read input regs     Yes
//  modbus_write_bit            0x05        force single coil   Yes
//  modbus_write_register       0x06        preset single register  No
//  modbus_write_bits           0x0F        orce multiple coils     No
//  modbus_write_registers      0x10        preset multiple registers   Yes
//  modbus_write_and_read_registers 0x17    read/write multiples        No
//
//  **
// *****************************************************************************


// -----------------------------------------------------------------------------
static
int     get_coil_value (modbus_t *ctx, const int coilNum, const char *description)
{
    int         numBits = 1;                  
    uint8_t     value = 0;
        
    pthread_mutex_lock( &aMutex );
    
    //
    //  Modbux Function 0x01 - read coil status
    //
    if (modbus_read_bits( ctx, coilNum, numBits, &value ) == -1) {
        Logger_LogError( "%s -- read_bits on coil %d failed: %s\n", description, coilNum, modbus_strerror( errno ));
    }
    pthread_mutex_unlock( &aMutex );    

    //
    // Mask off the top 7 just in case
    return ( value & 0b00000001 );
}

// -----------------------------------------------------------------------------
static
void    set_coil_value (modbus_t *ctx, const int coilNum, const int value, const char *description)
{
    assert( (value == TRUE) || (value == FALSE) );
    
    //Logger_LogDebug( "%s - setting %d to %d\n", description, coilNum, value );
    //
    // Modbus function 0x05
    if (modbus_write_bit( ctx, coilNum, value ) == -1) {
        Logger_LogError( "write_bit on coil %d failed: %s\n", coilNum, modbus_strerror( errno ));
    }
}

// -----------------------------------------------------------------------------
static
void    float_write_registers (modbus_t *ctx, const int registerAddress, float floatValue)
{
    uint16_t    buffer[ 2 ];
   
    memset( buffer, '\0', sizeof buffer );
    buffer[ 0 ] = (uint16_t) (floatValue * 100.0);
        
    if (modbus_write_registers( ctx, registerAddress, 0x01, buffer ) == -1) {
        Logger_LogError( "float_write_registers() - write of value %0.2f to register %X failed: %s\n", floatValue, registerAddress, modbus_strerror( errno ));
    }    
}

// -----------------------------------------------------------------------------
static
void    int_write_registers (modbus_t *ctx, const int registerAddress, const int intValue)
{
    uint16_t    buffer[ 2 ];

    memset( buffer, '\0', sizeof buffer );
    buffer[ 0 ] = (uint16_t) intValue;
       
    if (modbus_write_registers( ctx, registerAddress, 0x01, buffer ) == -1) {
        Logger_LogError( "int_write_registers() - write of value %d to register %X failed: %s\n", intValue, registerAddress, modbus_strerror( errno ));
    }    
}

// ----------------------------------------------------------------------------
static
float   float_read_input_register ( modbus_t *ctx,
                                    const int registerAddress,
                                    const int numBytes,
                                    const char *description,
                                    const float badReadValue)
{
    assert( (numBytes == 1) || (numBytes == 2) );
    
    uint16_t    buffer[ 32  ];
    memset( buffer, '\0', sizeof buffer );
    
    float   returnValue = badReadValue;
    int     status = 0;
    
    pthread_mutex_lock( &aMutex );
    //
    // Modbus function 0x04
    status = modbus_read_input_registers( ctx, registerAddress, numBytes, buffer );
    pthread_mutex_unlock( &aMutex );    
    
    if (status == -1) {
        Logger_LogError( "%s - Read of %d bytes at address %X failed: %s\n", description, numBytes, registerAddress, modbus_strerror( errno ));
    } else {
        if (numBytes == 2) {
            long temp  = buffer[ 0x01 ] << 16;
            temp |= buffer[ 0x00 ];
            returnValue =  (float) temp / 100.0;
        } else {
            returnValue =  buffer[ 0x00 ] / 100.0;
        }
    }
    
    return returnValue;
}

// ----------------------------------------------------------------------------
static
int     int_read_input_register ( modbus_t *ctx,
                                    const int registerAddress,
                                    const int numBytes,
                                    const char *description,
                                    const int badReadValue)
{
    assert( (numBytes == 1) || (numBytes == 2) );
    
    uint16_t    buffer[ 32  ];
    memset( buffer, '\0', sizeof buffer );

    int status = 0;
    int returnValue = badReadValue;
    
    pthread_mutex_lock( &aMutex );
    
    //
    //  Modbus function 0x04
    status = modbus_read_input_registers( ctx, registerAddress, numBytes, buffer );
    pthread_mutex_unlock( &aMutex );    
    
    if (status == -1) {
        Logger_LogError( "%s - Read of %d bytes at address %X failed: %s\n", description, numBytes, registerAddress, modbus_strerror( errno ));
        returnValue =  badReadValue;
        
    } else {
        if (numBytes == 2) {
            long temp  = buffer[ 0x01 ] << 16;
            temp |= buffer[ 0x00 ];
            returnValue =  (int) temp;
        } else {
            returnValue =  ((int) buffer[ 0x00 ]);
        }
    }
    
    return returnValue;
}

// ----------------------------------------------------------------------------
static
float   float_read_register ( modbus_t *ctx,
                                    const int registerAddress,
                                    const int numBytes,
                                    const char *description,
                                    const float badReadValue)
{
    assert( (numBytes == 1) || (numBytes == 2) );
    
    uint16_t    buffer[ 32  ];
    memset( buffer, '\0', sizeof buffer );

    float   returnValue = badReadValue;
    int     status = 0;
    
    pthread_mutex_lock( &aMutex );
    
    //
    //  Modbus Function 0x03
    status = modbus_read_registers( ctx, registerAddress, numBytes, buffer );
    pthread_mutex_unlock( &aMutex );    
    
    if (status == -1) {
        Logger_LogError( "%s - Read of %d bytes at address %X failed: %s\n", description, numBytes, registerAddress, modbus_strerror( errno ));
        returnValue = badReadValue;
    } else {

        if (numBytes == 2) {
            long temp  = buffer[ 0x01 ] << 16;
            temp |= buffer[ 0x00 ];
            returnValue = (float) temp / 100.0;
        } else {
            //
            // Add experimental Negative Value handling - from looking at GitHub
            //  Sources
            long temp = buffer[ 0x00 ];
            if (temp > 0x7FFF) {
                Logger_LogDebug( "Floating point value @buffer[0x00] > 0x7FFF: [%x] [%ld]\n", buffer[ 0x00 ], temp );
                temp = (0xFFFF - temp) * -1.0;
                Logger_LogDebug( " temp is now calculated to be: %ld\n", temp );
            }
            returnValue =  (temp / 100.0);
        }
    }

    return returnValue;
}


// ----------------------------------------------------------------------------
static
int     int_read_register ( modbus_t *ctx,
                                    const int registerAddress,
                                    const int numBytes,
                                    const char *description,
                                    const int badReadValue)
{
    assert( (numBytes == 1) || (numBytes == 2) );
    
    uint16_t    buffer[ 32  ];
    memset( buffer, '\0', sizeof buffer );
    
    int status = 0;
    int returnValue = badReadValue;
    
    pthread_mutex_lock( &aMutex );
    
    //
    //  Modbus function 0x03
    status = modbus_read_registers( ctx, registerAddress, numBytes, buffer );
    pthread_mutex_unlock( &aMutex );    
    
    if (status == -1) {
        Logger_LogError( "%s - Read of %d bytes at address %X failed: %s\n", description, numBytes, registerAddress, modbus_strerror( errno ));
        returnValue = badReadValue;
    } else {

        if (numBytes == 2) {
            long temp  = buffer[ 0x01 ] << 16;
            temp |= buffer[ 0x00 ];
            returnValue =  (int) temp;
        } else {
            returnValue = ((int) buffer[ 0x00 ]);
        }
    }
    
    return returnValue;
}

// -----------------------------------------------------------------------------
void    forceLoadOn  (modbus_t *ctx)
{
    set_coil_value( ctx, 5, 1, "EnableLoadTestMode - Set On (Coil 0x05)" );  
    forceLoadOnOff( ctx, 1 );
    set_coil_value( ctx, 5, 0, "EnableLoadTestMode - Set Off (Coil 0x05)" );  
}

// -----------------------------------------------------------------------------
void    forceLoadOff  (modbus_t *ctx)
{
    set_coil_value( ctx, 5, 1, "EnableLoadTestMode - Set On (Coil 0x05)" );  
    forceLoadOnOff( ctx, 0 );
    set_coil_value( ctx, 5, 0, "EnableLoadTestMode - Set Off (Coil 0x05)" );  
}
