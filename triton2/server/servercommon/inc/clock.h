// ***************************************************************
//  File:	clock.h   version:  1.0
//	Date:	05/23/2008
// 	Desc:	封装时钟函数
//  
//  Copyright (C) 2008 - LineKong Entertainment Technology Co.,Ltd
//						All Rights Reserved 
// ***************************************************************
#pragma once



class Clock
{
public:
    Clock();
    ~Clock();

    bool				active() const	{ return _active; }
    int					elapsed() const;
    unsigned int		start();
    unsigned int		reset() { return start(); }
    int					stop();

    static unsigned long long   now();

private:

    unsigned int	_start;		/// 开始的时间
    int				_elapsed;	/// 结束时经过的时间
    bool			_active;	/// 是否处于活动状态
};
