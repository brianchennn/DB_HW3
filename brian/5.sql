
/*question 5 */
select if(new_t.win=0,'lose','win') as win_lose, count(new_t.match_id) as cnt
from(
    select P.match_id, S.win, AVG(S.longesttimespentliving)
    from participant P,stat S
    where P.player_id = S.player_id
    group by P.match_id,S.win
    having 1200 <= AVG(S.longesttimespentliving) 
    order by P.match_id desc
) as new_t
group by new_t.win;
