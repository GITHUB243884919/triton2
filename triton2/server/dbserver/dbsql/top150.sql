select c.user_name, b.role_name, b.metier_id, a.order_id, a.rank_value, a.value2 from 
(
select role_id, order_id, rank_value, value2 from UMS_RANK_PLAYER_OUTDATED
where rank_date = '2010-03-30' and rank_type = 1 and rank_country = 1
) a, UMS_ROLE b, UMS_ACCOUNT c
where a.role_id = b.role_id and b.account_id = c.account_id
 order by a.order_id;
 