
/*question 7*/
select new2_t.position, C.champion_name, new2_t.max_kda
from(
    select new_t.position,max(new_t.kda) as max_kda
    from(
        select P.position,P.champion_id,(sum(S.kills)+sum(S.assists))/sum(S.deaths) as kda
        from participant as P, stat as S
        where P.player_id = S.player_id
        group by P.position,P.champion_id
        order by kda desc
    )as new_t
    where new_t.position != 'DUO' and new_t.position != 'SOLO' and new_t.position != 'NONE' 
    group by new_t.position
)as new2_t,(
    select P.position,P.champion_id,(sum(S.kills)+sum(S.assists))/sum(S.deaths) as kda
    from participant as P, stat as S
    where P.player_id = S.player_id
    group by P.position,P.champion_id
    order by kda desc
    ) as new3_t, champ as C
where C.champion_id = new3_t.champion_id and new3_t.kda = new2_t.max_kda
order by new2_t.position asc;