# Portfolio-Work

// SFML Compile command

alias com_sf="g++ main.cpp -I/opt/homebrew/Cellar/sfml/2.5.1_2/include -L/opt/homebrew/Cellar/sfml/2.5.1_2/lib -lsfml-graphics -lsfml-window -lsfml-system -std=c++11 -arch arm64 -o app && ./app"

// Raylib Compile command

alias com_ray="g++ main.cpp -I/opt/homebrew/Cellar/raylib/4.2.0/include -L/opt/homebrew/Cellar/raylib/4.2.0/lib -lraylib -std=c++11 -arch arm64 -o app && ./app"

// Pong

Known Issues:

- Very randomly, a ball may slowly ignore a paddle?

To Add:

- Convert maths to rmath where possible
- Make a game.h / game.cpp
- Convert .h files into proper .h / .cpp files
- Add a pause state
- Add sounds
- Add some graphics for scoring
- add a win / lose state
- If a paddle is out, make their goal a wall
- Deflector mechanic?
