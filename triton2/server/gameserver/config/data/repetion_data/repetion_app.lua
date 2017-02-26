local RepetionInfo = { };
local TimeSyncDefault = { camp = 0, kind = RepetionTimeSyncKind.NONE, key = RepetionTimeSyncKey.CLOSE };

--dofile( "../config/data/repetion_data/repetions/pantao/pantao_class.lua" );
--dofile( "../config/data/repetion_data/repetions/longgong/longgong_class.lua" );

function init_repetion( vDir )
	local tDir = CDir( vDir .. "/class/" );
	local tFile = tDir:file( );
	while tFile ~= nil do
		dofile( vDir .. "/class/" .. tFile );
		tFile = tDir:file( );
	end

	tDir = CDir( vDir .. "/repetions/" );
	tFile = tDir:file( );
	math.randomseed( os.time( ) );
	while tFile  ~= nil do
		Infor( "Load File: " .. vDir .. "/repetions/" .. tFile .. "Begin" );
		if not dofile( vDir .."/repetions/" .. tFile ) then
			if repetion.baseinfo ~= nil then
				local load_new = true;
				if RepetionInfo[ repetion.baseinfo.repetion_index ] == nil then
					RepetionInfo[ repetion.baseinfo.repetion_index ] = repetion;
					RepetionInfo[ repetion.baseinfo.repetion_index ].filename = tFile;
					Infor( "Add RepetionIndex " .. RepetionInfo[ repetion.baseinfo.repetion_index ].baseinfo.repetion_index .. " Arrary Index Is " .. repetion.baseinfo.repetion_index .. " Begin Regist" );

					RegisterRepetion( RepetionInfo[ repetion.baseinfo.repetion_index ].baseinfo );
				else
					Error( "File " .. tFile .. " and File " .. 
						RepetionInfo[ repetion.baseinfo.repetion_index ].filename .. " RepetionIndex is Same" );
					load_new = false;
				end

				if load_new == true then
					if repetion.baseinfo.commtakemap ~= nil then
						table.foreachi( repetion.baseinfo.commtakemap, function( i, v ) 
							RegisterCommTakeMap( repetion.baseinfo.repetion_index, v );
						end);
					end

					if repetion.timer ~= nil then
						table.foreachi( repetion.timer, function( i, v )
--							RegisteTimeEvent( repetion.baseinfo.repetion_index, i, v.kind, v.result, v.time, v.loop, 
--								v.func ~= nil, 
							RegisteTimeEvent( repetion.baseinfo.repetion_index, i, v, v.func ~= nil );
--								function( v ) if v.sync ~= nil then return v.sync else return TimeSyncDefault end end );
						end );
					end

					if repetion.baseinfo.open ~= nil then
						table.foreachi( repetion.baseinfo.open, function( i, v )
							RegisteOpenTime( repetion.baseinfo.repetion_index, v );
						end );
					end
                                        --add for signup 
                                        if repetion.baseinfo.signup_open ~= nil then
                                            table.foreachi( repetion.baseinfo.signup_open, function( i, v )
                                                RegisteSignUpTime( repetion.baseinfo.repetion_index, v );
                                            end );
                                        end

					
					if repetion.event ~= nil then
						table.foreach( repetion.event, function( i, v )
							if v.func ~= nil then
								Debug( "Load Event" );
								RegisteEvent( repetion.baseinfo.repetion_index, i );
							else
								Debug( "Load Event Err" .. i );
								end
						end );
					end

                    if repetion.battle ~= nil then
                        if repetion.battle.camp ~= nil then
                            table.foreachi( repetion.battle.camp, function( i, v )
                                RegisterBattleCamp( repetion.baseinfo.repetion_index, v );
                            end);
                        end
                        if repetion.battle.level ~= nil then
                            table.foreachi( repetion.battle.level, function( i, v )
                                RegisterBattleWaitLevel( repetion.baseinfo.repetion_index, v );
                            end);
                        end
                        if repetion.battle.open ~= nil then
                            RegisteBattleOpenSpace( repetion.baseinfo.repetion_index, repetion.battle.open );
                        end
					end
				end
			else
				Error( "RepetionMap Info File( " .. vDir .. "/repetions/" .. tFile .. " ) Fail" );
			end
		end
		Infor( "Load File: " .. vDir .. "/repetions/" .. tFile .. "End" );
		tFile = tDir:file( );
	end

--	Infor( "Begin Register Reptions Info, Repetions Count Is " .. table.getn( RepetionInfo ) );
--	if RepetionInfo ~= nil and table.getn( RepetionInfo ) > 0 then
--		for i = 1, table.getn( RepetionInfo ) do
--			if RepetionInfo[ i ].baseinfo ~= nil then
--				RegisterRepetion( RepetionInfo[ i ].baseinfo );
--			end
--		end
--	end
	Infor( "End Register Repetion Info" );
end


function do_event( vRepetionIndex, vRepetionID, vEventType, vParame )--, vParame1, vParame2 )
	if RepetionInfo[ vRepetionIndex ] ~= nil then
		if RepetionInfo[ vRepetionIndex ].event ~= nil then
			if RepetionInfo[ vRepetionIndex ].event[ vEventType ] ~= nil then
				if RepetionInfo[ vRepetionIndex ].event[ vEventType ].func ~= nil then
					RepetionInfo[ vRepetionIndex ].event[ vEventType ].func( vRepetionID, vParame );
				else
					Error( "Repetion " .. vRepetionIndex .. " DoEvent " .. vEntitType .. " Func Is Nil" );
				end
			else
				Error( "Repetion " .. vRepetionIndex .. " DoEvent " .. vEntitType .. " Is Nil" );
			end
		end
	end
end

function time_event( vRepetionIndex, vRepetionID, vTimeIndex, vLoopIndex )
	local t;

	if RepetionInfo[ vRepetionIndex ] == nil then
		Error( "RepetionMap[ " .. vRepetionIndex .. " ] Is Nil" );
		return;
	end

	if RepetionInfo[ vRepetionIndex ].timer == nil  then
		Error( "RepetionInfo[ " .. vRepetionIndex .. " ] Time Is Nil" );
		return;
	end

	if RepetionInfo[ vRepetionIndex ].timer[ vTimeIndex ] == nil then
		Error( "RepetionInfo[ " .. vRepetionIndex .. " ] Time[ " .. vTimeIndex .. " ] Is Nil" );
		return;
	end

	t = RepetionInfo[ vRepetionIndex ].timer[ vTimeIndex ];

	if t.func == nil then
		Error( "RepetionMap[ " .. vRepetionIndex .." ] Time[ " .. vTimeIndex .. " ] Func Is Nil" );
		return;
	end

	t.func( vRepetionIndex, vRepetionID, t.args, vLoopIndex );
end

local insert_parame = { };
insert_parame.reason = 0;
function insert_item( repetion_id, entityid, itemid, count, reason )
	insert_parame.reason = reason ;
	InsertItem( repetion_id, entityid, itemid, count, insert_parame );
end

function insert_item_by_camp( repetion_id, campid, itemid, count, reason )
	insert_parame.reason = reason ;
	InsertItemByCamp( repetion_id, campid, itemid, count, insert_parame );
end

function insert_item_all( repetion_id, itemid, count, reason )
	insert_parame.reason = reason ;
	InsertItemAll( repetion_id, itemid, count, insert_parame );
end


function get_random_place (repetion_index)
    rand_group= math.random(1,table.getn( RepetionInitPost[ repetion_index] )); 
    rand_post= math.random(1,table.getn( RepetionInitPost[ repetion_index][ rand_group] )); 


    return RepetionInitPost[ repetion_index ][ rand_group ][ rand_post ];
end


function get_tele_charge( repetion_index, entityid )
        
        post_charge = {} 

        if repetion_index == 26 then
            count = GetPlayerDieCount( repetion_index, entityid )
            temp = 9 - count
            if temp < 0 then
                temp = 0
            end

         -- post_charge.charge = ( 0.7^temp )* 0.7
            post_charge.charge = 0 
            post_charge.mapid = RepetionTeleCharge[ repetion_index ].mapid;
            rand_post = math.random(1 , table.getn( RepetionTeleCharge[ repetion_index ].post)); 
            post_charge.post = RepetionTeleCharge[ repetion_index ].post[ rand_post ]
            post_charge.dietimes = count
        end
        
        return post_charge;
end
    
