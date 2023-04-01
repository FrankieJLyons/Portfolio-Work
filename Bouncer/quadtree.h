#include <vector>
#include <algorithm>
using namespace std;

#include <raylib.h>

#include "ball.h"

const int MAX_OBJECTS = 8;
const int MAX_LEVELS = 4;

class Quadtree {
public:
    Quadtree() {}
    Quadtree(int level, Rectangle bounds);
    ~Quadtree();

    void Clear();
    void Insert(Ball * ball);
    void Query(vector<Ball*> & nearbyBalls, float x1, float y1, float x2, float y2) const;

    private:
    int level_;

    vector<Ball *> objects_;
    Rectangle bounds_;
    Quadtree *nodes_[4];

    void Split();
    bool Intersects(float x1, float y1, float x2, float y2) const;
    int GetIndex(const Ball* ball) const;
};