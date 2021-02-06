#!/bin/sh

for s in "110" "111" "112" "113" "60" "61" "62" "63" "86" "87" "88" "89"
do
 echo -n $s "="
 cat /sys/class/gpio/gpio$s/value
done
