#!/bin/bash
# Source the Poky Dunfell environment setup script
source /opt/poky/3.1.31/environment-setup-cortexa7t2hf-neon-vfpv4-poky-linux-gnueabi
# Call make with all passed arguments
make "$@"
