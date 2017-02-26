repetion = 
{
	baseinfo = 
	{
		repetion_index = RepetionID.SHITUOLING,
		playercountmax = 6,		-- 
		mapid = { 16 },
		taskid = 0,			-- into task condition 
		posx = 26,
		posy = 70,
	--	teamin = true,
		pernumber = 0,			-- everday into max number 
		minlevel = 10,
		maxlevel = 120,

		team = 
		{
			teamin = false,		
			count = 1,			-- team redirect player number less
		},
		

		backpos = 
		{
			mapid = 1,
			posx = 33,
			posy = 13
		}
	},

	timer = 
	{ 
		{
			kind = RepetionTimeBeginKind.START_BEGIN,
			result = RepetionTimeResultKind.CLOSE,
			time = 1000 * 60 * 5
		},
	},

}


