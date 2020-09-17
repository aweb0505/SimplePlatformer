#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <string>
using namespace std;
// Override base class with your custom functionality
class PlatformerGame : public olc::PixelGameEngine
{
public:
	PlatformerGame()
	{
		sAppName = "Platforming Game";
	}

private:
	wstring sLevel;
	int nLevelWidth;
	int nLevelHeight;

	bool bPlayerOnGround;

	float fPlayerPosX = 0.0f;
	float fPlayerPosY = 0.0f;

	float fPlayerVelX = 0.0f;
	float fPlayerVelY = 0.0f;

	float fCameraPosX = 0.0f;
	float fCameraPosY = 0.0f;

public:
	bool OnUserCreate() override
	{
		nLevelWidth = 64;
		nLevelHeight = 16;
		sLevel += L"................................................................";
		sLevel += L"...........#.#..................................................";
		sLevel += L".........#....#.................................................";
		sLevel += L"..........####..........#.......................................";
		sLevel += L"......................##..........##............................";
		sLevel += L"...................###............##............................";
		sLevel += L"..................###...........................................";
		sLevel += L"#############################.##########.....###################";
		sLevel += L"............................#.#...........#.....................";
		sLevel += L"...................##########.#..........#......................";
		sLevel += L"...................#...........#........#.......................";
		sLevel += L"...................#..##########.......#........................";
		sLevel += L"...................#..................#.........................";
		sLevel += L"...................###################..........................";
		sLevel += L"................................................................";
		sLevel += L"................................................................";


		
		return true;
	}
	bool OnUserUpdate(float fElapsedTime) override
	{
		/*Utility Lambda functions (from what I can gather these are just
			little inline functions within a function that does some stuff that is only really relevant to one method.)
		*/
		
		auto GetTile = [&](int x, int y)
		{
			if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
				return sLevel[y * nLevelWidth + x];
			else
				return L' ';
		};

		auto SetTile = [&](int x, int y, wchar_t c)
		{
			if (x >= 0 && x < nLevelWidth && y >= 0 && y < nLevelHeight)
				sLevel[y * nLevelWidth + x] = c;
		};

		//Handle Input


		if (IsFocused()) 
		{
			if (GetKey(olc::Key::A).bHeld)
			{
				fPlayerVelX += (bPlayerOnGround ?-12.0f : -8.0) * fElapsedTime;
			}

			if (GetKey(olc::Key::D).bHeld)
			{
				fPlayerVelX += (bPlayerOnGround ? 12.0f : 8.0) * fElapsedTime;
			}

			if (GetKey(olc::Key::SPACE).bPressed) {
				if (fPlayerVelY == 0) {
					fPlayerVelY = -10.0f;
				}
			}


		}

		fPlayerVelY += 20.0f * fElapsedTime;

		if (bPlayerOnGround) {
			fPlayerVelX += -3.0f * fPlayerVelX * fElapsedTime;
			if (fabs(fPlayerVelX) < 0.01f) {
				fPlayerVelX = 0.0f;
			}
		}

		fPlayerPosX = fPlayerPosX + fPlayerVelX * fElapsedTime;
		fPlayerPosY = fPlayerPosY + fPlayerVelY * fElapsedTime;

		float fNewPlayerPosX = fPlayerPosX + fPlayerVelX * fElapsedTime;
		float fNewPlayerPosY = fPlayerPosY + fPlayerVelY * fElapsedTime;

		if (fPlayerVelX > 10.0f)
			fPlayerVelX = 10.0f;

		if (fPlayerVelX < -10.0f)
			fPlayerVelX = -10.0f;

		if (fPlayerVelY > 100.0f)
			fPlayerVelY = 100.0f;

		if (fPlayerVelY < -100.0f)
			fPlayerVelY = -100.0f;

		//Collision
		if (fPlayerVelX <= 0) {
			if (GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.0f) != L'.' || GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 0.9f) != L'.') {
				fNewPlayerPosX = (int)fNewPlayerPosX + 1;
				fPlayerVelX = 0;
			}
		}
		else {
			if (GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.0f) != L'.' || GetTile(fNewPlayerPosX + 1.0f, fPlayerPosY + 0.9f) != L'.') {
				fNewPlayerPosX = (int)fNewPlayerPosX;
				fPlayerVelX = 0;
			}
		}

		bPlayerOnGround = false;
		if (fPlayerVelY <= 0) {
			if (GetTile(fNewPlayerPosX + 0.0f, fNewPlayerPosY) != L'.' || GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY + 0.0f) != L'.') {
				fNewPlayerPosY = (int)fNewPlayerPosY + 1;
				fPlayerVelY = 0;
			}
		}\
		else {
			if (GetTile(fNewPlayerPosX + 0.0f, fPlayerPosY + 1.0f) != L'.' || GetTile(fNewPlayerPosX + 0.9f, fNewPlayerPosY + 1.0f) != L'.') {
				fNewPlayerPosY = (int)fNewPlayerPosY;
				fPlayerVelY = 0;
				bPlayerOnGround = true;
			}
		}

		fPlayerPosX = fNewPlayerPosX;
		fPlayerPosY = fNewPlayerPosY;


		fCameraPosX = fPlayerPosX;
		fCameraPosY = fPlayerPosY;
		
		//Draw Level
		int nTileWidth = 8;
		int nTileHeight = 8;
		int nVisibleTilesX = ScreenWidth() / nTileWidth;
		int nVisibleTilesY = ScreenHeight() / nTileHeight;

		
		float fOffsetX = fCameraPosX - (float)nVisibleTilesX / 2.0f;
		float fOffsetY = fCameraPosY - (float)nVisibleTilesY / 2.0f;
		

		//Clamp camera to game bounds
		if (fOffsetX < 0) fOffsetX = 0;
		if (fOffsetY < 0) fOffsetY = 0;
		if (fOffsetX > nLevelWidth - nVisibleTilesX) fOffsetX = nLevelWidth - nVisibleTilesX;
		if (fOffsetY > nLevelHeight - nVisibleTilesY) fOffsetY = nLevelHeight - nVisibleTilesY;

		//Get offsets to smooth movement out
		float fTileOffsetX = (fOffsetX - (int)fOffsetX) * nTileWidth;
		float fTileOffsetY = (fOffsetY - (int)fOffsetY) * nTileHeight;

		//Draw visible tile map
		for (int x = -1; x < nVisibleTilesX +1; x++) {
			for (int y = -1; y < nVisibleTilesY+1; y++) {
				wchar_t sTileID = GetTile(x + fOffsetX, y + fOffsetY);
				switch (sTileID)
				{
				case L'.':
					FillRect(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, (x + 1) * nTileWidth - fTileOffsetX, (y + 1) * nTileHeight - fTileOffsetY, olc::CYAN); //mess around with this to fully understand the math
					break;
				case L'#':
					FillRect(x * nTileWidth - fTileOffsetX, y * nTileHeight - fTileOffsetY, (x + 1) * nTileWidth - fTileOffsetX, (y + 1) * nTileHeight - fTileOffsetY, olc::RED);
					break;
				default:
					break;
				}
			}
		}

		//Draw Player
		FillRect((fPlayerPosX - fOffsetX) * nTileWidth, (fPlayerPosY - fOffsetY) * nTileWidth, nTileWidth, nTileHeight, olc::GREEN);
		return true;

	}
};
int main()
{
	
	PlatformerGame game;
	if (game.Construct(160, 120, 8, 8))
		game.Start();
	return 0;
}