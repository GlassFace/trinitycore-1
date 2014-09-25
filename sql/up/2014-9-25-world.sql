-- 其拉斩灵者
UPDATE `creature_template` SET AIName='SmartAI' WHERE `entry` = 15246;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 15246;
INSERT INTO `smart_scripts` VALUES
(15246,0,0,0,0,0,100,2,  500,  500,10000,10000,11,26079,0,0,0,0,0,5,0,0,0,0,0,0,0,"Qiraji Mindslayer - In Combat - Cast 'Cause Insanity' (No Repeat) (Normal Dungeon)"),
(15246,0,1,0,0,0,100,2,23000,23000,23000,23000,11,26049,0,0,0,0,0,2,0,0,0,0,0,0,0,"Qiraji Mindslayer - In Combat - Cast 'Mana Burn' (No Repeat) (Normal Dungeon)"),
(15246,0,2,0,0,0,100,2, 5000, 5000,10000,10000,11,26048,0,0,0,0,0,2,0,0,0,0,0,0,0,"Qiraji Mindslayer - In Combat - Cast 'Mind Blast' (No Repeat) (Normal Dungeon)"),
(15246,0,3,0,0,0,100,2, 7000, 7000,10000,10000,11,26044,0,0,0,0,0,2,0,0,0,0,0,0,0,"Qiraji Mindslayer - In Combat - Cast 'Mind Flay' (No Repeat) (Normal Dungeon)");

-- 维克尼守护者
UPDATE `creature_template` SET AIName='SmartAI' WHERE `entry` = 15233;
DELETE FROM `smart_scripts` WHERE `entryorguid` = 15233;
INSERT INTO `smart_scripts` VALUES
(15233,0,0,1, 2,0,100,3,    0,   20,    0,    0,11, 8599,0,0,0,0,0,1,0,0,0,0,0,0,0,"Vekniss Guardian - Between 0-20% Health - Cast 'Enrage' (No Repeat) (Normal Dungeon)"),
(15233,0,1,0,61,0,100,0,    0,    0,    0,    0, 1,    0,0,0,0,0,0,1,0,0,0,0,0,0,0,"Vekniss Guardian - Between 0-20% Health - Say Line 0 (No Repeat) (Normal Dungeon)"),
(15233,0,2,0, 0,0,100,2,30000,30000,30000,30000,11,26025,0,0,0,0,0,2,0,0,0,0,0,0,0,"Vekniss Guardian - In Combat - Cast 'Impale' (No Repeat) (Normal Dungeon)");


-- 血色传教士
DELETE FROM `creature_loot_template` WHERE `entry` = 1536;
INSERT INTO `creature_loot_template` VALUES 
(1536,2875,-40,1,0,1,1),
(1536,11111,0.1,1,0,-11111,1);

-- 吸血夜行蝙蝠
DELETE FROM `creature_loot_template` WHERE `entry` = 1554;
INSERT INTO `creature_loot_template` VALUES 
(1554,2876,-50,1,0,1,1),
(1554,11111,0.1,1,0,-11111,1);

-- 混血腐皮豺狼人
DELETE FROM `creature_loot_template` WHERE `entry` = 1675;
INSERT INTO `creature_loot_template` VALUES 
(1675,2834,-40,1,0,1,1),
(1675,11111,0.1,1,0,-11111,1);

-- 血色战士缺少对话
DELETE FROM `creature_text` WHERE `entry` = 1535;
INSERT INTO `creature_text` VALUES
(1535,0,0,"血色十字军是不可战胜的！",14,0,100,0,0,0,"血色战士",0),
(1535,0,1,"圣光永存！",14,0,100,0,0,0,"血色战士",0);


-- 
Delete From `creature_queststarter`  WHERE `quest` = 8344;
Insert Into `creature_queststarter`  Values (15283,8344);

Delete From `creature_questender`  WHERE `quest` = 8344;
Insert Into `creature_questender`  Values (15283,8344);

-- 不正确掉落
UPDATE `creature_template` set `skinloot` = 0 where `minlevel` > 79 and `skinloot` = 100011;
UPDATE `creature_template` set `skinloot` = 0 where `minlevel` > 79 and `skinloot` = 100010;
UPDATE `creature_template` set `skinloot` = 0 where `minlevel` > 79 and `skinloot` = 100009;

-- 定义混乱
UPDATE `creature_template` set `skinloot` = 100001 where `skinloot` = 100008;

-- 追加掉落
UPDATE `creature_template` set `skinloot` = 100008 where `entry` = 1042;
UPDATE `creature_template` set `skinloot` = 100008 where `entry` = 1043;
UPDATE `creature_template` set `skinloot` = 100008 where `entry` = 1069;


-- 错误
UPDATE `skinning_loot_template` set `item` = 4232 where `entry` = 100007 and `item` = 4332;