// ReSharper disable CppClangTidyBugproneNotNullTerminatedResult
#include "pch.h"
#include "GameElement.h"


constexpr long long STATIC_BINARY_SIZE = ((42)+(6*sizeof(int)));
/**
 * \brief Returns the GameElementType with the starting Letter (or other for '\0')
 * \param c the Starting character in {U,E,I,W,B,S,R, \0}
 * \return the specific enum value representing this starting character, or type "UNKNOWN"
 */
ROA::e_GameElementTypes ROA::charToGameElementEnum(const char c)
{
	switch(c)
	{
	case 'U': return e_GameElementTypes::unit;
	case 'E': return e_GameElementTypes::effect;
	case 'I': return e_GameElementTypes::item;
	case 'W': return e_GameElementTypes::weapon;
	case 'B': return e_GameElementTypes::building;
	case 'S': return e_GameElementTypes::static_other;
	case 'R': return e_GameElementTypes::race;
	case '\0': return e_GameElementTypes::other;
	default: return e_GameElementTypes::UNKNOWN;
	}
}


char ROA::gameElementEnumToChar(const e_GameElementTypes type)
{
	switch(type)
	{
	case e_GameElementTypes::UNKNOWN: return -1;
	case e_GameElementTypes::building: return 'B';
	case e_GameElementTypes::effect: return 'E';
	case e_GameElementTypes::item: return 'I';
	case e_GameElementTypes::other: return 0;
	case e_GameElementTypes::race: return 'R';
	case e_GameElementTypes::static_other: return 'S';
	case e_GameElementTypes::tile: return 'T';
	case e_GameElementTypes::unit: return 'U';
	case e_GameElementTypes::weapon: return 'W';
	}
	return -1;
}

ROA::GameElementBaseInformation::GameElementBaseInformation(std::string id, std::string descriptionName,
                                                            std::string inGameName, std::string description,
                                                            std::string descriptionImageName,
                                                            std::string inGameImageName, const e_GameElementTypes type, const std::vector<GameElement*>& refs)
	: m_id(std::move(id)), m_descriptionName(std::move(descriptionName)), m_inGameName(std::move(inGameName)), m_description(
		  std::move(description)), m_descriptionImageName(std::move(descriptionImageName)), m_inGameImageName(
		  std::move(inGameImageName)), m_type(type)
{
	for(const auto obj : refs)
	{
		switch(obj->type())
		{
		case e_GameElementTypes::UNKNOWN: continue;
		case e_GameElementTypes::building: this->v_buildings.emplace_back(dynamic_cast<Building*>(obj)); break;
		case e_GameElementTypes::effect: this->v_effects.emplace_back(dynamic_cast<Effect*>(obj)); break;
		case e_GameElementTypes::item:  this->v_pickupItems.emplace_back(dynamic_cast<PickupItem*>(obj)); break;
		case e_GameElementTypes::other: this->v_otherReferences.emplace_back(obj);  break;
		case e_GameElementTypes::race:  this->v_races.emplace_back(dynamic_cast<Race*>(obj)); break;
		case e_GameElementTypes::static_other:  this->v_staticElements.emplace_back(dynamic_cast<StaticElement*>(obj)); break;
		case e_GameElementTypes::tile: this->v_tiles.emplace_back(dynamic_cast<Tile*>(obj));  break;
		case e_GameElementTypes::unit:  this->v_units.emplace_back(dynamic_cast<Unit*>(obj)); break;
		case e_GameElementTypes::weapon:  this->v_weapons.emplace_back(dynamic_cast<Weapon*>(obj)); break;
		}
	}
}

/**
 * \brief 
 * \return the sum of all the size of all the strings in this struct + all the strings in every reference vector
 */
unsigned long long ROA::GameElementBaseInformation::calculateSizeOfAllStrings() const
{
	unsigned long long size = 0;
	size += m_description.size();
	size += m_descriptionImageName.size();
	size += m_descriptionName.size();
	size += m_id.size();
	size += m_inGameImageName.size();
	size += m_inGameName.size();

	for(const auto & obj : v_buildings)
		size+=obj->id().size();
	for(const auto & obj : v_pickupItems)
		size+=obj->id().size();
	for(const auto & obj : v_effects)
		size+=obj->id().size();
	for(const auto & obj : v_tiles)
		size+=obj->id().size();
	for(const auto & obj : v_staticElements)
		size+=obj->id().size();
	for(const auto & obj : v_races)
		size+=obj->id().size();
	for(const auto & obj : v_otherReferences)
		size+=obj->id().size();
	for(const auto & obj : v_units)
		size+=obj->id().size();
	for(const auto & obj : v_weapons)
		size+=obj->id().size();

	return size;
}

/**
 * \brief calculates the count of all objects and puts them all as reference into an vector
 * \param putAllReferencesHere expects an empty vector --> empties it, if it's not empty
 * \return 
 */
unsigned long long ROA::GameElementBaseInformation::elementCount(std::vector<const GameElement *>& putAllReferencesHere) const
{
	const unsigned long long size =
		v_pickupItems.size()
	+	v_effects.size()
	+	v_otherReferences.size()
	+	v_races.size()
	+	v_staticElements.size()
	+	v_tiles.size()
	+	v_units.size()
	+	v_weapons.size()
	+	v_buildings.size();

	putAllReferencesHere.clear();
	putAllReferencesHere.reserve(size);

	for(const auto & obj : v_buildings)
		putAllReferencesHere.emplace_back(obj);
	for(const auto & obj : v_pickupItems)
		putAllReferencesHere.emplace_back(obj);
	for(const auto & obj : v_effects)
		putAllReferencesHere.emplace_back(obj);
	for(const auto & obj : v_tiles)
		putAllReferencesHere.emplace_back(obj);
	for(const auto & obj : v_staticElements)
		putAllReferencesHere.emplace_back(obj);
	for(const auto & obj : v_races)
		putAllReferencesHere.emplace_back(obj);
	for(const auto & obj : v_otherReferences)
		putAllReferencesHere.emplace_back(obj);
	for(const auto & obj : v_units)
		putAllReferencesHere.emplace_back(obj);
	for(const auto & obj : v_weapons)
		putAllReferencesHere.emplace_back(obj);

	return putAllReferencesHere.size();
}


void ROA::GameElement::initBinaryInformation() {}

ROA::GameElement::GameElement(std::string id, std::string descriptionName, std::string inGameName,
                              std::string description, std::string descriptionImageName, std::string inGameImageName,
                              const e_GameElementTypes type, const std::vector<GameElement*>& refs)
		: m_baseInformation(std::move(id), std::move(descriptionName), std::move(inGameName), std::move(description), std::move(descriptionImageName), std::move(inGameImageName), type, refs)
{
}

ROA::GameElement::~GameElement()
{
	if(this->m_binaryRepresentationOwnership)
	{
		delete[] static_cast<char*>(this->m_binaryRepresentation);
		this->m_binaryRepresentation = nullptr;
		this->m_binaryRepresentationOwnership = false;
	}
}

ROA::e_GameElementTypes ROA::GameElement::type() const
{
	return this->m_baseInformation.m_type;
}

const std::string& ROA::GameElement::id() const
{
	return this->m_baseInformation.m_id;
}


ROA::DEBUG::RETURNCODE ROA::GameElement::loadFromBinary(void* binaryAddr, std::vector<GameElement>& gameElementContainer)
{
	if(!binaryAddr) return DEBUG::RETURNCODE::NULLERROR;
	if(this->m_binaryRepresentationOwnership && this->m_binaryRepresentation != binaryAddr)
	{
		free(m_binaryRepresentation);
		m_binaryRepresentation = binaryAddr;
		m_binaryRepresentationOwnership = false;
	}

	const char * pos = static_cast<char*>(binaryAddr);

	// VALIDATE DOCUMENT
	if(strncmp(pos, "ROA\n", 4) != 0) return DEBUG::RETURNCODE::BIN_VALIDATION_ERROR;
	long indexCount = 0;
	pos += 4;
	indexCount += 4;

	// GET TYPE
	if(pos[1] != 10) return DEBUG::RETURNCODE::BIN_VALIDATION_ERROR;

	this->m_baseInformation.m_type = charToGameElementEnum(*pos);
	if (this->m_baseInformation.m_type == e_GameElementTypes::UNKNOWN) return DEBUG::RETURNCODE::BIN_VALIDATION_ERROR;
	pos+=2;
	indexCount += 2;

	// VALIDATE GENERAL HEADER
	if(strncmp(pos, "(G)\n", 4) != 0 ) return DEBUG::RETURNCODE::BIN_VALIDATION_ERROR;
	pos+=4;
	indexCount +=4;

	DEBUG::RETURNCODE returnCode = DEBUG::RETURNCODE::OK;

	// READ HEADERS
	while(strncmp(pos, "END\n", 4) != 0)
	{
		// READ STRING
		const unsigned int stringLength = static_cast<int>(*(pos + sizeof(int)));
		char *strStart = const_cast<char*>(pos)+4 +sizeof(int);
		const char tmp = strStart[stringLength];
		strStart[stringLength] = 0;
		const std::string s(strStart);
		strStart[stringLength] = tmp;

		// READ TYPE
		if(strncmp(pos, "SID\n", 4) == 0)
			this->m_baseInformation.m_id = s;
		else if(strncmp(pos, "DSN\n", 4) == 0)
			this->m_baseInformation.m_descriptionName = s;
		else if(strncmp(pos, "IGN\n", 4) == 0)
			this->m_baseInformation.m_inGameName = s;
		else if(strncmp(pos, "DES\n", 4) == 0)
			this->m_baseInformation.m_description = s;
		else if(strncmp(pos, "DIN\n", 4) == 0)
			this->m_baseInformation.m_descriptionImageName = s;
		else if(strncmp(pos, "IIN\n", 4) == 0)
			this->m_baseInformation.m_inGameImageName = s;
		else returnCode = DEBUG::RETURNCODE::OK_WITH_BINWARNINGS; // Can't read that property

		pos = strStart;
		// NEXT BLOCK
		pos +=stringLength;
		indexCount += 4 + static_cast<long>(stringLength) + static_cast<long>(sizeof(int));
	}
	pos+=4;

	// LOAD REFERENCES
	while(strncmp(pos, "END\n", 4) != 0)
	{
		const unsigned int stringLength = static_cast<int>(*(pos+2));
		char *strStart = const_cast<char*>(pos)+6;
		const e_GameElementTypes readType = charToGameElementEnum(*pos);
		if(readType == e_GameElementTypes::UNKNOWN)
		{
			returnCode = DEBUG::RETURNCODE::OK_WITH_BINWARNINGS; // Can't read that property
			pos = strStart + stringLength;
			indexCount += static_cast<long>(stringLength) + 6;
			continue;
		}
		this->m_binaryRepresentationLength = -1;
		const char tmp = strStart[stringLength];
		strStart[stringLength] = 0;
		std::string s(strStart);
		strStart[stringLength] = tmp;

		// find the reference inside
		auto foundElIt = std::find(gameElementContainer.begin(), gameElementContainer.end(), s);
		if(foundElIt != gameElementContainer.end())
		{
			GameElement * elem = &(*foundElIt);
			switch(readType)
			{
			case e_GameElementTypes::building: 
				this->m_baseInformation.v_buildings.emplace_back(dynamic_cast<Building *>(elem));
				break;				
			case e_GameElementTypes::effect:
				this->m_baseInformation.v_effects.emplace_back(dynamic_cast<Effect *>(elem));
				break;	
			case e_GameElementTypes::item:
				this->m_baseInformation.v_pickupItems.emplace_back(dynamic_cast<PickupItem *>(elem));
				break;	
			case e_GameElementTypes::other:
				this->m_baseInformation.v_otherReferences.emplace_back(elem);
				break;	
			case e_GameElementTypes::race: 
				this->m_baseInformation.v_races.emplace_back(dynamic_cast<Race *>(elem));
				break;	
			case e_GameElementTypes::static_other:
				this->m_baseInformation.v_staticElements.emplace_back(dynamic_cast<StaticElement *>(elem));
				break;	
			case e_GameElementTypes::tile: 
				this->m_baseInformation.v_tiles.emplace_back(dynamic_cast<Tile *>(elem));
				break;	
			case e_GameElementTypes::unit: 
				this->m_baseInformation.v_units.emplace_back(dynamic_cast<Unit *>(elem));
				break;	
			case e_GameElementTypes::weapon: 
				this->m_baseInformation.v_weapons.emplace_back(dynamic_cast<Weapon *>(elem));
				break;
			case e_GameElementTypes::UNKNOWN: break;
			}
		}
		else returnCode = DEBUG::RETURNCODE::OK_WITH_REFERENCEWARNING; // Didn't find the reference

		pos = strStart + stringLength;
		indexCount += static_cast<long>(stringLength) + 6;
	}
	this->m_staticMemoryStartIndexInBinary = (indexCount+5);
	if( !this->mapStaticFromMemory())
	{
		return DEBUG::RETURNCODE::NULLERROR;
	}

	this->m_binaryRepresentationLength = indexCount+5 + m_staticSize;
	return	returnCode;
}

ROA::DEBUG::RETURNCODE ROA::GameElement::generateBinary()
{
	if(this->m_binaryRepresentationOwnership && this->m_binaryRepresentation)
	{
		free(m_binaryRepresentation);
		m_binaryRepresentation = nullptr;
	}
	this->m_binaryRepresentationOwnership = true;
	unsigned long long size = STATIC_BINARY_SIZE + m_staticSize;
	size+= m_baseInformation.calculateSizeOfAllStrings();
	std::vector<const GameElement *> allReferences;
	const unsigned long long elemCount = m_baseInformation.elementCount(allReferences);
	size+= elemCount * 2;
	size+= elemCount * sizeof(int);
	

	this->m_binaryRepresentation = new char[size];
	this->m_binaryRepresentationLength = size;

	
	char * pos = static_cast<char*>(m_binaryRepresentation);
	char * DEBUG_STRING  = pos;

	pos[0] = 'R';
	pos[1] = 'O';
	pos[2] = 'A';
	pos[3] = '\n';
	pos+=4;
	const char t = gameElementEnumToChar(this->m_baseInformation.m_type);
	pos[0] = t;
	pos[1] = '\n';
	pos+=2;
	pos[0] = '(';
	pos[1] = 'G';
	pos[2] = ')';
	pos[3] = '\n';
	pos+=4;

	// ID
	pos[0] = 'S';
	pos[1] = 'I';
	pos[2] = 'D';
	pos[3] = '\n';
	pos+=4;
	int s = static_cast<int>(m_baseInformation.m_id.size());
	memcpy(pos, &s, sizeof(s)); 
	pos+=sizeof(s);
	memcpy(pos, m_baseInformation.m_id.c_str(), s);
	pos+=s;


	// Description Name
	pos[0] = 'D';
	pos[1] = 'S';
	pos[2] = 'N';
	pos[3] = '\n';
	pos+=4;
	s = static_cast<int>(m_baseInformation.m_descriptionName.size());
	memcpy(pos, &s, sizeof(s)); 
	pos+=sizeof(s);
	memcpy(pos, m_baseInformation.m_descriptionName.c_str(), s);
	pos+=s;

	

	// In Game Name
	pos[0] = 'I';
	pos[1] = 'G';
	pos[2] = 'N';
	pos[3] = '\n';
	pos+=4;
	s = static_cast<int>(m_baseInformation.m_inGameName.size());
	memcpy(pos, &s, sizeof(s)); 
	pos+=sizeof(s);
	memcpy(pos, m_baseInformation.m_inGameName.c_str(), s);
	pos+=s;

	

	// Description
	pos[0] = 'D';
	pos[1] = 'E';
	pos[2] = 'S';
	pos[3] = '\n';
	pos+=4;
	s = static_cast<int>(m_baseInformation.m_description.size());
	memcpy(pos, &s, sizeof(s)); 
	pos+=sizeof(s);
	memcpy(pos, m_baseInformation.m_description.c_str(), s);
	pos+=s;

	

	// image name description
	pos[0] = 'D';
	pos[1] = 'I';
	pos[2] = 'N';
	pos[3] = '\n';
	pos+=4;
	s = static_cast<int>(m_baseInformation.m_descriptionImageName.size());
	memcpy(pos, &s, sizeof(s)); 
	pos+=sizeof(s);
	memcpy(pos, m_baseInformation.m_descriptionImageName.c_str(), s);
	pos+=s;

	

	// image name in game
	pos[0] = 'I';
	pos[1] = 'I';
	pos[2] = 'N';
	pos[3] = '\n';
	pos+=4;
	s = static_cast<int>(m_baseInformation.m_inGameImageName.size());
	memcpy(pos, &s, sizeof(s)); 
	pos+=sizeof(s);
	memcpy(pos, m_baseInformation.m_inGameImageName.c_str(), s);
	pos+=s;
	

	pos[0] = 'E';
	pos[1] = 'N';
	pos[2] = 'D';
	pos[3] = '\n';
	pos+=4;

	for(const auto ref : allReferences)
	{
		pos[0] = gameElementEnumToChar(ref->type());
		pos[1] = '\n';
		pos+=2;

		s = static_cast<int>(ref->m_baseInformation.m_id.size());
		memcpy(pos, &s, sizeof(s));
		pos+=sizeof(s);
		memcpy(pos, ref->m_baseInformation.m_id.c_str(), s);
		pos+=s;

	}

	pos[0] = 'E';
	pos[1] = 'N';
	pos[2] = 'D';
	pos[3] = '\n';
	pos+=4;

	memcpy(pos, this->m_startingPointStaticData, this->m_staticSize);
	this->m_staticMemoryStartIndexInBinary = size - this->m_staticSize;

	return DEBUG::RETURNCODE::OK;
}

void* ROA::GameElement::getAddressStaticStart() const { return this->m_startingPointStaticData; }

/**
 * \brief 
 * \return 
 */
const void * ROA::GameElement::mapStaticFromMemory() const
{
	if(!m_binaryRepresentation || !m_startingPointStaticData) return nullptr;

	const void * copiedTo = memcpy(m_startingPointStaticData, static_cast<char*>(m_binaryRepresentation) + m_staticMemoryStartIndexInBinary, m_staticSize);
	if(!copiedTo) return nullptr;
	return copiedTo;
}

bool ROA::GameElement::operator==(const GameElement& other) const
{
	return m_baseInformation.m_id == other.m_baseInformation.m_id;
}

bool ROA::GameElement::operator==(const std::string& other) const
{
	return m_baseInformation.m_id == other;
}


ROA::UnitStats::UnitStats(const unsigned level, const unsigned xp, const unsigned hp, const unsigned moves, const unsigned actions)
	:level(level), XPNeeded(xp), HP(hp), MoveAmount(moves), ActionAmount(actions)
{
	
}


ROA::Unit::Unit()
{
	initBinaryInformation();
}

ROA::Unit::Unit(std::string id, std::string descriptionName, std::string inGameName, std::string description,
	std::string descriptionImageName, std::string inGameImageName, const e_GameElementTypes type,
	const std::vector<GameElement*>& refs,
	unsigned level, unsigned xp, unsigned hp, unsigned moves, unsigned actions)
	: GameElement(std::move(id), 
		std::move(descriptionName), std::move(inGameName), std::move(description), 
		std::move(descriptionImageName), std::move(inGameImageName), type, refs),
		m_stats_base(level, xp, hp, moves, actions),
		m_stats_actual(level, xp, hp, moves, actions),
		m_stats_current(level, xp, hp, moves, actions)
{
	initBinaryInformation();
}

void ROA::Unit::initBinaryInformation()
{
	GameElement::initBinaryInformation();
	this->m_startingPointStaticData = &(this->m_stats_base);
	this->m_staticSize = sizeof(UnitStats);
	this->m_baseInformation.m_type = e_GameElementTypes::unit;
	this->m_binaryRepresentationOwnership = false;
	this->m_binaryRepresentation = nullptr;
}
ROA::Unit::~Unit()
= default;
