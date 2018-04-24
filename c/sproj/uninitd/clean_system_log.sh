#!/bin/bash
if [ $# -eq 1 ]
then
	sed -i '2,100000d' $1
else
	echo "para is not 2"
fi
sync

