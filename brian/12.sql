
/*question 12*/
select t1.total_triplekills as tri_kill_num, sum(t1.win)/count(t1.win) as win_ratio
from(select P.match_id, if(P.player>=6,'R','B'), sum(S.doublekills) as total_doublekills,sum(S.triplekills) as total_triplekills, S.win
    from stat as S,participant as P
    where P.player_id = S.player_id 
    group by P.match_id,P.player,S.win
    having total_triplekills > 0) as t1
group by t1.total_triplekills;


