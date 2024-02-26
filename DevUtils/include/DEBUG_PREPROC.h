#pragma once

constexpr const char * LOGFILENAME = "ErrorLogFile.txt";
inline FILE * LOGFILE = nullptr;

// ----------------- DEFINE ROA_DEBUG TO ENABLE DEBUGGING -----------------
#define ROA_DEBUG
// ------------------------------------------------------------------------
// - DEFINE ROA_TESTING TO ENABLE PRIVATE AND PROTECTED TESTING -----------
#define ROA_TESTING
// ------------------------------------------------------------------------
namespace ROA::DEBUG
{
	inline void initLogFile(const char * FILENAME)
	{
		printf("INITIALIZING LOGFILE WITH NAME: %s\n", FILENAME);
		if(!FILENAME) return;
		FILE * debFile = fopen(LOGFILENAME, "a+");
		if(!debFile) return;

		const time_t currentTime = time(nullptr);
		if(currentTime == -1) return;

		fprintf_s(debFile, "FILE CREATED at: %s\n", asctime(gmtime(&currentTime)));
		LOGFILE = debFile;
		printf("LOGFILE CREATED!\n");
	}

	inline void destroyLogFile()
	{
		if(LOGFILE)
		{
			printf("\nCLOSING LOGFILE!\n");
			const time_t currentTime = time(nullptr);
			if(currentTime == -1) return;
			fprintf_s(LOGFILE, "\nFILE DESTROYED at: %s\n\n", asctime(gmtime(&currentTime)));
			fclose(LOGFILE);
			printf("\nSUCCESS\n");
		}
	}
}

#ifdef ROA_DEBUG
#define ROA_INITLOG() using namespace ROA::DEBUG; if(LOGFILE == nullptr) initLogFile(LOGFILENAME)
#define ROA_CLOSELOG() destroyLogFile()
#define ROA_LOG(...) fprintf_s(LOGFILE, __VA_ARGS__)
#define ROA_LOGERRORCODE(X) ROA::DEBUG::PRINTERRORMESSAGE(X, LOGFILE)
#else
#define ROA_INIT() ;
#define ROA_DESTROY() ;
#define ROA_LOG(...) ;
#define ROA_LOGERRORCODE(X) ;
#endif

#ifdef ROA_TESTING
#define private public
#define protected public
#endif