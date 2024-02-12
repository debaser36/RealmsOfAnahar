#pragma once
#include "../../DevUtils/include/FunctionCollection.h"
#include "../../DevUtils/include/GeneralUtil.h"


namespace ROA
{
	enum class e_GameElementTypes
	{
		UNKNOWN = 0,
		unit,
		tile,
		item,
		weapon,
		building,
		static_other
	};
	enum class e_GameElementValues
	{
		custom = 0,
		tile_defense,
		unit_hp,
		unit_xp,
		unit_moves,
		unit_actionCount,
		unit_movementProfile,
		weapon_damage,
		weapon_attackCount,
		weapon_attackRange,
		weapon_damageType,
		player_moneyIncome
	};
	enum class e_GameEffectType
	{
		custom = 0,
		valueBasedAbsolut,
		valueBasedPercentage,
		functionBased
	};
	struct EffectProfile
	{
		e_GameEffectType effectType;
		bool b_isPermanent;
		unsigned int roundsLeft;
		std::vector<e_GameElementValues> effectedValues_Names;
		std::vector<unsigned char> effectedValues_Values;
		UTIL::FunctionCollection functionsToCall;

		/**
		 * \brief just a wrapper for GameElementEffect --> use that instead!
		 */
		EffectProfile() = default;
		/**
		 * \brief please don't copy/move --> use your braincells instead :-) TODO maybe change this later, depending on usage
		 */
		EffectProfile(const EffectProfile& other) = delete;
		/**
		 * \brief please don't copy/move --> use your braincells instead :-) TODO maybe change this later, depending on usage
		 */
		EffectProfile(const EffectProfile&& other) = delete;
	};

	/**
	 * \brief This class actually has a lot of similarities to GameElement, regarding reading/storing its information.
	 * We still use it not in the game-hierarchy, since it can be "assigned" to all the GameElements.
	 */
	class GameElementEffect
	{
		std::string m_id;
		EffectProfile m_effectType;

	public:
		GameElementEffect() = delete;
		GameElementEffect(std::string id, bool permanent, const unsigned int rounds, const UTIL::FunctionCollection& functions);
		GameElementEffect(std::string& id, bool permanent, const unsigned int rounds, bool absolutValues, const std::vector<e_GameElementValues>& effectedValues_Names, const std::vector<unsigned char>& effectedValues_Values);
		bool operator==(const GameElementEffect& other) const;
		bool operator!=(const GameElementEffect& other) const;

		const std::string& id();
	};

	struct Coordinate
	{
		int x = 0;
		int y = 0;
	};

	class GameElementImageInformation
	{
		void * m_imageInMemory = 0;
		Coordinate m_size = {0,0};
		unsigned int m_numberOfSprites = 1;
		unsigned int m_numberOfAnimations = 1;
		std::vector<Range> AnimationSpriteIndices;

		// TODO constructor and so on
	};

	class GameElement
	{
		// Relevant to describing this GameElement
		std::string m_id;
		std::string m_name;
		std::string m_description;
		std::string m_descriptionImage;
		e_GameElementTypes m_type;
		std::vector<GameElementEffect *> m_allEffects;
		std::vector<GameElementEffect *> m_activeEffects;
		std::vector<GameElementEffect *> m_effectsToBeCalculated;

		// Relevant mostly to game mechanics
		Coordinate m_position = {0,0};
		
		GameElementImageInformation m_spriteSheet;

	public:
		GameElement();
		explicit GameElement(std::string id);
		explicit GameElement(const GameElementEffect other) = delete;
		GameElement(const GameElement& other) = delete;
		GameElement(const GameElement&& other) = delete;
		virtual ~GameElement() = default;
		const std::string& id();
		[[nodiscard]] e_GameElementTypes type() const;

		virtual DEBUG::RETURNCODE loadFromBinary(void * binaryAddr) = 0;
	};

	class Race : public GameElement
	{
		// TODO
	};

	class Weapon : public GameElement
	{
		// TODO
	};

	struct UnitStats
	{
		unsigned int level;

		unsigned int base_XPNeeded;
		unsigned int actual_XPNeeded; 
		unsigned int current_XPNeeded;

		unsigned int base_HP;
		unsigned int actual_HP;
		unsigned int current_HP;

		unsigned int base_MoveAmount;
		unsigned int actual_MoveAmount;
		unsigned int current_MoveAmount;

		unsigned int base_ActionAmount;
		unsigned int actual_ActionAmount;
		unsigned int current_ActionAmount;

		UnitStats();
	};

	class Unit : public GameElement
	{
		UnitStats m_stats;
		std::vector<Weapon *> m_weapons;
	};
}


