#!/bin/bash

# based off the following two scripts
# http://www.theunsupported.com/2012/07/block-malicious-ip-addresses/
# http://www.cyberciti.biz/tips/block-spamming-scanning-with-iptables.html

# list of known spammers
URL="www.spamhaus.org/drop/drop.lasso";

# save local copy here
FILE="./drop.lasso";

# get a copy of the spam list
wget -qc $URL -O $FILE

# iterate through all known spamming hosts
for IP in $( cat $FILE | egrep -v '^;' | awk '{ print $1}' ); do

    echo $IP

done

echo "Done!"

# remove the spam list
unlink $FILE
