#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
using namespace std;
class Game :public olc::PixelGameEngine
{
	olc::Sprite* bird;
	olc::Sprite* birdUp;
	olc::Decal* dBird = nullptr;
	olc::Decal* dBirdUp = nullptr;
	float fBirdPosition = 0.0f;
	float fBirdVelocity = 0.0f;
	float fBirdAcceleration = 0.0f;
	float fGravity = 200.0f;
	float fSectionWidth;
	list<int> listSection;
	float fLevelPosition;
	bool hasColided;
	float birdX;
	bool gameStopped = false;
	float birdScale = 0.1f;
	float flightSpeed = 0.5f;
	int passed = 0;
	

public:
	Game()
	{
		sAppName = "Flappy Bird";
	}

	bool OnUserCreate() override
	{
		bird = new olc::Sprite("bird.png");
		birdUp = new olc::Sprite("bird_up.png");
		dBird = new olc::Decal(bird);
		dBirdUp = new olc::Decal(birdUp);
		listSection = { 0,0,0 };
		fSectionWidth = (float)ScreenWidth() / (float)(listSection.size() - 1);
		birdX = (ScreenWidth() / 3.0f);
		fLevelPosition = 0;
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::WHITE);

		olc::vf2d birdCenter = { (bird->width * birdScale) / 2 + birdX ,fBirdPosition + (bird->height * birdScale) / 2 };
		int birdRadius = (bird->height * birdScale) / 2;
		DrawCircle(birdCenter, birdRadius, olc::RED);

		if (!gameStopped)
		{
			if (GetKey(olc::RIGHT).bHeld)
			{
				flightSpeed += 0.05;
			}
			if (GetKey(olc::LEFT).bHeld && flightSpeed > 0.2)
			{
				flightSpeed -= 0.05;
			}

			if ((GetKey(olc::UP).bPressed || GetKey(olc::SPACE).bPressed) && fBirdVelocity >= fBirdAcceleration / 15.0f)
			{
				fBirdAcceleration = 0.0f;
				fBirdVelocity = -fGravity / 2;
			}
			else
			{
				fBirdAcceleration += fGravity * fElapsedTime;
			}

			if (fBirdAcceleration >= fGravity)
				fBirdAcceleration = fGravity;
			fBirdVelocity += fBirdAcceleration * fElapsedTime;

			if (fBirdPosition <= ScreenHeight() - dBird->sprite->height / 10)
			{
				fBirdPosition += fBirdVelocity * fElapsedTime;
			}
			else
			{
				gameStopped = true;
			}

			fLevelPosition += flightSpeed;

			if (fLevelPosition > fSectionWidth * (passed + 1))
			{
				int i = rand() % (ScreenHeight() - 200);
				if (i <= 10) i = 0;
				listSection.push_back(i);
				listSection.pop_front();
 				passed++;
			}
			
		}

		int nSection = 0;
		for (auto s : listSection)
		{
			if (s != 0)
			{
				int x = nSection * fSectionWidth + 10 - fLevelPosition;
				int y = ScreenHeight() - s / 2;
				int w = fSectionWidth / 3;
				FillRect(x, 0, w, s, olc::GREEN);
				int d = y + s - 150;
				FillRect(x, d, w, ScreenHeight() - d, olc::GREEN);
				if (birdCenter.x + birdRadius > x && birdCenter.x - birdRadius < (x + w) && (birdCenter.y + birdRadius > d || birdCenter.y - birdRadius < s))
				{
					hasColided = true;
					gameStopped = true;
				}
				if (hasColided)
				{
					FillRect(10, 10, 10, 10, olc::RED);
				}
				else
				{
					FillRect(10, 10, 10, 10, olc::GREEN);
				}
			}
			nSection++;
		}

		if (fBirdVelocity > 0)
		{
			DrawDecal({ (ScreenWidth() / 3.0f),fBirdPosition }, dBird, { birdScale,birdScale });
		}
		else
		{
			DrawDecal({ (ScreenWidth() / 3.0f),fBirdPosition }, dBirdUp, { birdScale,birdScale });
		}
		if (gameStopped)
		{
			DrawString({ ScreenHeight() / 2,20 }, "Press S to restart", olc::RED);
			if (GetKey(olc::S).bPressed)
			{
				RestartGame();
			}
		}
		DrawString({ ScreenHeight() / 2,5 }, "passed: " + to_string(passed), olc::RED);
		
		return true;
	}
	void RestartGame()
	{
		hasColided = false;
		gameStopped = false;
		listSection = { 0,0,0 };
		fSectionWidth = (float)ScreenWidth() / (float)(listSection.size() - 1);
		birdX = (ScreenWidth() / 3.0f);
		fLevelPosition = 0;
		fBirdPosition = 0.0f;
		passed = 0;
	}

	bool CheckCollistion()
	{
		return false;
	}
};

int main()
{
	Game g;
	g.Construct(800, 600, 1, 1);
	g.Start();
	return 0;
}