#include"raylib.h"
#include<cstdlib>
#include<ctime>
#include<unistd.h>


struct tiles{
    Rectangle rec;
    Vector2 pos;
};

int main(){
    
    int windowWidth = 800;
    int windowHeight = 500;
    
    InitWindow(windowWidth,windowHeight,"FLAPPY BIRDS");
    InitAudioDevice();
    SetTargetFPS(60);
    
    //difficulty
    int mode = -3;
    
    int score = 0;
    
    //Start game
    bool isStarted = false;
    
    Texture2D start = LoadTexture("textures/start.png");
    Rectangle startRec ;
    startRec.height = start.height;
    startRec.width = start.width;
    startRec.x = 0;
    startRec.y = 0;
    Vector2 startPos;
    startPos.x = windowWidth/2 - startRec.width/2;
    startPos.y = windowHeight/2 - startRec.height/2;
    
    
    //pause gameTexture2D paused = LoadTexture("textures/paused.png");
    Texture2D paused = LoadTexture("textures/paused.png");
    Rectangle pausedRec;
    pausedRec.x = 0;
    pausedRec.y = 0;
    pausedRec.height = paused.height;
    pausedRec.width = paused.width;
    Vector2 pausedPos;
    pausedPos.x = windowWidth/2 - paused.width/2;
    pausedPos.y = windowHeight/2 - paused.height/2;
    

    // if the bird is in air
    bool inAir = true;
    bool pause = false;
    
    //movement of the bird
    int gravity = 500;
    int velocity = 250;
    
    
    //bird import and dimensions
    Texture2D flappy = LoadTexture("textures/bird.png");
    Rectangle flappyRec;
    flappyRec.width = flappy.width/3;
    flappyRec.height = flappy.height;
    flappyRec.x = 0;
    flappyRec.y = 0;
    Vector2 flappyPos;
    flappyPos.x = windowWidth/2 - flappyRec.width/2;
    flappyPos.y = flappyRec.height;
    
    //animation frame flappy 
    int flappyFrame = 0;
    float updateTime = 1.0 / 12.0;
    float runningTime = 0.0;
    
    
    
    //import tiles
    Texture2D tileU = LoadTexture("textures/tileUP.png");
    Texture2D tileD = LoadTexture("textures/tileD.png");
    
    
    
    
    int numTiles = 100;
    
    
    //creating an array to create random height for the tiles
    int randomHeight[numTiles];
    /*srand(time(0));
    
    for(int i=0;i<numTiles;i++){
        randomHeight[i] = 1 + (rand() + 50);
    }
    */
    int sign = -1;
    int change = 30;
    
    
    srand(time(0));
    for(int i=0;i<numTiles;i++){
        
        randomHeight[i] = (change * sign) + (1 + (rand() % 90));
        sign *= -1;
        
        //change += 38 ;
    }
    
    
    tiles up[numTiles];
    tiles down[numTiles];
    
    int tilesDistance = 200;
    
    for(int i=0 ; i < numTiles ; i++){
        up[i].rec.width = tileU.width;
        up[i].rec.height = tileU.height;
        up[i].rec.x = 0;
        up[i].rec.y = 0;
        up[i].pos.y = -250 + (randomHeight[i]*sign)  ;
        
        down[i].rec.width = tileD.width;
        down[i].rec.height = tileD.height;
        down[i].rec.x = 0;
        down[i].rec.y = 0;
        down[i].pos.y = windowHeight - tileD.height + (randomHeight[i]*sign) ;
        
        
        up[i].pos.x = windowWidth - tileU.width + i * tilesDistance ;
        down[i].pos.x = windowWidth - tileD.width + i * tilesDistance ;
        
        sign = sign * -1;
    }
    
    //finish line
    float finish = up[numTiles-1].pos.x;
    
    //imoport background
    Texture2D bg = LoadTexture("textures/bg.png");
    float bgX = 0;
    
    //import ground
    
    Texture2D ground = LoadTexture("textures/g.png");
    float groundX = 0;
    float groundY = windowHeight - ground.height;
    
    
     //game over img
    Texture2D gameOver = LoadTexture("textures/gameover.png");
    Rectangle gameOverRec;
    gameOverRec.height = gameOver.height;
    gameOverRec.width = gameOver.width;
    gameOverRec.x = 0;
    gameOverRec.y = 0;
    Vector2 gameOverPos;
    gameOverPos.x = windowWidth/2 - gameOverRec.width/2;
    gameOverPos.y = windowHeight/2 - gameOverRec.height/2;
    
    
    
    //import sounds
    Sound jump = LoadSound("sounds/wing.wav");
    Sound ded = LoadSound("sounds/hit.wav");
    Sound inc = LoadSound("sounds/point.wav");
    
    while(WindowShouldClose()==false){
        const float dT = GetFrameTime();
        BeginDrawing();
        ClearBackground(WHITE);
        
        Vector2 bgPos = {bgX,-50.0};
        Vector2 bgPos2 = {bgX + bg.width*0.3, -50};
        
        Vector2 groundPos = {groundX, groundY};
        Vector2 groundPos2 = {groundX + ground.width * 1.0, groundY};
        
        //animating background
        bgX -= 20 * dT;
        if(bgX <= -bg.width*0.3){
            bgX = 0.0;
        }
        
        
        //animating ground
        groundX -= 25 * dT;
        if(groundX <= -ground.width*1.0){
            groundX = 0.0;
        }
    
    
        //drawing background
        DrawTextureEx(bg , bgPos,0.0, 0.3,WHITE);
        DrawTextureEx(bg , bgPos2, 0.0, 0.3, WHITE);
        
        
        
        if(IsKeyPressed(KEY_ENTER)) {
            isStarted = true;
        }

        if(!isStarted){
            ClearBackground(WHITE);
            DrawTextureRec(start,startRec,startPos,WHITE);
        }
        
        else{
            if(IsKeyPressed(KEY_P)) pause = !pause;
            
            if(pause){
                DrawTextureRec(paused,pausedRec,pausedPos,WHITE);
            }
            else{
               
                //if the bird touches the ground game over!
                 if(!inAir){
                    DrawTextureRec(gameOver,gameOverRec,gameOverPos,WHITE);
                    
                 }
                 
                 else{
                     
                     
                     //if the bird is at ground
                    if(flappyPos.y > windowHeight - ground.height - flappyRec.height  || flappyPos.y < 0+flappyRec.height){
                        PlaySound(ded);
                        velocity = 0;
                        inAir = false;
                        
                    }
                    
                    else{
                        velocity += gravity * dT;
                    }
                    
                    if(flappyPos.y < 0 ){
                        flappyPos.y = 0;
                    }
                    
                    if(IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        velocity -= 300;
                        PlaySound(jump);
                    }
                    
                    //position of bird
                    flappyPos.y += velocity * dT;
                    
                    
                    //position of the tiles
                    for(int i=0 ; i<numTiles ; i++){                    
                        up[i].pos.x += mode;
                        down[i].pos.x += mode;
                        
                        
                    }
                    
                    //update finish line
                    finish -= 2*dT; 
                    
                    
                    
                    //running time updation
                    runningTime += dT;
                    if(runningTime >= updateTime){ 
                        runningTime = 0.0;
                         //updating frame of flappy 
                        flappyRec.x = flappyFrame*flappyRec.width;
                        flappyFrame++;
                        if(flappyFrame > 5) flappyFrame = 0;
                    }
                    
                    
                    
                    //collision check
                    bool collisionUp = false;
                    for(tiles tile : up){
                        Rectangle tileRec = {
                            tile.pos.x,
                            tile.pos.y,
                            tile.rec.width,
                            tile.rec.height
                        };  
                        
                        Rectangle birdRec = {
                            flappyPos.x,
                            flappyPos.y,
                            flappyRec.width,
                            flappyRec.height
                        };
                        
                        if(CheckCollisionRecs(birdRec,tileRec)){
                            collisionUp = true;
                        }
                        
                    }
                    bool collisionDown = false;
                    for(tiles tile : down){
                        Rectangle tileRec = {
                            tile.pos.x,
                            tile.pos.y,
                            tile.rec.width,
                            tile.rec.height
                        };  
                        
                        Rectangle birdRec = {
                            flappyPos.x,
                            flappyPos.y,
                            flappyRec.width,
                            flappyRec.height
                        };
                        if(CheckCollisionRecs(birdRec,tileRec)){
                            collisionDown = true;
                        }
                    }
                    
                    //if collided then game over
                    if(collisionDown || collisionUp){
                        PlaySound(ded);
                        inAir = false;
                    }
                    else{
                        score++;
                    }
                    
                    
                    //updating score
                    
                    for(int i=0;i<numTiles;i++){
                        for(int j=i;j<numTiles;j++){
                            if(up[j].pos.x == flappyPos.x){
                                PlaySound(inc);
                                break;
                            }
                        }
                    }
                    
                    // drawing tiles
                    for(int i=0;i<numTiles;i++){                 
                        
                        
                        
                        DrawTextureRec(tileU,up[i].rec,up[i].pos,WHITE);
                        DrawTextureRec(tileD,down[i].rec,down[i].pos,WHITE);
                        
                        
                    }
                    
                    
                    
                    //drawing bird
                    DrawTextureRec(flappy,flappyRec,flappyPos,WHITE);
                    
                 }
                }
        }
         
        //drawing ground
        DrawTextureEx(ground , groundPos,0.0, 1.0,WHITE);
        DrawTextureEx(ground , groundPos2,0.0, 1.0,WHITE);
        DrawText("Made by Herumb Nagpal",windowWidth/2-50,windowHeight-20,10,RED);
        DrawText( TextFormat("Score : %i",score/100), windowWidth/2+70,windowHeight-70,30,RED);

         
        //ClearBackground(BLACK);
        EndDrawing();
    }
    UnloadSound(ded);
    UnloadSound(jump);
    UnloadSound(inc);
    UnloadTexture(tileD);
    UnloadTexture(tileU);
    UnloadTexture(start);
    UnloadTexture(gameOver);
    UnloadTexture(ground);
    UnloadTexture(flappy);
    UnloadTexture(bg);
    
    CloseWindow();
}