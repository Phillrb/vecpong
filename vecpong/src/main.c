//
//  main.c
//
//  Created by Phillip Riscombe-Burton on 05/04/2015.
//  Copyright (c) 2015 Phillip Riscombe-Burton. All rights reserved.
//
//	Permission is hereby granted, free of charge, to any person obtaining a
//	copy of this software and associated documentation files (the
//	"Software"), to deal in the Software without restriction, including
//	without limitation the rights to use, copy, modify, merge, publish,
//	distribute, sublicense, and/or sell copies of the Software, and to
//	permit persons to whom the Software is furnished to do so, subject to
//	the following conditions:
//
//	The above copyright notice and this permission notice shall be included
//	in all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//	OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//	CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//	TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "vectrex.h"
#include "main.h"
#include "graphics.h"
#include "stdbool.h"
#include "sounds.h"
#include "tunes.h"

// NOTE: By using -mint8 compiler flag there is no need to explictly use
// the int8 type all the time

//DRAWING LIMITS
const int drawScaleSprite = 0x40;
const int drawScaleScreen = 0xFF;
const int screenMaxFromCentre = 63;
//ball
const int ballLeftMax = -45;
const int ballRightMax = 50;
const int ballTopMax = 62;
const int ballBottomMax = -59;
//court
const int courtMaxWidthFromCentre = 50;

//Game limits
const int maxScoreForPlayer = 10;

const int player1YPos = 57;
const int player2YPos = -57;
const int paddleHeight = 5;
const int paddleWidth = 10;
const int paddleLeftMax = -40;
const int paddleRightMax = 40;

const int menuXpos = -40;

//
//The 'cartridge init block' in crt0.s executes and then calls this main function.
//You can change the startup music, title and info in that block.
//

// Main entry point and game loop
int main()
{
    //Put printStr char height and width back to defaults (after splash changed them)
    charHeight = 0xf8;
    charWidth = 0x50;
    
    //Initialize settings that get passed around everywhere
    GameVars gameVars = {
        GSTitle,    //state
        0,          //frame
        0,          //frame multiplier
        MSStopped,   //Music state - no tune on start up credits
        GMusicTitle, //Current music track
        //Sounds array - index must match enum
            GSoundPaddleBounce, 5, 0, //sound name, duration, frame
            GSoundWallBounce, 4, 0,
        //ball
        {   false,          //invisible
            ballLeftMax,    //left
            ballTopMax,     //top
            1,              //moving right
            -1,             //moving down
            false           //speed up
        },
        //Player 1
        {
            0,      //p1 score
            0       //P1 xPos
        },
        //Player 2
        {
            0,      //p2 score
            0       //P2 xPos
        },
        true        //p1 to serve
    };

    //Setup joysticks - only need left & right on both
    //p1LR, P1UD, P2LR, P2UD
    joyEnableFlags(true, false, true, false);
    joyAnalogResolution(0x00); //v accurate
    
    //MAIN LOOP
    while (true)
    {
        //Music selection and preparation
        prepMusic(&gameVars);
        
        // wait for frame boundary (one frame = 30,000 cyles = 50 Hz)
        waitRecal();
        
        //Music execution
        runMusic(&gameVars);
        
        //Execute current state
        runGameState(&gameVars);

        //Game sound effects and music management
        runSound(&gameVars);
    }
}



/* 
 * STATE ENGINE
 */

void runGameState(GameVars* gameVars)
{
    //Game State manager
    switch (gameVars->gameState) {
        
        //Title screen
        case GSTitle:
        {
            runTitle(gameVars);
        }
            break;
        
        //Menu screen
        case GSMenu:
        {
            runMenu(gameVars);
        }
            break;
            
        case GSCredits:
        {
            runCredits(gameVars);
        }
            break;
            
        //Game engine
        case GSGameStart:
        case GSWaitForServe:
        case GSPlay:
        case GSScored:
        case GSGameOver:
        {
            runGameplay(gameVars);
        }
            break;
            
        default:
            break;
    }
}

//Deal with transcient game states
void moveToNextGameState(GameVars* gameVars, GameEvent gameEvent)
{
    switch (gameVars->gameState) {
        
        //Title to Menu
        case GSTitle: {
            gameVars->gameState = GSMenu;
            requestPlayMusic(GMusicMenu, gameVars);
            
        } break;
         
        //Menu to Game or credits
        case GSMenu:
        {
            switch (gameEvent)
            {
                case GEPlayAgain: {
                    gameVars->gameState = GSGameStart;
                    gameVars->musicState = MSStop;
                } break;
                case GEShowCredits: {
                    gameVars->gameState = GSCredits;
                    requestPlayMusic(GMusicTitle, gameVars);
                } break;
                default: break;
            }
        }
            break;
           
        //Credits to Menu
        case GSCredits: {
            gameVars->gameState = GSMenu;
            requestPlayMusic(GMusicMenu, gameVars);
        } break;
            
        //Game intro to serve
        case GSGameStart: {
            prepareForServeState(gameVars);
            gameVars->gameState = GSWaitForServe;
            gameVars->musicState = MSStop;
        } break;
        
        //Serve to play
        case  GSWaitForServe:
        {
            switch(gameEvent) {
                case GEWinner: {
                    gameVars->gameState = GSGameOver;
                    requestPlayMusic(GMusicWinner, gameVars);
                } break;
                case GENone: { gameVars->gameState = GSPlay; } break;
                default: break;
            }
        }
            break;

        //Play to goal or timeup
        case GSPlay:
        {
            switch (gameEvent) {
                case GEGoal: {
                    gameVars->gameState = GSScored;
                    requestPlayMusic(GMusicScored, gameVars);
                } break;
                default: break;
            }
        }
            break;
        
        //Goal to serve
        case GSScored: {
            prepareForServeState(gameVars);
            
            //Go to serve state
            gameVars->gameState = GSWaitForServe;
            gameVars->musicState = MSStop;
            
        } break;
            
        //Game Over to Play Again or Menu
        case GSGameOver:
        {
            switch (gameEvent) {
                case GEBackToMenu: {
                    gameVars->gameState = GSMenu;
                    requestPlayMusic(GMusicMenu, gameVars);
                } break;
                case GEPlayAgain: {
                    gameVars->gameState = GSWaitForServe;
                    gameVars->musicState = MSStop;
                } break;
                default: break;
            }
        }
            break;
            
        default: break;
    }

    //Run new state
    gameVars->currentFrame = 0; //Reset frame
    gameVars->currentFrameMultiplier = 0; //Reset frame multiplier
}

void waitForFramesThenMoveOnToNextState(int maxFrames, GameVars* gameVars)
{
    //pause until limit
    gameVars->currentFrame++;
    if(gameVars->currentFrame >= maxFrames)
    {
        moveToNextGameState(gameVars, GENone);
    }
}

/*
 *  TITLE SCREEN
 */

void runTitle(GameVars* gameVars)
{
#ifdef DEBUG
    debugPrint("TITLE\x80");
#endif
    
    //Display Title
    drawNEScart(0x80);
    
    //Display Title for short duration
    waitForFramesThenMoveOnToNextState(50, gameVars);
}

/*
 *  MENU SCREEN
 */

void runMenu(GameVars* gameVars)
{
#ifdef DEBUG
    debugPrint("MENU\x80");
#endif
    
    //Display menu until user starts game
    drawMainMenu(0x50);
    
    //wait for user to start game with button 1
    readButton();
    
    //Player 1 Button 1 - Play
    if (_BTN_CUR_MASK & _JOY1_B1)
    {
        //Move to Play game state
        moveToNextGameState(gameVars, GEPlayAgain);
    }
    else if (_BTN_CUR_MASK & _JOY1_B2)
    {
        //Show credits screen
        moveToNextGameState(gameVars, GEShowCredits);
    }
}

/*
 * CREDITS SCREEN
 */

void runCredits(GameVars* gameVars)
{
#ifdef DEBUG
    debugPrint("CREDITS\x80");
#endif
    //Display credits
    drawCredits(0x80);
    
    //wait for user to go back with button 1
    readButton();
    
    //Player 1 Button 1 - Play
    if (_BTN_CUR_MASK & _JOY1_B1)
    {
        //Move to Play game state
        moveToNextGameState(gameVars, GENone);
    }
}

/*
 * PLAY 'SCREEN' - Play state machine
 */

void runGameplay(GameVars* gameVars)
{
    //GAME
    
    //Draw the court
    drawCourt();
    
    //Draw the scores
    drawScores(gameVars);
    
    //Game play states
    switch (gameVars->gameState) {
        case GSGameStart:
        {
#ifdef DEBUG
            debugPrint("START\x80");
#endif
            //Draw Ready message
            drawReady(0x80);
            
            //Wait for a number of frames
            waitForFramesThenMoveOnToNextState(60, gameVars);
        }
            break;
            
        case GSWaitForServe:
        {
#ifdef DEBUG
            debugPrint("SERVE\x80");
#endif
            //Score max?
            if (gameVars->player1.score >= maxScoreForPlayer
                || gameVars->player2.score >= maxScoreForPlayer)
            {
                //Served - move to next state
                moveToNextGameState(gameVars, GEWinner);
                return;
            }
            
            //Draw player paddles
            movePaddles(gameVars);

            //Inform who is about to serve
            drawServe(gameVars->player1ServeNext);
            
            //wait for user to start game with button 4
            readButton();
            
            //Player 1 Button 4 or P2 Button 4
            if (
                (gameVars->player1ServeNext && (_BTN_CUR_MASK & _JOY1_B4))
                ||
                (!gameVars->player1ServeNext && (_BTN_CUR_MASK & _JOY2_B4))
                )
            {
                //Position the ball and make visible
                serve(gameVars);
                
                //Served - move to next state
                moveToNextGameState(gameVars, GENone);
            }

        }
            break;
            
        case GSPlay:
        {
#ifdef DEBUG
            debugPrint("PLAY\x80");
#endif
            //GAME PLAY ACTION
            
            drawCenterLine(gameVars);
            
            //Draw player paddles
            movePaddles(gameVars);
            
            //Only deal with visible balls
            if(gameVars->ball.visibility == false) return;
            
            //Move ball - check for goal
            moveBallSprite(gameVars);
        }
            break;
            
        case GSScored:
        {
#ifdef DEBUG
            debugPrint("SCORED\x80");
#endif
            //Goal!
            drawPoint(!gameVars->player1ServeNext);
            
            waitForFramesThenMoveOnToNextState(60, gameVars);
        }
            break;
            
        case GSGameOver:
        {
#ifdef DEBUG
            debugPrint("GAMEOVER\x80");
#endif
            //Display 'Game Over'
            drawGameOver(gameVars->player1.score > gameVars->player2.score);
            
            //wait for user to select replay or menu
            readButton();
            //Player 1 Button 1
            if (_BTN_CUR_MASK & _JOY1_B1)
            {
                //Reset scores
                resetPlayerScores(gameVars);
                
                //Replay - move to next state
                moveToNextGameState(gameVars, GEPlayAgain);
            }
            else if (_BTN_CUR_MASK & _JOY1_B2)
            {
                //Reset scores
                resetPlayerScores(gameVars);
                
                //Menu - move to next state
                moveToNextGameState(gameVars, GEBackToMenu);
            }
        }
            break;
            
        default:
            break;
    }
    
}


/*
 * BALL & PLAYER MOVEMENT
 */
void moveBallSprite(GameVars *gameVars){
    
    //Reposition ball - check for events
    moveBall(gameVars);
    
    //Draw ball
    drawBall(gameVars->ball);
}

//Move ball by the vel of its axis - and bounce or goal
void moveBall(GameVars* gameVars)
{
    //Move ball X
    gameVars->ball.xPos = gameVars->ball.xPos + gameVars->ball.xVel;

    //LEFT & RIGHT WALL
    if (gameVars->ball.xPos > ballRightMax) //Right wall
    {
        //Bounce wall sound
        requestPlaySound(GSoundWallBounce, gameVars);
        
        gameVars->ball.xPos = ballRightMax - 1;
        gameVars->ball.xVel = -(gameVars->ball.xVel); //bounce
    }
    else if(gameVars->ball.xPos < ballLeftMax) //Left wall
    {
        //Bounce wall sound
        requestPlaySound(GSoundWallBounce, gameVars);
        
        gameVars->ball.xPos = ballLeftMax + 1;
        gameVars->ball.xVel = -(gameVars->ball.xVel); //bounce
    }
    
    //Move ball Y
    gameVars->ball.yPos = gameVars->ball.yPos + gameVars->ball.yVel;
    
    //Speed-up movement
    handleBallSpeed(gameVars);
    
    //Scored?
    if (gameVars->ball.yPos < ballBottomMax) //Bottom wall
    {
        //GOAL
        playerScored(0, gameVars);
    }
    else if(gameVars->ball.yPos > ballTopMax) //Top wall
    {
        //GOAL
        playerScored(1, gameVars);
    }
    
    //HIT PLAYER 1 TOP SURFACE (up to 4 pixels deep)
    else if(gameVars->ball.yPos >= (player1YPos - paddleHeight) && gameVars->ball.yPos <= (player1YPos - paddleHeight + 4)
            && gameVars->ball.xPos <= (gameVars->player1.xPos + paddleWidth) && gameVars->ball.xPos >= (gameVars->player1.xPos - paddleWidth))
    {
        //Bounce paddle sound
        requestPlaySound(GSoundPaddleBounce, gameVars);
        
        //Bounce off player 1
        gameVars->ball.yPos = player1YPos - paddleHeight;
        gameVars->ball.yVel = -(gameVars->ball.yVel); //bounce
        
        //Angle change?
        checkIfBallEdged(&gameVars->player1, &gameVars->ball);
        
        //Speed up on Y velocity
        checkForSpeedUp(gameVars);
    }
    //HIT PLAYER 2 TOP SURFACE (up to 4 pixels deep)
    else if(gameVars->ball.yPos <= (player2YPos + paddleHeight) && gameVars->ball.yPos >= (player2YPos + paddleHeight - 4)
            && gameVars->ball.xPos <= (gameVars->player2.xPos + paddleWidth) && gameVars->ball.xPos >= (gameVars->player2.xPos - paddleWidth))
    {
        //Bounce paddle sound
        requestPlaySound(GSoundPaddleBounce, gameVars);
        
        //Bounce off player 2
        gameVars->ball.yPos = player2YPos + paddleHeight + 1;
        gameVars->ball.yVel = -(gameVars->ball.yVel); //bounce
        
        //Angle change?
        checkIfBallEdged(&gameVars->player2, &gameVars->ball);
        
        //Speed up on Y velocity
        checkForSpeedUp(gameVars);
    }
}

//Speed up ball on certain thresholds of time
void handleBallSpeed(GameVars* gameVars)
{
    gameVars->currentFrame++;
    if(gameVars->currentFrame >= 255)
    {
        gameVars->currentFrame = 0;
        
        //Don't go too fast!
        if(gameVars->currentFrameMultiplier < 15)
        {
            gameVars->currentFrameMultiplier++; //multiplier to get more frames
            
            //Check time thresholds
            if(gameVars->currentFrameMultiplier == 2
               || gameVars->currentFrameMultiplier == 5
               || gameVars->currentFrameMultiplier == 9
               || gameVars->currentFrameMultiplier == 15)
            {
                gameVars->ball.isAwaitingSpeedUp = true;
            }
        }
    }
}

//Is ball hitting edge of paddle - increase or reduce angle of ball (X AXIS VELOCITY)
void checkIfBallEdged(Player* player, Ball* ball)
{
    if((*ball).xPos >= (*player).xPos + paddleWidth - 4)
    {
        //Right Edge
        if((*ball).xVel >= 0) (*ball).xVel++; //Increase angle
        else if ((*ball).xVel <= -1) (*ball).xVel++; //decrease angle
    }
    else if((*ball).xPos <= ((*player).xPos - paddleWidth + 4))
    {
        //Left Edge
        if((*ball).xVel >= 1) (*ball).xVel--; //decrease angle
        else if ((*ball).xVel <= 0) (*ball).xVel--; //Increase angle
    }
}

//Speed up ball if waiting for it - speed up only occurs off a paddle
void checkForSpeedUp(GameVars* gameVars)
{
    if(gameVars->ball.isAwaitingSpeedUp)
    {
        gameVars->ball.isAwaitingSpeedUp = false;
        
        if(gameVars->ball.yVel > 0) gameVars->ball.yVel++;
        else gameVars->ball.yVel--;
    }
}

//Move players
void movePaddles(GameVars* gameVars)
{
    //Read joysticks
    joyAnalog();
    
    //Player 1 movement
    checkPaddleSpeedAndPos(&gameVars->player1, pot0);
    
    //Player 2 movement
    checkPaddleSpeedAndPos(&gameVars->player2, pot2);
    
    //Draw them
    drawPaddles(gameVars);
}

//Update position analog and limit placement
void checkPaddleSpeedAndPos(Player* player, int playerControllerXpot)
{
    //Stepped speed - make the use of the analogue controller
    if(      playerControllerXpot > 100) player->xPos += 3;
    else if( playerControllerXpot > 50) player->xPos += 2;
    else if( playerControllerXpot > 0) player->xPos++;
    else if( playerControllerXpot < -100) player->xPos -= 3;
    else if( playerControllerXpot < -50) player->xPos -= 2;
    else if( playerControllerXpot < 0) player->xPos--;
    
    //Limit placememt of paddles - stop them going out of court
    if (player->xPos >= paddleRightMax) {
        player->xPos = paddleRightMax;
    }else if (player->xPos < paddleLeftMax) {
        player->xPos = paddleLeftMax;
    }
}

/*
 * GAME EVENTS
 */

//Goal
void playerScored(int playerNumber, GameVars* gameVars)
{
    //Increment score
    if(playerNumber == 0)
    {
        gameVars->player1.score++;
        gameVars->player1ServeNext = false;
    }
    else
    {
        gameVars->player2.score++;
        gameVars->player1ServeNext = true;
    }
    
    //Hide ball
    gameVars->ball.visibility = false;
    
    //Inform state machine
    moveToNextGameState(gameVars, GEGoal);
}

//Make ball visible - for serve
void serve(GameVars* gameVars)
{
    //Put at player position
    resetBall(gameVars);
    gameVars->ball.visibility = true; //serve
}

//Reset ball values
void resetBall(GameVars* gameVars)
{
    if (gameVars->player1ServeNext) {
        //Ball to serve by player 1
        gameVars->ball.xPos = gameVars->player1.xPos;
        gameVars->ball.yPos = ballTopMax - 10;
        gameVars->ball.xVel = 1;
        gameVars->ball.yVel = -1;
    }
    else
    {
        //Ball for serve by player 2
        gameVars->ball.xPos = gameVars->player2.xPos;
        gameVars->ball.yPos = ballBottomMax + 10;
        gameVars->ball.xVel = -1;
        gameVars->ball.yVel = 1;
    }
}

//Reset player scores
void resetPlayerScores(GameVars* gameVars)
{
    gameVars->player1.score = 0;
    gameVars->player2.score = 0;
    gameVars->player1ServeNext = true;
}

//Reset player position
void resetPlayerPositions(GameVars* gameVars)
{
    gameVars->player1.xPos = 0;
    gameVars->player2.xPos = 0;
}

//Put ball and players back
void prepareForServeState(GameVars* gameVars)
{
    gameVars->ball.visibility = false;
    resetPlayerPositions(gameVars);
}

/*
 * SOUND
 */

//Music selection
void prepMusic(GameVars* gameVars)
{

    //Take action on transcient music states
    switch(gameVars->musicState)
    {
        case MSPlay:
        {
            musicFlag = true;
            gameVars->musicState = MSPlaying;
        }
            break;

        case MSPause:
        {
            musicFlag = false;
            gameVars->musicState = MSPausing;
            clearSound();
        }
            break;
            
        case MSContinue:
        {
            musicFlag = 0x80;
            gameVars->musicState = MSPlaying;
        }
            break;
            
        case MSStop:
        {
            musicFlag = false;
            gameVars->musicState = MSStopping;
            clearSound();
        }
            break;
            
        default:
        {
            
        }
            break;
    }
    
    //Continue to select song to play
    if(gameVars->musicState == MSPlaying)
    {
        switch (gameVars->musicTrack) {
            case GMusicTitle:
            {
                //Custom music
                //(has twang and adsr already in it)
                initMusicCHK(gameStart);
            }
                break;
                
            case GMusicMenu:
            {
                //inbuilt music
                adsr = adsr_8;
                twang = twang_flat;
                initMusicCHK(music_8);
            }
                break;
                
            case GMusicScored:
            {
                //inbuilt music
                adsr = adsr_3_13;
                twang = twang_flat;
                initMusicCHK(music_13);
            }
                break;
                
            case GMusicWinner:
            {
                //inbuilt music
                adsr = adsr_2_12;
                twang = twang_flat;
                initMusicCHK(music_12);
            }
                break;
                
            default:
                break;
        }
    }
}

//Play any music
void runMusic(GameVars* gameVars)
{
    //Execute music commands
    switch (gameVars->musicState) {
        case MSPlaying:
        case MSStopping:
        case MSPausing:
        {
            doSound();
        }
            break;
            
        default:
            break;
    }
    
    //Move transcient music states on
    switch (gameVars->musicState) {
        case MSStopping:
        {
            gameVars->musicState = MSStopped;
        }
            break;
            
        case MSPausing:
        {
            gameVars->musicState = MSPaused;
        }
            break;
            
        default:
            break;
    }

}

//Request a new music track to play
void requestPlayMusic(GMusic musicTrack, GameVars* gameVars)
{
    gameVars->musicTrack = musicTrack;
    gameVars->musicState = MSPlay;
}

//In-game sound effects are using voice 3

void runSound(GameVars* gameVars)
{
    //Check through all sounds and see if any are playing
    int i=0;
    while (i < noOfSounds)
    {
        //Sound about to start?
        if (gameVars->sounds[i].currentSoundFrame == 1)
        {
            playSound(gameVars->sounds[i], gameVars);
        }
        
        //Sound playing?
        if (gameVars->sounds[i].currentSoundFrame > 0) {
            //Play or stop playing
            playSoundForDuration((GSound)i, gameVars->sounds[i].duration, gameVars);
        }
        i++;
    }
}

//Will stop a sound when it expires
void playSoundForDuration(GSound sound, int duration, GameVars* gameVars)
{
    //Play sound for its number of frames
    if(delaySoundEnd(sound, duration, gameVars))
    {
        stopSoundOnVoice3();
    }
}

//Check for sound end
bool delaySoundEnd(GSound sound, int duration, GameVars* gameVars)
{
    ((Sound)gameVars->sounds[sound]).currentSoundFrame++;
    if(((Sound)gameVars->sounds[sound]).currentSoundFrame >= duration)
    {
        ((Sound)gameVars->sounds[sound]).currentSoundFrame = 0;
        return true;
    }
    return false;
}

//Stop all sounds on voice 3
void stopSoundOnVoice3()
{
    soundByte(SCAllVoicesOff, SCVoiceGenerators); //Turn off all voices
    soundByte(0, SCVoice3Volume);    //volume on voice 3
}

//Mark sound as playing - use GSound as index
void requestPlaySound(GSound sound, GameVars* gameVars)
{
    ((Sound)gameVars->sounds[sound]).currentSoundFrame = 1;
}

//Play correct noise for sound on voice 1
void playSound(Sound sound, GameVars* gameVars)
{
    soundByte(SCVoice3Only, SCVoiceGenerators); //Turn on voice 3 only
    soundByte(15, SCVoice3Volume);    //full volume on voice 3
    
    switch (sound.gameSound) {
        case GSoundPaddleBounce:
        {
            //Play sound off bat
            soundByte(GS6, SCVoice3Data);  //Note to voice 3
        }
            break;
        
        case GSoundWallBounce:
        {
            //Play sound off wall
            soundByte(GS7, SCVoice3Data);  //Note to voice 3
        }
            break;
            
        default:
            break;
    }
}


/*
 * DRAWING
 */

//Outline the court
void drawCourt()
{
    intensA(0x3f);
    scale = drawScaleScreen;
    
    reset0Ref();//top-right down
    moveToD(courtMaxWidthFromCentre, screenMaxFromCentre);
    drawLineD(0,-2 * screenMaxFromCentre);
    
    reset0Ref();//bottom-right left
    moveToD(courtMaxWidthFromCentre, -screenMaxFromCentre);
    drawLineD(-2 * courtMaxWidthFromCentre,0);
    
    reset0Ref(); //bottom-left up
    moveToD(-courtMaxWidthFromCentre, -screenMaxFromCentre);
    drawLineD(0,2 * screenMaxFromCentre);
    
    reset0Ref(); //top-left right
    moveToD(-courtMaxWidthFromCentre, screenMaxFromCentre);
    drawLineD(2 * courtMaxWidthFromCentre,0);
}

void drawScores(GameVars* gameVars)
{
    resetText();
    intensA(0x2f); //faint
    
    if (gameVars->player1.score < maxScoreForPlayer
        && gameVars->player2.score < maxScoreForPlayer) {
        
        //display scores (convert p2 score to char)
        printShipsX(gameVars->player1.score, gameVars->player2.score + '0', -31, 28);
    }
}

void drawCenterLine()
{
    intensA(0x2f);
    scale = drawScaleScreen;
    
    reset0Ref();
    moveToD(courtMaxWidthFromCentre, 0);
    drawLineD(-2 * courtMaxWidthFromCentre, 0);
}

void drawCredits(int drawScale)
{
    drawNEScart(drawScale);
}

void drawPaddles(GameVars* gameVars)
{
    intensA(0x5f);
    drawSpriteWithScaleAtPos(drawPlayer1Sprite, 0x4F, gameVars->player1.xPos, player1YPos);
    drawSpriteWithScaleAtPos(drawPlayer2Sprite, 0x4F, gameVars->player2.xPos, player2YPos);
}

void drawMainMenu(int drawScale)
{
    resetText();
    moveToD(menuXpos, 20);
    printStr("VECPONG:\x80");
    moveToD(menuXpos, 10);
    printStr("MAIN MENU\x80");
    
    reset0Ref();
    moveToD(menuXpos, -5);
    intensA(0x5f);
    printStr("1 PLAY GAME\x80");
    moveToD(menuXpos, -15);
    printStr("2 CREDITS\x80");
}

void drawServe(bool isPlayer1)
{
    resetText();
    moveToD(-32, 0);
    
    if(isPlayer1) printStr("a SERVE a\x80"); //a = up arrow
    else          printStr("c SERVE c\x80"); //c = down arrow
}

//Get position of ball from array
void drawBall(Ball ball)
{
    intensA(0x5f);
    drawSpriteWithScaleAtPos(ballSprite, 0x28 , ball.xPos, ball.yPos);
}

//Draw 'ready'
void drawReady()
{
    resetText();
    moveToD(-22, 0);
    printStr("READY?\x80");
}

//Draw 'point'
void drawPoint(bool isPlayer1Goal)
{
    resetText();
    moveToD(-32, 0);
    if(isPlayer1Goal) printStr("P1 SCORED!\x80");
    else              printStr("P2 SCORED!\x80");
}

//Draw 'game over'
void drawGameOver(bool isPlayer1Winner)
{
    resetText();
    moveToD(-45, 15);
    if(isPlayer1Winner) printStr("PLAYER 1 WINS!\x80");
    else                printStr("PLAYER 2 WINS!\x80");
    
    reset0Ref();
    moveToD(menuXpos, -5);
    intensA(0x5f);
    printStr("1 PLAY AGAIN\x80");
    moveToD(menuXpos, -15);
    printStr("2 MAIN MENU\x80");
}

//Draw NES cart on title screen
void drawNEScart(int drawScale)
{
    reset0Ref();
    intensA(0x5f);
    drawSpriteWithScaleAtPos(nesCartSprite, drawScale, 5, 0);
    moveToD(-70, -10);
    printStr("NES\x80");
    reset0Ref();
    intensA(0x5f);
    moveToD(-20, 40);
    printStr("4LIFE\x80");
}

//Generic draw sprite
void drawSpriteWithScaleAtPos(const int sprite[], int drawScale, int xPos, int yPos)
{
    reset0Ref();
    scale = drawScaleScreen;
    moveToD(xPos, yPos);
    scale = drawScale;
    
    drawVLp(sprite,0);
}

void resetText()
{
    reset0Ref();
    intensA(0x5f);
    scale = drawScaleScreen;
}

#ifdef DEBUG
void debugPrint(char * str)
{
    resetText();
    moveToD(-40, -60);
    intensA(0x5f);
    printStr(str);
}
#endif
