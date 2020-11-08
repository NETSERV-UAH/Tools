#!/bin/bash

xterm -e "openssl speed" &
mpstat -P ALL 1 20
