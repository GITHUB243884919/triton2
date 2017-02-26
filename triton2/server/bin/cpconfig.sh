#!/bin/sh

cp template.dat                                    ../gameserver/config/template.dat
cp ef.txt                                          ../gameserver/config/ef.txt
cp data/task_data/task_client_interface.lua        ../gameserver/config/data/task_data/task_client_interface.lua
cp data/task_data/server_base_data.lua             ../gameserver/config/data/task_data/server_base_data.lua
cp data/task_data/tasks/*.lua                      ../gameserver/config/data/task_data/tasks/
cp data/task_data/task_interface.lua               ../gameserver/config/data/task_data/task_interface.lua
cp data/task_data/task_global.lua                  ../gameserver/config/data/task_data/task_global.lua
cp data/task_data/task_func.lua                    ../gameserver/config/data/task_data/task_func.lua
cp data/task_data/task_comm.lua                    ../gameserver/config/data/task_data/task_comm.lua
cp data/policy/Strategy.xml                        ../gameserver/config/data/policy/Strategy.xml
cp data/scenetpl/worldscene.xml                    ../gameserver/config/data/scenetpl/worldscene.xml
cp data/scenetpl/龙城国都.dat                      ../gameserver/config/data/scenetpl/龙城国都.dat
cp data/scenetpl/梅家村.dat                        ../gameserver/config/data/scenetpl/梅家村.dat
cp data/scenetpl/新手村.dat                        ../gameserver/config/data/scenetpl/新手村.dat
cp data/scenetpl/黄风岭.dat			   ../gameserver/config/data/scenetpl/黄风岭.dat
cp cf.txt                                          ../gameserver/config/cf.txt
cp sceneconfig.xml       						   ../gateserver/config/sceneconfig.xml
cp data/scenetpl/worldscene.xml        				../gateserver/config/data/scenetpl/worldscene.xml

