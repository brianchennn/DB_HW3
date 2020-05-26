

/*question 8*/
select C.champion_name
from champ as C
where C.champion_name not in(
        select distinct C.champion_name
        from (
            select T.champion_id
            from (select M.match_id,M.version from match_info as M where M.version like '7.7.%')    as MM, teamban as T
            where MM.match_id  = T.match_id
        )as new_t, champ as C
        where new_t.champion_id = C.champion_id)
order by C.champion_name asc;
