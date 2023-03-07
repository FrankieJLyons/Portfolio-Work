# Portfolio-Work

// SFML Compile command

alias com_sf="g++ main.cpp -I/opt/homebrew/Cellar/sfml/2.5.1_2/include -L/opt/homebrew/Cellar/sfml/2.5.1_2/lib -lsfml-graphics -lsfml-window -lsfml-system -std=c++11 -arch arm64 -o app && ./app"

// Raylib Compile command

alias com_ray="g++ main.cpp -I/opt/homebrew/Cellar/raylib/4.2.0/include -L/opt/homebrew/Cellar/raylib/4.2.0/lib -lraylib -std=c++11 -arch arm64 -o app && ./app"

// Pong Compile command

alias com_pong="g++ main.cpp ball.cpp collisions.cpp game.cpp paddle.cpp quadtree.cpp menu.cpp screenmanager.cpp -I/opt/homebrew/Cellar/raylib/4.2.0/include -L/opt/homebrew/Cellar/raylib/4.2.0/lib -lraylib -std=c++11 -arch arm64 -o app && ./app"

Known Issues:

- Very randomly, a ball may ignore a paddle? (Caused by time factor, may need tweaking)
- Balls may bounce off each other and go same direction

To Add:

- next level mechanic
- Menu
- Deflector mechanic?
- Magnet mechananic?
- Reflector mechanic?
- Quick spawner mechanic?
- Wind mechanic?
