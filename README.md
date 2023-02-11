# Portfolio-Work

// SFML Compile command

alias com_sf="g++ main.cpp -I/opt/homebrew/Cellar/sfml/2.5.1_2/include -L/opt/homebrew/Cellar/sfml/2.5.1_2/lib -lsfml-graphics -lsfml-window -lsfml-system -std=c++11 -arch arm64 -o app && ./app"

// Raylib Compile command

alias com_ray="g++ main.cpp -I/opt/homebrew/Cellar/raylib/4.2.0/include -L/opt/homebrew/Cellar/raylib/4.2.0/lib -lraylib -std=c++11 -arch arm64 -o app && ./app"

// Pong Compile command

alias com_pong="g++ main.cpp ball.cpp collisions.cpp game.cpp paddle.cpp -I/opt/homebrew/Cellar/raylib/4.2.0/include -L/opt/homebrew/Cellar/raylib/4.2.0/lib -lraylib -std=c++11 -arch arm64 -o app && ./app"

Known Issues:

- Very randomly, a ball may ignore a paddle?

To Add:

- Add a pause state
- Add sounds
- Add some graphics for scoring
- add a win / lose state
- If a paddle is out, make their goal a wall
- Deflector mechanic?
