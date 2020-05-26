
/*question 3*/
select C.champion_name as champion_name,count(P.match_id) as cnt
from champ as C, (select PP.match_id,PP.champion_id from participant as PP where PP.position="JUNGLE") as P
where C.champion_id = P.champion_id
group by P.champion_id 
order by cnt desc
limit 3;
