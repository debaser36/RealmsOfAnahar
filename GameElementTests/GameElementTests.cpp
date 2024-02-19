#include "pch.h"
#include "CppUnitTest.h"

#define private public
#define protected public

#include "GameElement.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace ROA;

#define BUFFER_SIZE 10000000
static char * DEBUG_BUFFER;

#define LOG(...) memset(DEBUG_BUFFER, 0, BUFFER_SIZE); sprintf_s(DEBUG_BUFFER, BUFFER_SIZE, __VA_ARGS__); (Logger::WriteMessage(DEBUG_BUFFER))

static void FILLRANDOM(char * addr, size_t size)
{
	srand(time(nullptr));
	for(int i = 0; i < size; i++)
	{
		const unsigned char j = rand() % (256);
		addr[i] = j;
	}
}

namespace GameElementTests
{
	TEST_CLASS(TestsBasic)
	{

	public:
		ROA::GameElement gameElement;
		ROA::Effect effect;
		ROA::Tile tile;
		ROA::Building building;
		ROA::Unit unit;
		ROA::Weapon weapon;
		ROA::PickupItem item;
		ROA::StaticElement staticElem;
		ROA::Race race;

			
		TEST_METHOD_INITIALIZE(Init)
		{
			DEBUG_BUFFER = static_cast<char*>(malloc(BUFFER_SIZE * sizeof(char)));
			memset(DEBUG_BUFFER, 0, BUFFER_SIZE);
			FILLRANDOM(static_cast<char*>(unit.m_startingPointStaticData), unit.m_staticSize);
			unit.m_baseInformation.m_id = "TestUnitID";
			building.m_baseInformation.m_id = "TestBuilding";
			unit.m_baseInformation.v_buildings.emplace_back(&building);
		}
		
		TEST_METHOD(ValidStartingAdresses)
		{
			// LOG("Adress of Unit:%p , Adress of Startingpoint:%p Adress of first element:%p ", &unit, unit.m_startingPointStaticData, &(unit.m_stats_base));
			Assert::AreEqual(gameElement.getAddressStaticStart(), (void*)nullptr);
			Assert::AreEqual(unit.getAddressStaticStart(), static_cast<void*>(&unit.m_stats_base));
			
		}

		TEST_METHOD(ValidBinaryRepresentUnitSimple)
		{

			auto EXITCODE = unit.generateBinary();
			Assert::AreEqual(static_cast<int>(EXITCODE), static_cast<int>(DEBUG::RETURNCODE::OK), L"Something went wrong creating the BinaryData");

			const char * binaryStaticStart = static_cast<char*>(unit.m_binaryRepresentation) + unit.m_staticMemoryStartIndexInBinary;
			const char * objStaticStart = static_cast<char*>(unit.m_startingPointStaticData);

			for(int i = 0; i < unit.m_staticSize; i++)
			{
				Assert::AreEqual(binaryStaticStart[i], objStaticStart[i], L"MemCpy didn't work for static data!");
			}
		}

		TEST_METHOD(ValidBinaryToObjectUnit)
		{
			unit.generateBinary();
			memset(unit.getAddressStaticStart(), 0, unit.m_staticSize);

			std::vector<ROA::GameElement> tV;
			unit.loadFromBinary(unit.m_binaryRepresentation, tV);

			const char * binaryStaticStart = static_cast<char*>(unit.m_binaryRepresentation) + unit.m_staticMemoryStartIndexInBinary;
			const char * objStaticStart = static_cast<char*>(unit.m_startingPointStaticData);
			for(int i = 0; i < unit.m_staticSize; i++)
			{
				Assert::AreEqual(binaryStaticStart[i], objStaticStart[i], L"Copy from binary didn't work");
			}
		}
	};
}
