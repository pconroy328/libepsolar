/*
 */
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <modbus/modbus.h>

#include "log4c.h"
#include "libepsolar.h"


static char         *version = "libepsolar v1.5 - strerror";


//static  char    *defaultPortName = "/dev/ttyXRUSB0";
static  char    *defaultPortName = "/dev/ttyACM1";
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
        Logger_LogFatal( "Unable to create the libmodbus context [%s]\n", modbus_strerror( errno ) );
        return FALSE;
    }
    
    Logger_LogInfo( "Setting slave ID to 0x%X\n", slaveNumber );
    modbus_set_slave( ctx, slaveNumber );

    if (modbus_connect( ctx ) == -1) {
        Logger_LogFatal( "Connection failed: %s\n", modbus_strerror( errno ) );
        modbus_free( ctx );
        return FALSE;
    }
    
    Logger_LogInfo( "Port to Solar Charge Controller is open.\n", defaultPortName );

#ifdef RPI
    
    uint32_t to_sec;
    uint32_t to_usec;
    
    modbus_get_byte_timeout( ctx, &to_sec, &to_usec );
    Logger_LogWarning( "Modbus get_byte timeout value is %ld secs, %ld usecs\n", (long) to_sec, (long) to_usec );

    modbus_get_response_timeout( ctx, &to_sec, &to_usec );
    Logger_LogWarning( "Modbus get_response timeout value is %ld secs, %ld usecs\n", (long) to_sec, (long) to_usec );
#else

    //
    // I'm getting the occasional error and timeout - let's add some Modbus debugging calls

    struct  timeval timeout;
    errno = 0;
    modbus_get_byte_timeout( ctx, &timeout );
    if (errno != 0)
        Logger_LogWarning( "Modbus get_byte timeout error [%s]\n", modbus_strerror( errno ) );
    else 
        Logger_LogWarning( "Modbus get_byte timeout value is %ld secs, %ld usecs\n", (long) timeout.tv_sec, (long) timeout.tv_usec );

    errno = 0;
    modbus_get_response_timeout( ctx, &timeout );
    if (errno != 0)
        Logger_LogWarning( "Modbus modbus_get_response_timeout error [%s]\n", modbus_strerror( errno ) );
    else 
        Logger_LogWarning( "Modbus get_response timeout value is %ld secs, %ld usecs\n", (long) timeout.tv_sec, (long) timeout.tv_usec );
#endif
    
    return TRUE;
}

// -----------------------------------------------------------------------------
char    *findController (const char *deviceNameBase, int maxDevNum, const int leaveOpen)
{
    int             done = FALSE;
    int             found = FALSE;
    char            devBuffer[ 40 ];
    
    //
    //  With the CH341 adapter, the device appears on /dev/ttyACMx
    //  We're going to see if we can find it.

    //
    //  Sanity check some of the input values
    if (maxDevNum > 255)
        maxDevNum = 255;

    //
    //  loop thru making device names, try to see if there's a WS2300 there
    int devNum = 0;
    while (!done) {
        //  make a string like "/dev/ttyACM0"
        snprintf( devBuffer, sizeof devBuffer, "%s%d", deviceNameBase, devNum );

        Logger_LogDebug( "findController - trying to open [%s]\n", devBuffer );
        if (epsolarModbusConnect( devBuffer, 1 )) {
            Logger_LogDebug( "findController - port opened, seeing if it's a controller\n" );

            //
            // module level var 'ctx' will be set for us by Connect()
            
            // See if we can read the controller clock
            int seconds = -99;              // Set them to something obviously impossible
            int minutes = -99;
            int hours = -99;
            int day = -99;
            int month = -99;
            int year = -99;
            
            getRealtimeClock( ctx, &seconds, &minutes, &hours, &day, &month, &year );
            
            if ( (seconds >= 0 && seconds < 60) &&
                 (minutes >= 0 && minutes < 60) &&
                 (hours >= 0 && hours < 24)) {
                Logger_LogInfo( "findController - found a controller on device [%s]\n", devBuffer );
                found = TRUE;
                done = TRUE;
                
                //
                // Connect sets the static var 'defaultPortName' for us
                if (!leaveOpen) {
                    Logger_LogInfo( "findController - leave open param is false, so closing device [%s]\n", defaultPortName );
                    epsolarModbusDisconnect();
                }
            } else {
                Logger_LogInfo( "findController - set clock call on device [%s] failed\n", devBuffer );
                devNum += 1;
            }
        }   // ConnectCall failed - keep going
        
        done = (found || (devNum > maxDevNum));
        if (!done)
            sleep( 2 );
    }
    
    if (found)
        Logger_LogInfo( "findController - found a controller on port [%s]\n", defaultPortName );
    else {
        Logger_LogWarning( "findController - could NOT find a controller on port with a base of [%s]\n", deviceNameBase );
        defaultPortName = NULL;
    }
    
    return defaultPortName;
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
    memset( rtData, '\0', sizeof( epsolarRealTimeData_t ) );
    
    if (epsolarModbusGetContext() == NULL) {
        Logger_LogError( "Modbus Context is Zero - did you forget to connect?\n" );
        return;
    }
        
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
    rtData->batteryMinVoltage = eps_getMinimumBatteryVoltageToday();
    rtData->batteryMaxVoltage = eps_getMaximumBatteryVoltageToday();
    rtData->batteryChargingStatus = eps_getChargingStatus( chargingEquipmentStatusBits );       // Not BatteryStatusBits!
    
    //
    uint16_t dischargingStatusBits = eps_getDischargingEquipmentStatusBits();
    rtData->loadVoltage = eps_getLoadVoltage();
    rtData->loadCurrent = eps_getLoadCurrent();
    rtData->loadPower = eps_getLoadPower();
    rtData->loadLevel = getDischargingStatusOutputPower( dischargingStatusBits );
    rtData->loadIsOn = (eps_isDischargeStatusRunning( dischargingStatusBits ) ? TRUE : FALSE );
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
    if (epsolarModbusGetContext() == NULL) {
        Logger_LogError( "Modbus Context is Zero - did you forget to connect?\n" );
        return "NOT CONNECTED";
    }
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
    if (epsolarModbusGetContext() == NULL) {
        Logger_LogError( "Modbus Context is Zero - did you forget to connect?\n" );
        return "NOT CONNECTED";
    }

    uint16_t    lcm = eps_getLoadControllingMode();
    
    Logger_LogDebug( "getLoadControlMode - lcmBits [%0X]\n", lcm );
    
    if (lcm == 0x00)    return "Manual";
    if (lcm == 0x01)    return "Dusk-Dawn";
    if (lcm == 0x02)    return "Dusk-Timer";
    if (lcm == 0x03)    return "Timer";
                
    return "???";            
}