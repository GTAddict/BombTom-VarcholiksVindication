#include "pch.h"
#include "Sounds.h"

using namespace DirectX;

Sounds::Sounds(std::shared_ptr<DX::DeviceResources> deviceResources)
	: GameComponent(deviceResources)
{
	AUDIO_ENGINE_FLAGS eFlags = AudioEngine_Default;
	mAudioEngine = std::make_unique<AudioEngine>(eFlags);

	mTomSounds.push_back(std::make_unique<SoundEffect>(mAudioEngine.get(), L"Content\\Sounds\\Assignments.wav"));
	mTomSounds.push_back(std::make_unique<SoundEffect>(mAudioEngine.get(), L"Content\\Sounds\\Exams.wav"));
	mTomSounds.push_back(std::make_unique<SoundEffect>(mAudioEngine.get(), L"Content\\Sounds\\Finals.wav"));
	mTomSounds.push_back(std::make_unique<SoundEffect>(mAudioEngine.get(), L"Content\\Sounds\\Homework.wav"));
	mTomSounds.push_back(std::make_unique<SoundEffect>(mAudioEngine.get(), L"Content\\Sounds\\Projects.wav"));
}


Sounds::~Sounds()
{
}

void Sounds::Update(const StepTimer& timer)
{
	UNREFERENCED_PARAMETER(timer);

	if (!mAudioEngine->Update())
	{
		DX::ThrowIfFailed(mAudioEngine->IsCriticalError());
	}
}

void Sounds::RandomTom()
{
	int randomDialogue = rand() % mTomSounds.size();
	mTomSounds.at(randomDialogue)->Play();
}
