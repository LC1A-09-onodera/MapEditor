#pragma once
#include <iostream>
#include <speechapi_cxx.h>

using namespace Microsoft::CognitiveServices::Speech;
using namespace Microsoft::CognitiveServices::Speech::Intent;
using namespace std;

class SpeechInput
{

	static shared_ptr<SpeechConfig> config;
	static shared_ptr<IntentRecognizer> intentRecognizer;
public:
	static void Initialize();
	static void TakeSpeechUpdate();
	static void CheckSpeech();
};