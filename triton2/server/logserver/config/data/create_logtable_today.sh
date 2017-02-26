#!/bin/bash
if [ $# != 6 ]; then
	echo "today cannt get enough parame"
	exit;
fi;


DAY=`date "+%Y%m%d"`
#YESTERDAY=`date -d "yesterday"  "+%Y%m%d"`

echo "Create Today LogDB Begin"
$1 $2 $3 $4 $5 $6 $DAY
echo "Create Today LogDB End"

#echo

#echo "Create Index On $YESTERDAY LogDB Begin"
#chmod +x ../config/data/create_logindex.sh
#../config/data/create_logindex.sh $2 $3 $4 $5 $6 $YESTERDAY 
#echo "Create Index On $YESTERDAY LogDB End"

exit;
