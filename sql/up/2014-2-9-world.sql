-- ���²���ֻ���YOLINLINԭʼ���ݿ�

-- ��ֹ��ʴDBC���ش���

-- �޸��Ի����벢����
UPDATE `quest_template` SET `OfferRewardText` = '<�������Ķ�����ż��������е����ݸе��𾪡�>$B$B����һֱ���������Ĳ���ͱ�������֮��ĺ�����ϵ���������֤ʵ�����ǵ����ǡ����ű������߰��й�Ӧ��ħ����װ�����ֲ�ͼ�ڽ���Ϊ�����µĵ��ˡ����ֶ������ƽ�������˶���Ķ���������' Where `id` = 11185;
UPDATE `quest_template` SET `OfferRewardText` = '��������ŵ�������ʵ����ô�������߱����������Ľ���! $B$B�ֲ�ͼ����Ū����ЩС����һֱ�����������������������ġ����ǹ�ȥһֱ������Ϊţͷ��һ������⣬��������Ž��ı���һ�С� $B$B������ҪһЩʱ������������£����Ǳ��������̸���������������š�' Where `id` = 11186;

-- �ֲ�ͼ��˫ɱ����
UPDATE `creature_template` set `KillCredit1` = 0 Where `entry` = 23595;

-- ����Ħ���Ǳ�еȼ���30831��̫��,վ��ǰ��������
UPDATE `creature_template_addon` set `auras` = '20540' Where `entry` = 4834;

-- ���ߵ��Ļ겻��Ҫ�Ի�
UPDATE `creature_template` set `npcflag` = 0 Where `entry` = 23861;

-- ���ײݲ����������
DELETE FROM `creature_loot_template` Where `item` = 33112;

-- ֩�������
UPDATE `creature_loot_template` set `mincountOrRef` = 2 Where `item` = 5884;
UPDATE `creature_loot_template` set `maxcount`      = 2 Where `item` = 5884;


-- TC˵��ʲô�ã��������޸��˾Ͳ����Լ�����һ���𣿷ǵ����Ҳ���д����
-- ˳���޸��������˶Ի�
-- �޸��Ի����벢����(����)
UPDATE `quest_template` SET `OfferRewardText` = '<Ѳ��Ա��һ�ֲ����ε��������㡣 > $B$B����˵��Ҫ��...Э����εĵ��顣�õúܡ� $B$B���ڷ���ʹ����Щ�۳�����������Ѱ�ҿ��ܱ��ϳ������ߵ��������������κ��빥����صĺۼ����⡣��Ҫ��ԥȥ������п��ɵĶ�����' Where `id` = 11124;
UPDATE `quest_template` SET `OfferRewardText` = '��ӡ�������룬�п�����Ұ���ˣ������ǰ�����ƶ�֮���в��ٰ�����Ҫ˵����������������飬��һ��Ҳ��������֡�$B$B�Ҹ���˵����?���������õ걻���ˡ��������������źͺ��Ӷ���ɱ�ˣ�������̫�����ˡ�$B$B�����������һ��ɿ���¡ȥ�鿴��Щ�㼣ͨ�����$B' Where `id` = 1268;
UPDATE `quest_template` SET `OfferRewardText` = '����ʶ��������߶�����ɪ���ˡ����Ǹ������ļһ����������ͬ�飬һ���˶��������$B$B�����˰¸�¡ȥ׷�����������˻�����ɪ͵�����ǵ�����װ��������¸�¡�����Ϸ���ȥ�ˡ�$B' Where `id` = 1269;
UPDATE `quest_template` SET `OfferRewardText` = '��Ȼ�����װ��Ʒζ����֣������Ҿ������ǲ����һ�����ܹ��ڱ�¯�Ϸ���װ�Ρ������ǷŻ����ùݵ��˷����Ƕ��ġ�$B$B���Ǹ�����?�����Ǹ���в?$B' Where `id` = 1251;
UPDATE `quest_template` SET `OfferRewardText` = '������ƺܲ��������ּ�Ӳ�������Ÿ߶�����Ҫ�����Լ������ƣ�������ƣ�$N��$B$B��Ӧ��֪����$B' Where `id` = 1321;
UPDATE `quest_template` SET `OfferRewardText` = '�ҿɲ�ϲ����ô���֩�룬��Ϊ�Ҳ���һ�Ű����Ǹ��ȱ⡣$B$Bлл��������Щ��$N�����Ұ������ң��Ѷ�Һ�ε������ϡ���' Where `id` = 1322;
UPDATE `quest_template` SET `OfferRewardText` = 'Ī����?�ܶ��졭������һʱ֮�䡭��Ҳ������������������$B$B���ܲ���ʲô��Ҫ�Ķ������������ûʲô�����ԣ���һ����ʲô��������塭��$B��' Where `id` = 1323;
UPDATE `quest_template` SET `OfferRewardText` = '�ǵģ����ȷ�ǶԵģ����ȷ�������Ķ��ơ�$B$B�����ҿ���Ҳû��̫�����õ���Ϣ���Ը����㣬$N���Ҵ����˺ܶ಻ͬ�Ķ��ƺͻ��ף�������֪��ÿһ��������ȥ����$B' Where `id` = 1276;
UPDATE `quest_template` SET `OfferRewardText` = '��!��ɪ�����ٹ�����������������ˣ�������̫���ˡ��������඼��ô�����������СС�ļ��˶��ָ����ˣ��湻�����ġ�$B$B���������ƺ���û�н�չ�ˣ����ǵ���������������' Where `id` = 1273;
-- �޸��Ի����벢����(����)
UPDATE `quest_template` SET `OfferRewardText` = '<Ѳ��Ա��һ�ֲ����ε��������㡣 > $B$B����˵��Ҫ��...Э����εĵ��顣�õúܡ� $B$B���ڷ���ʹ����Щ�۳�����������Ѱ�ҿ��ܱ��ϳ������ߵ��������������κ��빥����صĺۼ����⡣��Ҫ��ԥȥ������п��ɵĶ�����' Where `id` = 11123;
UPDATE `quest_template` SET `OfferRewardText` = '��ӡ������?�������󸽽���ȷ��Ϊ���Ǳ�Ѱ���İ������ڻ��������ȥƶ�֮�ص�·�Ͽ��������ǵ�Ӫ�غ;ݵ㡣$B$B�һ��ɳ����ǵ���������������˹�϶����������ܲ����ҵ���Щ��ӡͨ���Ķ���$B' Where `id` = 1284;
UPDATE `quest_template` SET `OfferRewardText` = '�ҵ���������Ļ��¶�����˵�ɲ���ʲô����Ϣ�� $B$B��ʱ�������ζ����������ս���ϣ����һ��ðѱ��˵���Ϣ�������ǵļ��ˣ���ʱ������Զ��֪�����Ƿ�����ʲô������Щʱ��������ܱ��������еĸ���⡣' Where `id` = 1252;
UPDATE `quest_template` SET `OfferRewardText` = '�ţ��ҿɲ������������ͻ��׵�ר�ҡ���֪�����ǵ���;�����ǲ���������졣$B$B������ƺ�һ�����ûʲô����ֻ���ùݵĴ��������������Щ�ۼ���' Where `id` = 1253;
-- �����3�����񣨲��䣩
UPDATE `quest_template` SET `PrevQuestId` = 11124 WHERE `id` = 1268;
UPDATE `quest_template` SET `PrevQuestId` = 11124 WHERE `id` = 1269;
UPDATE `quest_template` SET `PrevQuestId` = 11124 WHERE `id` = 1251;
-- �����3���������ˣ�
UPDATE `quest_template` SET `PrevQuestId` = 11123 WHERE `id` = 1284;
UPDATE `quest_template` SET `PrevQuestId` = 11123 WHERE `id` = 1252;
UPDATE `quest_template` SET `PrevQuestId` = 11123 WHERE `id` = 1253;
-- ��д����ԱSAI
UPDATE `creature_template` SET AIName='SmartAI' WHERE `entry` = 23567;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 23567;
INSERT INTO `smart_scripts` VALUES
(23567,0,0,0,20,0,100,1,11123,0,0,0, 1,    0,0,0,0,0,0,1,0,0,0,0,0,0,0,""),
(23567,0,1,0,20,0,100,1,11124,0,0,0, 1,    0,0,0,0,0,0,1,0,0,0,0,0,0,0,""),
(23567,0,2,3,62,0,100,0, 8761,0,0,0,11,42169,0,0,0,0,0,7,0,0,0,0,0,0,0,""),
(23567,0,3,0,61,0,100,0,    0,0,0,0,72,    0,0,0,0,0,0,7,0,0,0,0,0,0,0,"");
-- �¸�¡����
UPDATE `creature_template` SET `unit_flags`=   0 WHERE `entry` = 4983;
UPDATE `creature_template` SET `faction_A` = 232 WHERE `entry` = 4983;
UPDATE `creature_template` SET `faction_H` = 232 WHERE `entry` = 4983;
-- ���SAI����
UPDATE `creature_template` SET AIName='SmartAI' WHERE `entry` = 4983;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 4983;
INSERT INTO `smart_scripts` VALUES
(4983,0,0,0, 6,0,100,1,   0,   0,0,0, 6,    1273,0,0,0,0,0,7,0,0,0,0,0,0,      0,"�¸�¡��������ʧ��"),
(4983,0,1,0,19,0,100,1,1273,   0,0,0,80,  498300,0,0,0,0,0,1,0,0,0,0,0,0,      0,"����ʼִ�нű�00"),
(4983,0,2,0,40,0,100,1,   5,498300,0,0,54,  3000,0,0,0,0,0,1,0,0,0,0,0,0,      0,"·��5����ͣ"),
(4983,0,3,4,55,0,100,1,   5,498300,0,0, 1,     1,0,0,0,0,0,1,0,0,0,0,0,0,      0,"·��5��˵��"),
(4983,0,4,0,61,0,100,1,   0,     0,0,0,66,       0,0,0,0,0,0,8,0,0,0,0,0,0,6.281,"·��5��ת����ɪ"),
(4983,0,5,0,58,0,100,1,   7,498300,0,0,80,498301,0,0,0,0,0,1,0,0,0,0,0,0,      0,"·��7ִ�нű�01"),
(4983,0,6,0, 6,0,100,1, 500,1500,0,0, 1,       8,0,0,0,0,0,1,0,0,0,0,0,0,      0,"ս���԰�"),
(4983,0,7,0,58,0,100,1,   2,498301,0,0,80,498302,0,0,0,0,0,1,0,0,0,0,0,0,      0,"·��7ִ�нű�02");
DELETE FROM `smart_scripts` WHERE `entryorguid` = 498001 and `id` =  7;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 498301 and `id` = 13;
INSERT INTO `smart_scripts` VALUES
(498301,9,13,0, 0,0,100,0,13000,13000,0,0, 53,0,498301,0,0,0,2,1,0,0,0,0,0,0,    0,"�ٻ�13S���ƶ�");
UPDATE `smart_scripts` SET `action_param2` =498300 WHERE `entryorguid` = 498300 and `id` = 1;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 5046 and `id` = 2;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 498302;
INSERT INTO `smart_scripts` VALUES
(498302,9,0,0, 0,0,100,1, 2000, 2000,0,0,  1,6,0,0,0,0,0,1,0,0,0,0,0,0,    0,"�Ի�"),
(498302,9,1,0, 0,0,100,1, 2000, 2000,0,0,  1,7,0,0,0,0,0,1,0,0,0,0,0,0,    0,"�Ի�"),
(498302,9,2,0, 0,0,100,1,10000,10000,0,0, 41,0,0,0,0,0,0,1,0,0,0,0,0,0,    0,"NPC��ʧ");
-- ����·��
DELETE FROM `waypoints` WHERE `entry` = 4983;
DELETE FROM `waypoints` WHERE `entry` = 498300;
DELETE FROM `waypoints` WHERE `entry` = 498301;
INSERT INTO `waypoints` VALUES
(498300,1,-3349.78,-3134.81,28.5267,"Ogron"),
(498300,2,-3366.01,-3144.31,35.2659,"Ogron"),
(498300,3,-3367.34,-3164.11,35.9618,"Ogron"),
(498300,4,-3372.77,-3174.81,35.8517,"Ogron"),
(498300,5,-3378.53,-3191.64,35.7204,"Ogron"),
(498300,6,-3370.92,-3190.48,34.1671,"Ogron"),
(498300,7,-3366.86,-3209.47,33.9713,"Ogron");
INSERT INTO `waypoints` VALUES
(498301,1,-3368.86,-3211.39,34.3195,"Ogron"),
(498301,2,-3366.86,-3209.47,33.9713,"Ogron");
-- �������(��ħ->ʳ��ħ)
UPDATE `creature_text` SET `text` = '�ҡ����ðɣ���Ҳ����õ�������һЩ������������Ϊʲôʳ��ħҪ�������?' WHERE `entry` = 4980 and `groupid` = 2;
UPDATE `creature_text` SET `text` = '��Ҫ������ʳ��ħ!' WHERE `entry` = 4980 and `groupid` = 3;
UPDATE `creature_text` SET `text` = '���߶�����ɪ!����ҵ����ˡ������ں�ʳ��ħ����һ����?��Ҫ���£������Ǳ�����Ҳ��õ���ˡ��' WHERE `entry` = 5046 and `groupid` = 0;




-- ���²���ֻ���YOLINLINԭʼ���ݿ�
DELETE FROM `smart_scripts` WHERE `entryorguid` = 24601;
INSERT INTO `smart_scripts` VALUES
(24601,0,0,0, 11,0,100,0,   0,   0,    0,    0, 11,36151,0,0,0,0,0,1,0,0,0,0,0,0,0,"Steam Rager - On Respawn - Cast Cosmetic: Steam Aura"),
(24601,0,1,0,  0,0,100,0,4000,8000,11000,14000, 11,50375,1,0,0,0,0,2,0,0,0,0,0,0,0,"Steam Rager - In Combat - Cast Steam Blast"),
(24601,0,2,0,  6,0,100,0,   0,   0,    0,    0, 11,46377,0,0,0,0,0,7,0,0,0,0,0,0,0,"Steam Rager - On Death - Kill Credit");










