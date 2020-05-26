
/*question 4*/
select M.match_id ,sec_to_time(M.duration) as time
from match_info as M 
order by time desc
limit 5;
