
/*question 10 Renekton:58*/
EXPLAIN 
select T.self_name as self_champ_name, sum(T.self_win)/count(T.self_win) as win_ratio, 
(sum(T.self_kills)+sum(T.self_assists))/sum(T.self_deaths) as self_kda, avg(T.self_goldearned) as self_avg_gold,T.enemy_name as enemy_champ_name, 
(sum(T.enemy_kills)+sum(T.enemy_assists))/sum(T.enemy_deaths) as enemy_kda, avg(T.enemy_goldearned) as enemy_avg_gold, count(*) as battle_record

from (
select t1.match_id ,t1.champion_name as self_name,t1.win as self_win, t1.kills as self_kills,t1.assists as self_assists,
 t1.deaths as self_deaths, t1.goldearned as self_goldearned ,t2.champion_name as enemy_name,t2.kills as enemy_kills,t2.assists as enemy_assists,
 t2.deaths as enemy_deaths,t2.goldearned as enemy_goldearned
from(
    select PP.player_id,PP.match_id,PP.champion_id,C.champion_name,S.win,S.kills,S.assists,S.deaths,S.goldearned
    from(
        select P.match_id,P.champion_id
        from participant as P 
        where  P.position = 'TOP' and P.champion_id=58
    )as t1,participant as PP, champ as C,stat as S
    where PP.match_id = t1.match_id and PP.position='TOP' and PP.champion_id= C.champion_id and S.player_id=PP.player_id and PP.champion_id != 58
) as t1,
(
    select PP.player_id,PP.match_id,PP.champion_id,C.champion_name,S.win,S.kills,S.assists,S.deaths,S.goldearned
    from(
        select P.match_id,P.champion_id
        from participant as P 
        where  P.position = 'TOP' and P.champion_id=58
    )as t1,participant as PP, champ as C,stat as S
    where PP.match_id = t1.match_id and PP.position='TOP' and PP.champion_id= C.champion_id and S.player_id=PP.player_id and PP.champion_id = 58
) as t2
where t1.match_id = t2.match_id
) as T
group by T.self_name
having count(*) > 100
order by win_ratio DESC
limit 5;

