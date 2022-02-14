
#include "SpeechRecognitionCoreModule.h"
#include "SpeechRecognitionCoreLog.h"

DEFINE_LOG_CATEGORY(LogSpeechRecognitionCore)

void FSpeechRecognitionCoreModule::StartupModule()
{
    
}

void FSpeechRecognitionCoreModule::ShutdownModule()
{
}

#if defined(REQUIRE_EXPLICIT_GMALLOC_INIT) && REQUIRE_EXPLICIT_GMALLOC_INIT
extern "C" void IMPLEMENT_MODULE_SpeechRecognitionCore()
{
}
#else
IMPLEMENT_MODULE(FSpeechRecognitionCoreModule, SpeechRecognitionCore);
#endif
