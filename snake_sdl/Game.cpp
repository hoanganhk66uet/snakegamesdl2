
#include<iostream>
#include "Game.hpp"

using namespace std;


Game::Game() {
    for (int i = 0; i < GRID_WIDTH; i++) {
        for (int  j = 0; j < GRID_HEIGHT; j++) {
            grid[i][j] = Block::empty;
        }
    }
    srand(static_cast<unsigned int>(time(0)));
}
void Game::Run() {

    //Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        exit(EXIT_FAILURE);
    }
    if (TTF_Init() < 0)
    {
        cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        exit(EXIT_FAILURE);
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        cerr << "SDL could not initialize! SDL_Error: " << Mix_GetError() << endl;
    }

    // Create Window
    window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (window == NULL)
    {
        cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
        exit(EXIT_FAILURE);
    }

    // Create Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL)
    {
        cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
        exit(EXIT_FAILURE);
    }

    GameLoop();
    
}


void Game::GameLoop() {

    PlayMusic("music1.mp3");

    Uint32 before, after;
    int frame_time;

    while (is_running)
    {
        before = SDL_GetTicks();
        if (again || start) Reset();
        PollEvents();
        Update();
        Render();

        after = SDL_GetTicks();
        frame_time = after - before;
        if (FRAME_RATE > frame_time)
        {
            SDL_Delay(FRAME_RATE - frame_time);
        }
    }
    
}

void Game::PollEvents() {
    if (start) { PollEvents_Startmenu(); }

    if (!alive) { PollEvents_Gameover(); }

    if (pause) { PollEvents_Menu(); }

    if (!pause && alive && !start) {
        SDL_Event e;

        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                is_running = false;
                Close();
            }
            else {
                if (e.type == SDL_KEYDOWN) {
                    switch (e.key.keysym.sym)
                    {

                    case SDLK_ESCAPE:
                    case SDLK_SPACE:
                        //pause music
                        Mix_PauseMusic();
                        pause = true;

                        break;
                    case SDLK_UP:
                        if (last_dir != Move::DOWN ) dir = Move::UP; break;
                    case SDLK_DOWN:
                        if (last_dir != Move::UP ) dir = Move::DOWN; break;
                    case SDLK_LEFT:
                        if (last_dir != Move::RIGHT ) dir = Move::LEFT; break;
                    case SDLK_RIGHT:
                        if (last_dir != Move::LEFT ) dir = Move::RIGHT; break;
                    }
                }
            }
        }
    }

}
void Game::Update() {

    if (!pause && !start && alive) {

        //change position
        switch (dir) {
        case Move::UP:
            pos.y -= speed;
            break;
        case Move::DOWN:
            pos.y += speed;
            break;
        case Move::LEFT:
            pos.x -= speed;
            break;
        case Move::RIGHT:
            pos.x += speed;
            break;
        }

        //through the wall
        if (pos.x < 0) pos.x = GRID_WIDTH - 1;
        else if (pos.x > GRID_WIDTH) pos.x = 0;
        if (pos.y < 0) pos.y = GRID_HEIGHT - 1;
        else if (pos.y > GRID_HEIGHT) pos.y = 0;

        // toa do new head
        int new_x = static_cast<int>(pos.x);
        int new_y = static_cast<int>(pos.y);

        //change of snakes
        if (new_x != head.x || new_y != head.y) {
             
            last_dir = dir;
            if (growing > 0) {

                body.push_back(head);          //add size,
                
                growing--;
                grid[head.x][head.y] = Block::body;
            }
            else {
                SDL_Point free = head;
                vector<SDL_Point>::reverse_iterator rit = body.rbegin();
                for (; rit != body.rend(); rit++) {
                    grid[free.x][free.y] = Block::body;
                    swap(*rit, free);
                }
                grid[free.x][free.y] = Block::empty;
            }
            //color change status
            if (ate) { colorchange = true; dem--; }
            if (dem < 0) {
                ate = false;
                dem = 10;
                colorchange = false;
            }
        }

        head.x = new_x;
        head.y = new_y;
        Block& next = grid[head.x][head.y];

        // contact
        if (next == Block::food)
        {
            ate = true;
            for (auto& foods : food) {
                if (foods.x == new_x && foods.y == new_y) {
                    PlayWAV("eat.wav");
                    ReplaceFood(foods);
                    GrowBody(1);
                    //score++;
                    break;

                }
            }
        }
        if (next == Block::body)
        {
            Mix_PauseMusic();
            PlayWAV("gameover.wav");
            alive = false;
           
        }
        next = Block::head;
        

    }
    
}

void Game::CreateFood() {
    int x, y;

    for (int i = 0; i < numberfood; i++) {
        SDL_Point point;

        while (true) {
            x = rand() % GRID_WIDTH;
            y = rand() % GRID_HEIGHT;
            if (grid[x][y] == Block::empty) {
                grid[x][y] = Block::food;
                point.x = x;
                point.y = y;
                food.push_back(point);
                break;
            }
        }
    }
}
void Game::startbody() {
    SDL_Point _body;
    _body.x = GRID_WIDTH / 2;
    _body.y = GRID_HEIGHT / 2;
    for (int i = 0; i < startsize; i++) {
        //_body.y += 1;
        body.push_back(_body);
    }
}


void Game::ReplaceFood(SDL_Point& foods) {
    int x, y;
    while (true) {
        x = rand() % GRID_WIDTH;
        y = rand() % GRID_HEIGHT;
        if (grid[x][y] == Block::empty) {
            grid[x][y] = Block::food;
            foods.x = x;
            foods.y = y;

            break;
        }
    }
}
void Game::GrowBody(int quantity)
{
    growing += quantity;
}


void Game::Reset() {

    
    Mix_ResumeMusic();
    again = false;
   
    //score = 0;
    dir = Move::UP;
    last_dir = Move::UP;

    //delete food,body
    for (auto& _food : food) {
        grid[_food.x][_food.y] = Block::empty;
    }
    for (auto& _body : body) {
        grid[_body.x][_body.y] = Block::empty;
    }
    grid[head.x][head.y] = Block::empty;
    pos.x = GRID_WIDTH / 2; 
    pos.y = GRID_HEIGHT / 2;
    head = { static_cast<int>(pos.x), static_cast<int>(pos.y) };

    body.clear();
    food.clear();

    startbody();
    CreateFood();
}

void Game::PollEvents_Startmenu() {
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            
            Close();
            break;
        }
        else if (e.type == SDL_KEYDOWN)
        {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                is_running = false;
                Close();
                break;
            }
            if (e.key.keysym.sym == SDLK_SPACE) {
                start = false;
                break;
            }

        }
        else {
            int mouse_x = e.motion.x;
            int mouse_y = e.motion.y;
            if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                int mouse_x = e.motion.x;
                int mouse_y = e.motion.y;
                if ((mouse_x > 250 && mouse_x < 380) && (mouse_y > 325 && mouse_y < 375))
                {
                    start = false;

                }
            }
            if (e.type == SDL_MOUSEMOTION)
            {
                if ((mouse_x > 250 && mouse_x < 380) && (mouse_y > 325 && mouse_y < 375))   //start
                {
                    MouseMotion = true;
                    break;

                }
                else  MouseMotion = false; break;
            }

        }
    }
}

void Game::PollEvents_Menu() {
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            is_running = false;
            Close();
            break;
        }
        else if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
            case SDLK_SPACE:
                pause = false;
                //resume music
                Mix_ResumeMusic();
                break;

            case SDLK_ESCAPE:
                //is_running = false;
                again = true;
                pause = false;
                start = true;
                break;
            }
        }
    }
}
void Game::PollEvents_Gameover() {
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            is_running = false;
            Close();
            break;
        }
        else if (e.type == SDL_KEYDOWN)
        {
            if (e.key.keysym.sym== SDLK_ESCAPE)
            {
            
                is_running = false;
                Close();
                break;
            }
            else if (e.key.keysym.sym == SDLK_SPACE)
            {

                alive = true;
                again = true;
                break;
            }
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            int mouse_x = e.motion.x;
            int mouse_y = e.motion.y;
            if ((mouse_x > 250 && mouse_x < 306) && (mouse_y > 459 && mouse_y < 495))   
            {
                alive = true;
                again = true;
                break;
            }
            if ((mouse_x > 330 && mouse_x < 410) && (mouse_y > 459 && mouse_y < 495))
            {
                start = true;
                alive = true;
                again = true;
                
            }
        }
    }
}

void Game::Render()
{
    if (start) { RenderStartmenu(renderer); }

    if (!alive) { RenderGameover(renderer); }

    if (pause) { RenderMenu(renderer); }

    if (!pause && alive && !start) {
        SDL_Rect block;
        block.w = SCREEN_WIDTH / GRID_WIDTH;
        block.h = SCREEN_WIDTH / GRID_HEIGHT;

        // background color
        SDL_SetRenderDrawColor(renderer, 124, 252, 0, 0);
        SDL_RenderClear(renderer);

        // Render food
        for (SDL_Point& point : food) {
            block.x = point.x * block.w; 
            block.y = point.y * block.h;

            image = IMG_Load("apple1.jpg");
            texture = SDL_CreateTextureFromSurface(renderer, image);
            SDL_RenderCopy(renderer, texture, NULL, &block);
        }

        // Render snake's body
        int r = rand() % 255;
        int g = rand() % 255;
        int b = rand() % 255;
        
        for (SDL_Point& point : body)
        {
           
            block.x = point.x * block.w;
            block.y = point.y * block.h;
            // discoloration
            if (colorchange) { SDL_SetRenderDrawColor(renderer, r, g, b, 255);}
            else SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
            SDL_RenderFillRect(renderer, &block);


        }

        // Render snake's head
        block.x = head.x * block.w;
        block.y = head.y * block.h;

        image = IMG_Load("head_right.jpg");
        texture = SDL_CreateTextureFromSurface(renderer, image);
        SDL_RenderCopy(renderer, texture, NULL, &block);

        switch (dir) {
        case Move::UP:
            // image = IMG_Load("head_up.jpg");
            SDL_RenderCopyEx(renderer, texture, nullptr, &block, -90, nullptr, SDL_FLIP_NONE);
            break;
        case Move::RIGHT:
            // image = IMG_Load("head_right.jpg");
            SDL_RenderCopyEx(renderer, texture, nullptr, &block, 0, nullptr, SDL_FLIP_NONE);
            break;
        case Move::DOWN:
            // image = IMG_Load("head_down.jpg");
            SDL_RenderCopyEx(renderer, texture, nullptr, &block, 90, nullptr, SDL_FLIP_NONE);
            break;
        case Move::LEFT:
            // image = IMG_Load("head_left.jpg");
            SDL_RenderCopyEx(renderer, texture, nullptr, &block, 180, nullptr, SDL_FLIP_NONE);
            break;
        }

        // Update Screen
        SDL_RenderPresent(renderer);
    }
    
}

void Game::RenderStartmenu(SDL_Renderer* renderer) {

    //background image
    image = IMG_Load("bg.jpg");
    texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    
   

    TTF_Font* font = TTF_OpenFont("Bypass Regular 400.ttf", 30);
    SDL_Color color = { 255,255,255 };

    // draw text "SNAKE"
    SDL_Surface* surface1 = TTF_RenderText_Solid(font, "SNAKE", color);
    SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer, surface1);
    SDL_Rect rect = { 3 * SCREEN_WIDTH / 10, 3 * SCREEN_HEIGHT / 10, 256, 64 };
    SDL_RenderCopy(renderer, texture1, NULL, &rect);
    
    if(MouseMotion) color = { 255,0,0 };
    

    //draw text "Play"
    font = TTF_OpenFont("Bypass Regular 400.ttf", 22);
    surface1 = TTF_RenderText_Solid(font, "PLAY", color);
    texture1 = SDL_CreateTextureFromSurface(renderer, surface1);
    rect = { 4 * SCREEN_WIDTH / 10, 5 * SCREEN_HEIGHT / 10, 128, 64 };
    SDL_RenderCopy(renderer, texture1, NULL, &rect);

    SDL_RenderPresent(renderer);
   
    //destroy
    SDL_DestroyTexture(texture1);
    SDL_FreeSurface(surface1);
}
void Game::RenderMenu(SDL_Renderer* renderer) {

    TTF_Font* font = TTF_OpenFont("font.ttf", 23);
    SDL_Color color = { 0,0,0 };

    //draw text "(Space) Continue"
    SDL_Surface* surface = TTF_RenderText_Blended(font, "(Space) Continue", color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = { 3 * SCREEN_WIDTH / 10,2 * SCREEN_HEIGHT / 10, 300, 64 };
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    
    //draw text "Exit"
    surface = TTF_RenderText_Blended(font, "(Esc) Exit", color);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    rect = { 4 * SCREEN_WIDTH / 10, 4 * SCREEN_HEIGHT / 10, 150, 64 };
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    SDL_RenderPresent(renderer);

    //destroy
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void Game::RenderGameover(SDL_Renderer* renderer) {

    image = IMG_Load("gameover.jpg");
    texture = SDL_CreateTextureFromSurface(renderer, image);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

}

void Game::PlayWAV(std::string link)
{

    chunk = Mix_LoadWAV(link.c_str());
    Mix_PlayChannel(-1, chunk, 0);
}
void Game::PlayMusic(std::string link) {

    nhac = Mix_LoadMUS(link.c_str());
    Mix_PlayMusic(nhac, -1);
}

void Game::Close()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(texture);
    TTF_Quit();
    Mix_CloseAudio();
    SDL_Quit();
}
