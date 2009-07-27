#!/bin/bash

# test test test
for i in $(seq 1 100)
do
   echo 66.249.$i.13 - - [18/Sep/2004:11:07:48 +1000] "GET /robots.txt HTTP/1.0" 200 4$i "-" "Googlebot/2.1"  >> apache.log
   sleep 1
done
