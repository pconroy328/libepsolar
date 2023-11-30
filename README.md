# libepsolar
C Library for EPSolar Solar Charge Controllers

The RS485 Modbus documentation for the TRIRON 3210N family of MPPT Solar Charge Controllers turned
into a library of useful calls.  Some of the bitbanging into the SCC is pretty obscure. Hope this helps.

For example, to get the battery voltage:

  if (!epsolarModbusConnect( devPort, 1 )) {
     Logger_LogFatal( "Unable to open %s to connect to the solar charge controller\n", devPort );
     return( EXIT_FAILURE );
  }

  Logger_LogInfo( "Setting controller clock to 'now' and Load Control to Manual and On!\n" );
  eps_setRealtimeClockToNow();
  eps_setLoadControllingMode( 0 );
  eps_forceLoadOn();
  
  float   batteryVoltage = eps_getBatteryVoltage();
  Logger_LogWarning( "Battery Voltage: %f\n", batteryVoltage );
  
  // .. snipped copde
  
  Logger_LogInfo( "Setting Timer 1 ON to: %d:%d:%d\n", hour, tmPtr->tm_min, tmPtr->tm_sec );
  eps_setTurnOnTiming1( hour, tmPtr->tm_min, tmPtr->tm_sec );
            
  Logger_LogInfo( "Setting Timer 1 OFF to: %d:%d:%d\n", tmPtr->tm_hour, minute, tmPtr->tm_sec );
  eps_setTurnOffTiming1( tmPtr->tm_hour, minute, tmPtr->tm_sec );

  Logger_LogInfo( "Setting Load Control Mode to 0x03 - Timer Control\n" );
  eps_setLoadControllingMode( 3 );
    
  Logger_LogInfo( "Disconnecting from charge controller\n" );
  epsolarModbusDisconnect();
    
