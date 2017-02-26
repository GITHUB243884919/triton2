local last_one_single ={};
last_one_single.map_id = 129;
last_one_single.create_hiero_time= 3*60*1000;
last_one_single.battle_begin_time = 5 * 60 * 1000;
last_one_single.ballte_end_time = 23 * 60 * 1000;
last_one_single.close_time = 1* 60 *1000;
last_one_single.close_time_left = 1* 60 *1000;
last_one_single.end_time = 30* 60 *1000;
last_one_single.creat_readstone_time = 2 * 60 *1000;
last_one_single.hierogram_num = 20;
last_one_single.player_num_limit = 2; 
last_one_single.needfire_id = 71387; 
last_one_single.creat_needfire_time = 1 * 60 * 1000;
last_one_single.needfire_begin_time = 50 * 1000;


local LEVEL_QUOTIETY=
{
    { min_level = 50, max_level = 59,   quotiety = 1,   },
    { min_level = 60, max_level = 69,   quotiety = 1.2, },
    { min_level = 70, max_level = 79,   quotiety = 1.2, },
    { min_level = 80, max_level = 89,   quotiety = 1.4, },
    { min_level = 90, max_level = 100,  quotiety = 1.6, }, 
    { min_level = 100, max_level = 110, quotiety = 1.8, }
};
local STONE_MAP_INFO=
{
    mapid = 129,
    posx = 18,
    posy = 26
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
    [1] = {128000, 71323},
    [2] = {128001, 71324},
    [3] = {128002, 71333},
    [4] = {128003, 71334},
    [5] = {128004, 71325},
    [6] = {128005, 71326},
    [7] = {128006, 71327},
    [8] = {128007, 71328},
    [9] = {128008, 71329},
    [10] = {128009, 71330},
    [11] = {128010, 71331},
    [12] = {128011, 71332},
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
    protect_buffid = {71199, },

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
    HISTORY_MARK_POINT = 0,  
    KILL_PALYER_NUM = 1, 
    LIVE_TIME = 2,
}

local LO_DATA_KEY = 
{
    MAX_KILL_NUM = 1,  
    LO_PHASE= 2, 
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


--local func
--创建一个神符
local function create_hierogram(repetion_index ,  repetion_id)
    rand_hierogramid_index = math.random(0, table.getn(lo_hierogramid))
    post = get_random_place(repetion_index)
    CreateHierogram(repetion_id, lo_hierogramid[rand_hierogramid_index], post.x, post.y )
end


--TIMER

--战斗开始
local function begin_battle( repetion_index, repetion_id, args, loop_index )
    player_num = GetPlayerNumByMapID(repetion_id, last_one_single.map_id);

    if player_num < last_one_single.player_num_limit  then
        EventNoticeAll( repetion_id , RepetionNoticeEventKind.CLOSE, last_one_single.close_time, 1 );
        TimeBegin( repetion_id , 5 );
        return 
    end

    --消除保护buff
    table.foreachi( lo_buffid.protect_buffid, function(i, v) 
        PlayerRemoveBuffByMapID( repetion_id, v,args.map_id );
    end);

    --发场内剩余人数
    left_player_num = GetPlayerNumByMapID( repetion_id , last_one_single.map_id);

    EventNoticeAll( repetion_id ,RepetionNoticeEventKind.TAKE_COUNT , 7, left_player_num )

    SetIntData( repetion_id, LO_DATA_KEY.LO_PHASE, 1)
    --开始战斗结束倒计时
    TimeBegin( repetion_id , 2 );
    --开始创建火焰倒计时
    TimeBegin( repetion_id , 8 );
    TimeBegin( repetion_id , 9 );
    --创建内丹
    DestroyAliveTempOgre( repetion_id, STONE_NPC[1][2], 0)
    CreateNpc( repetion_id, STONE_NPC[2][1] );
    TimeBegin( repetion_id , 7 );
end

--战斗结束
local function end_battle( repetion_index, repetion_id, args, loop_index )
    level = GetRepeLevel( repetion_id )
    table.foreachi( LEVEL_QUOTIETY, function( i, v )
        if level >= v.min_level and level <= v.max_level then
            quotiety = v.quotiety   
        end
    end);

    -- 插入保护buff
    table.foreachi( lo_buffid.protect_buffid, function(i, v) 
        PlayerInsertBuffByMapID( repetion_id, v , args.map_id  );
    end);

    --    local live_time =  (last_one_single.ballte_end_time - last_one_single.battle_begin_time) * 1000 
    --    --发布成绩单
    --    --发布存活时间
    --    EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, 3, live_time )    
    --    --发布存活积分
    --    live_time_point_mark = live_time / 1000000 * quotiety
    --    EventNoticeAll( repetion_id, RepetionNoticeEventKind.TAKE_COUNT, 6, live_time_point_mark )    
    --    AddIntData2AllPlayer( repetion_id, 0, live_time_point_mark )
    SetIntData( repetion_id, LO_DATA_KEY.LO_PHASE, 2)

    EventNoticeAll( repetion_id , RepetionNoticeEventKind.CLOSE, last_one_single.close_time, 2 );
    TimeBegin( repetion_id , 5 );
    CreateNpc( repetion_id, STONE_NPC[12][1] );
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
        CreateNeedfire( repetion_id, last_one_single.needfire_id , v.x, v.y ) 
    end);

    TimeBegin( repetion_id , 9 );
end

--创建火焰倒计时
local function needfire_begin_time( repetion_index, repetion_id, args, loop_index )
    time = GetTime( repetion_id, 8 ); 
    EventNoticeAll( repetion_id ,RepetionNoticeEventKind.COUNT_DOWN, 3, time) 
end

--event

--创建玩家
local function create_player( repetion_id, vParame )

    --掉线或者从新进入 踢出副本
    if vParame.new == 0 then
        --去掉 (隐藏状态)
        --        DetachPlayerState(vParame.entityid, state_id.STATE_MASKED , 0, 0, 0)
        -- PlayerLeave( repetion_id, vParame.entityid ); 
        if vParame.first == 1 then
            PlayerLeaveDirect( repetion_id,vParame.entityid  )
            return;
        end
    else
        --利用分阵营的形式 使玩家可以pk 利用玩家数+1作为camp_id 
        SetCampID( repetion_id, vParame.entityid, GetPlayerNumByMapID( repetion_id, last_one_single.map_id) + 2 );
        --隐藏玩家信息

        AttachPlayerState(vParame.entityid, state_id.STATE_MASKED , 0, 0, 0)

        if GetIntData( repetion_id, LO_DATA_KEY.LO_PHASE) == 0 then
            --插入保护buff
            table.foreachi( lo_buffid.protect_buffid , function(i, v)
                EntityInsertBuff( repetion_id, vParame.entityid, v);
            end);
        end

        --利用分阵营的形式 使玩家可以pk 利用玩家数+1作为camp_id 
        SetCampID( repetion_id, vParame.entityid, GetPlayerNumByMapID( repetion_id, last_one_single.map_id) + 2 );
        time = GetTime( repetion_id, 1 );
        EventNoticeSingle( repetion_id, vParame.entityid, RepetionNoticeEventKind.COUNT_DOWN, 1, time);

        SetIntData2Player( repetion_id, vParame.entityid , LO_PLAYER_DATA_KEY.KILL_PALYER_NUM , 0 )
        SetIntData2Player(repetion_id, vParame.entityid , LO_PLAYER_DATA_KEY.LIVE_TIME, 0)

        ObserverEntityDeath(repetion_id, vParame.entityid)
    end


end

local function entity_death( repetion_id, vParame )

    if vParame.killerid ~= nil and vParame.killerid ~= vParame.killedid then
        --被人杀死
        level = GetRepeLevel( repetion_id)
        table.foreachi( LEVEL_QUOTIETY, function( i, v )
            if level >= v.min_level and level <= v.max_level then
                quotiety = v.quotiety   
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
        --杀人积分
        kill_make_point =  nNum * 70 * quotiety
        EventNoticeSingle( repetion_id, vParame.killerid, RepetionNoticeEventKind.TAKE_COUNT, 5, kill_make_point )

        --增加总积分 
        AddIntData2Player( repetion_id, vParame.killerid , LO_PLAYER_DATA_KEY.HISTORY_MARK_POINT, kill_make_point )

        --给被杀者结算积分
        account_mark_point( repetion_id, vParame.killedid)        

        --        DetachPlayerState( vParame.killedid , state_id.STATE_MASKED , 0, 0, 0)
        EventNoticeSingle( repetion_id, vParame.killedid, RepetionNoticeEventKind.TAKE_COUNT, 10, 2)
        PlayerLeaveDirect( repetion_id, vParame.killedid )
    elseif vParame.killerid == vParame.killedid then
        --被火焰吞噬
        EventNoticeSingle( repetion_id, vParame.killedid, RepetionNoticeEventKind.TAKE_COUNT, 10, 1)
        PlayerLeaveDirect( repetion_id, vParame.killedid )
    else
        PlayerLeaveDirect( repetion_id, vParame.killedid )
    end
end

local function repetion_begin( repetion_id, vParame )
    SetIntData( repetion_id, LO_DATA_KEY.MAX_KILL_NUM , 0)    
    CreateNpc( repetion_id, STONE_NPC[1][1])
end

local function leave_repetion( repetion_id, vParame )
    level = GetRepeLevel( repetion_id )
    table.foreachi( LEVEL_QUOTIETY, function( i, v )
        if level >= v.min_level and level <= v.max_level then
            quotiety = v.quotiety   
        end
    end);

    if GetIntData( repetion_id, LO_DATA_KEY.LO_PHASE) == 1 then

        --存活时间
        killed_live_time =  last_one_single.ballte_end_time   - GetTime(repetion_id, 2)
        EventNoticeSingle( repetion_id, vParame.entityid, RepetionNoticeEventKind.TAKE_COUNT, 3, killed_live_time)

        live_time_make_point = killed_live_time/1000 * quotiety 
        EventNoticeSingle( repetion_id, vParame.entityid, RepetionNoticeEventKind.TAKE_COUNT, 6, live_time_make_point)
        AddIntData2Player( repetion_id, vParame.killerid , LO_PLAYER_DATA_KEY.HISTORY_MARK_POINT, live_time_make_point )
        kill_num = GetIntData2Player( repetion_id, vParame.entityid, LO_PLAYER_DATA_KEY.KILL_PALYER_NUM )
        --计算获得经验值
        lo_exp = (100 + vParame.playerlevel * 8)* 150 * ( 1 + live_time_make_point/1500 + kill_num/15 )
    elseif GetIntData( repetion_id, LO_DATA_KEY.LO_PHASE) == 2 then
        --存活时间
        local killed_live_time =  last_one_single.ballte_end_time   
        EventNoticeSingle( repetion_id, vParame.entityid, RepetionNoticeEventKind.TAKE_COUNT, 3, killed_live_time)

        live_time_make_point = killed_live_time/1000 * quotiety 
        EventNoticeSingle( repetion_id, vParame.entityid, RepetionNoticeEventKind.TAKE_COUNT, 6, live_time_make_point)
        --AddIntData2AllPlayer( repetion_id, 0,live_time_make_point )
        AddIntData2Player( repetion_id, vParame.killerid , LO_PLAYER_DATA_KEY.HISTORY_MARK_POINT, live_time_make_point )
        kill_num = GetIntData2Player( repetion_id, vParame.entityid, LO_PLAYER_DATA_KEY.KILL_PALYER_NUM )
        --计算获得经验值
        lo_exp = (100 + vParame.playerlevel * 8)* 150 * ( 1 + live_time_make_point/1500 + kill_num/15 )
    else 
        live_time_make_point = 0 
        lo_exp = 0
    end


    ObtainRepetionExp( repetion_id, vParame.entityid, lo_exp)
    EventNoticeSingle( repetion_id, vParame.entityid, RepetionNoticeEventKind.TAKE_COUNT, 9, lo_exp)

    --场内剩余人数
    left_player_num = GetPlayerNumByMapID( repetion_id , last_one_single.map_id) - 1;

    EventNoticeAll( repetion_id ,RepetionNoticeEventKind.TAKE_COUNT , 7, left_player_num )

    -- 插入保护buff 这里插入buff为了防止再次死亡
    table.foreachi( lo_buffid.protect_buffid, function(i, v) 
        PlayerInsertBuffByMapID( repetion_id, v , args.map_id  );
    end);
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
    --    local killed_live_time = ( last_one_single.ballte_end_time - last_one_single.battle_begin_time ) * 1000 - GetTime(repetion_id, 2)
    --    EventNoticeSingle( repetion_id, vEntityID, RepetionNoticeEventKind.TAKE_COUNT, 3, killed_live_time)
    --    --存活积分
    --    local live_time_make_point = killed_live_time/1000000 * quotiety 
    --    EventNoticeSingle( repetion_id, vEntityID, RepetionNoticeEventKind.TAKE_COUNT, 6, live_time_make_point)
    --
    --    --增加总积分 
    --    AddIntData2Player(repetion_id, vEntityID, LO_PLAYER_DATA_KEY.MAKE_POINT, live_time_make_point  )
    --
end


-- 副本配置如下
repetion = 
{

    baseinfo = 
    {
        repetion_index = RepetionID.LAST_ONE_TEAM,
        playercountmax = 400,		-- 副本最大人数
        mapid = { last_one_single.map_id },--90,			-- 副本地图ID
        taskid = 0,			-- 进入副本的前置任务
        posx = 94,				-- 暂时不用
        posy = 124,				-- 暂时不用
        pernumber = 0,			-- 每天进入副本的次数； 0-不限
        minlevel = 50,			-- 最低进入等级
        maxlevel = 110,
        unrelive = 1,
        team_level_flag = 1,

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
        signup_open =
        {

            {   
                start = { year = 0, month = 0, week = 7, day = 0, hr = 13, minute = 30, sec = 0 },
                finish = { year = 0, month = 0, week = 7, day = 0, hr = 14, minute = 0, sec = 0 } 
            },  

            {   
                start = { year = 0, month = 0, week = 7, day = 0, hr = 15, minute = 30, sec = 0 },
                finish = { year = 0, month = 0, week = 7, day = 0, hr = 16, minute = 0, sec = 0 } 
            },  

            {   
                start = { year = 0, month = 0, week = 7, day = 0, hr = 17, minute = 30, sec = 0 },
                finish = { year = 0, month = 0, week = 7, day = 0, hr = 18, minute = 0, sec = 0 } 
            },  
        }
    },

    battle =
    {

        level =
        {
            { levelmin = 50,    levelmax = 69},
            { levelmin = 70,    levelmax = 79 },
            { levelmin = 80,    levelmax = 89 },
            { levelmin = 90,    levelmax = 99 },
            { levelmin = 100,    levelmax = 110 },
        },          

        open =  
        {
            {
                start = { year = 0, month = 0, week = 0, day = 0, hr = 14, minute = 0, sec = 0 },
                finish = { year = 0, month = 0, week = 0, day = 0, hr = 14, minute = 0, sec = 10 }
            },
            {
                start = { year = 0, month = 0, week = 7, day = 0, hr = 16, minute = 0, sec = 0 },
                finish = { year = 0, month = 0, week = 7, day = 0, hr = 16, minute = 0, sec = 10 }
            },
            {
                start = { year = 0, month = 0, week = 7, day = 0, hr = 18, minute = 0, sec = 0 },
                finish = { year = 0, month = 0, week = 7, day = 0, hr = 18, minute = 0, sec = 10 }
            },
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
            time = last_one_single.ballte_end_time,
            loop = 1,
            func = end_battle,
            args = { map_id = last_one_single.map_id },
            sync = { kind = RepetionTimeSyncKind.ALL, key = 2 }
        },

        --3 创建神符
        {
            kind = RepetionTimeBeginKind.START_BEGIN,
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
            loop = 7,
            func = create_hierogram_time,
            args = { }
        },
        --5
        {   
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = last_one_single.close_time_left,
            loop = 1,
            func = close_reption, 
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
            loop = 9,
            func = create_redstone,
            args = { }
        },
        --8 创建火焰
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = last_one_single.creat_needfire_time,
            loop = 22,
            func = creat_needfire,
            args = { }

        },
        --9火焰创建显示倒计时
        {
            kind = RepetionTimeBeginKind.EVENT_BEGIN,
            result = RepetionTimeResultKind.NONE,
            time = last_one_single.needfire_begin_time,
            loop = 1,
            func = needfire_begin_time,
            args = { }

        }

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
    },
}
