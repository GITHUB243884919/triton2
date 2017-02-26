color=
{
	WHITE = 1,
	BLUE    = 2,
	PURPLE = 3,
	GOLD    = 4,
};
function get_refreshed_type( task_id )
	local tab = {};
	if task_refresh_awards[task_id] ~= nil then
		tab = {
			task_refresh_awards[task_id][1].prob, 
			task_refresh_awards[task_id][2].prob, 
			task_refresh_awards[task_id][3].prob, 
			task_refresh_awards[task_id][4].prob, 
		};
	end 
	return get_rand_index(tab);
end
