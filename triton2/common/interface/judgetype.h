#pragma once

// add need data struct

#define JUDGE_SCORE_MAX		500

#define JUDGE_COOL_TIME		300	// 秒
#define JUDGE_BASE_LEVEL	30	// 可评价级别

#define JUDGE_OTHER_MAX		20	// 评价人数上限
#define RECORD_EXIST		1	// 

#define ERR_NEED_NOTIFY(x) ( x < -47000 ? true : false )	

// 评价类型
enum JudgeType
{
	Judge_Favor = 0,	// 好感
	Judge_Bad,			// 恶感
};

// 查询类型
enum JudgeQueryType
{
	Judge_Query_Time = 0,	// 时间排序
	Judge_Query_Value,		// 值排序
	Judge_Query_Sum,		// Sum
	Judge_Query_Rec_Cnt,	// 收到评价个数
	Judge_Query_Rec_Max,	// 收到的评价最大值
	Judge_Query_Rec_Sum,	// 收到的总值
};

// 行为类型
enum JudgeActionType
{
	Judge_Other = 0,	// 对他人
	Judge_Rec,			// 收到得
};