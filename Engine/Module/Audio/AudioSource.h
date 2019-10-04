#pragma once

namespace DirectX
{
	class Audio;

	class AudioSource final :public Component<AudioSource>
	{
	private:
		std::weak_ptr<Audio>audio;
		bool IsLoop = true;
	public:
		AudioSource(EntityID OwnerID);
		~AudioSource();
	public:
		void GetAsset(std::string name);
		void Play();
		void SetLoop(bool loop);
		void Stop();
		virtual void OnDestroy() override;
	};
}