#include "pch.h"

#include "GameElement.h"

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

ROA::GameElement::GameElement(void* binaryInformation, std::vector<GameElement>& referenceList): m_baseInformation()
{
	// TODO
}

ROA::GameElement::~GameElement()
{
	if(this->m_binaryRepresentationOwnership)
	{
		free(this->m_binaryRepresentation);
		this->m_binaryRepresentation = nullptr;
	}
}

ROA::DEBUG::RETURNCODE ROA::GameElement::loadFromBinary(void* binaryAddr, std::vector<GameElement>& gameElementContainer)
{
	if(!binaryAddr) return DEBUG::RETURNCODE::NULLERROR;
	if(this->m_binaryRepresentationOwnership)
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
		const unsigned int stringLength = static_cast<int>(*(pos + 4));
		char *strStart = const_cast<char*>(pos)+8;
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
		
		// NEXT BLOCK
		pos = strStart + stringLength;
		indexCount += static_cast<long>(stringLength) + 8;
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
	this->m_staticMemoryStartIndex = (indexCount+5);
	this->mapStaticFromMemory();

	return	returnCode;
}

ROA::DEBUG::RETURNCODE ROA::GameElement::mapStaticFromMemory()
{
	if(!m_binaryRepresentation || m_staticMemoryStartIndex < 0) return DEBUG::RETURNCODE::NULLERROR;
	
	void * cpyDest = getAddrOfInheritanceStart();
	memcpy(cpyDest, static_cast<char*>(m_binaryRepresentation) + m_staticMemoryStartIndex, sizeof(*this) - sizeof(GameElement));
	return DEBUG::RETURNCODE::OK;
}

void* ROA::GameElement::getAddrOfInheritanceStart() { return this + (sizeof(*this)- (sizeof(*this) - sizeof(GameElement))); }
bool ROA::GameElement::operator==(const GameElement& other) const
{
	return m_baseInformation.m_id == other.m_baseInformation.m_id;
}

bool ROA::GameElement::operator==(const std::string& other) const
{
	return m_baseInformation.m_id == other;
}

ROA::UnitStats::UnitStats()
{
	memset(this, 0, sizeof(UnitStats));
}

ROA::Unit::~Unit()
= default;
