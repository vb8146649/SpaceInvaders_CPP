#include "mysteryship.h"

MysteryShip::MysteryShip()
{
    image=LoadTexture("src/mystery.png");
    alive=false;
}

void MysteryShip::Spawn(){
    position.y=90;
    int side=GetRandomValue(0,1);
    if(side==0){
        position.x=0;
        speed=3;
    }else{
        position.x = GetScreenWidth()-image.width;
        speed=-3;
    }
    alive=true;
}

void MysteryShip::Update(){
    if(alive){
        position.x+=speed;
        if(position.x>GetScreenWidth()-image.width|| position.x<0){
            alive=false;
        }
    }
}

void MysteryShip::Draw(){
    if(alive){
        DrawTextureV(image,position,WHITE);
    }
}