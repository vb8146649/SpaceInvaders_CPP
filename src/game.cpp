#include "game.h"

Game::Game()
{
    obstacles=CreateObstacles();
    aliens=CreateAliens();
    aliensDirection=1;
    timeLastAlienFired=0.0;
    mysteryship.Spawn();
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
    for(auto& laser:alienLasers){
        laser.Update();
    }
    AlienShootLaser();
    DeleteInactiveLasers();
    MoveAliens();
    mysteryship.Update();
}

void Game::Draw()
{
    spaceship.Draw();
    for(auto& laser:spaceship.lasers){
        laser.Draw();
    }
    for(auto& obstacle:obstacles){
        obstacle.Draw();
    }
    for(auto& alien:aliens){
        alien.Draw();
    }
    for(auto& laser:alienLasers){
        laser.Draw();
    }
    mysteryship.Draw();

}

vector<Obstacle> Game::CreateObstacles()
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
    for(auto it = alienLasers.begin();it!=alienLasers.end();){
        if(!it->active){
            it = alienLasers.erase(it);
        }else{
            it++;
        }
    }
}

void Game::AlienShootLaser()
{
    if(GetTime()-timeLastAlienFired>=alienLaserShootInterval && !aliens.empty()){
        int randomIndex=GetRandomValue(0,aliens.size()-1);
        Alien& alien=aliens[randomIndex];
        alienLasers.push_back(Laser({alien.position.x+alien.alienImages[alien.type-1].width/2,alien.position.y+alien.alienImages[alien.type-1].height},-6));
        timeLastAlienFired=GetTime();
    }
}

void Game::MoveAliens()
{
    for(auto& alien:aliens){
        if(alien.position.x+alien.alienImages[alien.type-1].width>GetScreenWidth()){
            aliensDirection=-1;
            MoveDownAliens(4);
        }
        if(alien.position.x<0){
            aliensDirection=1;
            MoveDownAliens(4);
        }
        alien.Update(aliensDirection);
    }
}

void Game::MoveDownAliens(int distance)
{
    for(auto& alien:aliens){
        alien.position.y+=distance;
    }
}

vector<Alien> Game::CreateAliens()
{
    vector<Alien> aliens;
    for(int row=0;row<5;row++){
        for(int column =0;column<11;column++){
            int alienType;
            if(row==0){
                alienType=1;
            }else if(row==1 || row==2){
                alienType=2;
            }else{
                alienType=3;
            }
            float x=75+column*55;
            float y=110+row*55;
            aliens.push_back(Alien(alienType,{x,y}));
        }
    }
    return aliens;
}
