#include "Sound.h"

ComPtr<IXAudio2> Sound::xAudio2;
IXAudio2MasteringVoice *Sound::masterVoice;

void Sound::CreateVoice()
{
    //音
    BaseDirectX::result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
    BaseDirectX::result = xAudio2->CreateMasteringVoice(&masterVoice);
}
void Sound::Updete(float volume)
{
    masterVoice->SetVolume(volume);
}
void SoundLoad(const char *filename, SoundData &sound)
{
    //ファイルオープン---------------
    ifstream file;
    file.open(filename, ios_base::binary);
    assert(file.is_open());

    //読み込み----------------
    RiffHeader riff;
    file.read((char *)&riff, sizeof(riff));
    if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
    {
        assert(0);
    }
    if (strncmp(riff.type, "WAVE", 4) != 0)
    {
        assert(0);
    }

    FormatChunk format = {};
    file.read((char *)&format, sizeof(ChunkHeader));
    if (strncmp(format.chunk.id, "fmt ", 4) != 0)
    {
        assert(0);
    }
    assert(format.chunk.size <= sizeof(format.fmt));
    file.read((char *)&format.fmt, format.chunk.size);

    ChunkHeader data;
    file.read((char *)&data, sizeof(data));
    if (strncmp(data.id, "JUNK", 4) == 0)
    {
        file.seekg(data.size, ios_base::cur);
        file.read((char *)&data, sizeof(data));
    }
    if (strncmp(data.id, "data", 4) != 0)
    {
        assert(0);
    }

    char *pBuffer = new char[data.size];
    file.read(pBuffer, data.size);
    file.close();

    SoundData sounddata = {};
    sounddata.wfex = format.fmt;
    sounddata.pBuffer.reset(reinterpret_cast<BYTE *>(pBuffer));
    sounddata.bufferSize = data.size;
    sounddata.pSourceVoice = nullptr;
    BaseDirectX::result = Sound::xAudio2->CreateSourceVoice(&sounddata.pSourceVoice, &sounddata.wfex);
    assert(SUCCEEDED(BaseDirectX::result));
    sounddata.buf.pAudioData = sounddata.pBuffer.get();
    sounddata.buf.AudioBytes = sounddata.bufferSize;
    sounddata.buf.Flags = XAUDIO2_END_OF_STREAM;
    sound = sounddata;
}
void SoundUnload(SoundData *sounddata)
{
    delete[] sounddata->pBuffer.get();
    sounddata->pBuffer = 0;
    sounddata->bufferSize = 0;
    sounddata->wfex = {};
}
void SoundPlayerWave(SoundData &soundData)
{
    BaseDirectX::result = soundData.pSourceVoice->SubmitSourceBuffer(&soundData.buf);
    BaseDirectX::result = soundData.pSourceVoice->Start();
}
void SoundStopWave(SoundData &soundData)
{
    BaseDirectX::result = soundData.pSourceVoice->Stop();
    BaseDirectX::result = soundData.pSourceVoice->FlushSourceBuffers();
    //BaseDirectX::result = soundData.pSourceVoice->SubmitSourceBuffer(&soundData.buf);//これあると安定しない
}
void SoundPlayOnce(SoundData &soundData)
{
    IXAudio2SourceVoice *pSourceVoice = nullptr;
    BaseDirectX::result = Sound::xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
    assert(SUCCEEDED(BaseDirectX::result));
    XAUDIO2_BUFFER buf{};
    buf.pAudioData = soundData.pBuffer.get();
    buf.AudioBytes = soundData.bufferSize;
    buf.Flags = XAUDIO2_END_OF_STREAM;
    BaseDirectX::result = soundData.pSourceVoice->SubmitSourceBuffer(&soundData.buf);
    BaseDirectX::result = soundData.pSourceVoice->Start();
}
void SoundPlayLoop(SoundData& soundData)
{
    IXAudio2SourceVoice* pSourceVoice = nullptr;
    BaseDirectX::result = Sound::xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
    assert(SUCCEEDED(BaseDirectX::result));
    XAUDIO2_BUFFER buf{};
    buf.pAudioData = soundData.pBuffer.get();
    buf.AudioBytes = soundData.bufferSize;
    buf.Flags = XAUDIO2_END_OF_STREAM;
    buf.LoopCount = XAUDIO2_LOOP_INFINITE;
    BaseDirectX::result = soundData.pSourceVoice->SubmitSourceBuffer(&soundData.buf);
    BaseDirectX::result = soundData.pSourceVoice->Start();
}
void SoundVolume(SoundData &soundData, float volume)
{
    soundData.pSourceVoice->SetVolume(volume);
}

SoundControl::SoundControl()
{

}

SoundControl::~SoundControl()
{

}

SoundControl* SoundControl::GetSound()
{
    static SoundControl soundControl;
    return &soundControl;
}

SoundData::~SoundData()
{
    
}
