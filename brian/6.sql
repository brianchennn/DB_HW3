
/*question 6 */
-- SET optimizer_trace="enabled=on";
-- explain 
select T1.position,C.champion_name
from(
    select t1.position,max(t1.cnt) as max_cnt
    from(select P.position,P.champion_id,count(*) cnt
        from participant as P, match_info as M
        where M.duration between 2400 and 3000 and P.match_id=M.match_id
        group by P.position ,P.champion_id  
        order by cnt desc
    )as t1
    group by t1.position
)as T1,
(
    select P.position,P.champion_id,count(*) cnt
    from participant as P, match_info as M
    where M.duration between 2400 and 3000 and P.match_id=M.match_id 
    group by P.position ,P.champion_id  
    order by cnt desc
)as T2,
champ as C
where T1.position=T2.position and T1.max_cnt=T2.cnt and C.champion_id=T2.champion_id and T1.position != "DUO" 
        and T1.position != "SOLO" and T1.position != "NONE" 
order by T1.position;
 -- SET optimizer_trace="enabled=off";