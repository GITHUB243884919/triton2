
--Apex2系统中的踢人时的错误号列表.

function AddErrNo()	

				AddErrAction( 61039, 1 );	
				AddErrAction( 61061, 1 );
				AddErrAction( 61112, 1 );
				AddErrAction( 61073, 1 );
				AddErrAction( 63001, 1 );
				AddErrAction( 63002, 1 );
				AddErrAction( 61033, 1 );
				AddErrAction( 61087, 1 );
				AddErrAction( 61086, 1 );
				AddErrAction( 61084, 1 );
				AddErrAction( 61094, 1 );
				AddErrAction( 61118, 1 );
				AddErrAction( 47200, 1 );
	
			    AddErrActionRange( 41100, 41200, 1 );
			    AddErrActionRange( 41700, 41750, 1 );
				AddErrActionRange( 43000, 43100, 1 );
				AddErrActionRange( 44000, 44010, 1 );
				AddErrActionRange( 47000, 47050, 1 );
				AddErrActionRange( 47500, 47530, 1 );
				AddErrActionRange( 47600, 47631, 1 );
				AddErrActionRange( 62500, 62505, 1 );
					
				DelErrNo( 41116);
				DelErrNo( 47022);

			
				AddKillTimes_Every(1000); 
				AddKillTimes_One(47011,200000); 
				AddKillTimes_One(61112,200000); 
				AddKillTimes_One(61073,50000);
				AddKillTimes_One(47009,50000);  
				AddKillTimes_One(47501,20000); 
				AddKillTimes_One(47001,50000);
				AddKillTimes_One(63001,50000);
				AddKillTimes_One(63002,50000);
				AddKillTimes_One(61094,50000);
				AddKillTimes_One(62500,200000);
				AddKillTimes_One(62501,200000);
				AddKillTimes_One(61086,200000);
				AddKillTimes_One(61087,200000);
				AddKillTimes_One(62502,200000);
				AddKillTimes_One(62503,200000);
				AddKillTimes_One(61118,200000);
				AddKillTimes_One(47200,200000);
				
				
				SetKillSpeed_Every(30);          --单位时间内设置每个错误号踢人的次数			
				SetKillSpeed_One(47011,500);     --单位时间内单独设置错误号踢人的次数
				SetKillSpeed_One(61112,500);
				SetKillSpeed_One(61073,80);
				SetKillSpeed_One(47009,100);
				SetKillSpeed_One(47001,200);
				SetKillSpeed_One(63001,200);
				SetKillSpeed_One(63002,200);
				SetKillSpeed_One(61094,200);
				SetKillSpeed_One(62500,500);
				SetKillSpeed_One(62501,500);
				SetKillSpeed_One(61086,500);
				SetKillSpeed_One(61087,500);
				SetKillSpeed_One(62502,500);
				SetKillSpeed_One(62503,500);
				SetKillSpeed_One(61118,500);
				SetKillSpeed_One(47200,500);
				SetKillSpeed_UnitTime(1*60*1000);--设置踢人的时间长度

				
				--AddKillTimes_One(61062,400);         --此错误号第三次就不踢了,这个次数按实际情况设定
				SetDelayKill( 1, 1 );                --第一个参数是延时开关，第二个参数是延时偏差
				--AddKillDelay( 61081, 10000);         --第一个参数错误号，第二个参数踢人延时时长，即在查到后直到10s才踢
				AddKillDelay( 47200, 20000);
end

AddErrNo();


