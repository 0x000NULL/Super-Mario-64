#!/bin/bash

OBJDUMP="mips-linux-gnu-objdump -D -z -bbinary -mmips -EB"
OPTIONS="--start-address=$(($1)) --stop-address=$(($2))"
$OBJDUMP $OPTIONS baserom.u.z64 > baserom.u.dump
$OBJDUMP $OPTIONS build/us/sm64.u.z64 > sm64.u.dump
diff baserom.u.dump sm64.u.dump | colordiff

