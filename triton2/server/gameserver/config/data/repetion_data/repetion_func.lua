function create_npc( map_id, repetion_id, args, loop_index )
	if args == nil then
		Error( "create_ncp func args is nil, mapid " .. map_id );
	end
	if args[ loop_index ] == nil then
		Error( "create_npc func args[ " .. loop_index .. " ] is nil, mapid " .. map_id );
	end
	Infor( "Create Npc( " .. args[ loop_index ] .. " )" );
	CreateNpc( repetion_id, args[ loop_index ] )
end

function create_npc_all( repetion_id, list )
	table.foreachi( list, function( i, v )
		CreateNpc( repetion_id, v );
	end);
end

function time_begin( map_id, repetion_id, args, loop_index )
	if args == nil then
		Error( "time_begin func args is nil, mapid " .. map_id );
	end
	
	if args[ loop_index ] == nil then
		Error( "time_begin func args[ " .. loop_index .. " ] is nil, mapid " .. map_id );
	end

	TimeBegin( repetion_id, args[ loop_index ] );
end

--
local data_
function add_intdata( repetion_id, index, data )
	data_ = GetIntData( repetion_id, index );
	data_ = data_ + data;
	SetIntData( repetion_id, index, data_ );
	return data_;
end

local stop_patrol = { };
function entity_stop_patrol( repetion_id, entity_id )
	NpcPatrol( repetion_id, entity_id, -1, stop_patrol );
end

function add_mapintdata( repetion_id, map_id, index, data )
	data_ = GetMapIntData( repetion_id, map_id, index );
	data_ = data_ + data;
	SetMapIntData( repetion_id, map_id, index, data_ );
	return data_;
end

function create_npc_real_map_list( repetion_id, real_map, list )
	table.foreachi( list, function( i, v )
		CreateNpcRealMap( repetion_id, real_map, v );
	end);	
end



