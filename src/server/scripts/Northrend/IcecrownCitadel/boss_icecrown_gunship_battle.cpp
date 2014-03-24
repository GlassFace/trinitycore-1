/*
 * Copyright (C) 2008-2014 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "CellImpl.h"
#include "CreatureTextMgr.h"
#include "GridNotifiersImpl.h"
#include "GossipDef.h"
#include "MoveSpline.h"
#include "MoveSplineInit.h"
#include "PassiveAI.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellAuraEffects.h"
#include "SpellScript.h"
#include "Transport.h"
#include "TransportMgr.h"
#include "Vehicle.h"
#include "icecrown_citadel.h"

enum Texts
{
    // High Overlord Saurfang
    SAY_SAURFANG_INTRO_1                = 0,
    SAY_SAURFANG_INTRO_2                = 1,
    SAY_SAURFANG_INTRO_3                = 2,
    SAY_SAURFANG_INTRO_4                = 3,
    SAY_SAURFANG_INTRO_5                = 4,
    SAY_SAURFANG_INTRO_6                = 5,
    SAY_SAURFANG_INTRO_A                = 6,
    SAY_SAURFANG_BOARD                  = 7,
    SAY_SAURFANG_ENTER_SKYBREAKER       = 8,
    SAY_SAURFANG_AXETHROWERS            = 9,
    SAY_SAURFANG_ROCKETEERS             = 10,
    SAY_SAURFANG_MAGES                  = 11,
    SAY_SAURFANG_VICTORY                = 12,
    SAY_SAURFANG_WIPE                   = 13,

    // Muradin Bronzebeard
    SAY_MURADIN_INTRO_1                 = 0,
    SAY_MURADIN_INTRO_2                 = 1,
    SAY_MURADIN_INTRO_3                 = 2,
    SAY_MURADIN_INTRO_4                 = 3,
    SAY_MURADIN_INTRO_5                 = 4,
    SAY_MURADIN_INTRO_6                 = 5,
    SAY_MURADIN_INTRO_7                 = 6,
    SAY_MURADIN_INTRO_H                 = 7,
    SAY_MURADIN_BOARD                   = 8,
    SAY_MURADIN_ENTER_ORGRIMMS_HAMMER   = 9,
    SAY_MURADIN_RIFLEMAN                = 10,
    SAY_MURADIN_MORTAR                  = 11,
    SAY_MURADIN_SORCERERS               = 12,
    SAY_MURADIN_VICTORY                 = 13,
    SAY_MURADIN_WIPE                    = 14,

    SAY_ZAFOD_ROCKET_PACK_ACTIVE        = 0,
    SAY_ZAFOD_ROCKET_PACK_DISABLED      = 1,

    SAY_OVERHEAT                        = 0
};

enum Events
{
    // High Overlord Saurfang
    EVENT_INTRO_H_1                 = 1,
    EVENT_INTRO_H_2                 = 2,
    EVENT_INTRO_SUMMON_SKYBREAKER   = 3,
    EVENT_INTRO_H_3                 = 4,
    EVENT_INTRO_H_4                 = 5,
    EVENT_INTRO_H_5                 = 6,
    EVENT_INTRO_H_6                 = 7,

    // Muradin Bronzebeard
    EVENT_INTRO_A_1                 = 1,
    EVENT_INTRO_A_2                 = 2,
    EVENT_INTRO_SUMMON_ORGRIMS_HAMMER = 3,
    EVENT_INTRO_A_3                 = 4,
    EVENT_INTRO_A_4                 = 5,
    EVENT_INTRO_A_5                 = 6,
    EVENT_INTRO_A_6                 = 7,
    EVENT_INTRO_A_7                 = 8,

    EVENT_KEEP_PLAYER_IN_COMBAT     = 9,
    EVENT_SUMMON_MAGE               = 10,
    EVENT_ADDS                      = 11,
    EVENT_ADDS_BOARD_YELL           = 12,
    EVENT_CHECK_RIFLEMAN            = 13,
    EVENT_CHECK_MORTAR              = 14,
    EVENT_CLEAVE                    = 15,

    EVENT_BLADESTORM                = 16,
    EVENT_WOUNDING_STRIKE           = 17
};

enum Spells
{
    // Applied on friendly transport NPCs
    SPELL_FRIENDLY_BOSS_DAMAGE_MOD          = 70339,
    SPELL_CHECK_FOR_PLAYERS                 = 70332,
    SPELL_GUNSHIP_FALL_TELEPORT             = 67335,
    SPELL_TELEPORT_PLAYERS_ON_RESET_A       = 70446,
    SPELL_TELEPORT_PLAYERS_ON_RESET_H       = 71284,
    SPELL_TELEPORT_PLAYERS_ON_VICTORY       = 72340,
    SPELL_ACHIEVEMENT                       = 72959,
    SPELL_AWARD_REPUTATION_BOSS_KILL        = 73843,

    // Murading Bronzebeard
    // High Overlord Saurfang
    SPELL_BATTLE_FURY                       = 69637,
    SPELL_RENDING_THROW                     = 70309,
    SPELL_CLEAVE                            = 15284,
    SPELL_TASTE_OF_BLOOD                    = 69634,

    // Applied on enemy NPCs
    SPELL_MELEE_TARGETING_ON_SKYBREAKER     = 70219,
    SPELL_MELEE_TARGETING_ON_ORGRIMS_HAMMER = 70294,

    // Gunship Hull
    SPELL_EXPLOSION_WIPE                    = 72134,
    SPELL_EXPLOSION_VICTORY                 = 72137,

    // Hostile NPCs
    SPELL_TELEPORT_TO_ENEMY_SHIP            = 70104,
    SPELL_BATTLE_EXPERIENCE                 = 71201,
    SPELL_EXPERIENCED                       = 71188,
    SPELL_VETERAN                           = 71193,
    SPELL_ELITE                             = 71195,
    SPELL_ADDS_BERSERK                      = 72525,

    // Skybreaker Sorcerer
    // Kor'kron Battle-Mage
    SPELL_SHADOW_CHANNELING                 = 43897,
    SPELL_BELOW_ZERO                        = 69705,

    // Skybreaker Rifleman
    // Kor'kron Axethrower
    SPELL_SHOOT                             = 70162,
    SPELL_HURL_AXE                          = 70161,
    SPELL_BURNING_PITCH_A                   = 70403,
    SPELL_BURNING_PITCH_H                   = 70397,
    SPELL_BURNING_PITCH                     = 69660,

    // Skybreaker Mortar Soldier
    // Kor'kron Rocketeer
    SPELL_ROCKET_ARTILLERY_A                = 70609,
    SPELL_ROCKET_ARTILLERY_H                = 69678,
    SPELL_BURNING_PITCH_DAMAGE_A            = 70383,
    SPELL_BURNING_PITCH_DAMAGE_H            = 70374,

    // Skybreaker Marine
    // Kor'kron Reaver
    SPELL_DESPERATE_RESOLVE                 = 69647,

    // Skybreaker Sergeant
    // Kor'kron Sergeant
    SPELL_BLADESTORM                        = 69652,
    SPELL_WOUNDING_STRIKE                   = 69651,

    //
    SPELL_LOCK_PLAYERS_AND_TAP_CHEST        = 72347,
    SPELL_ON_SKYBREAKER_DECK                = 70120,
    SPELL_ON_ORGRIMS_HAMMER_DECK            = 70121,

    // Rocket Pack
    SPELL_ROCKET_PACK_DAMAGE                = 69193,
    SPELL_ROCKET_BURST                      = 69192,
    SPELL_ROCKET_PACK_USEABLE               = 70348,

    // Alliance Gunship Cannon
    // Horde Gunship Cannon
    SPELL_OVERHEAT                          = 69487,
    SPELL_EJECT_ALL_PASSENGERS_BELOW_ZERO   = 68576,
    SPELL_EJECT_ALL_PASSENGERS_WIPE         = 50630
};

enum MiscData
{
    ITEM_GOBLIN_ROCKET_PACK = 49278,

    PHASE_COMBAT            = 0,
    PHASE_INTRO             = 1,

    MUSIC_ENCOUNTER         = 17289
};

enum EncounterActions
{
    ACTION_SPAWN_MAGE       = 1,
    ACTION_SPAWN_ALL_ADDS   = 2,
    ACTION_CLEAR_SLOT       = 3,
    ACTION_SET_SLOT         = 4,
    ACTION_SHIP_VISITS      = 5
};

Position const SkybreakerAddsSpawnPos = { 15.91131f, 0.0f, 20.4628f, M_PI };
Position const OrgrimsHammerAddsSpawnPos = { 60.728395f, 0.0f, 38.93467f, M_PI };

// Horde encounter
Position const SkybreakerTeleportPortal  = { 6.666975f, 0.013001f, 20.87888f, 0.0f };
Position const OrgrimsHammerTeleportExit = { 7.461699f, 0.158853f, 35.72989f, 0.0f };

// Alliance encounter
Position const OrgrimsHammerTeleportPortal = { 47.550990f, -0.101778f, 37.61111f, 0.0f };
Position const SkybreakerTeleportExit      = { -17.55738f, -0.090421f, 21.18366f, 0.0f };

enum PassengerSlots
{
    // Freezing the cannons
    SLOT_FREEZE_MAGE    = 0,

    // Channeling the portal, refilled with adds that board player's ship
    SLOT_MAGE_1         = 1,
    SLOT_MAGE_2         = 2,

    // Rifleman
    SLOT_RIFLEMAN_1     = 3,
    SLOT_RIFLEMAN_2     = 4,
    SLOT_RIFLEMAN_3     = 5,
    SLOT_RIFLEMAN_4     = 6,

    // Additional Rifleman on 25 man
    SLOT_RIFLEMAN_5     = 7,
    SLOT_RIFLEMAN_6     = 8,
    SLOT_RIFLEMAN_7     = 9,
    SLOT_RIFLEMAN_8     = 10,

    // Mortar
    SLOT_MORTAR_1       = 11,
    SLOT_MORTAR_2       = 12,

    // Additional spawns on 25 man
    SLOT_MORTAR_3       = 13,
    SLOT_MORTAR_4       = 14,

    // Marines
    SLOT_MARINE_1       = 15,
    SLOT_MARINE_2       = 16,

    // Additional spawns on 25 man
    SLOT_MARINE_3       = 17,
    SLOT_MARINE_4       = 18,

    // Sergeants
    SLOT_SERGEANT_1     = 19,

    // Additional spawns on 25 man
    SLOT_SERGEANT_2     = 20,

    MAX_SLOTS
};

struct SlotInfo
{
    uint32 Entry;
    Position TargetPosition;
    uint32 Cooldown;
};

SlotInfo const SkybreakerSlotInfo[MAX_SLOTS] =
{
    { NPC_SKYBREAKER_SORCERER, { -9.479858f, 0.05663967f, 20.77026f, 4.729842f }, 0 },

    { NPC_SKYBREAKER_SORCERER, { 6.385986f,  4.978760f, 20.55417f, 4.694936f }, 0 },
    { NPC_SKYBREAKER_SORCERER, { 6.579102f, -4.674561f, 20.55060f, 1.553343f }, 0 },

    { NPC_SKYBREAKER_RIFLEMAN, { -29.563900f, -17.95801f, 20.73837f, 4.747295f }, 30 },
    { NPC_SKYBREAKER_RIFLEMAN, { -18.017210f, -18.82056f, 20.79150f, 4.747295f }, 30 },
    { NPC_SKYBREAKER_RIFLEMAN, { -9.1193850f, -18.79102f, 20.58887f, 4.712389f }, 30 },
    { NPC_SKYBREAKER_RIFLEMAN, { -0.3364258f, -18.87183f, 20.56824f, 4.712389f }, 30 },

    { NPC_SKYBREAKER_RIFLEMAN, { -34.705810f, -17.67261f, 20.51523f, 4.729842f }, 30 },
    { NPC_SKYBREAKER_RIFLEMAN, { -23.562010f, -18.28564f, 20.67859f, 4.729842f }, 30 },
    { NPC_SKYBREAKER_RIFLEMAN, { -13.602780f, -18.74268f, 20.59622f, 4.712389f }, 30 },
    { NPC_SKYBREAKER_RIFLEMAN, { -4.3350220f, -18.84619f, 20.58234f, 4.712389f }, 30 },

    { NPC_SKYBREAKER_MORTAR_SOLDIER, { -31.70142f, 18.02783f, 20.77197f, 4.712389f }, 30 },
    { NPC_SKYBREAKER_MORTAR_SOLDIER, { -9.368652f, 18.75806f, 20.65335f, 4.712389f }, 30 },

    { NPC_SKYBREAKER_MORTAR_SOLDIER, { -20.40851f, 18.40381f, 20.50647f, 4.694936f }, 30 },
    { NPC_SKYBREAKER_MORTAR_SOLDIER, { 0.1585693f, 18.11523f, 20.41949f, 4.729842f }, 30 },

    { NPC_SKYBREAKER_MARINE, SkybreakerTeleportPortal, 0 },
    { NPC_SKYBREAKER_MARINE, SkybreakerTeleportPortal, 0 },

    { NPC_SKYBREAKER_MARINE, SkybreakerTeleportPortal, 0 },
    { NPC_SKYBREAKER_MARINE, SkybreakerTeleportPortal, 0 },

    { NPC_SKYBREAKER_SERGEANT, SkybreakerTeleportPortal, 0 },

    { NPC_SKYBREAKER_SERGEANT, SkybreakerTeleportPortal, 0 }
};

SlotInfo const OrgrimsHammerSlotInfo[MAX_SLOTS] =
{
    { NPC_KOR_KRON_BATTLE_MAGE, { 13.58548f, 0.3867192f, 34.99243f, 1.53589f }, 0 },

    { NPC_KOR_KRON_BATTLE_MAGE, { 47.29290f, -4.308941f, 37.55550f, 1.570796f }, 0 },
    { NPC_KOR_KRON_BATTLE_MAGE, { 47.34621f,  4.032004f, 37.70952f, 4.817109f }, 0 },

    { NPC_KOR_KRON_AXETHROWER, { -12.09280f, 27.65942f, 33.58557f, 1.53589f }, 30 },
    { NPC_KOR_KRON_AXETHROWER, { -3.170555f, 28.30652f, 34.21082f, 1.53589f }, 30 },
    { NPC_KOR_KRON_AXETHROWER, { 14.928040f, 26.18018f, 35.47803f, 1.53589f }, 30 },
    { NPC_KOR_KRON_AXETHROWER, { 24.703310f, 25.36584f, 35.97845f, 1.53589f }, 30 },

    { NPC_KOR_KRON_AXETHROWER, { -16.65302f, 27.59668f, 33.18726f, 1.53589f }, 30 },
    { NPC_KOR_KRON_AXETHROWER, { -8.084572f, 28.21448f, 33.93805f, 1.53589f }, 30 },
    { NPC_KOR_KRON_AXETHROWER, {  7.594765f, 27.41968f, 35.00775f, 1.53589f }, 30 },
    { NPC_KOR_KRON_AXETHROWER, { 20.763390f, 25.58215f, 35.75287f, 1.53589f }, 30 },

    { NPC_KOR_KRON_ROCKETEER, { -11.44849f, -25.71838f, 33.64343f, 1.518436f }, 30 },
    { NPC_KOR_KRON_ROCKETEER, {  12.30336f, -25.69653f, 35.32373f, 1.518436f }, 30 },

    { NPC_KOR_KRON_ROCKETEER, { -0.05931854f, -25.46399f, 34.50592f, 1.518436f }, 30 },
    { NPC_KOR_KRON_ROCKETEER, { 27.62149000f, -23.48108f, 36.12708f, 1.518436f }, 30 },

    { NPC_KOR_KRON_REAVER, OrgrimsHammerTeleportPortal, 0 },
    { NPC_KOR_KRON_REAVER, OrgrimsHammerTeleportPortal, 0 },

    { NPC_KOR_KRON_REAVER, OrgrimsHammerTeleportPortal, 0 },
    { NPC_KOR_KRON_REAVER, OrgrimsHammerTeleportPortal, 0 },

    { NPC_KOR_KRON_SERGEANT, OrgrimsHammerTeleportPortal, 0 },

    { NPC_KOR_KRON_SERGEANT, OrgrimsHammerTeleportPortal, 0 }
};

class PassengerController
{
public:
    PassengerController()
    {
        ResetSlots(HORDE);
    }

    void SetTransport(Transport* transport) { _transport = transport; }

    void ResetSlots(uint32 team)
    {
        _transport = NULL;
        memset(_controlledSlots, 0, sizeof(uint64)* MAX_SLOTS);
        memset(_respawnCooldowns, 0, sizeof(time_t)* MAX_SLOTS);
        _spawnPoint = team == HORDE ? &OrgrimsHammerAddsSpawnPos : &SkybreakerAddsSpawnPos;
        _slotInfo = team == HORDE ? OrgrimsHammerSlotInfo : SkybreakerSlotInfo;
    }

    bool SummonCreatures(PassengerSlots first, PassengerSlots last)
    {
        if (!_transport)
            return false;

        bool summoned = false;
        time_t now = time(NULL);
        for (int32 i = first; i <= last; ++i)
        {
            if (_respawnCooldowns[i] > now)
                continue;

            if (_controlledSlots[i])
            {
                Creature* current = ObjectAccessor::GetCreature(*_transport, _controlledSlots[i]);
                if (current && current->IsAlive())
                    continue;
            }

            if (Creature* passenger = _transport->SummonPassenger(_slotInfo[i].Entry, SelectSpawnPoint(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, NULL, 15000))
            {
                _controlledSlots[i] = passenger->GetGUID();
                _respawnCooldowns[i] = time_t(0);
                passenger->AI()->SetData(ACTION_SET_SLOT, i);
                summoned = true;
            }
        }

        return summoned;
    }

    void ClearSlot(PassengerSlots slot)
    {
        _controlledSlots[slot] = 0;
        _respawnCooldowns[slot] = time(NULL) + _slotInfo[slot].Cooldown;
    }

    bool SlotsNeedRefill(PassengerSlots first, PassengerSlots last) const
    {
        for (int32 i = first; i <= last; ++i)
            if (!_controlledSlots[i])
                return true;

        return false;
    }

private:
    Position SelectSpawnPoint() const
    {
        Position newPos;
        float angle = frand(-M_PI * 0.5f, M_PI * 0.5f);
        newPos.m_positionX = _spawnPoint->GetPositionX() + 2.0f * std::cos(angle);
        newPos.m_positionY = _spawnPoint->GetPositionY() + 2.0f * std::sin(angle);
        newPos.m_positionZ = _spawnPoint->GetPositionZ();
        newPos.SetOrientation(_spawnPoint->GetOrientation());
        return newPos;
    }

    Transport* _transport;
    uint64 _controlledSlots[MAX_SLOTS];
    time_t _respawnCooldowns[MAX_SLOTS];
    Position const* _spawnPoint;
    SlotInfo const* _slotInfo;
};

class DelayedMovementEvent : public BasicEvent
{
public:
    DelayedMovementEvent(Creature* owner, Position const& dest) : _owner(owner), _dest(dest) { }

    bool Execute(uint64, uint32) OVERRIDE
    {
        if (!_owner->IsAlive())
            return true;

        _owner->GetMotionMaster()->MovePoint(EVENT_CHARGE_PREPATH, *_owner, false);

        Movement::MoveSplineInit init(_owner);
        init.DisableTransportPathTransformations();
        init.MoveTo(_dest.GetPositionX(), _dest.GetPositionY(), _dest.GetPositionZ(), false);
        init.Launch();

        return true;
    }

private:
    Creature* _owner;
    Position const& _dest;
};

class ResetEncounterEvent : public BasicEvent
{
public:
    ResetEncounterEvent(Unit* caster, uint32 spellId, uint64 otherTransport) : _caster(caster), _spellId(spellId), _otherTransport(otherTransport) { }

    bool Execute(uint64, uint32) OVERRIDE
    {
        _caster->CastSpell(_caster, _spellId, true);
        _caster->GetTransport()->AddObjectToRemoveList();

        if (GameObject* go = HashMapHolder<GameObject>::Find(_otherTransport))
            go->AddObjectToRemoveList();

        return true;
    }

private:
    Unit* _caster;
    uint32 _spellId;
    uint64 _otherTransport;
};

class BattleExperienceEvent : public BasicEvent
{
public:
    static uint32 const ExperiencedSpells[5];
    static uint32 const ExperiencedTimes[5];

    BattleExperienceEvent(Creature* creature) : _creature(creature), _level(0) { }

    bool Execute(uint64 timer, uint32 /*diff*/) OVERRIDE
    {
        if (!_creature->IsAlive())
            return true;

        _creature->RemoveAurasDueToSpell(ExperiencedSpells[_level]);
        ++_level;

        _creature->CastSpell(_creature, ExperiencedSpells[_level], TRIGGERED_FULL_MASK);
        if (_level < (_creature->GetMap()->IsHeroic() ? 4 : 3))
        {
            _creature->m_Events.AddEvent(this, timer + ExperiencedTimes[_level]);
            return false;
        }

        return true;
    }

private:
    Creature* _creature;
    int32 _level;
};

uint32 const BattleExperienceEvent::ExperiencedSpells[5] = { 0, SPELL_EXPERIENCED, SPELL_VETERAN, SPELL_ELITE, SPELL_ADDS_BERSERK };
uint32 const BattleExperienceEvent::ExperiencedTimes[5] = { 100000, 70000, 60000, 90000, 0 };

struct gunship_npc_AI : public ScriptedAI
{
    gunship_npc_AI(Creature* creature) : ScriptedAI(creature),
        Instance(creature->GetInstanceScript()), Slot(NULL), Index(uint32(-1))
    {
        BurningPitchId = Instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE ? SPELL_BURNING_PITCH_A : SPELL_BURNING_PITCH_H;
        me->setRegeneratingHealth(false);
    }

    void SetData(uint32 type, uint32 data) OVERRIDE
    {
        if (type == ACTION_SET_SLOT && data < MAX_SLOTS)
        {
            SetSlotInfo(data);

            me->SetReactState(REACT_PASSIVE);

            float x, y, z, o;
            Slot->TargetPosition.GetPosition(x, y, z, o);

            me->SetTransportHomePosition(Slot->TargetPosition);
            float hx = x, hy = y, hz = z, ho = o;
            me->GetTransport()->CalculatePassengerPosition(hx, hy, hz, &ho);
            me->SetHomePosition(hx, hy, hz, ho);

            me->GetMotionMaster()->MovePoint(EVENT_CHARGE_PREPATH, Slot->TargetPosition, false);

            Movement::MoveSplineInit init(me);
            init.DisableTransportPathTransformations();
            init.MoveTo(x, y, z, false);
            init.Launch();
        }
    }

    void EnterEvadeMode() OVERRIDE
    {
        if (!me->IsAlive() || !me->IsInCombat())
            return;

        me->DeleteThreatList();
        me->CombatStop(true);
        me->GetMotionMaster()->MoveTargetedHome();
    }

    void JustDied(Unit* /*killer*/) OVERRIDE
    {
        if (Slot)
            if (Creature* captain = me->FindNearestCreature(Instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE ? NPC_IGB_MURADIN_BRONZEBEARD : NPC_IGB_HIGH_OVERLORD_SAURFANG, 200.0f))
                captain->AI()->SetData(ACTION_CLEAR_SLOT, Index);
    }

    void MovementInform(uint32 type, uint32 pointId) OVERRIDE
    {
        if (type != POINT_MOTION_TYPE)
            return;

        if (pointId == EVENT_CHARGE_PREPATH && Slot)
        {
            me->SetFacingTo(Slot->TargetPosition.GetOrientation());
            me->m_Events.AddEvent(new BattleExperienceEvent(me), me->m_Events.CalculateTime(BattleExperienceEvent::ExperiencedTimes[0]));
            DoCast(me, SPELL_BATTLE_EXPERIENCE, true);
            me->SetReactState(REACT_AGGRESSIVE);
        }
    }

    bool CanAIAttack(Unit const* target) const OVERRIDE
    {
        return target->HasAura(Instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE ? SPELL_ON_ORGRIMS_HAMMER_DECK : SPELL_ON_SKYBREAKER_DECK);
    }

protected:
    void SetSlotInfo(uint32 index)
    {
        Index = index;
        Slot = &((Instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE ? SkybreakerSlotInfo : OrgrimsHammerSlotInfo)[Index]);
    }

    bool SelectVictim()
    {
        if (Instance->GetBossState(DATA_ICECROWN_GUNSHIP_BATTLE) != IN_PROGRESS)
            return false;

        if (!me->HasReactState(REACT_PASSIVE))
        {
            if (Unit* victim = me->SelectVictim())
                AttackStart(victim);
            return me->GetVictim();
        }
        else if (me->getThreatManager().isThreatListEmpty())
        {
            EnterEvadeMode();
            return false;
        }

        return true;
    }

    void TriggerBurningPitch()
    {
        if (Instance->GetBossState(DATA_ICECROWN_GUNSHIP_BATTLE) == IN_PROGRESS &&
            !me->HasUnitState(UNIT_STATE_CASTING) && !me->HasReactState(REACT_PASSIVE) &&
            !me->HasSpellCooldown(BurningPitchId))
        {
            DoCastAOE(BurningPitchId, true);
            me->_AddCreatureSpellCooldown(BurningPitchId, time(NULL) + urand(3000, 4000) / IN_MILLISECONDS);
        }
    }

    InstanceScript* Instance;
    SlotInfo const* Slot;
    uint32 Index;
    uint32 BurningPitchId;
};

class npc_gunship : public CreatureScript
{
    public:
        npc_gunship() : CreatureScript("npc_gunship") { }

        struct npc_gunshipAI : public NullCreatureAI
        {
            npc_gunshipAI(Creature* creature) : NullCreatureAI(creature),
                _teamInInstance(creature->GetInstanceScript()->GetData(DATA_TEAM_IN_INSTANCE)),
                _summonedFirstMage(false), _died(false)
            {
                me->setRegeneratingHealth(false);
            }

            void DamageTaken(Unit* /*source*/, uint32& damage) OVERRIDE
            {
                if (damage > me->GetHealth())
                {
                    JustDied(NULL);
                    damage = me->GetHealth() - 1;
                    return;
                }

                if (_summonedFirstMage)
                    return;

                if (me->GetTransport()->GetEntry() != uint32(_teamInInstance == HORDE ? GO_THE_SKYBREAKER_H : GO_ORGRIMS_HAMMER_A))
                    return;

                if (!me->HealthBelowPctDamaged(90, damage))
                    return;

                _summonedFirstMage = true;
                if (Creature* captain = me->FindNearestCreature(_teamInInstance == HORDE ? NPC_IGB_MURADIN_BRONZEBEARD : NPC_IGB_HIGH_OVERLORD_SAURFANG, 100.0f))
                    captain->AI()->DoAction(ACTION_SPAWN_MAGE);
            }

            void JustDied(Unit* /*killer*/) OVERRIDE
            {
                if (_died)
                    return;

                _died = true;

                bool isVictory = me->GetTransport()->GetEntry() == GO_THE_SKYBREAKER_H || me->GetTransport()->GetEntry() == GO_ORGRIMS_HAMMER_A;
                InstanceScript* instance = me->GetInstanceScript();
                if (Creature* creature = me->FindNearestCreature(me->GetEntry() == NPC_ORGRIMS_HAMMER ? NPC_THE_SKYBREAKER : NPC_ORGRIMS_HAMMER, 200.0f))
                {
                    instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, creature);
                    creature->RemoveAurasDueToSpell(SPELL_CHECK_FOR_PLAYERS);
                }

                instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                me->RemoveAurasDueToSpell(SPELL_CHECK_FOR_PLAYERS);

                me->GetMap()->SetZoneMusic(AREA_ICECROWN_CITADEL, 0);
                std::list<Creature*> creatures;
                GetCreatureListWithEntryInGrid(creatures, me, NPC_MARTYR_STALKER_IGB_SAURFANG, SIZE_OF_GRIDS);
                for (std::list<Creature*>::iterator itr = creatures.begin(); itr != creatures.end(); ++itr)
                    (*itr)->AI()->EnterEvadeMode();


                uint32 explosionSpell = isVictory ? SPELL_EXPLOSION_VICTORY : SPELL_EXPLOSION_WIPE;
                creatures.clear();
                GetCreatureListWithEntryInGrid(creatures, me, NPC_GUNSHIP_HULL, 200.0f);
                for (std::list<Creature*>::iterator itr = creatures.begin(); itr != creatures.end(); ++itr)
                {
                    Creature* hull = *itr;
                    if (hull->GetTransport() != me->GetTransport())
                        continue;

                    hull->CastSpell(hull, explosionSpell, TRIGGERED_FULL_MASK);
                }

                creatures.clear();
                GetCreatureListWithEntryInGrid(creatures, me, _teamInInstance == HORDE ? NPC_HORDE_GUNSHIP_CANNON : NPC_ALLIANCE_GUNSHIP_CANNON, 200.0f);
                for (std::list<Creature*>::iterator itr = creatures.begin(); itr != creatures.end(); ++itr)
                {
                    Creature* cannon = *itr;
                    if (isVictory)
                    {
                        cannon->CastSpell(cannon, SPELL_EJECT_ALL_PASSENGERS_BELOW_ZERO, TRIGGERED_FULL_MASK);

                        WorldPacket data(SMSG_PLAYER_VEHICLE_DATA, cannon->GetPackGUID().size() + 4);
                        data.append(cannon->GetPackGUID());
                        data << uint32(0);
                        cannon->SendMessageToSet(&data, true);

                        cannon->RemoveVehicleKit();
                    }
                    else
                        cannon->CastSpell(cannon, SPELL_EJECT_ALL_PASSENGERS_WIPE, TRIGGERED_FULL_MASK);
                }

                uint32 creatureEntry = NPC_IGB_MURADIN_BRONZEBEARD;
                uint8 textId = isVictory ? SAY_MURADIN_VICTORY : SAY_MURADIN_WIPE;
                if (_teamInInstance == HORDE)
                {
                    creatureEntry = NPC_IGB_HIGH_OVERLORD_SAURFANG;
                    textId = isVictory ? SAY_SAURFANG_VICTORY : SAY_SAURFANG_WIPE;
                }

                if (Creature* creature = me->FindNearestCreature(creatureEntry, 100.0f))
                    creature->AI()->Talk(textId);

                if (isVictory)
                {
                    if (GameObject* go = HashMapHolder<GameObject>::Find(instance->GetData64(DATA_ICECROWN_GUNSHIP_BATTLE)))
                        if (Transport* otherTransport = go->ToTransport())
                            otherTransport->EnableMovement(true);

                    me->GetTransport()->EnableMovement(true);

                    if (Creature* ship = me->FindNearestCreature(_teamInInstance == HORDE ? NPC_ORGRIMS_HAMMER : NPC_THE_SKYBREAKER, 200.0f))
                    {
                        ship->CastSpell(ship, SPELL_TELEPORT_PLAYERS_ON_VICTORY, TRIGGERED_FULL_MASK);
                        ship->CastSpell(ship, SPELL_ACHIEVEMENT, TRIGGERED_FULL_MASK);
                        ship->CastSpell(ship, SPELL_AWARD_REPUTATION_BOSS_KILL, TRIGGERED_FULL_MASK);
                    }
                }
                else
                {
                    uint32 teleportSpellId = _teamInInstance == HORDE ? SPELL_TELEPORT_PLAYERS_ON_RESET_H : SPELL_TELEPORT_PLAYERS_ON_RESET_A;
                    me->m_Events.AddEvent(new ResetEncounterEvent(me, teleportSpellId, me->GetInstanceScript()->GetData64(DATA_ENEMY_GUNSHIP)),
                        me->m_Events.CalculateTime(8000));
                }

                instance->SetBossState(DATA_ICECROWN_GUNSHIP_BATTLE, isVictory ? DONE : FAIL);
            }

            void SetGUID(uint64 guid, int32 id/* = 0*/) OVERRIDE
            {
                if (id != ACTION_SHIP_VISITS)
                    return;

                std::map<uint64, uint32>::iterator itr = _shipVisits.find(guid);
                if (itr == _shipVisits.end())
                    _shipVisits[guid] = 1;
                else
                    ++itr->second;
            }

            uint32 GetData(uint32 id) const OVERRIDE
            {
                if (id != ACTION_SHIP_VISITS)
                    return 0;

                uint32 max = 0;
                for (std::map<uint64, uint32>::const_iterator itr = _shipVisits.begin(); itr != _shipVisits.end(); ++itr)
                    max = std::max(max, itr->second);

                return max;
            }

        private:
            uint32 _teamInInstance;
            std::map<uint64, uint32> _shipVisits;
            bool _summonedFirstMage;
            bool _died;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            if (!creature->GetTransport())
                return NULL;

            return GetIcecrownCitadelAI<npc_gunshipAI>(creature);
        }
};

class npc_high_overlord_saurfang_igb : public CreatureScript
{
    public:
        npc_high_overlord_saurfang_igb() : CreatureScript("npc_high_overlord_saurfang_igb") { }

        struct npc_high_overlord_saurfang_igbAI : public ScriptedAI
        {
            npc_high_overlord_saurfang_igbAI(Creature* creature) : ScriptedAI(creature),
                _instance(creature->GetInstanceScript())
            {
                _controller.ResetSlots(HORDE);
                _controller.SetTransport(creature->GetTransport());
                me->setRegeneratingHealth(false);
                me->m_CombatDistance = 70.0f;
            }

            void InitializeAI() OVERRIDE
            {
                ScriptedAI::InitializeAI();

                _events.Reset();
                _firstMageCooldown = time(NULL) + 60;
                _axethrowersYellCooldown = time_t(0);
                _rocketeersYellCooldown = time_t(0);
            }

            void EnterCombat(Unit* /*target*/) OVERRIDE
            {
                _events.SetPhase(PHASE_COMBAT);
                DoCast(me, _instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE ? SPELL_FRIENDLY_BOSS_DAMAGE_MOD : SPELL_MELEE_TARGETING_ON_ORGRIMS_HAMMER, true);
                DoCast(me, SPELL_BATTLE_FURY, true);
                _events.ScheduleEvent(EVENT_CLEAVE, urand(2000, 10000));
            }

            void EnterEvadeMode() OVERRIDE
            {
                if (!me->IsAlive())
                    return;

                me->DeleteThreatList();
                me->CombatStop(true);
                me->GetMotionMaster()->MoveTargetedHome();

                Reset();
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action == ACTION_ENEMY_GUNSHIP_TALK)
                {
                    if (Creature* muradin = me->FindNearestCreature(NPC_IGB_MURADIN_BRONZEBEARD, 100.0f))
                        muradin->AI()->DoAction(ACTION_SPAWN_ALL_ADDS);

                    Talk(SAY_SAURFANG_INTRO_5);
                    _events.ScheduleEvent(EVENT_INTRO_H_5, 4000);
                    _events.ScheduleEvent(EVENT_INTRO_H_6, 11000);
                    _events.ScheduleEvent(EVENT_KEEP_PLAYER_IN_COMBAT, 1);

                    _instance->SetBossState(DATA_ICECROWN_GUNSHIP_BATTLE, IN_PROGRESS);
                    // Combat starts now
                    if (Creature* skybreaker = me->FindNearestCreature(NPC_THE_SKYBREAKER, 100.0f))
                        _instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, skybreaker, 1);

                    if (Creature* orgrimsHammer = me->FindNearestCreature(NPC_ORGRIMS_HAMMER, 100.0f))
                    {
                        _instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, orgrimsHammer, 2);
                        orgrimsHammer->CastSpell(orgrimsHammer, SPELL_CHECK_FOR_PLAYERS, TRIGGERED_FULL_MASK);
                    }

                    me->GetMap()->SetZoneMusic(AREA_ICECROWN_CITADEL, MUSIC_ENCOUNTER);
                }
                else if (action == ACTION_SPAWN_MAGE)
                {
                    time_t now = time(NULL);
                    if (_firstMageCooldown < now)
                        _events.ScheduleEvent(EVENT_SUMMON_MAGE, (now - _firstMageCooldown) * IN_MILLISECONDS);
                    else
                        _events.ScheduleEvent(EVENT_SUMMON_MAGE, 1);
                }
                else if (action == ACTION_SPAWN_ALL_ADDS)
                {
                    _events.ScheduleEvent(EVENT_ADDS, 12000);
                    _events.ScheduleEvent(EVENT_CHECK_RIFLEMAN, 13000);
                    _events.ScheduleEvent(EVENT_CHECK_MORTAR, 13000);
                    if (Is25ManRaid())
                        _controller.SummonCreatures(SLOT_MAGE_1, SLOT_MORTAR_4);
                    else
                    {
                        _controller.SummonCreatures(SLOT_MAGE_1, SLOT_MAGE_2);
                        _controller.SummonCreatures(SLOT_MORTAR_1, SLOT_MORTAR_2);
                        _controller.SummonCreatures(SLOT_RIFLEMAN_1, SLOT_RIFLEMAN_4);
                    }
                }
            }

            void SetData(uint32 type, uint32 data) OVERRIDE
            {
                if (type == ACTION_CLEAR_SLOT)
                {
                    _controller.ClearSlot(PassengerSlots(data));
                    if (data == SLOT_FREEZE_MAGE)
                        _events.ScheduleEvent(EVENT_SUMMON_MAGE, urand(30000, 33500));
                }
            }

            void sGossipSelect(Player* /*player*/, uint32 /*sender*/, uint32 /*action*/) OVERRIDE
            {
                me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                me->GetTransport()->EnableMovement(true);
                _events.SetPhase(PHASE_INTRO);
                _events.ScheduleEvent(EVENT_INTRO_H_1, 5000, 0, PHASE_INTRO);
                _events.ScheduleEvent(EVENT_INTRO_H_2, 16000, 0, PHASE_INTRO);
                _events.ScheduleEvent(EVENT_INTRO_SUMMON_SKYBREAKER, 24600, 0, PHASE_INTRO);
                _events.ScheduleEvent(EVENT_INTRO_H_3, 29600, 0, PHASE_INTRO);
                _events.ScheduleEvent(EVENT_INTRO_H_4, 39200, 0, PHASE_INTRO);
            }

            void DamageTaken(Unit* , uint32& damage) OVERRIDE
            {
                if (me->HealthBelowPctDamaged(65, damage) && !me->HasAura(SPELL_TASTE_OF_BLOOD))
                    DoCast(me, SPELL_TASTE_OF_BLOOD, true);

                if (damage > me->GetHealth())
                    damage = me->GetHealth() - 1;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim() && !_events.IsInPhase(PHASE_INTRO) && _instance->GetBossState(DATA_ICECROWN_GUNSHIP_BATTLE) != IN_PROGRESS)
                    return;

                _events.Update(diff);

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_INTRO_H_1:
                            Talk(SAY_SAURFANG_INTRO_1);
                            break;
                        case EVENT_INTRO_H_2:
                            Talk(SAY_SAURFANG_INTRO_2);
                            break;
                        case EVENT_INTRO_SUMMON_SKYBREAKER:
                            sTransportMgr->CreateTransport(GO_THE_SKYBREAKER_H, 0, me->GetMap());
                            break;
                        case EVENT_INTRO_H_3:
                            Talk(SAY_SAURFANG_INTRO_3);
                            break;
                        case EVENT_INTRO_H_4:
                            Talk(SAY_SAURFANG_INTRO_4);
                            break;
                        case EVENT_INTRO_H_5:
                            if (Creature* muradin = me->FindNearestCreature(NPC_IGB_MURADIN_BRONZEBEARD, 100.0f))
                                muradin->AI()->Talk(SAY_MURADIN_INTRO_H);
                            break;
                        case EVENT_INTRO_H_6:
                            Talk(SAY_SAURFANG_INTRO_6);
                            break;
                        case EVENT_KEEP_PLAYER_IN_COMBAT:
                            if (_instance->GetBossState(DATA_ICECROWN_GUNSHIP_BATTLE) == IN_PROGRESS)
                            {
                                _instance->DoCastSpellOnPlayers(SPELL_LOCK_PLAYERS_AND_TAP_CHEST);
                                _events.ScheduleEvent(EVENT_KEEP_PLAYER_IN_COMBAT, urand(5000, 8000));
                            }
                            break;
                        case EVENT_SUMMON_MAGE:
                            Talk(SAY_SAURFANG_MAGES);
                            _controller.SummonCreatures(SLOT_FREEZE_MAGE, SLOT_FREEZE_MAGE);
                            break;
                        case EVENT_ADDS:
                            Talk(SAY_SAURFANG_ENTER_SKYBREAKER);
                            _controller.SummonCreatures(SLOT_MAGE_1, SLOT_MAGE_2);
                            _controller.SummonCreatures(SLOT_MARINE_1, Is25ManRaid() ? SLOT_MARINE_4 : SLOT_MARINE_2);
                            _controller.SummonCreatures(SLOT_SERGEANT_1, Is25ManRaid() ? SLOT_SERGEANT_2 : SLOT_SERGEANT_1);
                            if (Transport* orgrimsHammer = me->GetTransport())
                                orgrimsHammer->SummonPassenger(NPC_TELEPORT_PORTAL, OrgrimsHammerTeleportPortal, TEMPSUMMON_TIMED_DESPAWN, NULL, 21000);

                            if (GameObject* go = HashMapHolder<GameObject>::Find(_instance->GetData64(DATA_ICECROWN_GUNSHIP_BATTLE)))
                                if (Transport* skybreaker = go->ToTransport())
                                    skybreaker->SummonPassenger(NPC_TELEPORT_EXIT, SkybreakerTeleportExit, TEMPSUMMON_TIMED_DESPAWN, NULL, 23000);

                            _events.ScheduleEvent(EVENT_ADDS_BOARD_YELL, 6000);
                            _events.ScheduleEvent(EVENT_ADDS, 60000);
                            break;
                        case EVENT_ADDS_BOARD_YELL:
                            if (Creature* muradin = me->FindNearestCreature(NPC_IGB_MURADIN_BRONZEBEARD, 200.0f))
                                muradin->AI()->Talk(SAY_MURADIN_BOARD);
                            break;
                        case EVENT_CHECK_RIFLEMAN:
                            if (_controller.SummonCreatures(SLOT_RIFLEMAN_1, Is25ManRaid() ? SLOT_RIFLEMAN_8 : SLOT_RIFLEMAN_4))
                            {
                                if (_axethrowersYellCooldown < time(NULL))
                                {
                                    Talk(SAY_SAURFANG_AXETHROWERS);
                                    _axethrowersYellCooldown = time(NULL) + 5;
                                }
                            }
                            _events.ScheduleEvent(EVENT_CHECK_RIFLEMAN, 1000);
                            break;
                        case EVENT_CHECK_MORTAR:
                            if (_controller.SummonCreatures(SLOT_MORTAR_1, Is25ManRaid() ? SLOT_MORTAR_4 : SLOT_MORTAR_2))
                            {
                                if (_rocketeersYellCooldown < time(NULL))
                                {
                                    Talk(SAY_SAURFANG_ROCKETEERS);
                                    _rocketeersYellCooldown = time(NULL) + 5;
                                }
                            }
                            _events.ScheduleEvent(EVENT_CHECK_MORTAR, 1000);
                            break;
                        case EVENT_CLEAVE:
                            DoCastVictim(SPELL_CLEAVE);
                            _events.ScheduleEvent(EVENT_CLEAVE, urand(2000, 10000));
                            break;
                        default:
                            break;
                    }
                }

                if (me->IsWithinMeleeRange(me->GetVictim()))
                    DoMeleeAttackIfReady();
                else if (me->isAttackReady())
                {
                    DoCastVictim(SPELL_RENDING_THROW);
                    me->resetAttackTimer();
                }
            }

            bool CanAIAttack(Unit const* target) const OVERRIDE
            {
                return target->HasAura(SPELL_ON_ORGRIMS_HAMMER_DECK) || !target->IsControlledByPlayer();
            }

        private:
            EventMap _events;
            PassengerController _controller;
            InstanceScript* _instance;
            time_t _firstMageCooldown;
            time_t _axethrowersYellCooldown;
            time_t _rocketeersYellCooldown;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetIcecrownCitadelAI<npc_high_overlord_saurfang_igbAI>(creature);
        }
};

class npc_muradin_bronzebeard_igb : public CreatureScript
{
    public:
        npc_muradin_bronzebeard_igb() : CreatureScript("npc_muradin_bronzebeard_igb") { }

        struct npc_muradin_bronzebeard_igbAI : public ScriptedAI
        {
            npc_muradin_bronzebeard_igbAI(Creature* creature) : ScriptedAI(creature),
                _instance(creature->GetInstanceScript())
            {
                _controller.ResetSlots(ALLIANCE);
                _controller.SetTransport(creature->GetTransport());
                me->setRegeneratingHealth(false);
                me->m_CombatDistance = 70.0f;
            }

            void InitializeAI() OVERRIDE
            {
                ScriptedAI::InitializeAI();

                _events.Reset();
                _firstMageCooldown = time(NULL) + 60;
                _riflemanYellCooldown = time_t(0);
                _mortarYellCooldown = time_t(0);
            }

            void EnterCombat(Unit* /*target*/) OVERRIDE
            {
                _events.SetPhase(PHASE_COMBAT);
                DoCast(me, _instance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE ? SPELL_FRIENDLY_BOSS_DAMAGE_MOD : SPELL_MELEE_TARGETING_ON_SKYBREAKER, true);
                DoCast(me, SPELL_BATTLE_FURY, true);
                _events.ScheduleEvent(EVENT_CLEAVE, urand(2000, 10000));
            }

            void EnterEvadeMode() OVERRIDE
            {
                if (!me->IsAlive())
                    return;

                me->DeleteThreatList();
                me->CombatStop(true);
                me->GetMotionMaster()->MoveTargetedHome();

                Reset();
            }

            void DoAction(int32 action) OVERRIDE
            {
                if (action == ACTION_ENEMY_GUNSHIP_TALK)
                {
                    if (Creature* muradin = me->FindNearestCreature(NPC_IGB_HIGH_OVERLORD_SAURFANG, 100.0f))
                        muradin->AI()->DoAction(ACTION_SPAWN_ALL_ADDS);

                    Talk(SAY_MURADIN_INTRO_6);
                    _events.ScheduleEvent(EVENT_INTRO_A_6, 5000);
                    _events.ScheduleEvent(EVENT_INTRO_A_7, 11000);
                    _events.ScheduleEvent(EVENT_KEEP_PLAYER_IN_COMBAT, 1);

                    _instance->SetBossState(DATA_ICECROWN_GUNSHIP_BATTLE, IN_PROGRESS);
                    // Combat starts now
                    if (Creature* orgrimsHammer = me->FindNearestCreature(NPC_ORGRIMS_HAMMER, 100.0f))
                        _instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, orgrimsHammer, 1);

                    if (Creature* skybreaker = me->FindNearestCreature(NPC_THE_SKYBREAKER, 100.0f))
                    {
                        _instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, skybreaker, 2);
                        skybreaker->CastSpell(skybreaker, SPELL_CHECK_FOR_PLAYERS, TRIGGERED_FULL_MASK);
                    }

                    me->GetMap()->SetZoneMusic(AREA_ICECROWN_CITADEL, MUSIC_ENCOUNTER);
                }
                else if (action == ACTION_SPAWN_MAGE)
                {
                    time_t now = time(NULL);
                    if (_firstMageCooldown < now)
                        _events.ScheduleEvent(EVENT_SUMMON_MAGE, (now - _firstMageCooldown) * IN_MILLISECONDS);
                    else
                        _events.ScheduleEvent(EVENT_SUMMON_MAGE, 1);
                }
                else if (action == ACTION_SPAWN_ALL_ADDS)
                {
                    _events.ScheduleEvent(EVENT_ADDS, 12000);
                    _events.ScheduleEvent(EVENT_CHECK_RIFLEMAN, 13000);
                    _events.ScheduleEvent(EVENT_CHECK_MORTAR, 13000);
                    if (Is25ManRaid())
                        _controller.SummonCreatures(SLOT_MAGE_1, SLOT_MORTAR_4);
                    else
                    {
                        _controller.SummonCreatures(SLOT_MAGE_1, SLOT_MAGE_2);
                        _controller.SummonCreatures(SLOT_MORTAR_1, SLOT_MORTAR_2);
                        _controller.SummonCreatures(SLOT_RIFLEMAN_1, SLOT_RIFLEMAN_4);
                    }
                }
            }

            void SetData(uint32 type, uint32 data) OVERRIDE
            {
                if (type == ACTION_CLEAR_SLOT)
                {
                    _controller.ClearSlot(PassengerSlots(data));
                    if (data == SLOT_FREEZE_MAGE)
                        _events.ScheduleEvent(EVENT_SUMMON_MAGE, urand(30000, 33500));
                }
            }

            void sGossipSelect(Player* /*player*/, uint32 /*sender*/, uint32 /*action*/) OVERRIDE
            {
                me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                me->GetTransport()->EnableMovement(true);
                _events.SetPhase(PHASE_INTRO);
                _events.ScheduleEvent(EVENT_INTRO_A_1, 5000);
                _events.ScheduleEvent(EVENT_INTRO_A_2, 10000, 0, PHASE_INTRO);
                _events.ScheduleEvent(EVENT_INTRO_SUMMON_ORGRIMS_HAMMER, 28000, 0, PHASE_INTRO);
                _events.ScheduleEvent(EVENT_INTRO_A_3, 33000, 0, PHASE_INTRO);
                _events.ScheduleEvent(EVENT_INTRO_A_4, 39000, 0, PHASE_INTRO);
                _events.ScheduleEvent(EVENT_INTRO_A_5, 45000, 0, PHASE_INTRO);
            }

            void DamageTaken(Unit* , uint32& damage) OVERRIDE
            {
                if (me->HealthBelowPctDamaged(65, damage) && me->HasAura(SPELL_TASTE_OF_BLOOD))
                    DoCast(me, SPELL_TASTE_OF_BLOOD, true);

                if (damage > me->GetHealth())
                    damage = me->GetHealth() - 1;
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!UpdateVictim() && !_events.IsInPhase(PHASE_INTRO) && _instance->GetBossState(DATA_ICECROWN_GUNSHIP_BATTLE) != IN_PROGRESS)
                    return;

                _events.Update(diff);

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_INTRO_A_1:
                            Talk(SAY_MURADIN_INTRO_1);
                            break;
                        case EVENT_INTRO_A_2:
                            Talk(SAY_MURADIN_INTRO_2);
                            break;
                        case EVENT_INTRO_SUMMON_ORGRIMS_HAMMER:
                            sTransportMgr->CreateTransport(GO_ORGRIMS_HAMMER_A, 0, me->GetMap());
                            break;
                        case EVENT_INTRO_A_3:
                            Talk(SAY_MURADIN_INTRO_3);
                            break;
                        case EVENT_INTRO_A_4:
                            Talk(SAY_MURADIN_INTRO_4);
                            break;
                        case EVENT_INTRO_A_5:
                            Talk(SAY_MURADIN_INTRO_5);
                            break;
                        case EVENT_INTRO_A_6:
                            if (Creature* saurfang = me->FindNearestCreature(NPC_IGB_HIGH_OVERLORD_SAURFANG, 100.0f))
                                saurfang->AI()->Talk(SAY_SAURFANG_INTRO_A);
                            break;
                        case EVENT_INTRO_A_7:
                            Talk(SAY_MURADIN_INTRO_7);
                            break;
                        case EVENT_KEEP_PLAYER_IN_COMBAT:
                            if (_instance->GetBossState(DATA_ICECROWN_GUNSHIP_BATTLE) == IN_PROGRESS)
                            {
                                _instance->DoCastSpellOnPlayers(SPELL_LOCK_PLAYERS_AND_TAP_CHEST);
                                _events.ScheduleEvent(EVENT_KEEP_PLAYER_IN_COMBAT, urand(5000, 8000));
                            }
                            break;
                        case EVENT_SUMMON_MAGE:
                            Talk(SAY_MURADIN_SORCERERS);
                            _controller.SummonCreatures(SLOT_FREEZE_MAGE, SLOT_FREEZE_MAGE);
                            break;
                        case EVENT_ADDS:
                            Talk(SAY_MURADIN_ENTER_ORGRIMMS_HAMMER);
                            _controller.SummonCreatures(SLOT_MAGE_1, SLOT_MAGE_2);
                            _controller.SummonCreatures(SLOT_MARINE_1, Is25ManRaid() ? SLOT_MARINE_4 : SLOT_MARINE_2);
                            _controller.SummonCreatures(SLOT_SERGEANT_1, Is25ManRaid() ? SLOT_SERGEANT_2 : SLOT_SERGEANT_1);
                            if (Transport* skybreaker = me->GetTransport())
                                skybreaker->SummonPassenger(NPC_TELEPORT_PORTAL, SkybreakerTeleportPortal, TEMPSUMMON_TIMED_DESPAWN, NULL, 21000);

                            if (GameObject* go = HashMapHolder<GameObject>::Find(_instance->GetData64(DATA_ICECROWN_GUNSHIP_BATTLE)))
                                if (Transport* orgrimsHammer = go->ToTransport())
                                    orgrimsHammer->SummonPassenger(NPC_TELEPORT_EXIT, OrgrimsHammerTeleportExit, TEMPSUMMON_TIMED_DESPAWN, NULL, 23000);

                            _events.ScheduleEvent(EVENT_ADDS_BOARD_YELL, 6000);
                            _events.ScheduleEvent(EVENT_ADDS, 60000);
                            break;
                        case EVENT_ADDS_BOARD_YELL:
                            if (Creature* saurfang = me->FindNearestCreature(NPC_IGB_HIGH_OVERLORD_SAURFANG, 200.0f))
                                saurfang->AI()->Talk(SAY_SAURFANG_BOARD);
                            break;
                        case EVENT_CHECK_RIFLEMAN:
                            if (_controller.SummonCreatures(SLOT_RIFLEMAN_1, Is25ManRaid() ? SLOT_RIFLEMAN_8 : SLOT_RIFLEMAN_4))
                            {
                                if (_riflemanYellCooldown < time(NULL))
                                {
                                    Talk(SAY_MURADIN_RIFLEMAN);
                                    _riflemanYellCooldown = time(NULL) + 5;
                                }
                            }
                            _events.ScheduleEvent(EVENT_CHECK_RIFLEMAN, 1000);
                            break;
                        case EVENT_CHECK_MORTAR:
                            if (_controller.SummonCreatures(SLOT_MORTAR_1, Is25ManRaid() ? SLOT_MORTAR_4 : SLOT_MORTAR_2))
                            {
                                if (_mortarYellCooldown < time(NULL))
                                {
                                    Talk(SAY_MURADIN_MORTAR);
                                    _mortarYellCooldown = time(NULL) + 5;
                                }
                            }
                            _events.ScheduleEvent(EVENT_CHECK_MORTAR, 1000);
                            break;
                        case EVENT_CLEAVE:
                            DoCastVictim(SPELL_CLEAVE);
                            _events.ScheduleEvent(EVENT_CLEAVE, urand(2000, 10000));
                            break;
                        default:
                            break;
                    }
                }

                if (me->IsWithinMeleeRange(me->GetVictim()))
                    DoMeleeAttackIfReady();
                else if (me->isAttackReady())
                {
                    DoCastVictim(SPELL_RENDING_THROW);
                    me->resetAttackTimer();
                }
            }

            bool CanAIAttack(Unit const* target) const OVERRIDE
            {
                return target->HasAura(SPELL_ON_SKYBREAKER_DECK) || !target->IsControlledByPlayer();
            }

        private:
            EventMap _events;
            PassengerController _controller;
            InstanceScript* _instance;
            time_t _firstMageCooldown;
            time_t _riflemanYellCooldown;
            time_t _mortarYellCooldown;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetIcecrownCitadelAI<npc_muradin_bronzebeard_igbAI>(creature);
        }
};

class npc_zafod_boombox : public CreatureScript
{
    public:
        npc_zafod_boombox() : CreatureScript("npc_zafod_boombox") { }

        struct npc_zafod_boomboxAI : public gunship_npc_AI
        {
            npc_zafod_boomboxAI(Creature* creature) : gunship_npc_AI(creature)
            {
            }

            void Reset() OVERRIDE
            {
                me->SetReactState(REACT_PASSIVE);
            }

            void sGossipSelect(Player* player, uint32 /*sender*/, uint32 /*action*/) OVERRIDE
            {
                player->AddItem(ITEM_GOBLIN_ROCKET_PACK, 1);
                player->PlayerTalkClass->SendCloseGossip();
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE
            {
                UpdateVictim();
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetIcecrownCitadelAI<npc_zafod_boomboxAI>(creature);
        }
};

struct npc_gunship_boarding_addAI : public gunship_npc_AI
{
    npc_gunship_boarding_addAI(Creature* creature) : gunship_npc_AI(creature)
    {
        me->m_CombatDistance = 80.0f;
        _usedDesperateResolve = false;
    }

    void SetData(uint32 type, uint32 data) OVERRIDE
    {
        // detach from captain
        if (type == ACTION_SET_SLOT)
        {
            SetSlotInfo(data);

            me->SetReactState(REACT_PASSIVE);

            me->m_Events.AddEvent(new DelayedMovementEvent(me, Slot->TargetPosition), me->m_Events.CalculateTime(3000 * (Index - SLOT_MARINE_1)));

            if (Creature* captain = me->FindNearestCreature(Instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE ? NPC_IGB_MURADIN_BRONZEBEARD : NPC_IGB_HIGH_OVERLORD_SAURFANG, 200.0f))
                captain->AI()->SetData(ACTION_CLEAR_SLOT, Index);
        }
    }

    void MovementInform(uint32 type, uint32 pointId) OVERRIDE
    {
        if (type != POINT_MOTION_TYPE)
            return;

        if (pointId == EVENT_CHARGE_PREPATH && Slot)
        {
            Position const& otherTransportPos = Instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE ? OrgrimsHammerTeleportExit : SkybreakerTeleportExit;
            float x, y, z, o;
            otherTransportPos.GetPosition(x, y, z, o);

            Transport* myTransport = me->GetTransport();
            if (!myTransport)
                return;

            if (GameObject* go = HashMapHolder<GameObject>::Find(Instance->GetData64(DATA_ICECROWN_GUNSHIP_BATTLE)))
                if (Transport* destTransport = go->ToTransport())
                    destTransport->CalculatePassengerPosition(x, y, z, &o);

            float angle = frand(0, M_PI * 2.0f);
            x += 2.0f * std::cos(angle);
            y += 2.0f * std::sin(angle);

            me->SetHomePosition(x, y, z, o);
            myTransport->CalculatePassengerOffset(x, y, z, &o);
            me->SetTransportHomePosition(x, y, z, o);

            me->m_Events.AddEvent(new BattleExperienceEvent(me), me->m_Events.CalculateTime(BattleExperienceEvent::ExperiencedTimes[0]));
            DoCast(me, SPELL_BATTLE_EXPERIENCE, true);
            DoCast(me, SPELL_TELEPORT_TO_ENEMY_SHIP, true);
            DoCast(me, Instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE ? SPELL_MELEE_TARGETING_ON_ORGRIMS_HAMMER : SPELL_MELEE_TARGETING_ON_SKYBREAKER, true);
            me->_AddCreatureSpellCooldown(BurningPitchId, time(NULL) + 3);

            std::list<Player*> players;
            Trinity::UnitAuraCheck check(true, Instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE ? SPELL_ON_ORGRIMS_HAMMER_DECK : SPELL_ON_SKYBREAKER_DECK);
            Trinity::PlayerListSearcher<Trinity::UnitAuraCheck> searcher(me, players, check);
            me->VisitNearbyWorldObject(200.0f, searcher);

            players.remove_if([this](Player* player)
            {
                return !me->_IsTargetAcceptable(player) || !me->CanStartAttack(player, true);
            });

            if (!players.empty())
            {
                players.sort(Trinity::ObjectDistanceOrderPred(me));
                for (std::list<Player*>::iterator itr = players.begin(); itr != players.end(); ++itr)
                    me->AddThreat(*itr, 1.0f);

                AttackStart(players.front());
            }

            me->SetReactState(REACT_AGGRESSIVE);
        }
    }

    void DamageTaken(Unit* /*attacker*/, uint32& damage) OVERRIDE
    {
        if (_usedDesperateResolve)
            return;

        if (!me->HealthBelowPctDamaged(25, damage))
            return;

        _usedDesperateResolve = true;
        DoCast(me, SPELL_DESPERATE_RESOLVE, true);
    }

    void UpdateAI(uint32 /*diff*/) OVERRIDE
    {
        if (!SelectVictim())
        {
            TriggerBurningPitch();
            return;
        }

        if (!HasAttackablePlayerNearby())
            TriggerBurningPitch();

        DoMeleeAttackIfReady();
    }

    bool CanAIAttack(Unit const* target) const OVERRIDE
    {
        uint32 spellId = SPELL_ON_SKYBREAKER_DECK;
        uint32 creatureEntry = NPC_IGB_MURADIN_BRONZEBEARD;
        if (Instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE)
        {
            spellId = SPELL_ON_ORGRIMS_HAMMER_DECK;
            creatureEntry = NPC_IGB_HIGH_OVERLORD_SAURFANG;
        }

        return target->HasAura(spellId) || target->GetEntry() == creatureEntry;
    }

    bool HasAttackablePlayerNearby()
    {
        std::list<Player*> players;
        Trinity::UnitAuraCheck check(true, Instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE ? SPELL_ON_ORGRIMS_HAMMER_DECK : SPELL_ON_SKYBREAKER_DECK);
        Trinity::PlayerListSearcher<Trinity::UnitAuraCheck> searcher(me, players, check);
        me->VisitNearbyWorldObject(200.0f, searcher);

        players.remove_if([this](Player* player)
        {
            return !me->_IsTargetAcceptable(player) || !me->CanStartAttack(player, true);
        });

        return !players.empty();
    }

private:
    bool _usedDesperateResolve;
};

class npc_gunship_boarding_leader : public CreatureScript
{
    public:
        npc_gunship_boarding_leader() : CreatureScript("npc_gunship_boarding_leader") { }

        struct npc_gunship_boarding_leaderAI : public npc_gunship_boarding_addAI
        {
            npc_gunship_boarding_leaderAI(Creature* creature) : npc_gunship_boarding_addAI(creature)
            {
            }

            void EnterCombat(Unit* target) OVERRIDE
            {
                npc_gunship_boarding_addAI::EnterCombat(target);
                _events.ScheduleEvent(EVENT_BLADESTORM, urand(13000, 18000));
                _events.ScheduleEvent(EVENT_WOUNDING_STRIKE, urand(8000, 10000));
            }

            void UpdateAI(uint32 diff) OVERRIDE
            {
                if (!SelectVictim())
                {
                    TriggerBurningPitch();
                    return;
                }

                _events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING) || me->HasAura(SPELL_BLADESTORM))
                    return;

                if (!HasAttackablePlayerNearby())
                    TriggerBurningPitch();

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BLADESTORM:
                            DoCastAOE(SPELL_BLADESTORM);
                            _events.ScheduleEvent(EVENT_BLADESTORM, urand(25000, 30000));
                            break;
                        case EVENT_WOUNDING_STRIKE:
                            DoCastVictim(SPELL_WOUNDING_STRIKE);
                            _events.ScheduleEvent(EVENT_WOUNDING_STRIKE, urand(9000, 13000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            EventMap _events;
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetIcecrownCitadelAI<npc_gunship_boarding_leaderAI>(creature);
        }
};

class npc_gunship_boarding_add : public CreatureScript
{
    public:
        npc_gunship_boarding_add() : CreatureScript("npc_gunship_boarding_add") { }


        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetIcecrownCitadelAI<npc_gunship_boarding_addAI>(creature);
        }
};

class npc_gunship_gunner : public CreatureScript
{
    public:
        npc_gunship_gunner() : CreatureScript("npc_gunship_gunner") { }

        struct npc_gunship_gunnerAI : public gunship_npc_AI
        {
            npc_gunship_gunnerAI(Creature* creature) : gunship_npc_AI(creature)
            {
                creature->m_CombatDistance = 200.0f;
            }

            void AttackStart(Unit* target) OVERRIDE
            {
                me->Attack(target, false);
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                gunship_npc_AI::MovementInform(type, pointId);
                if (type == POINT_MOTION_TYPE && pointId == EVENT_CHARGE_PREPATH)
                    me->SetControlled(true, UNIT_STATE_ROOT);
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE
            {
                if (!SelectVictim())
                {
                    TriggerBurningPitch();
                    return;
                }

                DoSpellAttackIfReady(me->GetEntry() == NPC_SKYBREAKER_RIFLEMAN ? SPELL_SHOOT : SPELL_HURL_AXE);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetIcecrownCitadelAI<npc_gunship_gunnerAI>(creature);
        }
};

class npc_gunship_rocketeer : public CreatureScript
{
    public:
        npc_gunship_rocketeer() : CreatureScript("npc_gunship_rocketeer") { }

        struct npc_gunship_rocketeerAI : public gunship_npc_AI
        {
            npc_gunship_rocketeerAI(Creature* creature) : gunship_npc_AI(creature)
            {
                creature->m_CombatDistance = 200.0f;
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                gunship_npc_AI::MovementInform(type, pointId);
                if (type == POINT_MOTION_TYPE && pointId == EVENT_CHARGE_PREPATH)
                    me->SetControlled(true, UNIT_STATE_ROOT);
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE
            {
                if (!SelectVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                uint32 spellId = me->GetEntry() == NPC_SKYBREAKER_MORTAR_SOLDIER ? SPELL_ROCKET_ARTILLERY_A : SPELL_ROCKET_ARTILLERY_H;
                if (me->HasSpellCooldown(spellId))
                    return;

                DoCastAOE(spellId, true);
                me->_AddCreatureSpellCooldown(spellId, time(NULL) + 9);
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetIcecrownCitadelAI<npc_gunship_rocketeerAI>(creature);
        }
};

class npc_gunship_mage : public CreatureScript
{
    public:
        npc_gunship_mage() : CreatureScript("npc_gunship_mage") { }

        struct npc_gunship_mageAI : public gunship_npc_AI
        {
            npc_gunship_mageAI(Creature* creature) : gunship_npc_AI(creature)
            {
                me->SetReactState(REACT_PASSIVE);
            }

            void EnterEvadeMode() OVERRIDE
            {
                ScriptedAI::EnterEvadeMode();
            }

            void MovementInform(uint32 type, uint32 pointId) OVERRIDE
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                if (pointId == EVENT_CHARGE_PREPATH && Slot)
                {
                    SlotInfo const* slots = Instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE ? SkybreakerSlotInfo : OrgrimsHammerSlotInfo;
                    me->SetFacingTo(slots[Index].TargetPosition.GetOrientation());
                    switch (Index)
                    {
                        case SLOT_FREEZE_MAGE:
                            DoCastAOE(SPELL_BELOW_ZERO);
                            break;
                        case SLOT_MAGE_1:
                        case SLOT_MAGE_2:
                            DoCastAOE(SPELL_SHADOW_CHANNELING);
                            break;
                        default:
                            break;
                    }

                    me->SetControlled(true, UNIT_STATE_ROOT);
                }
            }

            void UpdateAI(uint32 /*diff*/) OVERRIDE
            {
                UpdateVictim();
            }

            bool CanAIAttack(Unit const* /*target*/) const OVERRIDE
            {
                return true;
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return GetIcecrownCitadelAI<npc_gunship_mageAI>(creature);
        }
};

/** @HACK This AI only resets MOVEMENTFLAG_ROOT on the vehicle.
          Currently the core always removes MOVEMENTFLAG_ROOT sent from client packets to prevent cheaters from freezing clients of other players
          but it actually is a valid flag - needs more research to fix both freezes and keep the flag as is (see WorldSession::ReadMovementInfo)

Example packet:
ClientToServer: CMSG_FORCE_MOVE_ROOT_ACK (0x00E9) Length: 67 ConnectionIndex: 0 Time: 03/04/2010 03:57:55.000 Number: 471326
Guid:
Movement Counter: 80
Movement Flags: OnTransport, Root (2560)
Extra Movement Flags: None (0)
Time: 52291611
Position: X: -396.0302 Y: 2482.906 Z: 249.86
Orientation: 1.468665
Transport GUID: Full: 0x1FC0000000000460 Type: MOTransport Low: 1120
Transport Position: X: -6.152398 Y: -23.49037 Z: 21.64464 O: 4.827727
Transport Time: 9926
Transport Seat: 255
Fall Time: 824
*/
class npc_gunship_cannon : public CreatureScript
{
    public:
        npc_gunship_cannon() : CreatureScript("npc_gunship_cannon") { }

        struct npc_gunship_cannonAI : public PassiveAI
        {
            npc_gunship_cannonAI(Creature* creature) : PassiveAI(creature)
            {
            }

            void OnCharmed(bool /*apply*/) OVERRIDE { }

            void PassengerBoarded(Unit* /*passenger*/, int8 /*seat*/, bool apply) OVERRIDE
            {
                if (!apply)
                {
                    me->SetControlled(false, UNIT_STATE_ROOT);
                    me->SetControlled(true, UNIT_STATE_ROOT);
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const OVERRIDE
        {
            return new npc_gunship_cannonAI(creature);
        }
};

class spell_igb_rocket_pack : public SpellScriptLoader
{
    public:
        spell_igb_rocket_pack() : SpellScriptLoader("spell_igb_rocket_pack") { }

        class spell_igb_rocket_pack_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_igb_rocket_pack_AuraScript);

            bool Validate(SpellInfo const* /*spellInfo*/) OVERRIDE
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_ROCKET_PACK_DAMAGE) ||
                    !sSpellMgr->GetSpellInfo(SPELL_ROCKET_BURST))
                    return false;

                return true;
            }

            void HandlePeriodic(AuraEffect const* /*aurEff*/)
            {
                if (GetTarget()->movespline->Finalized())
                    Remove(AURA_REMOVE_BY_EXPIRE);
            }

            void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes /*mode*/)
            {
                SpellInfo const* damageInfo = sSpellMgr->GetSpellInfo(SPELL_ROCKET_PACK_DAMAGE);
                GetTarget()->CastCustomSpell(SPELL_ROCKET_PACK_DAMAGE, SPELLVALUE_BASE_POINT0, 2 * (damageInfo->Effects[EFFECT_0].CalcValue() + aurEff->GetTickNumber() * aurEff->GetAmplitude()), NULL, TRIGGERED_FULL_MASK);
                GetTarget()->CastSpell(NULL, SPELL_ROCKET_BURST, TRIGGERED_FULL_MASK);
            }

            void Register() OVERRIDE
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_igb_rocket_pack_AuraScript::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
                OnEffectRemove += AuraEffectRemoveFn(spell_igb_rocket_pack_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_igb_rocket_pack_AuraScript();
        }
};

class spell_igb_rocket_pack_useable : public SpellScriptLoader
{
    public:
        spell_igb_rocket_pack_useable() : SpellScriptLoader("spell_igb_rocket_pack_useable") { }

        class spell_igb_rocket_pack_useable_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_igb_rocket_pack_useable_AuraScript);

            bool Load()
            {
                return GetOwner()->GetInstanceScript();
            }

            bool CheckAreaTarget(Unit* target)
            {
                return target->GetTypeId() == TYPEID_PLAYER && GetOwner()->GetInstanceScript()->GetBossState(DATA_ICECROWN_GUNSHIP_BATTLE) != DONE;
            }

            void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Creature* owner = GetOwner()->ToCreature())
                    if (Player* target = GetTarget()->ToPlayer())
                        if (target->HasItemCount(ITEM_GOBLIN_ROCKET_PACK, 1))
                            sCreatureTextMgr->SendChat(owner, SAY_ZAFOD_ROCKET_PACK_ACTIVE, target, CHAT_MSG_ADDON, LANG_ADDON, TEXT_RANGE_NORMAL, 0, TEAM_OTHER, false, target);
            }

            void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Creature* owner = GetOwner()->ToCreature())
                    if (Player* target = GetTarget()->ToPlayer())
                        if (target->HasItemCount(ITEM_GOBLIN_ROCKET_PACK, 1))
                            sCreatureTextMgr->SendChat(owner, SAY_ZAFOD_ROCKET_PACK_DISABLED, target, CHAT_MSG_ADDON, LANG_ADDON, TEXT_RANGE_NORMAL, 0, TEAM_OTHER, false, target);
            }

            void Register() OVERRIDE
            {
                DoCheckAreaTarget += AuraCheckAreaTargetFn(spell_igb_rocket_pack_useable_AuraScript::CheckAreaTarget);
                AfterEffectApply += AuraEffectApplyFn(spell_igb_rocket_pack_useable_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_igb_rocket_pack_useable_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_igb_rocket_pack_useable_AuraScript();
        }
};

class spell_igb_on_gunship_deck : public SpellScriptLoader
{
    public:
        spell_igb_on_gunship_deck() : SpellScriptLoader("spell_igb_on_gunship_deck") { }

        class spell_igb_on_gunship_deck_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_igb_on_gunship_deck_AuraScript);

            bool Load() OVERRIDE
            {
                if (InstanceScript* instance = GetOwner()->GetInstanceScript())
                    _teamInInstance = instance->GetData(DATA_TEAM_IN_INSTANCE);
                else
                    _teamInInstance = 0;
                return true;
            }

            bool CheckAreaTarget(Unit* unit)
            {
                return unit->GetTypeId() == TYPEID_PLAYER;
            }

            void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetSpellInfo()->Id == uint32(_teamInInstance == HORDE ? SPELL_ON_SKYBREAKER_DECK : SPELL_ON_ORGRIMS_HAMMER_DECK))
                    if (Creature* gunship = GetOwner()->FindNearestCreature(_teamInInstance == HORDE ? NPC_ORGRIMS_HAMMER : NPC_THE_SKYBREAKER, 200.0f))
                        gunship->AI()->SetGUID(GetTarget()->GetGUID(), ACTION_SHIP_VISITS);
            }

            void Register() OVERRIDE
            {
                DoCheckAreaTarget += AuraCheckAreaTargetFn(spell_igb_on_gunship_deck_AuraScript::CheckAreaTarget);
                AfterEffectApply += AuraEffectApplyFn(spell_igb_on_gunship_deck_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }

            uint32 _teamInInstance;
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_igb_on_gunship_deck_AuraScript();
        }
};

class spell_igb_periodic_trigger_with_power_cost : public SpellScriptLoader
{
    public:
        spell_igb_periodic_trigger_with_power_cost() : SpellScriptLoader("spell_igb_periodic_trigger_with_power_cost") { }

        class spell_igb_periodic_trigger_with_power_cost_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_igb_periodic_trigger_with_power_cost_AuraScript);

            void HandlePeriodicTick(AuraEffect const* /*aurEff*/)
            {
                PreventDefaultAction();
                GetTarget()->CastSpell(GetTarget(), GetSpellInfo()->Effects[EFFECT_0].TriggerSpell, TriggerCastFlags(TRIGGERED_FULL_MASK & ~TRIGGERED_IGNORE_POWER_AND_REAGENT_COST));
            }

            void Register() OVERRIDE
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_igb_periodic_trigger_with_power_cost_AuraScript::HandlePeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_igb_periodic_trigger_with_power_cost_AuraScript();
        }
};

class spell_igb_cannon_blast : public SpellScriptLoader
{
    public:
        spell_igb_cannon_blast() : SpellScriptLoader("spell_igb_cannon_blast") { }

        class spell_igb_cannon_blast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_igb_cannon_blast_SpellScript);

            bool Load()
            {
                return GetCaster()->GetTypeId() == TYPEID_UNIT;
            }

            void CheckEnergy()
            {
                if (GetCaster()->GetPower(POWER_ENERGY) >= 100)
                {
                    GetCaster()->CastSpell(GetCaster(), SPELL_OVERHEAT, TRIGGERED_FULL_MASK);
                    if (Vehicle* vehicle = GetCaster()->GetVehicleKit())
                        if (Unit* passenger = vehicle->GetPassenger(0))
                            sCreatureTextMgr->SendChat(GetCaster()->ToCreature(), SAY_OVERHEAT, passenger);
                }
            }

            void Register() OVERRIDE
            {
                AfterHit += SpellHitFn(spell_igb_cannon_blast_SpellScript::CheckEnergy);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_igb_cannon_blast_SpellScript();
        }
};

class spell_igb_incinerating_blast : public SpellScriptLoader
{
    public:
        spell_igb_incinerating_blast() : SpellScriptLoader("spell_igb_incinerating_blast") { }

        class spell_igb_incinerating_blast_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_igb_incinerating_blast_SpellScript);

            void StoreEnergy()
            {
                _energyLeft = GetCaster()->GetPower(POWER_ENERGY) - 10;
            }

            void RemoveEnergy()
            {
                GetCaster()->SetPower(POWER_ENERGY, 0);
            }

            void CalculateDamage(SpellEffIndex /*effIndex*/)
            {
                SetEffectValue(GetEffectValue() + _energyLeft * _energyLeft * 8);
            }

            void Register() OVERRIDE
            {
                OnCast += SpellCastFn(spell_igb_incinerating_blast_SpellScript::StoreEnergy);
                AfterCast += SpellCastFn(spell_igb_incinerating_blast_SpellScript::RemoveEnergy);
                OnEffectLaunchTarget += SpellEffectFn(spell_igb_incinerating_blast_SpellScript::CalculateDamage, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
            }

            uint32 _energyLeft;
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_igb_incinerating_blast_SpellScript();
        }
};

class spell_igb_overheat : public SpellScriptLoader
{
    public:
        spell_igb_overheat() : SpellScriptLoader("spell_igb_overheat") { }

        class spell_igb_overheat_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_igb_overheat_AuraScript);

            bool Load() OVERRIDE
            {
                if (GetAura()->GetType() != UNIT_AURA_TYPE)
                    return false;
                return GetUnitOwner()->IsVehicle();
            }

            void SendClientControl(uint8 value)
            {
                if (Vehicle* vehicle = GetUnitOwner()->GetVehicleKit())
                {
                    if (Unit* passenger = vehicle->GetPassenger(0))
                    {
                        if (Player* player = passenger->ToPlayer())
                        {
                            WorldPacket data(SMSG_CLIENT_CONTROL_UPDATE, GetUnitOwner()->GetPackGUID().size() + 1);
                            data.append(GetUnitOwner()->GetPackGUID());
                            data << uint8(value);
                            player->GetSession()->SendPacket(&data);
                        }
                    }
                }
            }

            void HandleApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                SendClientControl(0);
            }

            void HandleRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                SendClientControl(1);
            }

            void Register() OVERRIDE
            {
                AfterEffectApply += AuraEffectApplyFn(spell_igb_overheat_AuraScript::HandleApply, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_igb_overheat_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const OVERRIDE
        {
            return new spell_igb_overheat_AuraScript();
        }
};

class spell_igb_below_zero : public SpellScriptLoader
{
    public:
        spell_igb_below_zero() : SpellScriptLoader("spell_igb_below_zero") { }

        class spell_igb_below_zero_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_igb_below_zero_SpellScript);

            void RemovePassengers()
            {
                GetHitUnit()->CastSpell(GetHitUnit(), SPELL_EJECT_ALL_PASSENGERS_BELOW_ZERO, TRIGGERED_FULL_MASK);
            }

            void Register() OVERRIDE
            {
                BeforeHit += SpellHitFn(spell_igb_below_zero_SpellScript::RemovePassengers);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_igb_below_zero_SpellScript();
        }
};

class spell_igb_teleport_to_enemy_ship : public SpellScriptLoader
{
    public:
        spell_igb_teleport_to_enemy_ship() : SpellScriptLoader("spell_igb_teleport_to_enemy_ship") { }

        class spell_igb_teleport_to_enemy_ship_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_igb_teleport_to_enemy_ship_SpellScript);

            void RelocateTransportOffset(SpellEffIndex /*effIndex*/)
            {
                WorldLocation const* dest = GetHitDest();
                Unit* target = GetHitUnit();
                if (!dest || !target || !target->GetTransport())
                    return;

                float x, y, z, o;
                dest->GetPosition(x, y, z, o);
                target->GetTransport()->CalculatePassengerOffset(x, y, z, &o);
                target->m_movementInfo.transport.pos.Relocate(x, y, z, o);
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_igb_teleport_to_enemy_ship_SpellScript::RelocateTransportOffset, EFFECT_0, SPELL_EFFECT_TELEPORT_UNITS);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_igb_teleport_to_enemy_ship_SpellScript();
        }
};

class spell_igb_burning_pitch_selector : public SpellScriptLoader
{
    public:
        spell_igb_burning_pitch_selector() : SpellScriptLoader("spell_igb_burning_pitch_selector") { }

        class spell_igb_burning_pitch_selector_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_igb_burning_pitch_selector_SpellScript);

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                uint32 team = HORDE;
                if (InstanceScript* instance = GetCaster()->GetInstanceScript())
                    team = instance->GetData(DATA_TEAM_IN_INSTANCE);

                targets.remove_if([team](WorldObject* target) -> bool
                {
                    if (Transport* transport = target->GetTransport())
                        return transport->GetEntry() != uint32(team == HORDE ? GO_ORGRIMS_HAMMER_H : GO_THE_SKYBREAKER_A);
                    return true;
                });

                if (!targets.empty())
                {
                    WorldObject* target = Trinity::Containers::SelectRandomContainerElement(targets);
                    targets.clear();
                    targets.push_back(target);
                }
            }

            void HandleDummy(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                GetCaster()->CastSpell(GetHitUnit(), uint32(GetEffectValue()), TRIGGERED_NONE);
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_igb_burning_pitch_selector_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
                OnEffectHitTarget += SpellEffectFn(spell_igb_burning_pitch_selector_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_igb_burning_pitch_selector_SpellScript();
        }
};

class spell_igb_burning_pitch : public SpellScriptLoader
{
    public:
        spell_igb_burning_pitch() : SpellScriptLoader("spell_igb_burning_pitch") { }

        class spell_igb_burning_pitch_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_igb_burning_pitch_SpellScript);

            void HandleDummy(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                GetCaster()->CastCustomSpell(uint32(GetEffectValue()), SPELLVALUE_BASE_POINT0, 8000, NULL, TRIGGERED_FULL_MASK);
                GetHitUnit()->CastSpell(GetHitUnit(), SPELL_BURNING_PITCH, TRIGGERED_FULL_MASK);
            }

            void Register() OVERRIDE
            {
                OnEffectHitTarget += SpellEffectFn(spell_igb_burning_pitch_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_igb_burning_pitch_SpellScript();
        }
};

class spell_igb_rocket_artillery : public SpellScriptLoader
{
    public:
        spell_igb_rocket_artillery() : SpellScriptLoader("spell_igb_rocket_artillery") { }

        class spell_igb_rocket_artillery_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_igb_rocket_artillery_SpellScript);

            void SelectRandomTarget(std::list<WorldObject*>& targets)
            {
                if (!targets.empty())
                {
                    WorldObject* target = Trinity::Containers::SelectRandomContainerElement(targets);
                    targets.clear();
                    targets.push_back(target);
                }
            }

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                GetCaster()->CastSpell(GetHitUnit(), uint32(GetEffectValue()), TRIGGERED_NONE);
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_igb_rocket_artillery_SpellScript::SelectRandomTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnEffectHitTarget += SpellEffectFn(spell_igb_rocket_artillery_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_igb_rocket_artillery_SpellScript();
        }
};

class spell_igb_rocket_artillery_explosion : public SpellScriptLoader
{
    public:
        spell_igb_rocket_artillery_explosion() : SpellScriptLoader("spell_igb_rocket_artillery_explosion") { }

        class spell_igb_rocket_artillery_explosion_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_igb_rocket_artillery_explosion_SpellScript);

            void DamageGunship(SpellEffIndex /*effIndex*/)
            {
                if (InstanceScript* instance = GetCaster()->GetInstanceScript())
                    GetCaster()->CastCustomSpell(instance->GetData(DATA_TEAM_IN_INSTANCE) == HORDE ? SPELL_BURNING_PITCH_DAMAGE_A : SPELL_BURNING_PITCH_DAMAGE_H, SPELLVALUE_BASE_POINT0, 5000, NULL, TRIGGERED_FULL_MASK);
            }

            void Register() OVERRIDE
            {
                OnEffectHit += SpellEffectFn(spell_igb_rocket_artillery_explosion_SpellScript::DamageGunship, EFFECT_0, SPELL_EFFECT_TRIGGER_MISSILE);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_igb_rocket_artillery_explosion_SpellScript();
        }
};

class spell_igb_gunship_fall_teleport : public SpellScriptLoader
{
    public:
        spell_igb_gunship_fall_teleport() : SpellScriptLoader("spell_igb_gunship_fall_teleport") { }

        class spell_igb_gunship_fall_teleport_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_igb_gunship_fall_teleport_SpellScript);

            bool Load()
            {
                return GetCaster()->GetInstanceScript();
            }

            void SelectTransport(WorldObject*& target)
            {
                if (InstanceScript* instance = target->GetInstanceScript())
                    target = HashMapHolder<GameObject>::Find(instance->GetData64(DATA_ICECROWN_GUNSHIP_BATTLE));
            }

            void RelocateDest(SpellEffIndex /*effIndex*/)
            {
				static Position const woqu1 = {0.0f, 0.0f, 36.0f, 0.0f};
				static Position const woqu2 = {0.0f, 0.0f, 21.0f, 0.0f};
                if (GetCaster()->GetInstanceScript()->GetData(DATA_TEAM_IN_INSTANCE) == HORDE)
                    GetHitDest()->RelocateOffset(woqu1);
                else
                    GetHitDest()->RelocateOffset(woqu2);
            }

            void Register() OVERRIDE
            {
                OnObjectTargetSelect += SpellObjectTargetSelectFn(spell_igb_gunship_fall_teleport_SpellScript::SelectTransport, EFFECT_0, TARGET_DEST_NEARBY_ENTRY);
                OnEffectLaunch += SpellEffectFn(spell_igb_gunship_fall_teleport_SpellScript::RelocateDest, EFFECT_0, SPELL_EFFECT_TELEPORT_UNITS);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_igb_gunship_fall_teleport_SpellScript();
        }
};

class spell_igb_check_for_players : public SpellScriptLoader
{
    public:
        spell_igb_check_for_players() : SpellScriptLoader("spell_igb_check_for_players") { }

        class spell_igb_check_for_players_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_igb_check_for_players_SpellScript);

            bool Load() OVERRIDE
            {
                _playerCount = 0;
                return GetCaster()->GetTypeId() == TYPEID_UNIT;
            }

            void CountTargets(std::list<WorldObject*>& targets)
            {
                _playerCount = targets.size();
            }

            void TriggerWipe()
            {
                if (!_playerCount)
                    GetCaster()->ToCreature()->AI()->JustDied(NULL);
            }

            void TeleportPlayer(SpellEffIndex /*effIndex*/)
            {
                if (GetHitUnit()->GetPositionZ() < GetCaster()->GetPositionZ() - 10.0f)
                    GetHitUnit()->CastSpell(GetHitUnit(), SPELL_GUNSHIP_FALL_TELEPORT, TRIGGERED_FULL_MASK);
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_igb_check_for_players_SpellScript::CountTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
                AfterCast += SpellCastFn(spell_igb_check_for_players_SpellScript::TriggerWipe);
                OnEffectHitTarget += SpellEffectFn(spell_igb_check_for_players_SpellScript::TeleportPlayer, EFFECT_0, SPELL_EFFECT_DUMMY);
            }

            uint32 _playerCount;
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_igb_check_for_players_SpellScript();
        }
};

class spell_igb_teleport_players_on_victory : public SpellScriptLoader
{
    public:
        spell_igb_teleport_players_on_victory() : SpellScriptLoader("spell_igb_teleport_players_on_victory") { }

        class spell_igb_teleport_players_on_victory_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_igb_teleport_players_on_victory_SpellScript);

            bool Load() OVERRIDE
            {
                return GetCaster()->GetInstanceScript();
            }

            void FilterTargets(std::list<WorldObject*>& targets)
            {
                InstanceScript* instance = GetCaster()->GetInstanceScript();
                targets.remove_if([instance](WorldObject* target) -> bool
                {
                    return target->GetTransGUID() != instance->GetData64(DATA_ENEMY_GUNSHIP);
                });
            }

            void Register() OVERRIDE
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_igb_teleport_players_on_victory_SpellScript::FilterTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const OVERRIDE
        {
            return new spell_igb_teleport_players_on_victory_SpellScript();
        }
};

class achievement_im_on_a_boat : public AchievementCriteriaScript
{
    public:
        achievement_im_on_a_boat() : AchievementCriteriaScript("achievement_im_on_a_boat") { }

        bool OnCheck(Player* /*source*/, Unit* target) OVERRIDE
        {
            return target->GetAI() && target->GetAI()->GetData(ACTION_SHIP_VISITS) <= 2;
        }
};

void AddSC_boss_icecrown_gunship_battle()
{
    new npc_gunship();
    new npc_high_overlord_saurfang_igb();
    new npc_muradin_bronzebeard_igb();
    new npc_zafod_boombox();
    new npc_gunship_boarding_leader();
    new npc_gunship_boarding_add();
    new npc_gunship_gunner();
    new npc_gunship_rocketeer();
    new npc_gunship_mage();
    new npc_gunship_cannon();
    new spell_igb_rocket_pack();
    new spell_igb_rocket_pack_useable();
    new spell_igb_on_gunship_deck();
    new spell_igb_periodic_trigger_with_power_cost();
    new spell_igb_cannon_blast();
    new spell_igb_incinerating_blast();
    new spell_igb_overheat();
    new spell_igb_below_zero();
    new spell_igb_teleport_to_enemy_ship();
    new spell_igb_burning_pitch_selector();
    new spell_igb_burning_pitch();
    new spell_igb_rocket_artillery();
    new spell_igb_rocket_artillery_explosion();
    new spell_igb_gunship_fall_teleport();
    new spell_igb_check_for_players();
    new spell_igb_teleport_players_on_victory();
    new achievement_im_on_a_boat();
}
