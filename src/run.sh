#!/bin/bash

${NDDSHOME}/resource/scripts/rtisetenv_*.bash
rtiroutingservice -cfgFile ./RsTelemetryGateway.xml -cfgName "OpenTelemetryGateway"
