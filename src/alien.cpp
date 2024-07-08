#include "alien.h"

Texture2D Alien::alienImages[3]={};
Alien::Alien(int type, Vector2 position)
{
    this->type=type;
    this->position=position;
    if(alienImages[type-1].id==0){
        switch(type){
            case 1:
                alienImages[0]=LoadTexture("src/alien_1.png");
                break;
            case 2:
                alienImages[1]=LoadTexture("src/alien_2.png");
                break;
            case 3:
                alienImages[2]=LoadTexture("src/alien_3.png");
                break;
            default:
                alienImages[0]=LoadTexture("src/alien_1.png");
                break;
        }
    }
}

void Alien::Update(int direction)
{
    position.x+=direction;
}

void Alien::Draw()
{
    DrawTextureV(alienImages[type-1],position,WHITE);
}

Rectangle Alien::getRect()
{
    return {position.x,position.y,float(alienImages[type-1].width),float(alienImages[type-1].height)};
}

int Alien::GetType(){
    return type;
}

