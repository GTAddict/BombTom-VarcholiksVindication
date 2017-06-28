#pragma once
#include "GameComponent.h"

namespace DirectX
{
	class AudioEngine;
	class SoundEffect;
}

class Sounds : DX::GameComponent
{
public:
	Sounds(std::shared_ptr<DX::DeviceResources> deviceResources);
	~Sounds();

	void Update(const class StepTimer& timer);

	void RandomTom();

	std::unique_ptr<DirectX::AudioEngine> mAudioEngine;
	std::vector<std::unique_ptr<DirectX::SoundEffect>> mTomSounds;
};

