
repetion = 
{
	baseinfo = 
	{
		repetion_index = RepetionID.CHANGBAOTU,
		playercountmax = 6,		-- 
		mapid = { 52 },
		taskid = 0,			-- into task condition 
		posx = 25,
		posy = 25,
	--	teamin = true,
		pernumber = 0,			-- everday into max number 
		minlevel = 1,
		maxlevel = 110,
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


	ogre_change_tmpid = 
	{
		{	
			levelmin = 1, 
			levelmax = 10, 
			ogre = 
			{ 
				[ 32082 ] = 34122;
			}
		},
		{
			levelmin = 11,
			levelmax = 20,
			ogre = 
			{
				[ 32082 ] = 34125;
			}
		}
	}
}


