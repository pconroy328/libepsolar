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

extern  int         epsolarModbusConnect( const char *portName, const int slaveNumber );
extern  int         epsolarModbusDisconnect( void );
extern  modbus_t    *epsolarModbusGetContext( void );

#if 0
#define     eps_getBatteryTemperature()               getBatteryTemperature( epsolarModbusGetContext() )
#define     eps_getBatteryRealRatedVoltage()          getBatteryRealRatedVoltage( epsolarModbusGetContext() )
#define     eps_getRatedLoadCurrent()                 getRatedLoadCurrent( epsolarModbusGetContext() )
#define     eps_getRatedChargingCurrent()             getRatedChargingCurrent( epsolarModbusGetContext() )
#endif 

#ifdef __cplusplus
}
#endif

#endif /* LIBEPSOLAR_H */

