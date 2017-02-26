#!/bin/bash
if [ $# != 6 ]; then
	echo "tomorrow cannt get enough parame"
	exit;
fi;
DAY=`date -d "tomorrow" "+%Y%m%d"`

echo "Create Tommorrow LogDB Begin"
$1 $2 $3 $4 $5 $6 $DAY
echo "Create Tomorrow LogDB End"

exit;
#DAY=20091206
