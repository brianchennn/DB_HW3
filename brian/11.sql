
/*question 11*/


select  P.ss1,P.ss2,sum(S.win)/count(S.win) as win_rate ,sum(S.win) as n
from stat as S,participant as P
where S.player_id=P.player_id and P.ss1='Flash' and P.ss2='Ignite'and P.position='TOP';

select  P.ss1,P.ss2,sum(S.win)/count(S.win) as win_rate,sum(S.win) as n
from stat as S,participant as P
where S.player_id=P.player_id and P.ss1='Flash' and P.ss2='Teleport' and P.position='TOP';
