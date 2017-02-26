local shitu = {}
shitu.end_time = 1 * 60 * 1000 ; --副本关闭时间



--local function  close_reption( repetion_index, repetion_id, args, loop_index )
--    print("wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww")
--    RepetionClose( repetion_id )
--    print("wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww")
--end
repetion = 
{
    baseinfo = 
    {
        repetion_index = RepetionID.SHITU,
        playercountmax = 6,		-- 
        mapid = { 160 },
        taskid = 0,			-- into task condition 
        posx = 26,
        posy = 70,
        pernumber = 1,			-- everday into max number 
        --		weekpernumber = 2,
        minlevel = 20,
        maxlevel = 110,
        --check_time = 20000,		-- 检查副本人数时间(毫秒)
        question_check=1,		-- 是否进行副本答题验证

        special_check = 1,              --特殊检查,
        necessity = 75663,                   --进入副本的必须品
        team = 
        {
            teamin = true,		
            count = 2,			-- team redirect player number less
        },

    },

    timer = {

        {   
            kind = RepetionTimeBeginKind.START_BEGIN,
            result = RepetionTimeResultKind.CLOSE,
            loop = 1, 
            time = 1000 * 60 * 10,
	    sync = { kind = RepetionTimeSyncKind.ALL, key = 0 },
        },  

    }

--    event = 
--    {
--        [ RepetionEventKind.BEGIN_REPETION] = { func = repetion_begin},
--    },
}


