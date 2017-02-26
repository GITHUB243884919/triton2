
MONTH_BEGIN=1
MONTH_END=12
DAY_BEGIN=1
DAY_END=31
PWD=$1

while [ $MONTH_BEGIN -le $MONTH_END ]
do

        if [ "$MONTH_BEGIN" -lt "10" ]
        then
                realmonth='0'$MONTH_BEGIN
        else
                realmonth=$MONTH_BEGIN
        fi


	DAY_BEGIN=1

	while [ $DAY_BEGIN -le $DAY_END ]
	do

        if [ "$DAY_BEGIN" -lt "10" ]
        then
                realday='0'$DAY_BEGIN
        else
                realday=$DAY_BEGIN
        fi

	./dump_logtable.sh  127.0.0.1 3306 xyj $PWD LogDB 2010${realmonth}${realday}
	echo "dump" 2010${realmonth}${realday}	

	let "DAY_BEGIN= $DAY_BEGIN + 1"
	done
	

        let "MONTH_BEGIN= $MONTH_BEGIN + 1"
done
