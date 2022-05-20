#pragma once
#include<vector>
#include"SDL.h"
#include "SDL_image.h"
#include<string>
#include"SDL_ttf.h"
#include"SDL_mixer.h"

class Game {
public:
    Game();
    void Run();
private:
    bool is_running = true;
    bool alive = true;
    bool again = false;
    bool pause = false;
    bool start = true;
    bool MouseMotion = false;

    static const int FRAME_RATE = 1000 / 60;
    static const int SCREEN_WIDTH = 640;
    static const int SCREEN_HEIGHT = 640;
    static const int GRID_WIDTH = 20;
    static const int GRID_HEIGHT = 20;   

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Surface* image = NULL;
    SDL_Texture* texture = NULL;
    SDL_Surface* surface = NULL;
    Mix_Music* nhac = NULL;
    Mix_Chunk* chunk = NULL;

    enum class Block {
        head,
        body,
        food,
        empty
    };
    enum class Move {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    Move last_dir = Move::UP;
    Move dir = Move::UP;

    struct { float x = GRID_WIDTH / 2; float y = GRID_HEIGHT / 2; }pos;
    SDL_Point head = { static_cast<int>(pos.x), static_cast<int>(pos.y) };
    std::vector<SDL_Point> food;
    std::vector<SDL_Point>body;
    Block grid[GRID_WIDTH][GRID_HEIGHT];

    int startsize = 3;
    float speed = 0.3f;
    int growing = 0;//int score = 0;
    int numberfood = 3;//int maxscore;
    bool ate = false;
    int dem=8;
    bool colorchange = false;
  
    void GameLoop();
    void PollEvents();
    void Update();
    void Render();

    
    void CreateFood();
    void startbody();
    void ReplaceFood(SDL_Point& foods);
    void GrowBody(int quantity);
   
    void RenderStartmenu(SDL_Renderer* renderer);
    void PollEvents_Startmenu();
    void RenderMenu(SDL_Renderer* renderer);
    void PollEvents_Menu();
    void RenderGameover(SDL_Renderer* renderer);
    void PollEvents_Gameover();
   

    void PlayWAV(std::string link);
    void PlayMusic(std::string link);

    void Reset();
    void Close();
};
