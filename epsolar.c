/*
 */
#include <stdio.h>
#include <errno.h>
#include "log4c.h"

#include "libepsolar.h"

static char         *version = "libepsolar v1.1.2 (debug)";


static  char    *defaultPortName = "/dev/ttyXRUSB0";
static  int     defaultBaudRate = 115200;
static  char    defaultParity = 'N';
static  int     defaultDataBits = 8;
static  int     defaultStopBits = 1;

static  modbus_t    *ctx = NULL;


static  const char  *getPVStatus( const uint16_t chargingEquipmentStatusBits );
static  const char  *getControllerStatus( const uint16_t chargingEquipmentStatusBits );
static  const char  *getLoadControlMode();



// -----------------------------------------------------------------------------
char    *epsolarGetVersion (void)
{
    return version;
}


// -----------------------------------------------------------------------------
int epsolarModbusConnect (const char *portName, const int slaveNumber)
{
    Logger_LogInfo( "Compiled against libmodbus version %s\n", LIBMODBUS_VERSION_STRING );
    //
    // Use the passed in port name, unless it's null.
    if (portName != NULL)
        defaultPortName = (char *) portName;
    
    //
    // Modbus - open the SCC port
    Logger_LogInfo( "Opening %s, %d %d%c%d\n", 
            defaultPortName, defaultBaudRate,
            defaultDataBits, defaultParity, defaultStopBits );
#ifdef FAKEOUT
    Logger_LogError( "USING FAKED OUT LIBRARY CALLS!" );
    return;
#endif
    
    ctx = modbus_new_rtu( defaultPortName, defaultBaudRate, defaultParity, defaultDataBits, defaultStopBits );
    if (ctx == NULL) {
        Logger_LogFatal( "Unable to create the libmodbus context\n" );
        return FALSE;
    }
    
    Logger_LogInfo( "Setting slave ID to 0x%X\n", slaveNumber );
    modbus_set_slave( ctx, slaveNumber );

    if (modbus_connect( ctx ) == -1) {
        Logger_LogFatal( "Connection failed: %s\n", modbus_strerror( errno ) );
        modbus_free( ctx );
        return FALSE;
    }
    
    Logger_LogInfo( "Port to Solar Charge Controller is open.\n", portName );
    
    //
    // I'm getting the occasional error and timeout - let's add some Modbus debugging calls
#ifdef BUSTER
    uint32_t to_sec;
    uint32_t to_usec;
    
    modbus_get_byte_timeout( ctx, &to_sec, &to_usec );
    Logger_LogWarning( "Modbus get_byte timeout value is %ld secs, %ld usecs\n", (long) to_sec, (long) to_usec );

    modbus_get_response_timeout( ctx, &to_sec, &to_usec );
    Logger_LogWarning( "Modbus get_response timeout value is %ld secs, %ld usecs\n", (long) to_sec, (long) to_usec );
#else
    struct  timeval timeout;
    modbus_get_byte_timeout( ctx, &timeout );
    Logger_LogWarning( "Modbus get_byte timeout value is %ld secs, %ld usecs\n", (long) timeout.tv_sec, (long) timeout.tv_usec );

    modbus_get_response_timeout( ctx, &timeout );
    Logger_LogWarning( "Modbus get_response timeout value is %ld secs, %ld usecs\n", (long) timeout.tv_sec, (long) timeout.tv_usec );
#endif
    
    return TRUE;
}

// -----------------------------------------------------------------------------
int epsolarModbusDisconnect (void)
{
    if (ctx == NULL)
        return TRUE;
    
    modbus_close( ctx );
    modbus_free( ctx );
    
    ctx = NULL;
    return TRUE;
}

// -----------------------------------------------------------------------------
modbus_t    *epsolarModbusGetContext (void)
{
    return ctx;
}

// -----------------------------------------------------------------------------
const   char    *epsolarGetDefaultPortName (void)
{ 
    return defaultPortName;
}

// -----------------------------------------------------------------------------
void    epsolarSetDefaultPortName (const char *newName)
{ 
    defaultPortName = (char *) newName;
}

// -----------------------------------------------------------------------------
const   int epsolarGetDefaultBaudRate (void)
{
    return defaultBaudRate;
}

// -----------------------------------------------------------------------------
void    epsolarSetDefaultBaudRate (const int newRate)
{
    defaultBaudRate = newRate;
}

// -----------------------------------------------------------------------------
const   char    epsolarGetDefaultParity (void)
{ 
    return defaultParity;
}

// -----------------------------------------------------------------------------
void    epsolarSetDefaultParity (const char newParity)
{ 
    defaultParity = newParity;
}

// -----------------------------------------------------------------------------
const   int epsolarGetDataBits (void)
{
    return defaultDataBits;
}

// -----------------------------------------------------------------------------
void epsolarSetDefaultDataBits (const int newBits)
{
    defaultDataBits = newBits;
}

// -----------------------------------------------------------------------------
const   int epsolarGetStopBits (void)
{
    return defaultStopBits;
}

// -----------------------------------------------------------------------------
void    epsolarSetDefaultStopBits (const int newBits)
{
    defaultStopBits = newBits;
}

// -----------------------------------------------------------------------------
void    epsolarGetRealTimeData (epsolarRealTimeData_t *rtData)
{
    //
    //  NB: The PVArray, Charging and Controller Status are all crammed into
    //      register 0x3201
    //
    uint16_t  chargingEquipmentStatusBits = eps_getChargingEquipmentStatusBits();
    
    rtData->pvPower     = eps_getPVArrayInputPower();
    rtData->pvCurrent   = eps_getPVArrayInputCurrent();
    rtData->pvVoltage   = eps_getPVArrayInputVoltage();
    rtData->pvStatus    = getChargingEquipmentStatusInputVoltageStatus( chargingEquipmentStatusBits );
    
    //
    uint16_t     batteryStatusBits = eps_getBatteryStatusBits();
    rtData->batteryVoltage  = eps_getBatteryVoltage();
    rtData->batteryCurrent   = eps_getBatteryCurrent();
    rtData->batteryStateOfCharge = eps_getBatteryStateOfCharge();
    rtData->batteryTemperature = eps_getBatteryTemperature();
    rtData->batteryStatus = eps_getBatteryStatusVoltage( batteryStatusBits );
    rtData->batteryMaxVoltage = eps_getMinimumBatteryVoltageToday();
    rtData->batteryMinVoltage = eps_getMaximumBatteryVoltageToday();
    rtData->batteryChargingStatus = eps_getChargingStatus( chargingEquipmentStatusBits );       // Not BatteryStatusBits!
    
    //
    uint16_t dischargingStatusBits = eps_getdisChargingEquipmentStatusBits();
    rtData->loadVoltage = eps_getLoadVoltage();
    rtData->loadCurrent = eps_getLoadCurrent();
    rtData->loadPower = eps_getLoadPower();
    rtData->loadLevel = getDischargingStatusOutputPower( dischargingStatusBits );
    rtData->loadIsOn = (eps_isdischargeStatusRunning( dischargingStatusBits ) ? TRUE : FALSE );
    rtData->loadControlMode = (char *) getLoadControlMode();
    
    rtData->controllerTemp = eps_getDeviceTemperature();
    rtData->chargerStatusNormal = isChargingStatusNormal( chargingEquipmentStatusBits );
    rtData->chargerRunning = isChargingStatusRunning( chargingEquipmentStatusBits );

    rtData ->controllerStatusBits = chargingEquipmentStatusBits;
    
    rtData->isNightTime = eps_isNightTime();
    eps_getRealtimeClockStr(&(rtData->controllerClock[ 0 ]), sizeof( rtData->controllerClock ) );

    rtData->energyConsumedToday = eps_getConsumedEnergyToday();
    rtData->energyConsumedMonth = eps_getConsumedEnergyMonth();
    rtData->energyConsumedYear = eps_getConsumedEnergyYear();
    rtData->energyConsumedTotal = eps_getConsumedEnergyTotal();
    rtData->energyGeneratedToday = eps_getGeneratedEnergyToday();
    rtData->energyGeneratedMonth = eps_getGeneratedEnergyMonth();
    rtData->energyGeneratedYear = eps_getGeneratedEnergyYear();
    rtData->energyGeneratedTotal = eps_getGeneratedEnergyTotal();   
}


// -----------------------------------------------------------------------------
static
const char  *getControllerStatus (const uint16_t chargingEquipmentStatusBits)
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
     * D0: 1 Running, 0 Standby
     */
    Logger_LogDebug( "getControllerStatus - chargingStatusBits [%0X]\n", chargingEquipmentStatusBits );
    
    //                                  fedcba9876543210
    if (chargingEquipmentStatusBits & 0b0000000000000000)  return "Normal-Standby";
    if (chargingEquipmentStatusBits & 0b0000000000000001)  return "Normal-Running";
    if (chargingEquipmentStatusBits & 0b0000000000000010)  return "Fault-Standby";
    if (chargingEquipmentStatusBits & 0b0000000000000011)  return "Fault-Running";
    
    return "???";
}


// -----------------------------------------------------------------------------
static
const char  *getLoadControlMode ()
{
    uint16_t    lcm = eps_getLoadControllingMode();
    
    Logger_LogDebug( "getLoadControlMode - lcmBits [%0X]\n", lcm );
    
    if (lcm == 0x00)    return "Manual";
    if (lcm == 0x01)    return "Dusk-Dawn";
    if (lcm == 0x02)    return "Dusk-Timer";
    if (lcm == 0x03)    return "Timer";
                
    return "???";            
}