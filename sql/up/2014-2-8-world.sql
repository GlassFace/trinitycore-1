-- �Ӵ��ֺ���
UPDATE `quest_template` SET `OfferRewardText` = '���ȿ��������ģ�����?��Ҫ֪���������ȿ�����ȥΪ���ӹ���֮ǰ�����ǰ������������𾴵Ĺ���֮һ��$B$B�����������Ӻ�ĵ�һ���¾��ǿ�����˫���������������û��Ϊ����Э�Ṥ��������Щ�¶��Ͳ�����ô˳���ˣ���˭���������أ��԰�?��û�б����������˹��ĵ���!$B' Where `id` = 1178;



-- �޸�����-��ķʲ��ϲ��
UPDATE `smart_scripts` SET `action_param6` = 0 Where `entryorguid` = 779000 and `id` = 2;
UPDATE `smart_scripts` SET `target_type`   = 7 Where `entryorguid` = 779000 and `id` = 2;
-- ???��?��??30S????15S
UPDATE `smart_scripts` SET `event_param1` = 1000 Where `entryorguid` = 779000 and `id` = 0;
UPDATE `smart_scripts` SET `event_param2` = 1000 Where `entryorguid` = 779000 and `id` = 0;
UPDATE `smart_scripts` SET `event_param1` = 15000 Where `entryorguid` = 779000 and `id` = 1;
UPDATE `smart_scripts` SET `event_param2` = 15000 Where `entryorguid` = 779000 and `id` = 1;

-- ?????��??33S????3S
UPDATE `smart_scripts` SET `event_param1` = 3000 Where `entryorguid` = 779000 and `id` = 2;
UPDATE `smart_scripts` SET `event_param2` = 3000 Where `entryorguid` = 779000 and `id` = 2;

UPDATE `smart_scripts` SET `event_flags` = 1     Where `entryorguid` = 779000;


-- ???��???��??
UPDATE `smart_scripts` SET `action_param6` = 0 Where `entryorguid` = 785300 and `id` = 0;
UPDATE `smart_scripts` SET `target_type`   = 7 Where `entryorguid` = 785300 and `id` = 0; 





 










