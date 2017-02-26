. ~/.bash_profile

echo "---------------------------------"
echo "--    开始更新Alpha分支     -----"
echo "---------------------------------"

export TRITON_HOME=~/branch_1008_alpha

BUILD_LOG_PATH=~/build/log
touch $BUILD_LOG_PATH/dataserver.log
touch $BUILD_LOG_PATH/dataserver.err
touch $BUILD_LOG_PATH/loginserver.log
touch $BUILD_LOG_PATH/loginserver.err
touch $BUILD_LOG_PATH/gateserver.log
touch $BUILD_LOG_PATH/gateserver.err
touch $BUILD_LOG_PATH/gameserver.log
touch $BUILD_LOG_PATH/gameserver.err
touch $BUILD_LOG_PATH/dungeongameserver.log
touch $BUILD_LOG_PATH/dungeongameserver.err
touch $BUILD_LOG_PATH/tcpserver.log
touch $BUILD_LOG_PATH/tcpserver.err
touch $BUILD_LOG_PATH/proxyserver.log
touch $BUILD_LOG_PATH/proxyserver.err
touch $BUILD_LOG_PATH/err.log



cd $TRITON_HOME/common
svn up

cd $TRITON_HOME/server/gameserver
svn up

cd $TRITON_HOME/server/dungeongameserver
svn up

cd $TRITON_HOME/server/servercommon
svn up

cd $TRITON_HOME/server/logic
svn up 

cd $TRITON_HOME/server/gateserver
svn up 

cd $TRITON_HOME/server/loginserver
svn up

cd $TRITON_HOME/server/logserver
svn up 

cd $TRITON_HOME/server/dbserver
svn up

echo "---------------------------------"
echo "-- 开始编译Alpha分支 Release版 --"
echo "---------------------------------"

cd ../dbserver
cp ./bin/dbsvrd ./bin/dbsvrd.bak.`date +%Y%m%d_%T`
make clean
make  1>$BUILD_LOG_PATH/dataserver.log 2>$BUILD_LOG_PATH/dataserver.err &

cd ../loginserver
cp ./bin/loginserver ./bin/loginserver.bak.`date +%Y%m%d_%T`
make clean
make  1>$BUILD_LOG_PATH/loginserver.log 2>$BUILD_LOG_PATH/loginserver.err &

cd ../gateserver
cp ./bin/gateserver ./bin/gateserver.bak.`date +%Y%m%d_%T`
make clean
make  1>$BUILD_LOG_PATH/gateserver.log 2>$BUILD_LOG_PATH/gateserver.err &

cd ../gameserver
cp ./bin/sceneserver ./bin/sceneserver.bak.`date +%Y%m%d_%T`
make clean
make  1>$BUILD_LOG_PATH/gameserver.log 2>$BUILD_LOG_PATH/gameserver.err &

cd ../dungeongameserver
cp ./bin/sceneserver ./bin/sceneserver.bak.`date +%Y%m%d_%T`
make clean
make  1>$BUILD_LOG_PATH/dungeongameserver.log 2>$BUILD_LOG_PATH/dungeongameserver.err 



grep "错误" $BUILD_LOG_PATH/*.err > $BUILD_LOG_PATH/../err.log

cd  ~/build/

