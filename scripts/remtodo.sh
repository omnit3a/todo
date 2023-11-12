#!/bin/sh

if [ ! -e $1 ]
then
    echo "$1 doesn't exist"
    exit 1
fi

echo "Left to complete"
cat $1 | grep '^\[ \]*'
