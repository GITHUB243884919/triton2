#!/usr/bin/ksh

if `test  $# -ne 4`
then
	echo "wrong param"
	exit
fi

exec >table
/usr/local/mysql/bin/mysql -u$3 -p$4 << !
	use LogDB
	show tables;
!


exec > tablelist.sql
for tablename in `grep -e '[0-9]_.' table`
do
	date=$( echo $tablename | cut -c1-8 )
	if $( test "$date" -ge $1 -a "$date" -le $2 )
	then
		echo "drop table if exists $tablename;"
	fi
done

/usr/local/mysql/bin/mysql -u$3 -p$4 LogDB < tablelist.sql

