#!/bin/bash

sudo gnome-terminal -- bash -c './a.out 10.0.0.2 10.0.0.3 64' &
sudo vnstat -l -i enp0s3
