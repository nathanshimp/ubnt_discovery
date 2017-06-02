#!/bin/bash

TCPDUMP=$(which tcpdump)

if [ -n $TCPDUMP ]; then
	$TCPDUMP udp and port 10001
else
	echo "tcpdump command could not be found."
fi
