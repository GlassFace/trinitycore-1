-- ���²���ֻ���YOLINLINԭʼ���ݿ�
-- ������������������
DELETE FROM `creature_loot_template` Where `item` = 33071;


-- ��β�ݲ����������
DELETE FROM `creature_loot_template` Where `item` = 33175;

-- ��������������ʧ�Ի�
DELETE FROM `creature_text` WHERE `entry` = 4880;
INSERT INTO `creature_text` VALUES
(4880,0,0,"�õģ����ǳ����ɣ�",12,0,100,0,0,0,           "stinky - quest accepted"),
(4880,1,0,"����������ҪȥѰ��˯���ˡ�",12,0,100,0,0,0,   "stinky - say1"),
(4880,2,0,"������û��...",12,0,100,0,0,0,                "stinky - say2"),
(4880,3,0,"�⸽���϶����У����ٻ���һ��...",12,0,100,0,0,0,"stinky - say3"),
(4880,4,0,"��������һ�꣡",12,0,100,0,0,0,                  "stinky - say4"),
(4880,5,0,"����,$N!���ǸϿ��ȥ�ɼ���",12,0,100,0,0,0,      "stinky - say5"),
(4880,6,0,"���ˣ�����ץ��ʱ���뿪�ɣ�",12,0,100,0,0,0,       "stinky - say6"),
(4880,7,0,"�ҿ����Լ���ȥ�ġ�лл�㣬$N��ȥ���ҵ��ϰ�̸̸������úý�����ģ�",12,0,100,0,0,0,"stinky - quest complete"),
(4880,8,0,"������������Ұ���ڹ����ң�",12,0,100,0,0,0,       "stinky - aggro"),
(4880,9,0,"%S��ʧ�ڴ����",12,0,100,0,0,0,                 "stinky - emote disapper");

UPDATE `item_template` SET `BuyPrice` = 28 WHERE `entry` = 17058;
UPDATE `item_template` SET `SellPrice` = 7 WHERE `entry` = 17058;