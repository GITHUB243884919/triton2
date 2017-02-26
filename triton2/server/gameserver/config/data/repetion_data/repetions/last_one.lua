local last_one_single ={};
last_one_single.map_id = 165;
last_one_single.hell_map_id = 154;
last_one_single.create_hiero_time= 3*60*1000;
last_one_single.battle_begin_time = 1 * 60 * 1000;
last_one_single.battle_end_time = 20 * 60 * 1000;
last_one_single.close_time = 0.5 * 60 *1000;
last_one_single.close_time_left = 0.5 * 60 *1000;
last_one_single.end_time = 22 * 60 * 1000;
last_one_single.creat_readstone_time = 2 * 60 *1000;
last_one_single.hierogram_num = 20;
last_one_single.player_num_limit = 2; 
last_one_single.needfire_id = 71387; 
last_one_single.creat_needfire_time =   5 * 1000;
last_one_single.needfire_begin_time = 50 * 1000;
--以战斗开始为参照物/或者其他时间点
last_one_single.rest_count_down = 12* 60 * 1000 + 10 *1000;  --a 阶段结束倒计时，休息段开始倒计时
--last_one_single.rest_begin= 12* 60 * 1000 + 30 * 1000;  --a 阶段结束 ，休息段开始
last_one_single.rest_begin= 20 * 1000;  --a 阶段结束 ，休息段开始
last_one_single.rest_count_down_1 = 12* 60 * 1000 + 40 * 1000;  --a 阶段结束 ，休息段结束倒计时
last_one_single.rest_count_down_2 = 12* 60 * 1000 + 50 * 1000;  --a 阶段结束 ，休息段结束倒计时
last_one_single.rest_count_down_3 = 12* 60 * 1000 + 55 * 1000;  --a 阶段结束 ，休息段结束倒计时
last_one_single.B_begin= 20 * 1000 ;  --B 阶段开始
last_one_single.B_use_time= last_one_single.battle_end_time - last_one_single.B_begin - last_one_single.rest_count_down_1; -- B阶段用时间 
last_one_single.change_face_time = 10 * 60 * 1000 ;  --
last_one_single.template_level_id= 75772;  -- 经验表的模板id
last_one_single.live_mark_update =  2 * 60 * 1000; 
last_one_single.medal_id = 76639 ; 

local update_player_live_data 

local MESSAGE_ID =
{
   ID_S2G_BATTLE_CLOSE = 0xAC14;
};

local LEVEL_QUOTIETY=
{
    { min_level = 50, max_level = 64,   quotiety_A = 1,   quotiety_B = 1.4,   },
    { min_level = 65, max_level = 79,   quotiety_A = 1.2, quotiety_B = 1.6, },
    { min_level = 80, max_level = 94,   quotiety_A = 1.4, quotiety_B = 1.8, },
    { min_level = 95, max_level = 99, quotiety_A = 1.6, quotiety_B = 2.0, },
    { min_level = 100, max_level = 120, quotiety_A = 1.8, quotiety_B = 2.2, },
    
};
local NOTICE_EVENTID =
{
    NOTICE_BATTLE_WINER = -69563,
};

local BODY_TEMPID =
{
   [ 1 ] = 72836,
   [ 2 ] = 72837,
   [ 3 ] = 72838,
   [ 4 ] = 72839,
   [ 5 ] = 72840,
   [ 6 ] = 72841,
   [ 7 ] = 72842,
   [ 8 ] = 72843,
};

local ENTITY_TYPE =
{
        ENTITYTYPE_GHOST = 0x2000,            -- 可穿越实体
};

local NEEDFIRE_POST =
{
    [1] =  {
        {x = 0 , y = 1 },
        {x = 0 , y = 6 },
        {x = 0 , y = 11 },
        {x = 0 , y = 16 },
        {x = 0 , y = 21 },
        {x = 0 , y = 26 },
        {x = 0 , y = 31 },
        {x = 0 , y = 36 },
        {x = 0 , y = 41 },
        {x = 0 , y = 46 },
        {x = 0 , y = 51 },
        {x = 0 , y = 56 },
        {x = 0 , y = 61 },
        {x = 0 , y = 66 },
        {x = 0 , y = 71 },
        {x = 0 , y = 76 },
        {x = 0 , y = 81 },
        {x = 0 , y = 86 },
        {x = 0 , y = 91 },
        {x = 0 , y = 96 },
        {x = 0 , y = 101 },  

        {x = 99 , y = 1 },
        {x = 99 , y = 6 },
        {x = 99 , y = 11 },
        {x = 99 , y = 16 },
        {x = 99 , y = 21 },
        {x = 99 , y = 26 },
        {x = 99 , y = 31 },
        {x = 99 , y = 36 },
        {x = 99 , y = 41 },
        {x = 99 , y = 46 },
        {x = 99 , y = 51 },
        {x = 99 , y = 56 },
        {x = 99 , y = 61 },
        {x = 99 , y = 66 },
        {x = 99 , y = 71 },
        {x = 99 , y = 76 },
        {x = 99 , y = 81 },
        {x = 99 , y = 86 },
        {x = 99 , y = 91 },
        {x = 99 , y = 96 },
        {x = 99 , y = 101 },
    },

    [2] = {
        {x = 3 , y = 1 },
        {x = 6 , y = 1 },
        {x = 9 , y = 1 },
        {x = 12 , y = 1 },
        {x = 15 , y = 1 },
        {x = 18 , y = 1 },
        {x = 21 , y = 1 },
        {x = 24 , y = 1 },
        {x = 27 , y = 1 },
        {x = 30 , y = 1 },
        {x = 33 , y = 1 },
        {x = 36 , y = 1 },
        {x = 39 , y = 1 },
        {x = 42 , y = 1 },
        {x = 45 , y = 1 },
        {x = 48 , y = 1 },
        {x = 51 , y = 1 },
        {x = 54 , y = 1 },
        {x = 57 , y = 1 },
        {x = 60 , y = 1 },
        {x = 63 , y = 1 },
        {x = 66 , y = 1 },
        {x = 69 , y = 1 },
        {x = 72 , y = 1 },
        {x = 75 , y = 1 },
        {x = 78 , y = 1 },
        {x = 81 , y = 1 },
        {x = 84 , y = 1 },
        {x = 87 , y = 1 },
        {x = 90 , y = 1 },
        {x = 93 , y = 1 },
        {x = 96 , y = 1 },	    
    },  

    [3] = {
        {x = 3 , y = 6 },
        {x = 3 , y = 11 },
        {x = 3 , y = 16 },
        {x = 3 , y = 21 },
        {x = 3 , y = 26 },
        {x = 3 , y = 31 },
        {x = 3 , y = 36 },
        {x = 3 , y = 41 },
        {x = 3 , y = 46 },
        {x = 3 , y = 51 },
        {x = 3 , y = 56 },
        {x = 3 , y = 61 },
        {x = 3 , y = 66 },
        {x = 3 , y = 71 },
        {x = 3 , y = 76 },
        {x = 3 , y = 81 },
        {x = 3 , y = 86 },
        {x = 3 , y = 91 },
        {x = 3 , y = 96 },
        {x = 3 , y = 101 },

        {x = 96 , y = 6 },
        {x = 96 , y = 11 },
        {x = 96 , y = 16 },
        {x = 96 , y = 21 },
        {x = 96 , y = 26 },
        {x = 96 , y = 31 },
        {x = 96 , y = 36 },
        {x = 96 , y = 41 },
        {x = 96 , y = 46 },
        {x = 96 , y = 51 },
        {x = 96 , y = 56 },
        {x = 96 , y = 61 },
        {x = 96 , y = 66 },
        {x = 96 , y = 71 },
        {x = 96 , y = 76 },
        {x = 96 , y = 81 },
        {x = 96 , y = 86 },
        {x = 96 , y = 91 },
        {x = 96 , y = 96 },
        {x = 96 , y = 101 },	               	    
    },  

    [4] = {
        {x = 6 , y = 6 },
        {x = 9 , y = 6 },
        {x = 12 , y = 6 },
        {x = 15 , y = 6 },
        {x = 18 , y = 6 },
        {x = 21 , y = 6 },
        {x = 24 , y = 6 },
        {x = 27 , y = 6 },
        {x = 30 , y = 6 },
        {x = 33 , y = 6 },
        {x = 36 , y = 6 },
        {x = 39 , y = 6 },
        {x = 42 , y = 6 },
        {x = 45 , y = 6 },
        {x = 48 , y = 6 },
        {x = 51 , y = 6 },
        {x = 54 , y = 6 },
        {x = 57 , y = 6 },
        {x = 60 , y = 6 },
        {x = 63 , y = 6 },
        {x = 66 , y = 6 },
        {x = 69 , y = 6 },
        {x = 72 , y = 6 },
        {x = 75 , y = 6 },
        {x = 78 , y = 6 },
        {x = 81 , y = 6 },
        {x = 84 , y = 6 },
        {x = 87 , y = 6 },
        {x = 90 , y = 6 }, 
        {x = 93 , y = 6 },

        {x = 6 , y = 101 },
        {x = 9 , y = 101 },
        {x = 12 , y = 101 },
        {x = 15 , y = 101 },
        {x = 18 , y = 101 },
        {x = 21 , y = 101 },
        {x = 24 , y = 101 },
        {x = 27 , y = 101 },
        {x = 30 , y = 101 },
        {x = 33 , y = 101 },
        {x = 36 , y = 101 },
        {x = 39 , y = 101 },
        {x = 42 , y = 101 },
        {x = 45 , y = 101 },
        {x = 48 , y = 101 },
        {x = 51 , y = 101 },
        {x = 54 , y = 101 },
        {x = 57 , y = 101 },
        {x = 60 , y = 101 },
        {x = 63 , y = 101 },
        {x = 66 , y = 101 },
        {x = 69 , y = 101 },
        {x = 72 , y = 101 },
        {x = 75 , y = 101 },
        {x = 78 , y = 101 },
        {x = 81 , y = 101 },
        {x = 84 , y = 101 },
        {x = 87 , y = 101 },
        {x = 90 , y = 101 },
        {x = 93 , y = 101 },	    
    },  

    [5] = {
        {x = 6 , y = 11 },
        {x = 6 , y = 16 },
        {x = 6 , y = 21 },
        {x = 6 , y = 26 },
        {x = 6 , y = 31 },
        {x = 6 , y = 36 },
        {x = 6 , y = 41 },
        {x = 6 , y = 46 },
        {x = 6 , y = 51 },
        {x = 6 , y = 56 },
        {x = 6 , y = 61 },
        {x = 6 , y = 66 },
        {x = 6 , y = 71 },
        {x = 6 , y = 76 },
        {x = 6 , y = 81 },
        {x = 6 , y = 86 },
        {x = 6 , y = 91 },
        {x = 6 , y = 96 },

        {x = 93 , y = 11 },
        {x = 93 , y = 16 },
        {x = 93 , y = 21 },
        {x = 93 , y = 26 },
        {x = 93 , y = 31 },
        {x = 93 , y = 36 },
        {x = 93 , y = 41 },
        {x = 93 , y = 46 },
        {x = 93 , y = 51 },
        {x = 93 , y = 56 },
        {x = 93 , y = 61 },
        {x = 93 , y = 66 },
        {x = 93 , y = 71 },
        {x = 93 , y = 76 },
        {x = 93 , y = 81 },
        {x = 93 , y = 86 },
        {x = 93 , y = 91 },
        {x = 93 , y = 96 }, 
    },  

    [6] = {
        {x = 9 , y = 11 },
        {x = 12 , y = 11 },
        {x = 15 , y = 11 },
        {x = 18 , y = 11 },
        {x = 21 , y = 11 },
        {x = 24 , y = 11 },
        {x = 27 , y = 11 },
        {x = 30 , y = 11 },
        {x = 33 , y = 11 },
        {x = 36 , y = 11 },
        {x = 39 , y = 11 },
        {x = 42 , y = 11 },
        {x = 45 , y = 11 },
        {x = 48 , y = 11 },
        {x = 51 , y = 11 },
        {x = 54 , y = 11 },
        {x = 57 , y = 11 },
        {x = 60 , y = 11 },
        {x = 63 , y = 11 },
        {x = 66 , y = 11 },
        {x = 69 , y = 11 },
        {x = 72 , y = 11 },
        {x = 75 , y = 11 },
        {x = 78 , y = 11 },
        {x = 81 , y = 11 },
        {x = 84 , y = 11 },
        {x = 87 , y = 11 },
        {x = 90 , y = 11 },         

        {x = 9 , y = 96 },
        {x = 12 , y = 96 },
        {x = 15 , y = 96 },
        {x = 18 , y = 96 },
        {x = 21 , y = 96 },
        {x = 24 , y = 96 },
        {x = 27 , y = 96 },
        {x = 30 , y = 96 },
        {x = 33 , y = 96 },
        {x = 36 , y = 96 },
        {x = 39 , y = 96 },
        {x = 42 , y = 96 },
        {x = 45 , y = 96 },
        {x = 48 , y = 96 },
        {x = 51 , y = 96 },
        {x = 54 , y = 96 },
        {x = 57 , y = 96 },
        {x = 60 , y = 96 },
        {x = 63 , y = 96 },
        {x = 66 , y = 96 },
        {x = 69 , y = 96 },
        {x = 72 , y = 96 },
        {x = 75 , y = 96 },
        {x = 78 , y = 96 },
        {x = 81 , y = 96 },
        {x = 84 , y = 96 },
        {x = 87 , y = 96 },
        {x = 90 , y = 96 },
    },  

    [7] = {
        {x = 9 , y = 16 },
        {x = 9 , y = 21 },
        {x = 9 , y = 26 },
        {x = 9 , y = 31 },
        {x = 9 , y = 36 },
        {x = 9 , y = 41 },
        {x = 9 , y = 46 },
        {x = 9 , y = 51 },
        {x = 9 , y = 56 },
        {x = 9 , y = 61 },
        {x = 9 , y = 66 },
        {x = 9 , y = 71 },
        {x = 9 , y = 76 },
        {x = 9 , y = 81 },
        {x = 9 , y = 86 },
        {x = 9 , y = 91 },

        {x = 90 , y = 16 },
        {x = 90 , y = 21 },
        {x = 90 , y = 26 },
        {x = 90 , y = 31 },
        {x = 90 , y = 36 },
        {x = 90 , y = 41 },
        {x = 90 , y = 46 },
        {x = 90 , y = 51 },
        {x = 90 , y = 56 },
        {x = 90 , y = 61 },
        {x = 90 , y = 66 },
        {x = 90 , y = 71 },
        {x = 90 , y = 76 },
        {x = 90 , y = 81 },
        {x = 90 , y = 86 },
        {x = 90 , y = 91 },	    
    }, 

    [8] = {
        {x = 12 , y = 16 },
        {x = 15 , y = 16 },
        {x = 18 , y = 16 },
        {x = 21 , y = 16 },
        {x = 24 , y = 16 },
        {x = 27 , y = 16 },
        {x = 30 , y = 16 },
        {x = 33 , y = 16 },
        {x = 36 , y = 16 },
        {x = 39 , y = 16 },
        {x = 42 , y = 16 },
        {x = 45 , y = 16 },
        {x = 48 , y = 16 },
        {x = 51 , y = 16 },
        {x = 54 , y = 16 },
        {x = 57 , y = 16 },
        {x = 60 , y = 16 },
        {x = 63 , y = 16 },
        {x = 66 , y = 16 },
        {x = 69 , y = 16 },
        {x = 72 , y = 16 },
        {x = 75 , y = 16 },
        {x = 78 , y = 16 },
        {x = 81 , y = 16 },
        {x = 84 , y = 16 },
        {x = 87 , y = 16 },

        {x = 12 , y = 91 },
        {x = 15 , y = 91 },
        {x = 18 , y = 91 },
        {x = 21 , y = 91 },
        {x = 24 , y = 91 },
        {x = 27 , y = 91 },
        {x = 30 , y = 91 },
        {x = 33 , y = 91 },
        {x = 36 , y = 91 },
        {x = 39 , y = 91 },
        {x = 42 , y = 91 },
        {x = 45 , y = 91 },
        {x = 48 , y = 91 },
        {x = 51 , y = 91 },
        {x = 54 , y = 91 },
        {x = 57 , y = 91 },
        {x = 60 , y = 91 },
        {x = 63 , y = 91 },
        {x = 66 , y = 91 },
        {x = 69 , y = 91 },
        {x = 72 , y = 91 },
        {x = 75 , y = 91 },
        {x = 78 , y = 91 },
        {x = 81 , y = 91 },
        {x = 84 , y = 91 },
        {x = 87 , y = 91 },

    },  
    [9] = {
        {x = 12 , y = 21 },
        {x = 12 , y = 26 },
        {x = 12 , y = 31 },
        {x = 12 , y = 36 },
        {x = 12 , y = 41 },
        {x = 12 , y = 46 },
        {x = 12 , y = 51 },
        {x = 12 , y = 56 },
        {x = 12 , y = 61 },
        {x = 12 , y = 66 },
        {x = 12 , y = 71 },
        {x = 12 , y = 76 },
        {x = 12 , y = 81 },
        {x = 12 , y = 86 },

        {x = 87 , y = 21 },
        {x = 87 , y = 26 },
        {x = 87 , y = 31 },
        {x = 87 , y = 36 },
        {x = 87 , y = 41 },
        {x = 87 , y = 46 },
        {x = 87 , y = 51 },
        {x = 87 , y = 56 },
        {x = 87 , y = 61 },
        {x = 87 , y = 66 },
        {x = 87 , y = 71 },
        {x = 87 , y = 76 },
        {x = 87 , y = 81 },
    }, 

    [10] = {
        {x = 87 , y = 86 },
    },  

    [11] = {
        {x = 15 , y = 21 },
        {x = 15 , y = 26 },
        {x = 15 , y = 31 },
        {x = 15 , y = 36 },
        {x = 15 , y = 41 },
        {x = 15 , y = 46 },
        {x = 15 , y = 51 },
        {x = 15 , y = 56 },
        {x = 15 , y = 61 },
        {x = 15 , y = 66 },
        {x = 15 , y = 71 },
        {x = 15 , y = 76 },
        {x = 15 , y = 81 },
        {x = 15 , y = 86 },

        {x = 84 , y = 21 },
        {x = 84 , y = 26 },
        {x = 84 , y = 31 },
        {x = 84 , y = 36 },
        {x = 84 , y = 41 },
        {x = 84 , y = 46 },
        {x = 84 , y = 51 },
        {x = 84 , y = 56 },
        {x = 84 , y = 61 },
        {x = 84 , y = 66 },
        {x = 84 , y = 71 },
        {x = 84 , y = 76 },
        {x = 84 , y = 81 },
        {x = 84 , y = 86 },  
    },  
    [12] = {
        {x = 18 , y = 21 },
        {x = 21 , y = 21 },
        {x = 24 , y = 21 },
        {x = 27 , y = 21 },
        {x = 30 , y = 21 },
        {x = 33 , y = 21 },
        {x = 36 , y = 21 },
        {x = 39 , y = 21 },
        {x = 42 , y = 21 },
        {x = 45 , y = 21 },
        {x = 48 , y = 21 },
        {x = 51 , y = 21 },
        {x = 54 , y = 21 },
        {x = 57 , y = 21 },
        {x = 60 , y = 21 },
        {x = 63 , y = 21 },
        {x = 66 , y = 21 },
        {x = 69 , y = 21 },
        {x = 72 , y = 21 },
        {x = 75 , y = 21 },
        {x = 78 , y = 21 },
        {x = 81 , y = 21 },

        {x = 18 , y = 86 },
        {x = 21 , y = 86 },
        {x = 24 , y = 86 },
        {x = 27 , y = 86 },
        {x = 30 , y = 86 },
        {x = 33 , y = 86 },
        {x = 36 , y = 86 },
        {x = 39 , y = 86 },
        {x = 42 , y = 86 },
        {x = 45 , y = 86 },
        {x = 48 , y = 86 },
        {x = 51 , y = 86 },
        {x = 54 , y = 86 },
        {x = 57 , y = 86 },
        {x = 60 , y = 86 },
        {x = 63 , y = 86 },
        {x = 66 , y = 86 },
        {x = 69 , y = 86 },
        {x = 72 , y = 86 },
        {x = 75 , y = 86 },
        {x = 78 , y = 86 },
        {x = 81 , y = 86 },
    },  
    [13] = {
        {x = 18 , y = 26 },
        {x = 18 , y = 31 },
        {x = 18 , y = 36 },
        {x = 18 , y = 41 },
        {x = 18 , y = 46 },
        {x = 18 , y = 51 },
        {x = 18 , y = 56 },
        {x = 18 , y = 61 },
        {x = 18 , y = 66 },
        {x = 18 , y = 71 },
        {x = 18 , y = 76 },
        {x = 18 , y = 81 },

        {x = 81 , y = 26 },
        {x = 81 , y = 31 },
        {x = 81 , y = 36 },
        {x = 81 , y = 41 },
        {x = 81 , y = 46 },
        {x = 81 , y = 51 },
        {x = 81 , y = 56 },
        {x = 81 , y = 61 },
        {x = 81 , y = 66 },
        {x = 81 , y = 71 },
        {x = 81 , y = 76 },
        {x = 81 , y = 81 },
    },  
    [14] = {
        {x = 21 , y = 26 },
        {x = 24 , y = 26 },
        {x = 27 , y = 26 },
        {x = 30 , y = 26 },
        {x = 33 , y = 26 },
        {x = 36 , y = 26 },
        {x = 39 , y = 26 },
        {x = 42 , y = 26 },
        {x = 45 , y = 26 },
        {x = 48 , y = 26 },
        {x = 51 , y = 26 },
        {x = 54 , y = 26 },
        {x = 57 , y = 26 },
        {x = 60 , y = 26 },
        {x = 63 , y = 26 },
        {x = 66 , y = 26 },
        {x = 69 , y = 26 },
        {x = 72 , y = 26 },
        {x = 75 , y = 26 },
        {x = 78 , y = 26 },

        {x = 21 , y = 81 },
        {x = 24 , y = 81 },
        {x = 27 , y = 81 },
        {x = 30 , y = 81 },
        {x = 33 , y = 81 },
        {x = 36 , y = 81 },
        {x = 39 , y = 81 },
        {x = 42 , y = 81 },
        {x = 45 , y = 81 },
        {x = 48 , y = 81 },
        {x = 51 , y = 81 },
        {x = 54 , y = 81 },
        {x = 57 , y = 81 },
        {x = 60 , y = 81 },
        {x = 63 , y = 81 },
        {x = 66 , y = 81 },
        {x = 69 , y = 81 },
        {x = 72 , y = 81 },
        {x = 75 , y = 81 },
        {x = 78 , y = 81 },
    },  
    [15] = {
        {x = 21 , y = 31 },
        {x = 21 , y = 36 },
        {x = 21 , y = 41 },
        {x = 21 , y = 46 },
        {x = 21 , y = 51 },
        {x = 21 , y = 56 },
        {x = 21 , y = 61 },
        {x = 21 , y = 66 },
        {x = 21 , y = 71 },
        {x = 21 , y = 76 },

        {x = 78 , y = 31 },
        {x = 78 , y = 36 },
        {x = 78 , y = 41 },
        {x = 78 , y = 46 },
        {x = 78 , y = 51 },
        {x = 78 , y = 56 },
        {x = 78 , y = 61 },
        {x = 78 , y = 66 },
        {x = 78 , y = 71 },
        {x = 78 , y = 76 },    
    },  
    [16] = {
        {x = 24 , y = 31 },
        {x = 27 , y = 31 },
        {x = 30 , y = 31 },
        {x = 33 , y = 31 },
        {x = 36 , y = 31 },
        {x = 39 , y = 31 },
        {x = 42 , y = 31 },
        {x = 45 , y = 31 },
        {x = 48 , y = 31 },
        {x = 51 , y = 31 },
        {x = 54 , y = 31 },
        {x = 57 , y = 31 },
        {x = 60 , y = 31 },
        {x = 63 , y = 31 },
        {x = 66 , y = 31 },
        {x = 69 , y = 31 },
        {x = 72 , y = 31 },
        {x = 75 , y = 31 },

        {x = 24 , y = 76 },
        {x = 27 , y = 76 },
        {x = 30 , y = 76 },
        {x = 33 , y = 76 },
        {x = 36 , y = 76 },
        {x = 39 , y = 76 },
        {x = 42 , y = 76 },
        {x = 45 , y = 76 },
        {x = 48 , y = 76 },
        {x = 51 , y = 76 },
        {x = 54 , y = 76 },
        {x = 57 , y = 76 },
        {x = 60 , y = 76 },
        {x = 63 , y = 76 },
        {x = 66 , y = 76 },
        {x = 69 , y = 76 },
        {x = 72 , y = 76 },
        {x = 75 , y = 76 },   
    },  
    [17] = {
        {x = 24 , y = 36 },
        {x = 24 , y = 41 },
        {x = 24 , y = 46 },
        {x = 24 , y = 51 },
        {x = 24 , y = 56 },
        {x = 24 , y = 61 },
        {x = 24 , y = 66 },
        {x = 24 , y = 71 },

        {x = 75 , y = 36 },
        {x = 75 , y = 41 },
        {x = 75 , y = 46 },
        {x = 75 , y = 51 },
        {x = 75 , y = 56 },
        {x = 75 , y = 61 },
        {x = 75 , y = 66 },
        {x = 75 , y = 71 },         
    },  
    [18] = {
        {x = 27 , y = 36 },
        {x = 30 , y = 36 },
        {x = 33 , y = 36 },
        {x = 36 , y = 36 },
        {x = 39 , y = 36 },
        {x = 42 , y = 36 },
        {x = 45 , y = 36 },
        {x = 48 , y = 36 },
        {x = 51 , y = 36 },
        {x = 54 , y = 36 },
        {x = 57 , y = 36 },
        {x = 60 , y = 36 },
        {x = 63 , y = 36 },
        {x = 66 , y = 36 },
        {x = 69 , y = 36 },
        {x = 72 , y = 36 },

        {x = 27 , y = 71 },
        {x = 30 , y = 71 },
        {x = 33 , y = 71 },
        {x = 36 , y = 71 },
        {x = 39 , y = 71 },
        {x = 42 , y = 71 },
        {x = 45 , y = 71 },
        {x = 48 , y = 71 },
        {x = 51 , y = 71 },
        {x = 54 , y = 71 },
        {x = 57 , y = 71 },
        {x = 60 , y = 71 },
        {x = 63 , y = 71 },
        {x = 66 , y = 71 },
        {x = 69 , y = 71 },
        {x = 72 , y = 71 },    
    },  
    [19] = {
        {x = 27 , y = 41 },
        {x = 27 , y = 46 },
        {x = 27 , y = 51 },
        {x = 27 , y = 56 },
        {x = 27 , y = 61 },
        {x = 27 , y = 66 },          

        {x = 72 , y = 41 },
        {x = 72 , y = 46 },
        {x = 72 , y = 51 },
        {x = 72 , y = 56 },
        {x = 72 , y = 61 },
        {x = 72 , y = 66 },    
    },  

    [20] = {
        {x = 30 , y = 41 },
        {x = 33 , y = 41 },
        {x = 36 , y = 41 },
        {x = 39 , y = 41 },
        {x = 42 , y = 41 },
        {x = 45 , y = 41 },
        {x = 48 , y = 41 },
        {x = 51 , y = 41 },
        {x = 54 , y = 41 },
        {x = 57 , y = 41 },
        {x = 60 , y = 41 },
        {x = 63 , y = 41 },
        {x = 66 , y = 41 },
        {x = 69 , y = 41 },

        {x = 30 , y = 66 },
        {x = 33 , y = 66 },
        {x = 36 , y = 66 },
        {x = 39 , y = 66 },
        {x = 42 , y = 66 },
        {x = 45 , y = 66 },
        {x = 48 , y = 66 },
        {x = 51 , y = 66 },
        {x = 54 , y = 66 },
        {x = 57 , y = 66 },
        {x = 60 , y = 66 },
        {x = 63 , y = 66 },
        {x = 66 , y = 66 },
        {x = 69 , y = 66 },
    },  

    [21] = {            
        {x = 30 , y = 46 },
        {x = 30 , y = 51 },
        {x = 30 , y = 56 },
        {x = 30 , y = 61 },

        {x = 69 , y = 46 },
        {x = 69 , y = 51 },
        {x = 69 , y = 56 },
        {x = 69 , y = 61 },

        {x = 66 , y = 46 },
        {x = 66 , y = 51 },
        {x = 66 , y = 56 },
        {x = 66 , y = 61 },
    },  
    [22] = {
        {x = 33 , y = 46 },
        {x = 33 , y = 51 },
        {x = 33 , y = 56 },
        {x = 33 , y = 61 },

        {x = 63 , y = 46 },
        {x = 63 , y = 51 },
        {x = 63 , y = 56 },
        {x = 63 , y = 61 },                
    },  
}

local STONE_NPC =
{
    [1] =  {164000, 71323},
    [2] =  {164001, 71324},
    [3] =  {164004, 71325},
    [4] =  {164005, 71326},
    [5] =  {164006, 71327},
    [6] =  {164007, 71328},
    [7] =  {164008, 71329},
    [8] =  {164009, 71330},
    [9] =  {164010, 71331},
    [10] = {164011, 71332},
    [11] = {164002, 71333},
    [12] = {164003, 71334}
};

local lo_hierogramid =
{
    40688,  -- 全队法防减百分之10
    40690,  -- 全队法攻减百分之10
    40687,  -- 全队物防减百分之10
    40689,  -- 全队物攻减百分之10
    40691,  -- 全队血上限减百分之5
    40692,  -- 全队移动速度减百分之5
    40760,  -- 问号神符-单人HP回满
    40761,  -- 问号神符-单人MP回满
    40762,  -- 问号神符-全队HP回满
    40763,  -- 问号神符-全队HP上限增百分之10
    40764,  -- 问号神符-全队MP回满
    40765,  -- 问号神符-全队MP上限增百分之10
    40766,  -- 问号神符-全队爆伤增加百分之30
    40754,  -- 问号神符-全队法防减百分之10
    40767,  -- 问号神符-全队法防增百分之20
    40755,  -- 问号神符-全队法攻减百分之10
    40768,  -- 问号神符-全队法攻增百分之20
    40756,  -- 问号神符-全队物防减百分之10
    40771,  -- 问号神符-全队物防增百分之20
    40757,  -- 问号神符-全队物攻减百分之10
    40772,  -- 问号神符-全队物攻增百分之20
    40758,  -- 问号神符-全队血上限减百分之5
    40759,  -- 问号神符-全队移动速度减百分之5
    40773,  -- 问号神符-全队移动速度增百分之10
    40671,  -- 单人HP回满
    40673,  -- 单人MP回满
    40672,  -- 全队HP回满
    40682,  -- 全队HP上限增百分之10
    40674,  -- 全队MP回满
    40683,  -- 全队MP上限增百分之10
    40686,  -- 全队爆伤增加百分之30
    40679,  -- 全队法防增百分之20
    40681,  -- 全队法攻增百分之20
    40678,  -- 全队物防增百分之20
    40680,  -- 全队物攻增百分之20
    40685,  -- 全队移动速度增百分之10
}

local lo_buffid = 
{
    protect_buffid = { 71199, },
    protect_buffid_three = { 75673, },

}

local lo_maskbuff_id =
{
    75416,   
    75417,       
    75418,   
    75419,       
    75420,   
    75421,       
}

local player_deal = 
{
    skill = nil,
}

local block_list =
{

}

local LO_PLAYER_DATA_KEY =
{
    HISTORY_MARK_POINT_TMP = 0,  
    KILL_PALYER_NUM = 1, 
    USEABLE_TIME = 2,         
    DIE_TIME = 3,               --A阶段死亡时间点，参照点 battle_end
    IS_DIE = 4,
    MULTI_KILL = 5,             --当前连杀
    CONSUME_MARK_POINT= 6,      --消费积分
    MAX_MULTI_KILL = 7,         --最高连杀
    MARK_POINT = 8,             --本场积分 
    LIVE_TIME = 9,
    UPDATE_TIME = 10,
    HISTORY_MARK_POINT = 11,	--历史积分
    IS_AWARD = 12,		--是否奖励
}

local LO_DATA_KEY = 
{
    MAX_KILL_NUM = 1,  
    LO_PHASE= 2, 
    MAX_PLAYER_NUM= 3,
}
local PHASE =
{
    PREPARE = 0,
    A = 1,
    REST = 2,
    B = 3,
    CLOSE = 4,
}

local RELIVE_TYPE =
{
        RELIVE_WEAKGRAVE     = 1,                    -- 虚弱墓地复活
        RELIVE_NORMAL        = 2,                    -- 正常原地复活
        RELIVE_SKILL         = 3,                    -- 技能原地复活
        RELIVE_SYSTEM        = 4,                    -- 系统强行复活
}


--状态信息
local state_id =
{
    STATE_MASKED = 53,
}
--local state_from =
--{
--    NONE            = 0,
--    EQUITMENT       = 1,
--    BUFF            = 2,
--    SKILL           = 3,
--}
--
local rank_point = 
{

}


--local func
--创建一个神符
local function create_hierogram(repetion_index ,  repetion_id)
    rand_hierogramid_index = math.random(0, table.getn(lo_hierogramid))
   -- post = get_random_place(repetion_index)
    CreateHierogram(repetion_id, lo_hierogramid[rand_hierogramid_index], math.random( 60 , 80) , math.random( 60 , 80) )
end


--TIMER

--战斗开始
local function begin_battle( repetion_index, repetion_id, args, loop_index )




    player_num = GetPlayerNumByMapID(repetion_id, last_one_single.map_id);

--    if player_num < last_one_single.player_num_limit  then
--        EventNoticeAll( repetion_id , RepetionNoticeEventKind.CLOSE, last_one_single.close_time, 1 );
--        TimeBegin( repetion_id , 5 );
--        return 
--    end

    --消除保护buff
    table.foreachi( lo_buffid.protect_buffid, function(i, v) 
        PlayerRemoveBuffByMapID( repetion_id, v,args.map_id );
    end);

--    --发场内剩余人数
--    left_player_num = GetPlayerNumByMapID( repetion_id , last_one_single.map_id);
--
--    EventNoticeAll( repetion_id ,RepetionNoticeEventKind.TAKE_COUNT , 7, left_player_num )

    SetIntData( repetion_id, LO_DATA_KEY.LO_PHASE, PHASE.A)
    --开始战斗结束倒计时
    TimeBegin( repetion_id , 2 );
    --
    TimeBegin( repetion_id , 17 );
    --开始创建火焰倒计时
--    TimeBegin( repetion_id , 8 );
--    TimeBegin( repetion_id , 9 );
    --开始创建A阶段结束倒计时
--    TimeBegin( repetion_id , 10 );
--    --休息阶段开始计时器
--    TimeBegin( repetion_id , 11 );

    --创建内丹
    DestroyAliveTempOgre( repetion_id, STONE_NPC[1][2], 0) 
    CreateNpc( repetion_id, STONE_NPC[2][1] );
    TimeBegin( repetion_id , 7 );

--    --B 阶段开始倒计时1,
--    TimeBegin( repetion_id , 12 );
--    --B 阶段开始倒计时2,
--    TimeBegin( repetion_id , 13 );
--    --B 阶段开始倒计时3,
--    TimeBegin( repetion_id , 14 );
--    --B 阶段开始,
--  TimeBegin( repetion_id , 15 );
    -- 通知开始计时，在准备期已经进入的玩家
    EventNoticeAll( repetion_id ,RepetionNoticeEventKind.TAKE_COUNT , 11, 0 )

end

--战斗结束
local function end_battle( repetion_index, repetion_id, args, loop_index )

    -- 插入保护buff
    table.foreachi( lo_buffid.protect_buffid, function(i, v) 
        PlayerInsertBuffByMapID( repetion_id, v , args.map_id  );
    end);

    --    local live_time =  (last_one_single.battle_end_time - last_one_single.battle_begin_time) * 1000 
    --    --发布成绩单
    --    --发布存活时间
    --    EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, 3, live_time )    
    --    --发布存活积分
    --    live_time_point_mark = live_time / 1000000 * quotiety
    --    EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, 6, live_time_point_mark )    
    --    AddIntData2AllPlayer( repetion_id, 0, live_time_point_mark )
    SetIntData( repetion_id, LO_DATA_KEY.LO_PHASE, PHASE.CLOSE)

    EventNoticeAll( repetion_id , RepetionNoticeEventKind.CLOSE, last_one_single.close_time, 2 );
    TimeBegin( repetion_id , 5 );
    CreateNpc( repetion_id, STONE_NPC[12][1] );
    local message = {}
    message.battleid = RepetionID.LAST_ONE
    message.mapid = last_one_single.map_id

    SendMessageInLua( MESSAGE_ID.ID_S2G_BATTLE_CLOSE,  message )


    local entity_id_table =  GetRepetionPlayerID( repetion_id )
    if entity_id_table == nil then
        return   ;
    end
    local player_point = { }
    table.foreach( entity_id_table, function(v)
       -- 首先结算 玩家的本场积分
       --useable_time = GetIntData2Player( repetion_id, entity_id_table[v], LO_PLAYER_DATA_KEY.USEABLE_TIME)

       --if GetIntData2Player( repetion_id, entity_id_table[v], LO_PLAYER_DATA_KEY.IS_DIE) == 1  then 
       --    live_time = useable_time - GetIntData2Player( repetion_id, entity_id_table[v] , LO_PLAYER_DATA_KEY.DIE_TIME) 
       --else
       --    live_time = useable_time  
       --end
       update_player_live_data( repetion_id , entity_id_table[v] )
       live_time = GetIntData2Player( repetion_id, entity_id_table[v] , LO_PLAYER_DATA_KEY.LIVE_TIME ) 

       live_time_make_point = live_time / 1000  * GetLevelInLua( entity_id_table[v] ) / 500

       nNum = GetIntData2Player( repetion_id, entity_id_table[v], LO_PLAYER_DATA_KEY.KILL_PALYER_NUM )
       kill_make_point = GetLevelInLua( entity_id_table[v] ) / 500 * nNum * 30
       --multi_kill_point = 
       max_multi_kill = GetIntData2Player( repetion_id, entity_id_table[v] , LO_PLAYER_DATA_KEY.MAX_MULTI_KILL ) 
        tmp_multi_kill = max_multi_kill;
        if max_multi_kill >20 then
            tmp_multi_kill = 20
        end
       multi_kill_point = GetLevelInLua( entity_id_table[v] ) / 10 * (1.1 ^ tmp_multi_kill )
       
      -- Info("kill_make_point:"..kill_make_point.." live_time_make_point:"..live_time_make_point.." multi_kill_point:"..multi_kill_point )
       SetIntData2Player( repetion_id, entity_id_table[v] , LO_PLAYER_DATA_KEY.MARK_POINT, kill_make_point + live_time_make_point + multi_kill_point )

       table.insert( player_point,{ entityid = entity_id_table[v], point = GetIntData2Player( repetion_id, entity_id_table[v], LO_PLAYER_DATA_KEY.MARK_POINT)  }) 
    end);

    table.sort( player_point , function(a, b)
        return a.point > b.point        
    end);

   table.foreach( player_point ,function( i, v)  
            EventNoticeSingle( repetion_id, v.entityid , RepetionNoticeEventKind.TAKE_COUNT, 14, i )
            if i == 1 then
                EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, 15, v.point )    
                Notice("max point is :"..v.point)

            end
        end 
       ); 


end

--创建神符
local function create_hierogram_time(repetion_index, repetion_id, args, loop_index )
    for i = 0, last_one_single.hierogram_num do
        create_hierogram(repetion_index, repetion_id)
    end
end

--last_one 结束

local function lo_end( repetion_index, repetion_id, args, loop_index )

end

--创建内丹

local function create_redstone( repetion_index, repetion_id, args, loop_index )
    DestroyAliveTempOgre( repetion_id, STONE_NPC[loop_index + 1][2], 0) 
    CreateNpc( repetion_id, STONE_NPC[loop_index + 2][1] );
end

--副本结束

local function  close_reption( repetion_index, repetion_id, args, loop_index )
    RepetionClose( repetion_id )
end

--创建火焰陷阱
local function creat_needfire( repetion_index, repetion_id, args, loop_index )
    table.foreachi( NEEDFIRE_POST[ loop_index ] , function(i, v) 
        CreateNeedfire( repetion_id, last_one_single.needfire_id , v.x, v.y ,last_one_single.map_id ) 
    end);

--    TimeBegin( repetion_id , 9 );
end

--创建火焰倒计时
local function needfire_begin_time( repetion_index, repetion_id, args, loop_index )
--  time = GetTime( repetion_id, 8 ); 
    EventNoticeAll( repetion_id ,RepetionNoticeEventKind.COUNT_DOWN, 3, time) 
end

--event

--创建玩家
local function create_player( repetion_id, vParame )
    -- 这里的first mean player 直接登录，不是出送过来的 
    if vParame.first == 1 then
        PlayerLeave( repetion_id,vParame.entityid  )
        return;
    end

    --利用分阵营的形式 让entityid设为campid 肯定不同 
    SetCampID( repetion_id, vParame.entityid, vParame.entityid );
    --单人模式离开队伍
    KickPlayerFromTeam( vParame.entityid );

    --隐藏玩家信息

    AttachPlayerState(vParame.entityid, state_id.STATE_MASKED , 0, 0, 0)
    --插入变身buff
    EntityInsertBuff( repetion_id, vParame.entityid , lo_maskbuff_id[ math.random(1, table.getn(lo_maskbuff_id)) ] )
    EntityInsertBuff( repetion_id, vParame.entityid , lo_buffid.protect_buffid_three[1] )

    if vParame.new == 1 then
        --初始化玩家数据 , this new means player into  a new repetion
        SetIntData2Player( repetion_id, vParame.entityid , LO_PLAYER_DATA_KEY.KILL_PALYER_NUM , 0 )
        SetIntData2Player( repetion_id, vParame.entityid , LO_PLAYER_DATA_KEY.MULTI_KILL, 0 )
        SetIntData2Player( repetion_id, vParame.entityid , LO_PLAYER_DATA_KEY.IS_DIE, 0 )
        SetIntData2Player( repetion_id, vParame.entityid , LO_PLAYER_DATA_KEY.MARK_POINT, 0 )
        SetIntData2Player( repetion_id, vParame.entityid , LO_PLAYER_DATA_KEY.MAX_MULTI_KILL, 0 )
        SetIntData2Player( repetion_id, vParame.entityid , LO_PLAYER_DATA_KEY.LIVE_TIME, 0 )
--        SetIntData2Player( repetion_id, vParame.entityid , LO_PLAYER_DATA_KEY.UPDATE_TIME, GetTime(repetion_id, 2) )
		SetIntData2Player( repetion_id, vParame.entityid , LO_PLAYER_DATA_KEY.IS_AWARD, 0 )
    end

    --update_player_live_data( repetion_id ,vParame.entityid)

    if GetIntData( repetion_id, LO_DATA_KEY.LO_PHASE) == PHASE.PREPARE then

        --插入保护buff
        table.foreachi( lo_buffid.protect_buffid , function(i, v)
            EntityInsertBuff( repetion_id, vParame.entityid, v);
        end);

--        time = GetTime( repetion_id, 1 );
--        EventNoticeSingle( repetion_id, vParame.entityid, RepetionNoticeEventKind.COUNT_DOWN, 1, time);

--        useable_time = last_one_single.battle_end_time ; 
--       SetIntData2Player(repetion_id, vParame.entityid , LO_PLAYER_DATA_KEY.USEABLE_TIME, useable_time)
        SetIntData2Player( repetion_id, vParame.entityid , LO_PLAYER_DATA_KEY.UPDATE_TIME, last_one_single.battle_end_time )
    end

    if GetIntData( repetion_id, LO_DATA_KEY.LO_PHASE) == PHASE.A then

        --创建玩家第一次进入
        if vParame.new == 1  then
           -- 发消息计时开始
            EventNoticeSingle( repetion_id, vParame.entityid, RepetionNoticeEventKind.TAKE_COUNT, 11, 0 )

            EventNoticeSingle( repetion_id, vParame.entityid, RepetionNoticeEventKind.TAKE_COUNT, 9, GetIntData(repetion_id, LO_DATA_KEY.MAX_KILL_NUM ))
        SetIntData2Player( repetion_id, vParame.entityid , LO_PLAYER_DATA_KEY.UPDATE_TIME, GetTime(repetion_id, 2) )
        else
            if vParame.mapid == last_one_single.hell_map_id then
                --玩家从地府传入 可以存活的总时间 - 死亡的时间
         --       useable_time = GetIntData2Player( repetion_id, vParame.entityid, LO_PLAYER_DATA_KEY.USEABLE_TIME)
         --       die_time = GetIntData2Player( repetion_id, vParame.entityid, LO_PLAYER_DATA_KEY.DIE_TIME)

            else
                if GetIntData2Player( repetion_id, vParame.entityid, LO_PLAYER_DATA_KEY.IS_DIE) == 1 then
                    update_player_live_data( repetion_id ,vParame.entityid)
                    --状态变活
                    SetIntData2Player(repetion_id, vParame.entityid , LO_PLAYER_DATA_KEY.IS_DIE, 0 )
                    --存活时间
              --      useable_time = GetIntData2Player( repetion_id, vParame.entityid, LO_PLAYER_DATA_KEY.USEABLE_TIME)

              --      live_time =  useable_time - GetTime(repetion_id, 2)
                    live_time = GetIntData2Player( repetion_id, vParame.entityid, LO_PLAYER_DATA_KEY.LIVE_TIME )
                    EventNoticeSingle( repetion_id, vParame.entityid, RepetionNoticeEventKind.TAKE_COUNT, 11, live_time)
                end

            end
        end

    end
    --场内剩余人数
    left_player_num = GetPlayerNumByMapID( repetion_id , last_one_single.map_id);
    --max_palyer_num = 


    
    if vParame.mapid == last_one_single.map_id then 
        --人数加1 
        EventNoticeAll( repetion_id , RepetionNoticeEventKind.TAKE_COUNT , 7, left_player_num + 1)
        EventNoticeSingle( repetion_id, vParame.entityid, RepetionNoticeEventKind.TAKE_COUNT, 7, left_player_num + 1)

        ObserverEntityDeath(repetion_id, vParame.entityid)
    elseif vParame.mapid == last_one_single.hell_map_id then
        --人数减一 
        EventNoticeAll( repetion_id , RepetionNoticeEventKind.TAKE_COUNT , 7, left_player_num )
        EventNoticeSingle( repetion_id, vParame.entityid, RepetionNoticeEventKind.TAKE_COUNT, 7, left_player_num )
    end

end

local function entity_death( repetion_id, vParame )
    if vParame.killedisplayer == 1 then
        if vParame.killerid ~= nil and vParame.killerid ~= vParame.killedid then
            --被人杀死
            level = vParame.killerlevel 
            table.foreachi( LEVEL_QUOTIETY, function( i, v )
                if level >= v.min_level and level <= v.max_level then
                    quotiety_A = v.quotiety_A   
                    quotiety_B = v.quotiety_B   
                end
            end);

            --给杀人者增加积分
            --增加杀人数
            AddIntData2Player( repetion_id, vParame.killerid, LO_PLAYER_DATA_KEY.KILL_PALYER_NUM , 1)
            nNum = GetIntData2Player( repetion_id, vParame.killerid, LO_PLAYER_DATA_KEY.KILL_PALYER_NUM )
            EventNoticeSingle( repetion_id, vParame.killerid, RepetionNoticeEventKind.TAKE_COUNT, 4, nNum  )
            if nNum > GetIntData(repetion_id, LO_DATA_KEY.MAX_KILL_NUM )then
                SetIntData( repetion_id, LO_DATA_KEY.MAX_KILL_NUM , nNum )
                EventNoticeAll( repetion_id ,RepetionNoticeEventKind.TAKE_COUNT , 8, nNum)
            end
            kill_one_make_point  = 0
            if GetIntData( repetion_id, LO_DATA_KEY.LO_PHASE) == PHASE.A or GetIntData( repetion_id, LO_DATA_KEY.LO_PHASE) == PHASE.REST then
                --杀人积分
                kill_one_make_point = vParame.killerlevel / 500 * 1 * 30
            end

            --增加总积分 
            --（玩家当前等级/500）*杀人数*30
            AddIntData2Player( repetion_id, vParame.killerid , LO_PLAYER_DATA_KEY.HISTORY_MARK_POINT, kill_one_make_point )
            --增加杀人积分
            --增加消耗积分
            AddIntData2Player( repetion_id, vParame.killerid , LO_PLAYER_DATA_KEY.CONSUME_MARK_POINT, kill_one_make_point )
            --本场杀人积分

            EventNoticeSingle( repetion_id, vParame.killerid, RepetionNoticeEventKind.TAKE_COUNT, 5, kill_one_make_point * nNum )
            AddIntData2Player( repetion_id, vParame.killerid , LO_PLAYER_DATA_KEY.MULTI_KILL, 1 )

            --更新本场积分

            --更新最大连杀
            multi_kill = GetIntData2Player( repetion_id, vParame.killerid, LO_PLAYER_DATA_KEY.MULTI_KILL ) 
            EventNoticeSingle( repetion_id, vParame.killerid, RepetionNoticeEventKind.TAKE_COUNT, 13, multi_kill )
            if multi_kill > GetIntData2Player( repetion_id, vParame.killerid, LO_PLAYER_DATA_KEY.MAX_MULTI_KILL ) then 
                SetIntData2Player( repetion_id, vParame.killerid, LO_PLAYER_DATA_KEY.MAX_MULTI_KILL, multi_kill )
            end


            --给被杀者结算积分

            --        DetachPlayerState( vParame.killedid , state_id.STATE_MASKED , 0, 0, 0)
    --        EventNoticeSingle( repetion_id, vParame.killedid, RepetionNoticeEventKind.TAKE_COUNT, 10, 2)
        elseif vParame.killerid == vParame.killedid then
            --被火焰吞噬
--            EventNoticeSingle( repetion_id, vParame.killedid, RepetionNoticeEventKind.TAKE_COUNT, 10, 1)
        end

        SetIntData2Player( repetion_id, vParame.killedid, LO_PLAYER_DATA_KEY.MULTI_KILL, 0 )
        if GetIntData( repetion_id, LO_DATA_KEY.LO_PHASE) == PHASE.A then
            update_player_live_data( repetion_id, vParame.killedid ) 
            live_time =  GetIntData2Player( repetion_id, vParame.killedid, LO_PLAYER_DATA_KEY.LIVE_TIME)   
            EventNoticeSingle( repetion_id, vParame.killedid, RepetionNoticeEventKind.TAKE_COUNT, 12, live_time)
        end

        SetIntData2Player( repetion_id, vParame.killedid , LO_PLAYER_DATA_KEY.IS_DIE, 1 )
    end
end

local function repetion_begin( repetion_id, vParame )
    SetIntData( repetion_id, LO_DATA_KEY.MAX_KILL_NUM , 0)    
    SetIntData( repetion_id, LO_DATA_KEY.MAX_PLAYER_NUM , 0)    
    CreateNpc( repetion_id, STONE_NPC[1][1])

end

local function leave_repetion( repetion_id, vParame )
    level = vParame.playerlevel
    table.foreachi( LEVEL_QUOTIETY, function( i, v )
        if level >= v.min_level and level <= v.max_level then
            quotiety_A = v.quotiety_A   
            quotiety_B = v.quotiety_B   
        end
    end);

    useable_time = GetIntData2Player( repetion_id, vParame.entityid, LO_PLAYER_DATA_KEY.USEABLE_TIME)

    if GetIntData( repetion_id, LO_DATA_KEY.LO_PHASE) == PHASE.A   then

        --更新积分
        update_player_live_data( repetion_id, vParame.entityid ) 

    elseif GetIntData( repetion_id, LO_DATA_KEY.LO_PHASE) == PHASE.CLOSE then
        --存活时间
  --      if GetIntData2Player( repetion_id, vParame.entityid, LO_PLAYER_DATA_KEY.IS_DIE) == 1  then 
  --          live_time = useable_time - GetIntData2Player( repetion_id, vParame.entityid , LO_PLAYER_DATA_KEY.DIE_TIME) 
  --      else
  --          live_time = useable_time  
  --      end

  --      EventNoticeSingle( repetion_id, vParame.entityid, RepetionNoticeEventKind.TAKE_COUNT, 3, live_time)

  --     -- live_time_make_point = ( useable_time - last_one_single.B_use_time )/1000 * quotiety_A +  last_one_single.B_use_time/1000 * quotiety_B 
  --      live_time_make_point = live_time / 1000  * vParame.playerlevel / 500

  --      EventNoticeSingle( repetion_id, vParame.entityid, RepetionNoticeEventKind.TAKE_COUNT, 6, live_time_make_point)
  --      --AddIntData2AllPlayer( repetion_id, 0,live_time_make_point )
  --      --增加生存积分
  --      --增加总积分
  --      AddIntData2Player( repetion_id, vParame.entityid, LO_PLAYER_DATA_KEY.HISTORY_MARK_POINT, live_time_make_point )
  --      --增加消耗积分
  --      AddIntData2Player( repetion_id, vParame.entityid, LO_PLAYER_DATA_KEY.CONSUME_MARK_POINT, live_time_make_point )
        --计算获得经验值
    else 
        live_time_make_point = 0 
        lo_exp = 0
    end
    kill_num = GetIntData2Player( repetion_id, vParame.entityid, LO_PLAYER_DATA_KEY.KILL_PALYER_NUM )

    max_multi_kill = GetIntData2Player( repetion_id, vParame.entityid, LO_PLAYER_DATA_KEY.MAX_MULTI_KILL ) 

    EventNoticeSingle( repetion_id, vParame.entityid, RepetionNoticeEventKind.TAKE_COUNT, 16, max_multi_kill )
    tmp_multi_kill = max_multi_kill;
    if max_multi_kill >20 then
        tmp_multi_kill = 20
    end
    multi_kill_point =  vParame.playerlevel / 10 * (1.1 ^ tmp_multi_kill )
    --增加连杀积分
    --增加总积分
    AddIntData2Player( repetion_id, vParame.entityid, LO_PLAYER_DATA_KEY.HISTORY_MARK_POINT, multi_kill_point )
    --增加消耗积分
    AddIntData2Player( repetion_id, vParame.entityid, LO_PLAYER_DATA_KEY.CONSUME_MARK_POINT, multi_kill_point )
    
    live_time = GetIntData2Player( repetion_id, vParame.entityid, LO_PLAYER_DATA_KEY.LIVE_TIME)
    EventNoticeSingle( repetion_id, vParame.entityid, RepetionNoticeEventKind.TAKE_COUNT, 12,live_time)
  
    live_time_make_point = live_time / 1000  * vParame.playerlevel / 500
    mark_point = kill_num * 30 * vParame.playerlevel / 500 + live_time_make_point + multi_kill_point  
--    SetIntData2Player(repetion_id, vParame.killedid, LO_PLAYER_DATA_KEY.MARK_POINT, mark_point )
    EventNoticeSingle( repetion_id, vParame.entityid, RepetionNoticeEventKind.TAKE_COUNT, 1,mark_point )
    level_exp = GetTemplateLevelExp( repetion_id, vParame.entityid, last_one_single.template_level_id)
    lo_exp =  mark_point/vParame.playerlevel * 2000 * level_exp
    Notice("entityid:"..vParame.entityid.." live_time:"..live_time.." live_time_make_point:"..live_time_make_point.." multi_kill_point:"..multi_kill_point.."mark_point"..mark_point.." lo_exp"..lo_exp)

    if vParame.playermapid == last_one_single.map_id then
        --场内剩余人数
        left_player_num = GetPlayerNumByMapID( repetion_id , last_one_single.map_id) - 1;
        Notice(string.format("last_one's left player number is %d", left_player_num))

   --     EventNoticeAll( repetion_id ,RepetionNoticeEventKind.TAKE_COUNT , 7, left_player_num )
    end
	
    local point = 0;
    if live_time >= 900000 then
	point = 900000;
    else
	point = live_time;
    end    

    if GetIntData2Player( repetion_id, vParame.entityid, LO_PLAYER_DATA_KEY.IS_AWARD ) == 0 then
	ObtainRepetionExp( repetion_id, vParame.entityid, lo_exp);
	EventNoticeSingle( repetion_id, vParame.entityid, RepetionNoticeEventKind.TAKE_COUNT, 9, lo_exp );
	
	AwardFameSingle( repetion_id, vParame.entityid, 0, point*10000/900000 );
	
	if live_time > 3 * 60 * 1000  and  vParame.playerlevel > 80 then 
	    insert_item( repetion_id, vParame.entityid, last_one_single.medal_id, 1, RepetionInsertItemReason.WIN)
	end 
	
	SetIntData2Player( repetion_id, vParame.entityid, LO_PLAYER_DATA_KEY.IS_AWARD, 1 ) ;
    end
  

    -- 插入保护buff
    table.foreachi( lo_maskbuff_id , function(i, v) 
        if true == CharacterHasBuf( repetion_id, vParame.entityid, v )  then
            EntityRemoveBuff( repetion_id, vParame.entityid ,v);
        end
    end);
    
end
function repetion_close( repetion_id, vParame )
end

function player_relive( repetion_id, vParame )

    if vParame.relive_type ~= RELIVE_TYPE.RELIVE_WEAKGRAVE then
        
        --存活时间
   --     useable_time = GetIntData2Player( repetion_id, vParame.entityid, LO_PLAYER_DATA_KEY.USEABLE_TIME )
   --     --死亡时间
   --     die_time = GetIntData2Player( repetion_id, vParame.entityid, LO_PLAYER_DATA_KEY.DIE_TIME)
   --     useable_time = useable_time - ( die_time -  GetTime(repetion_id, 2) ) 
   --     --死亡时间
   --     SetIntData2Player(repetion_id, vParame.entityid , LO_PLAYER_DATA_KEY.USEABLE_TIME, useable_time)

   --     live_time =  useable_time - GetTime(repetion_id, 2)
        
        update_player_live_data( repetion_id, vParame.entityid )
        live_time = GetIntData2Player( repetion_id, vParame.entityid, LO_PLAYER_DATA_KEY.LIVE_TIME )
        EventNoticeSingle( repetion_id, vParame.entityid, RepetionNoticeEventKind.TAKE_COUNT, 11, live_time )

        ObserverEntityDeath(repetion_id, vParame.entityid)
        EntityInsertBuff( repetion_id, vParame.entityid , lo_maskbuff_id[ math.random(0, table.getn(lo_maskbuff_id)) ] )
        SetIntData2Player( repetion_id, vParame.entityid, LO_PLAYER_DATA_KEY.IS_DIE, 0 )
    end


end

--func 
function account_mark_point( repetion_id , vEntityID )

    --    level = GetRepeLevel( repetion_id )
    --    table.foreachi( LEVEL_QUOTIETY, function( i, v )
    --        if level >= v.min_level and level <= v.max_level then
    --            quotiety = v.quotiety   
    --        end
    --    end);
    --    --杀人数
    --    --        local nNum = GetIntData2Player( repetion_id, vEntityID, LO_PLAYER_DATA_KEY.KILL_PALYER_NUM )
    --    --        EventNoticeSingle( repetion_id, vEntityID, RepetionNoticeEventKind.TAKE_COUNT, 4, nNum  )
    --    --
    --    --                --杀人积分
    --    --        local kill_make_point =  nNum * 70 * quotiety
    --    --        EventNoticeSingle( repetion_id, vEntityID, RepetionNoticeEventKind.TAKE_COUNT, 5, kill_make_point)
    --    --存活时间
    --    local killed_live_time = ( last_one_single.battle_end_time - last_one_single.battle_begin_time ) * 1000 - GetTime(repetion_id, 2)
    --    EventNoticeSingle( repetion_id, vEntityID, RepetionNoticeEventKind.TAKE_COUNT, 3, killed_live_time)
    --    --存活积分
    --    local live_time_make_point = killed_live_time/1000000 * quotiety 
    --    EventNoticeSingle( repetion_id, vEntityID, RepetionNoticeEventKind.TAKE_COUNT, 6, live_time_make_point)
    --
    --    --增加总积分 
    --    AddIntData2Player(repetion_id, vEntityID, LO_PLAYER_DATA_KEY.MAKE_POINT, live_time_make_point  )
    --
end

--二期

--func
local function count_down_begin_rest( repetion_index, repetion_id, args, loop_index  )
--    time = GetTime( repetion_id, 11 ); 
--    EventNoticeAll( repetion_id ,RepetionNoticeEventKind.COUNT_DOWN, 4, time) 
    --休息阶段开始计时器
  --  TimeBegin( repetion_id , 11 );
        
end

--func
local function rest_begin( repetion_index, repetion_id, args, loop_index  )
    SetIntData( repetion_id, LO_DATA_KEY.LO_PHASE, PHASE.REST );
    -- 插入保护buff
    table.foreachi( lo_buffid.protect_buffid, function(i, v) 
        PlayerInsertBuffByMapID( repetion_id, v ,last_one_single.map_id  );
    end);
    PlayerReliveSys( repetion_id )

    PlayerLeaveByMapID(repetion_id,  last_one_single.hell_map_id)

    local message = {}
    message.battleid = RepetionID.LAST_ONE
    message.mapid = last_one_single.map_id

    SendMessageInLua( MESSAGE_ID.ID_S2G_BATTLE_CLOSE,  message )



end

--func 
local function rest_count_down_1( repetion_index, repetion_id, args, loop_index )
--    time = GetTime( repetion_id, 15 ); 
--    EventNoticeAll( repetion_id ,RepetionNoticeEventKind.COUNT_DOWN, 5, time) 

    --B 阶段开始,
 --   TimeBegin( repetion_id , 15 );
end
--func 
local function rest_count_down_2( repetion_index, repetion_id, args, loop_index  )
    time = GetTime( repetion_id, 15 ); 
    EventNoticeAll( repetion_id ,RepetionNoticeEventKind.COUNT_DOWN, 5, time) 

end
--func 
local function rest_count_down_3(repetion_index, repetion_id, args, loop_index )
    time = GetTime( repetion_id, 15 ); 
    EventNoticeAll( repetion_id ,RepetionNoticeEventKind.COUNT_DOWN, 5, time) 

end

--func 
local function B_begin( repetion_index, repetion_id, args, loop_index )

    SetIntData( repetion_id, LO_DATA_KEY.LO_PHASE, PHASE.B );
        
    --消除保护buff
    table.foreachi( lo_buffid.protect_buffid, function(i, v) 
        PlayerRemoveBuffByMapID( repetion_id, v,args.map_id );
    end);
end

--func  变身 
local function change_face( repetion_index, repetion_id, args, loop_index )
        
    entity_id_table =  GetRepetionPlayerID( repetion_id )
    table.foreach( entity_id_table, function(v)
       EntityInsertBuff( repetion_id, entity_id_table[v], lo_maskbuff_id[ math.random(1, table.getn(lo_maskbuff_id)) ] ); 
    end);

end

--func badly test
local function test (repetion_index, repetion_id, args, loop_index )

end

--func update data 

local function update_player_data( repetion_id, entityid )
        
end

-- func  update time_mark
--
-- 在改变  is_die 之前使用 
function update_player_live_data( repetion_id, entityid ) 

    --useable_time = GetIntData2Player( repetion_id, entityid , LO_PLAYER_DATA_KEY.USEABLE_TIME)
  --  update_time = GetIntData2Player( repetion_id, entityid , LO_PLAYER_DATA_KEY.UPDATE_TIME)
    is_die= GetIntData2Player( repetion_id, entityid , LO_PLAYER_DATA_KEY.IS_DIE )


    if is_die == 1 then 
        ---复活时

    else 
        update_time = GetIntData2Player( repetion_id, entityid , LO_PLAYER_DATA_KEY.UPDATE_TIME)

        if GetIntData( repetion_id, LO_DATA_KEY.LO_PHASE) == PHASE.CLOSE then
            update_live_time = update_time 
        else
            update_live_time = update_time - GetTime(repetion_id, 2)
        end
        if(update_live_time > 3 * 1000 * 60) then 
            return
        end
        
       -- =（玩家当前等级/500）*（存活时间
        update_live_time_make_point = update_live_time / 1000  * GetLevelInLua( entityid ) / 500
        --添加积分
        --AddIntData2Player( repetion_id, entityid, LO_PLAYER_DATA_KEY.HISTORY_MARK_POINT, update_live_time_make_point )
        --添加存活时间
        AddIntData2Player( repetion_id, entityid, LO_PLAYER_DATA_KEY.LIVE_TIME, update_live_time )
        live_time = GetIntData2Player( repetion_id, entityid , LO_PLAYER_DATA_KEY.LIVE_TIME)
        live_time_make_point = live_time / 1000  * GetLevelInLua( entityid ) / 500
   --     EventNoticeSingle( repetion_id, entityid, RepetionNoticeEventKind.TAKE_COUNT, 6, live_time_make_point )
   --     EventNoticeSingle( repetion_id, entityid, RepetionNoticeEventKind.TAKE_COUNT, 3, live_time )
        --增加总积分
        AddIntData2Player( repetion_id, entityid, LO_PLAYER_DATA_KEY.HISTORY_MARK_POINT, update_live_time_make_point )
        --增加消耗积分
        AddIntData2Player( repetion_id, entityid, LO_PLAYER_DATA_KEY.CONSUME_MARK_POINT, update_live_time_make_point )
    end
      
    if GetIntData( repetion_id, LO_DATA_KEY.LO_PHASE) == PHASE.CLOSE then
        SetIntData2Player(repetion_id, entityid , LO_PLAYER_DATA_KEY.UPDATE_TIME, 0 )
    else
        SetIntData2Player(repetion_id, entityid , LO_PLAYER_DATA_KEY.UPDATE_TIME, GetTime(repetion_id, 2))
    end

       
end



local function live_mark_update( repetion_index, repetion_id, args, loop_index )

    local entity_id_table =  GetRepetionPlayerID( repetion_id )
    if entity_id_table == nil then
        return   ;
    end
    table.foreach( entity_id_table, function(v)
            update_player_live_data( repetion_id, entity_id_table[ v ] )
              
    end);

end



-- 副本配置如下
repetion = 
{

    baseinfo = 
    {
        repetion_index = RepetionID.LAST_ONE,
        playercountmax = 400,		-- 副本最大人数
        mapid = { last_one_single.map_id ,last_one_single.hell_map_id },--90,			-- 副本地图ID
        taskid = 0,			-- 进入副本的前置任务
        posx = 94,				-- 暂时不用
        posy = 124,				-- 暂时不用
        pernumber = 0,			-- 每天进入副本的次数； 0-不限
        minlevel = 50,			-- 最低进入等级
        maxlevel = 120,
        unrelive = 0,
        ontimeopen = 1,                 -- 1按时开启  0 否
        mintimer = 21 * 60,
        notlimittime = 1,               -- 1传入副本不限制时间 0限制
        to_cleint_index = 6,            -- 传给客户端的数据 对应 playerrepetion 里的数据
        delay_time = 60 * 1000 * 3,     -- 给出玩家传送的时间 方式崩溃 可选 默认 500 暂时只有 南天门用到

        -- 组队信息
        team = 
        {
            teamin = true,		-- 是否组队进入； ture-需要组队; false - 不需组队
            count = 1,			-- 组队人数下限
        },


        backpos = 
        {
            mapid = 1,			-- 暂时不用
            posx = 33,			-- 暂时不用	
            posy = 13			-- 暂时不用
        },
		-- 副本开放时间
        open = 
        {
                -- 年, 月，周， 日， 时, 分， 秒
                -- 0 是不做限时， 周是每月第几周
                --第一个时段

                {
                        start = { year = 0, month = 0, week = 0, day = 0, hr = 14, minute = 30, sec = 0 },
                        finish = { year = 0, month = 0, week = 0, day = 0, hr = 14,minute = 51, sec = 0 }
                },
--                {
--                       start = { year = 0, month = 0, week = 7, day = 0, hr = 19, minute = 0, sec = 0 },
--                        finish = { year = 0, month = 0, week = 7, day = 0, hr = 19,minute = 21, sec = 0 }
--                },

        },

--        signup_open =
--        {
--
--            {   
--                start = { year = 0, month = 0, week = 0, day = 0, hr = 19, minute = 0, sec = 0 },
--                finish = { year = 0, month = 0, week = 0, day = 0, hr = 19, minute = 30, sec = 0 },
--            },  
--        },
    },

    battle =
    {

        level =
        {
            { levelmin = 50,    levelmax = 64},
            { levelmin = 65,    levelmax = 79 },
            { levelmin = 80,    levelmax = 94 },
            { levelmin = 95,    levelmax = 99 },
	    { levelmin = 100,    levelmax = 120 },
        },          

        open =  
        {
            {
                start = { year = 0, month = 0, week = 0, day = 0, hr = 14, minute = 30, sec = 0 },
                finish = { year = 0, month = 0, week = 0, day = 0, hr =14, minute = 30, sec = 10 }
            },
--            {
--               start = { year = 0, month = 0, week = 7, day = 0, hr = 19, minute = 0, sec = 0 },
--                finish = { year = 0, month = 0, week = 7, day = 0, hr =19, minute = 0, sec = 10 }
--            },
        }
    },

    -- 定时器
    timer =
    {
        --1 战争开始
        {
            kind = RepetionTimeBeginKind.START_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = last_one_single.battle_begin_time,
            loop = 1,
            func = begin_battle,
            args = { map_id = last_one_single.map_id },
            sync = { kind = RepetionTimeSyncKind.ALL, key = 1 }
        },

        --2 战争结束
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = last_one_single.battle_end_time,
            loop = 1,
            func = end_battle,
            args = { map_id = last_one_single.map_id },
            sync = { kind = RepetionTimeSyncKind.ALL, key = 2 }
        },

        --3 创建神符
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = last_one_single.battle_begin_time,
            loop = 1,
            func = create_hierogram_time,
            args = { }
        },

        --4 创建神符
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = last_one_single.create_hiero_time,
            loop = 5,
            func = create_hierogram_time,
            args = { }
        },
        --5
        {   
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.CLOSE,
            time = last_one_single.close_time_left,
            loop = 1,
       --     func = close_reption, 
            args = { } 
        },  
        --6
        {
            kind = RepetionTimeBeginKind.START_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = last_one_single.end_time,
            loop = 1,
            func = lo_end,
            args = { }
        },

        --7 创建内丹
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = last_one_single.creat_readstone_time,
            loop = 10,
            func = create_redstone,
            args = { }
        },
        --8 创建火焰
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = last_one_single.creat_needfire_time,
            loop = 20,
            func = creat_needfire,
            args = { }

        },
        --9火焰创建显示倒计时
        {
            --改为 EVENT_BEGIN
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = last_one_single.needfire_begin_time,
            loop = 1,
            func = needfire_begin_time,
            args = { }

        },
        --二期加入
        --10 A阶段结束倒计时,休息时间倒计时
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = last_one_single.rest_count_down,
            loop = 1,
            func = count_down_begin_rest,
            args = {  },
        },

        --11 A阶段结束,休息阶段开始
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = last_one_single.rest_begin,
            loop = 1,
            func = rest_begin,
            args = { map_id = last_one_single.map_id },
            sync = { kind = RepetionTimeSyncKind.ALL, key = 4 }
        },

        --12 B阶段开始倒计时1,休息阶段结束倒计时
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = last_one_single.rest_count_down_1,
            loop = 1,
            func = rest_count_down_1,
            args = {  },
        },

        --13 B 阶段开始倒计时2 ,休息阶段结束倒计时
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = last_one_single.rest_count_down_2,
            loop = 1,
            func = rest_count_down_2,
            args = {  },
        },

        --14 B 阶段开始倒计时2 ,休息阶段结束倒计时
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = last_one_single.rest_count_down_3,
            loop = 1,
            func = rest_count_down_3,
            args = {  },
        },
        --15 B 阶段开始
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = last_one_single.B_begin,
            loop = 1,
            func = B_begin,
            args = { map_id = last_one_single.map_id  },
            sync = { kind = RepetionTimeSyncKind.ALL, key = 5 }
        },
        --16 10分钟换变身
        {
            kind = RepetionTimeBeginKind.START_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = last_one_single.change_face_time ,
            loop = 2,
            func = change_face,
            sync = {  }
        },
        --17 数据更新
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = last_one_single.live_mark_update,
            loop = 9,
            func = live_mark_update,
            sync = {  }
        },
    },


    --事件
    event = 
    {
        --            [ RepetionEventKind.ENTITY_MOVE_STOP ] = { func = movestop },
        [ RepetionEventKind.OGRE_DEATH ] = { func = entity_death },
        --        [ RepetionEventKind.OGRE_CREATE ] = { func = create_ogre },
        [ RepetionEventKind.PLAYER_CREATE ] = { func = create_player },
        [ RepetionEventKind.CLOSE_EVENT] = { func = repetion_close},
        [ RepetionEventKind.BEGIN_REPETION] = { func = repetion_begin},
        [ RepetionEventKind.LEAVE_REPETION] = { func = leave_repetion},
        [ RepetionEventKind.PLAYER_RELIVE] = { func = player_relive},
    },
}
