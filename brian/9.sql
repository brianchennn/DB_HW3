

/*question 9     Teemo:17, Lee_Sin:64*/

select substring_index(M.version,'.',2)as version,sum(new_t.win) as win_cnt,count(win)-sum(win) as lose_cnt, sum(new_t.win)/count(win) as win_ratio
from(
        select PP.match_id, PP.Team,S.win    
        from(
            select P.player_id,P.match_id,P.champion_id,P.player, if(P.player>=6,'R','B') as Team
            from participant as P 
            where P.champion_id=17 or P.champion_id=64

        ) as PP,stat as S
        where PP.player_id = S.player_id
        group by PP.match_id,PP.Team,S.win 
        HAVING  sum(PP.champion_id)=81
    )as new_t ,match_info as M
where M.match_id = new_t.match_id  
group by substring_index(M.version,'.',2);