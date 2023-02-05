# Portfolio-Work

// SFML Compile command

alias com_sf="g++ main.cpp -I/opt/homebrew/Cellar/sfml/2.5.1_2/include -L/opt/homebrew/Cellar/sfml/2.5.1_2/lib -lsfml-graphics -lsfml-window -lsfml-system -std=c++11 -arch arm64 -o app && ./app"

// Raylib Compile command

alias com_ray="g++ main.cpp -I/opt/homebrew/Cellar/raylib/4.2.0/include -L/opt/homebrew/Cellar/raylib/4.2.0/lib -lraylib -std=c++11 -arch arm64 -o app && ./app"

// Pong

Known Issues:

- If the ball speed is too great it will pass through a paddle
- If the ball deflects from a paddle corner it may go the wrong direction

To Add:

- More balls
  -- Paddle follow logic
- Deflector mechanic?
