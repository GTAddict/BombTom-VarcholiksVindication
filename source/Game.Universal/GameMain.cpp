#include "pch.h"
#include "GameMain.h"

using namespace DX;
using namespace std;
using namespace DirectX;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::UI::Core;
using namespace Concurrency;

#define ENEMY_SPAWN_INTERVAL 1000
#define DIALOGUE_INTERVAL 2000

namespace DirectXGame
{
	// Loads and initializes application assets when the application is loaded.
	GameMain::GameMain(const shared_ptr<DX::DeviceResources>& deviceResources) :
		mDeviceResources(deviceResources)
	{
		// Register to be notified if the Device is lost or recreated
		mDeviceResources->RegisterDeviceNotify(this);

		auto camera = make_shared<OrthographicCamera>(mDeviceResources);
		mComponents.push_back(camera);
		camera->SetPosition(1920 / 2, 1080 / 2, 1);

		CoreWindow^ window = CoreWindow::GetForCurrentThread();
		mKeyboard = make_shared<KeyboardComponent>(mDeviceResources);		
		mKeyboard->Keyboard()->SetWindow(window);
		mComponents.push_back(mKeyboard);

		mMouse = make_shared<MouseComponent>(mDeviceResources);		
		mMouse->Mouse()->SetWindow(window);
		mComponents.push_back(mMouse);

		mGamePad = make_shared<GamePadComponent>(mDeviceResources);
		mComponents.push_back(mGamePad);

		auto fpsTextRenderer = make_shared<FpsTextRenderer>(mDeviceResources);
		mComponents.push_back(fpsTextRenderer);

		mSounds = make_shared<Sounds>(mDeviceResources);

		SpriteManagerHelper::GetInstance()->SetDeviceResources(mDeviceResources);

		auto spriteRenderer = make_shared<SpriteRenderer>(mDeviceResources, camera);
		SpriteManagerHelper::GetInstance()->SetRenderer(*spriteRenderer.get());
		mComponents.push_back(spriteRenderer);

		auto spriteCache = make_shared<SpriteCache>(mDeviceResources);
		SpriteManagerHelper::GetInstance()->SetCache(*spriteCache.get());
		mComponents.push_back(spriteCache);

		mTimer.SetFixedTimeStep(true);
		mTimer.SetTargetElapsedSeconds(1.0 / 60);

		IntializeResources();
		CreateGameplaySpecific();
	}

	GameMain::~GameMain()
	{
		mDeviceResources->RegisterDeviceNotify(nullptr);
	}

	// Updates application state when the window size changes (e.g. device orientation change)
	void GameMain::CreateWindowSizeDependentResources()
	{
		for (auto& component : mComponents)
		{
			component->CreateWindowSizeDependentResources();
		}
	}

	// Updates the application state once per frame.
	void GameMain::Update()
	{
		// Update scene objects.
		mTimer.Tick([&]()
		{
			long milliseconds = static_cast<long>(mTimer.GetElapsedSeconds() * 1000);
			enemyTimerElapsedMs += milliseconds;
			dialogueTimeElapsedMs += milliseconds;

			for (auto& component : mComponents)
			{
				component->Update(mTimer);
			}

			player->UpdateFrame(milliseconds);

			std::list<std::shared_ptr<Enemy>> pendingKill;

			for (auto enemy : enemyList)
			{
				if (enemy->IsAlive())
				{
					enemy->UpdateFrame(milliseconds);
				}
				else
				{
					mSounds->RandomTom();
					pendingKill.push_back(enemy);
				}
			}

			for (auto enemy : pendingKill)
			{
				enemyList.remove(enemy);
			}

			pendingKill.clear();

			if (enemyTimerElapsedMs >= ENEMY_SPAWN_INTERVAL)
			{
				enemyTimerElapsedMs -= ENEMY_SPAWN_INTERVAL;

				auto enemy = std::make_shared<Enemy>(*player);
				int randomX = (rand() % (1920 - player->GetWidth())) + player->GetWidth() / 2;

				enemy->SetPosition(randomX, 1080 - player->GetHeight() / 2);

				enemyList.push_back(enemy);
			}

			if (!player->GetAlive())
			{
				mSounds->RandomTom();
				RestartGameplay();
			}

			if (mKeyboard->WasKeyPressedThisFrame(Keys::Escape) ||
				mMouse->WasButtonPressedThisFrame(MouseButtons::Middle) ||
				mGamePad->WasButtonPressedThisFrame(GamePadButtons::Back))
			{
				CoreApplication::Exit();
			}
		});
	}

	// Renders the current frame according to the current application state.
	// Returns true if the frame was rendered and is ready to be displayed.
	bool GameMain::Render()
	{
		// Don't try to render anything before the first Update.
		if (mTimer.GetFrameCount() == 0)
		{
			return false;
		}

		auto context = mDeviceResources->GetD3DDeviceContext();

		// Reset the viewport to target the whole screen.
		auto viewport = mDeviceResources->GetScreenViewport();
		context->RSSetViewports(1, &viewport);

		// Reset render targets to the screen.
		ID3D11RenderTargetView *const targets[1] = { mDeviceResources->GetBackBufferRenderTargetView() };
		context->OMSetRenderTargets(1, targets, nullptr);

		// Clear the back buffer and depth stencil view.
		context->ClearRenderTargetView(mDeviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::Black);

		for (auto& component : mComponents)
		{
			auto drawableComponent = dynamic_pointer_cast<DrawableGameComponent>(component);
			if (drawableComponent != nullptr && drawableComponent->Visible())
			{
				drawableComponent->Render(mTimer);
			}
		}

		return true;
	}

	// Notifies renderers that device resources need to be released.
	void GameMain::OnDeviceLost()
	{
		for (auto& component : mComponents)
		{
			component->ReleaseDeviceDependentResources();
		}
	}

	// Notifies renderers that device resources may now be recreated.
	void GameMain::OnDeviceRestored()
	{
		IntializeResources();
	}

	void GameMain::IntializeResources()
	{
		for (auto& component : mComponents)
		{
			component->CreateDeviceDependentResources();
		}

		CreateWindowSizeDependentResources();
	}

	void GameMain::CreateGameplaySpecific()
	{
		bg = make_shared<Background>(1920, 1080);
		player = make_shared<Player>(mKeyboard, mGamePad);
		enemyTimerElapsedMs = 0;
		dialogueTimeElapsedMs = 0;
	}

	void GameMain::DestroyGameplaySpecific()
	{
		bg = nullptr;
		player = nullptr;
		enemyList.clear();
	}

	void GameMain::RestartGameplay()
	{
		DestroyGameplaySpecific();
		CreateGameplaySpecific();
	}
}