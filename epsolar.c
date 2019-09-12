/*
 */
#include <stdio.h>
#include <errno.h>
#include "log4c.h"

#include "libepsolar.h"

static char         *version = "libepsolar v1.0.0a";


static  char    *defaultPortName = "/dev/ttyXRUSB0";
static  int     defaultBaudRate = 115200;
static  char    defaultParity = 'N';
static  int     defaultDataBits = 8;
static  int     defaultStopBits = 1;

static  modbus_t    *ctx = NULL;



// -----------------------------------------------------------------------------
char    *epsolarGetVersion (void)
{
    return version;
}


// -----------------------------------------------------------------------------
int epsolarModbusConnect (const char *portName, const int slaveNumber)
{
    //
    // Use the passed in port name, unless it's null.
    if (portName != NULL)
        defaultPortName = (char *) portName;
    
    //
    // Modbus - open the SCC port
    Logger_LogInfo( "Opening %s, %d %d%c%d\n", 
            defaultPortName, defaultBaudRate,
            defaultDataBits, defaultParity, defaultStopBits );
    
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