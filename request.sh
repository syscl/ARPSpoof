#!/bin/sh
#
# (c) 2018 Yating Zhou (aka syscl)
#
# The script expects '0.5' but non-US localizations use '0,5' so we export
# LC_NUMERIC here (for the duration of the deploy.sh) to prevent errors.
#
export LC_NUMERIC="en_US.UTF-8"
export MG_DEBUG=0
#
# Prevent non-printable/control characters.
#
unset GREP_OPTIONS
unset GREP_COLORS
unset GREP_COLOR
# match against ethernet interface
gEthIF=$(ifconfig | grep -o 'h[[:digit:]]*-eth[[:digit:]]')
# echo "My ethernet interface is: ${gEthIF}"
ip addr flush dev ${gEthIF}
dhclient ${gEthIF}

exit 0
