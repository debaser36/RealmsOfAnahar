#pragma once
#include "../../DevUtils/include/FunctionCollection.h"
#include "../../DevUtils/include/GeneralUtil.h"


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
	};

	

	class GameElement
	{
	public: // TODO set private after testing

		GameElementBaseInformation m_baseInformation;
		void * m_binaryRepresentation = nullptr;
		bool m_binaryRepresentationOwnership = false;

		long m_staticMemoryStartIndex = -1;

	public:
		GameElement() = default; // completly empty;
		GameElement(void * binaryInformation, std::vector<GameElement>& referenceList);
		GameElement(const GameElement& other) = delete; // TODO
		GameElement(const GameElement&& other) = delete; // TODO
		virtual ~GameElement();

		 DEBUG::RETURNCODE loadFromBinary(void * binaryAddr, std::vector<GameElement>& gameElementContainer);
	public:  DEBUG::RETURNCODE mapStaticFromMemory(); // TODO set private
		 void * getAddrOfInheritanceStart();



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

	class Unit final : public GameElement
	{
		UnitStats m_stats;
	public:
		~Unit() override;
	};

	
}


