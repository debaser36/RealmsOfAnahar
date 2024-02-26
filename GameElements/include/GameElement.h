#pragma once
#include "../../DevUtils/include/DEBUG_PREPROC.h"
#include "../../DevUtils/include/ERRORTypeUtil.h"


namespace ROA
{
	//  --------------FORWARDS------------------
	class PickupItem;
	class Effect;
	class Unit;
	class Tile;
	class StaticElement;
	class Building;
	class Weapon;
	class Race;
	class GameElement;

	// --------- USEFUL ENUMS AND STATIC FUNCTIONS REGARDING THEM ---------------
	enum class e_GameElementTypes
	{
		UNKNOWN = 0,
		building,
		effect,
		item,
		other,
		race,
		static_other,
		tile,
		unit,
		weapon,
	};
	static e_GameElementTypes charToGameElementEnum(const char c);
	static char gameElementEnumToChar(e_GameElementTypes type);

	struct Coordinate
	{
		int x = 0;
		int y = 0;
	};

	typedef Coordinate position;
	typedef Coordinate coordinate;

	/**
	 * \brief This Class is a combining structures for all the elements, which can't be statically mapped from memory
	 * It contains strings that are important for describing the GameObject
	 * It also contains all the references to other GameElement Objects, which are important for this object.
	 * All the information, that is relevant for GAMEPLAY is **NOT HERE** --> Look for that in the inherited classes or other static fields.
	 * In most of the cases it surely makes sense to only have ONE list of references, but this task is not here.
	 * For example, you could have a gameManager, which has all the objects used in one game. The containers here are for SPECIFIC access.
	 * Also, most of the fields won't be used. We will analyze it later on, if all of this is necessary, but for now: more is better then less.
	 */ 
	class GameElementBaseInformation
	{
	public: // TODO set private after testing
		friend class GameElement;
		// Relevant to describing this GameElement
		std::string m_id;
		std::string m_descriptionName;
		std::string m_inGameName;
		std::string m_description;
		std::string m_descriptionImageName;
		std::string m_inGameImageName;
		e_GameElementTypes m_type;

		// Containers with references to other GameObjects
		std::vector<Unit *> v_units;
		std::vector<PickupItem *> v_pickupItems;
		std::vector<Effect *> v_effects;
		std::vector<Tile *> v_tiles;
		std::vector<Weapon *> v_weapons;
		std::vector<Building *> v_buildings;
		std::vector<StaticElement *> v_staticElements;
		std::vector<Race *> v_races;
		std::vector<GameElement *> v_otherReferences;

	public:
		GameElementBaseInformation() = default;
		GameElementBaseInformation(std::string id, std::string descriptionName, std::string inGameName,
		                           std::string description, std::string descriptionImageName,
		                           std::string inGameImageName,
		                           const e_GameElementTypes type, const std::vector<GameElement *>& refs);
		GameElementBaseInformation(GameElementBaseInformation & other) =  delete;
		GameElementBaseInformation(GameElementBaseInformation && other) = delete;
		[[nodiscard]] unsigned long long calculateSizeOfAllStrings() const;
		[[nodiscard]] unsigned long long elementCount(std::vector<const GameElement *>& putAllReferencesHere) const;
	};

	

	class GameElement
	{
	private:
		GameElementBaseInformation m_baseInformation;
		void * m_binaryRepresentation = nullptr;
		unsigned long long m_binaryRepresentationLength = 0;
		bool m_binaryRepresentationOwnership = false;
		unsigned long long m_staticMemoryStartIndexInBinary = 0;
	protected:
		void * m_startingPointStaticData = nullptr; // END OF OBJECT
		unsigned long long m_staticSize = 0;
		virtual void initBinaryInformation();
		

	public:
		GameElement() = default; // completely empty;
		GameElement(std::string id, std::string descriptionName, std::string inGameName,
	            std::string description, std::string descriptionImageName, std::string inGameImageName,
	            const e_GameElementTypes type, const std::vector<GameElement*>& refs);
		GameElement(const GameElement& other) = delete; // don't copy Units --> they will have the same ID
		GameElement(const GameElement&& other) = delete;
		virtual ~GameElement();
		GameElement& operator=(const GameElement&) = delete;
		GameElement& operator=(const GameElement&&) = delete;

		[[nodiscard]] e_GameElementTypes type() const;
		[[nodiscard]] const std::string& id() const;

		DEBUG::RETURNCODE loadFromBinary(void * binaryAddr, std::vector<GameElement>& gameElementContainer);
		DEBUG::RETURNCODE generateBinary();

	[[nodiscard]] void * getAddressStaticStart() const;
	private:
	[[nodiscard]] const void * mapStaticFromMemory() const; // TODO set private



	public:
		/**
		 * \brief compares the ID's of the objects. Everything is about ID's!
		 * \param other other Element
		 * \return 
		 */
		bool operator==(const GameElement& other) const;
		bool operator==(const std::string& other) const;
	};

	class PickupItem final : public GameElement
	{
		// TODO
	};

	class Effect final : public GameElement
	{
		// TODO
	};

	class Tile final : public GameElement
	{
		// TODO
	};

	class StaticElement : public GameElement
	{
		// TODO
	};

	class Building final : public StaticElement
	{
		// TODO
	};

	class Race final : public GameElement
	{
		// TODO
	};

	class Weapon final : public GameElement
	{
		// TODO
	};

	struct UnitStats
	{
	public:
		unsigned int level = 0;

		unsigned int XPNeeded = 0;

		unsigned int HP = 0;

		unsigned int MoveAmount = 0;

		unsigned int ActionAmount = 0;

		UnitStats() = default;
		UnitStats(unsigned level, unsigned xp, unsigned hp, unsigned moves, unsigned actions);
		UnitStats(const UnitStats& other) = delete;
		UnitStats(const UnitStats&& other) = delete;
		UnitStats& operator=(const UnitStats& other) = delete;
		UnitStats& operator=(const UnitStats&& other) = delete;
	
	};

	class Unit final : public GameElement
	{
	private:
		UnitStats m_stats_base;
		UnitStats m_stats_actual;
	protected:
		UnitStats m_stats_current;
	public:
		Unit();
		Unit(std::string id, std::string descriptionName, std::string inGameName,
		                           std::string description, std::string descriptionImageName,
		                           std::string inGameImageName,
		                           const e_GameElementTypes type, const std::vector<GameElement *>& refs,
			unsigned level, unsigned xp, unsigned hp, unsigned moves, unsigned actions);
	protected:
		void initBinaryInformation() override;
		~Unit() override;
	};

	
}


