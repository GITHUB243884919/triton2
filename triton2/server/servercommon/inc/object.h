#if 0
/******************************************************************************
   版权所有 (C ), 2008-2018, 蓝港在线（北京）科技有限公司


******************************************************************************
  文件名          : object.h
  版本号          : 1.0
  作者            : ZHL
  生成日期        : 2008-06-05
  最近修改        : 
  功能描述        : 主要是为放在共享内存中的对象使用，作为统一的基类，便于数据和接口分离。				
  函数列表        : 
  修改历史        : 
  1 日期          : 2008-06-05
    作者          : ZHL
    修改内容      : 生成
*******************************************************************************/
#endif
#pragma once

//// 对象创建模式
//enum
//{
//	Init	= 0,	// 初始化模式
//	Resume	= 1,	// 恢复模式
//};


#define OBJ_ID_BITS (24)  // 对象ID的范围最大为2^^24,即16777216个
#define INVALID_OBJ_ID	0 // 无效对象ID

class CObj
{
public:
	CObj()
	{
		// 只在初始化模式下, 设置id_为非法值
		if( msCreateMode == 1 )
		{
			invalid();
		}
	}

	virtual ~CObj()
	{
		// 在析构的时候，设置id_，可以保证内存中id_数据的合法性
		// 这是因为有时候需要通过id来访问数据，而在共享内存中的数据，并不会真正析构
		id_ = INVALID_OBJ_ID;
	}

	int get_id() const
	{
		return id_;
	}

	void set_id(int id)
	{
		id_ = id;
	}

	static int ID2TYPE(int id)
	{
		// 先移位然后计算结果，可以避免得到负数
		return (id >> OBJ_ID_BITS) & 0X000000FF;
	}

	// 判断object是否合法
	// 由于开启异常处理不太方便，在这里加上这个函数。如果构造函数失败，只需要设置id =INVALID_OBJ_ID即可
	// 这种方式虽然比较丑类,但是可以比较有效的不使用异常来处理构造函数中发现的错误
	int valid() const
	{
		return INVALID_OBJ_ID != id_;
	}

	// 设置对象非法
	void invalid()
	{
		id_ = INVALID_OBJ_ID;
	}

	// 初始化对象数据( 构造函数中根据共享内存启动模式调用 )
	virtual int Initialize() = 0;

	// 恢复对象数据-继续保留内存中的数据,用于共享内存热启动 ( 构造函数中根据共享内存启动模式调用 )
	virtual int Resume() = 0; 


private:
	int id_; // 所有对象的唯一标识

public:
	static char msCreateMode; // 对象创建方式
};                                               
                                                           



