#include "game.h"

Game::Game()
{
}

Game::~Game()
{
}

void Game::HandleInput()
{
    if(IsKeyDown(KEY_LEFT)){
        spaceship.MoveLeft();
    }else if(IsKeyDown(KEY_RIGHT)){
        spaceship.MoveRight();
    }
    if(IsKeyDown(KEY_UP)){
        spaceship.FireLaser();
    }
}

void Game::Update()
{
    for(Laser& laser:spaceship.lasers){
        laser.Update();
    }
    DeleteInactiveLasers();
}

void Game::Draw()
{
    spaceship.Draw();
    for(auto& laser:spaceship.lasers){
        laser.Draw();
    }
}

vector<Obstacle> Game::CreaeteObstacles()
{
    int obstacleWidth=Obstacle::grid[0].size()*3;
    float gap = (GetScreenWidth()-(4*obstacleWidth))/5;
    for(int i=0;i<4;i++){
        float offsetX=(i+1)*gap+i*obstacleWidth;
        obstacles.push_back(Obstacle({offsetX,float(GetScreenHeight()-100)}));
    }
    return obstacles;
}

void Game::DeleteInactiveLasers()
{
    for(auto it = spaceship.lasers.begin();it!=spaceship.lasers.end();){
        if(!it->active){
            it = spaceship.lasers.erase(it);
        }else{
            it++;
        }
    }
}
