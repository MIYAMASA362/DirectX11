#pragma once

namespace DirectX
{
	class Audio;

	class AudioSource final :public Component
	{
	private:
		std::weak_ptr<Audio>audio;
		bool IsLoop = true;
	public:
		AudioSource();
		~AudioSource();
	public:
		virtual const std::type_info& GetType() override { return typeid(*this); };
		void GetAsset(std::string name);
		void Play();
		void SetLoop(bool loop);
		void Stop();
		virtual void OnDestroy() override;
	};
}