#include "game.h"

Game::Game()
{
    font =LoadFontEx("src/monogram.ttf",64,0,0);
    InitAudioDevice();
    music=LoadMusicStream("src/music.ogg");
    laserBeam=LoadSound("src/laser.ogg");
    explosion=LoadSound("src/explosion.ogg");
    PlayMusicStream(music);
    GameReset();
}

Game::~Game()
{
    UnloadMusicStream(music);
    UnloadSound(laserBeam);
    UnloadSound(explosion);
    CloseAudioDevice();
}

void Game::HandleInput()
{
    if (gameOver == false)
    {
        if (IsKeyDown(KEY_LEFT))
        {
            spaceship.MoveLeft();
        }
        else if (IsKeyDown(KEY_RIGHT))
        {
            spaceship.MoveRight();
        }
        if (IsKeyDown(KEY_UP))
        {
            spaceship.FireLaser(laserBeam);
        }
    }
    else
    {
        int keyPressed = GetKeyPressed();
        if (keyPressed != 0)
        {
            GameReset();
        }
    }
}

void Game::Update()
{
    if (gameOver == false)
    {
        if (GetTime() - timeLastSpawn > mysteryShipSpawnInterval)
        {
            mysteryship.Spawn();
            timeLastSpawn = GetTime();
            mysteryShipSpawnInterval = GetRandomValue(10, 20);
        }
        for (Laser &laser : spaceship.lasers)
        {
            laser.Update();
        }
        for (auto &laser : alienLasers)
        {
            laser.Update();
        }
        AlienShootLaser();
        DeleteInactiveLasers();
        MoveAliens();
        mysteryship.Update();
        CheckForCollisions();
        if(aliens.empty()){
            gameOver=true;
        }
    }
}

void Game::Draw()
{
    spaceship.Draw();
    for (auto &laser : spaceship.lasers)
    {
        laser.Draw();
    }
    for (auto &obstacle : obstacles)
    {
        obstacle.Draw();
    }
    for (auto &alien : aliens)
    {
        alien.Draw();
    }
    for (auto &laser : alienLasers)
    {
        laser.Draw();
    }
    mysteryship.Draw();
    if(gameOver==true){
    DrawTextEx(font, "GAME OVER", {220, 350}, 64, 2, WHITE);
    }
}

void Game::Score()
{
    DrawTextEx(font, "Score", {560, 15}, 38, 2, WHITE);
    DrawRectangleRounded({540, 75, 140, 40}, 0.3, 6, {64, 47, 211, 140});
    sprintf(scoreChar, "%d",score);
    Vector2 textSize = MeasureTextEx(font, scoreChar, 38, 2);
    DrawTextEx(font, scoreChar, {540 + (140 - textSize.x) / 2, 80}, 38, 2, WHITE);
}

void Game::GameOver()
{
    gameOver = true;
}

void Game::GameReset()
{
    gameOver=false;
    obstacles = CreateObstacles();
    aliens = CreateAliens();
    aliensDirection = 1;
    timeLastAlienFired = 0.0;
    timeLastSpawn = 0.0;
    mysteryShipSpawnInterval = GetRandomValue(10, 20);
    score = 0;
    alienLasers.clear();
    spaceship.lasers.clear();
}

vector<Obstacle> Game::CreateObstacles()
{
    int obstacleWidth = Obstacle::grid[0].size() * 3;
    float gap = (GetScreenWidth() - (4 * obstacleWidth)) / 5;
    for (int i = 0; i < 4; i++)
    {
        float offsetX = (i + 1) * gap + i * obstacleWidth;
        obstacles.push_back(Obstacle({offsetX, float(GetScreenHeight() - 100)}));
    }
    return obstacles;
}

void Game::DeleteInactiveLasers()
{
    for (auto it = spaceship.lasers.begin(); it != spaceship.lasers.end();)
    {
        if (!it->active)
        {
            it = spaceship.lasers.erase(it);
        }
        else
        {
            it++;
        }
    }
    for (auto it = alienLasers.begin(); it != alienLasers.end();)
    {
        if (!it->active)
        {
            it = alienLasers.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void Game::AlienShootLaser()
{
    if (GetTime() - timeLastAlienFired >= alienLaserShootInterval && !aliens.empty())
    {
        int randomIndex = GetRandomValue(0, aliens.size() - 1);
        Alien &alien = aliens[randomIndex];
        alienLasers.push_back(Laser({alien.position.x + alien.alienImages[alien.type - 1].width / 2, alien.position.y + alien.alienImages[alien.type - 1].height}, -6));
        PlaySound(laserBeam);
        timeLastAlienFired = GetTime();
    }
}

void Game::MoveAliens()
{
    for (auto &alien : aliens)
    {
        if (alien.position.x + alien.alienImages[alien.type - 1].width > GetScreenWidth())
        {
            aliensDirection = -1;
            MoveDownAliens(4);
        }
        if (alien.position.x < 0)
        {
            aliensDirection = 1;
            MoveDownAliens(4);
        }
        alien.Update(aliensDirection);
    }
}

void Game::CheckForCollisions()
{
    for (auto &laser : spaceship.lasers)
    {
        auto it = aliens.begin();
        while (it != aliens.end())
        {
            if (CheckCollisionRecs(it->getRect(), laser.getRect()))
            {
                it = aliens.erase(it);
                laser.active = false;
                score += 20;
                PlaySound(explosion);
            }
            else
            {
                it++;
            }
        }
        for (auto &obstacle : obstacles)
        {
            auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end())
            {
                if (CheckCollisionRecs(it->getRect(), laser.getRect()))
                {
                    obstacle.blocks.erase(it);
                    laser.active = false;
                }
                else
                {
                    it++;
                }
            }
        }

        if (CheckCollisionRecs(mysteryship.getRect(), laser.getRect()))
        {
            mysteryship.alive = false;
            laser.active = false;
            PlaySound(explosion);
            score += 100;
        }
    }
    for (auto &laser : alienLasers)
    {
        for (auto &obstacle : obstacles)
        {
            auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end())
            {
                if (CheckCollisionRecs(it->getRect(), laser.getRect()))
                {
                    obstacle.blocks.erase(it);
                    laser.active = false;
                }
                else
                {
                    it++;
                }
            }
        }

        if (CheckCollisionRecs(spaceship.getRect(), laser.getRect()))
        {
            GameOver();
            laser.active = false;
        }
    }

    for (auto &alien : aliens)
    {
        for (auto &obstacle : obstacles)
        {
            auto it = obstacle.blocks.begin();
            while (it != obstacle.blocks.end())
            {
                if (CheckCollisionRecs(it->getRect(), alien.getRect()))
                {
                    it = obstacle.blocks.erase(it);
                }
                else
                {
                    it++;
                }
            }
        }

        if (CheckCollisionRecs(alien.getRect(), spaceship.getRect()))
        {
            GameOver();
        }
    }
}

void Game::MoveDownAliens(int distance)
{
    for (auto &alien : aliens)
    {
        alien.position.y += distance;
    }
}

vector<Alien> Game::CreateAliens()
{
    vector<Alien> aliens;
    for (int row = 0; row < 5; row++)
    {
        for (int column = 0; column < 11; column++)
        {
            int alienType;
            if (row == 0)
            {
                alienType = 1;
            }
            else if (row == 1 || row == 2)
            {
                alienType = 2;
            }
            else
            {
                alienType = 3;
            }
            float x = 75 + column * 55;
            float y = 50 + row * 55;
            aliens.push_back(Alien(alienType, {x, y}));
        }
    }
    return aliens;
}
