#include "quadtree.h"

Quadtree::Quadtree(int level, Rectangle bounds)
: level_(level), bounds_(bounds) {
    for (int i = 0; i < 4; ++i) {
        nodes_[i] = nullptr;
    }
}

Quadtree::~Quadtree() {
    for (int i = 0; i < 4; ++i) {
        if (nodes_[i] != nullptr) {
            delete nodes_[i];
        }
    }
}

void Quadtree::Clear() {
    objects_.clear();
    for (int i = 0; i < 4; ++i) {
        if (nodes_[i] != nullptr) {
            nodes_[i]->Clear();
            delete nodes_[i];
            nodes_[i] = nullptr;
        }
    }
}

void Quadtree::Insert(Ball * ball) {
    if (nodes_[0] != nullptr) {
    int index = 0;
    float midX = bounds_.x + bounds_.width / 2;
    float midY = bounds_.y + bounds_.height / 2;
    if (ball->position.x < midX && ball->position.y < midY) {
            index = 0;
        } else if (ball->position.x < midX) {
            index = 1;
        } else if (ball->position.y < midY) {
            index = 2;
        } else {
            index = 3;
        }

        nodes_[index]->Insert(ball);

        return;
    }

    objects_.push_back(ball);

    if (objects_.size() > MAX_OBJECTS && level_ < MAX_LEVELS) {
        if (nodes_[0] == nullptr) {
            Split();
        }

        int i = 0;
        while (i < objects_.size()) {
            int index = 0;
            float midX = bounds_.x + bounds_.width / 2;
            float midY = bounds_.y + bounds_.height / 2;

            if (objects_[i]->position.x < midX && objects_[i]->position.y < midY) {
                index = 0;
            } else if (objects_[i]->position.x < midX) {
                index = 1;
            } else if (objects_[i]->position.y < midY) {
                index = 2;
            } else {
                index = 3;
            }

            nodes_[index]->Insert(objects_[i]);
            objects_.erase(objects_.begin() + i);
        }
    }
}

void Quadtree::Query(vector<Ball*> &nearbyBalls, float x1, float y1, float x2, float y2) const {
    if (!Intersects(x1, y1, x2, y2)) {
        return;
    }

    for (const auto &ball : objects_) {
        if (ball->position.x >= x1 && ball->position.x <= x2 &&
            ball->position.y >= y1 && ball->position.y <= y2) {
            nearbyBalls.push_back(ball);
        }
    }

    if (nodes_[0] != nullptr) {
        for (const auto &node : nodes_) {
            node->Query(nearbyBalls, x1, y1, x2, y2);
        }
    }
}

bool Quadtree::Intersects(float x1, float y1, float x2, float y2) const {
    return !(x1 > bounds_.x + bounds_.width || x2 < bounds_.x ||
             y1 > bounds_.y + bounds_.height || y2 < bounds_.y);
}

void Quadtree::Split() {
    float subWidth = bounds_.width / 2;
    float subHeight = bounds_.height / 2;
    float x = bounds_.x;
    float y = bounds_.y;
    nodes_[0] = new Quadtree(level_ + 1, Rectangle{x, y, subWidth, subHeight});
    nodes_[1] = new Quadtree(level_ + 1, Rectangle{x + subWidth, y, subWidth, subHeight});
    nodes_[2] = new Quadtree(level_ + 1, Rectangle{x, y + subHeight, subWidth, subHeight});
    nodes_[3] = new Quadtree(level_ + 1, Rectangle{x + subWidth, y + subHeight, subWidth, subHeight});

    for (auto it = objects_.begin(); it != objects_.end(); ++it) {
        int index = GetIndex(*it);
        if (index != -1) {
            nodes_[index]->Insert(*it);
            it = objects_.erase(it);
            --it;
        }
    }
}

int Quadtree::GetIndex(const Ball* ball) const {
    int index = -1;
    float midX = bounds_.x + bounds_.width / 2;
    float midY = bounds_.y + bounds_.height / 2;

    bool topQuadrant = (ball->position.y < midY && ball->position.y + ball->radius < midY);
    bool bottomQuadrant = (ball->position.y > midY);

    if (ball->position.x < midX && ball->position.x + ball->radius < midX) {
        if (topQuadrant) {
            index = 0;
        } else if (bottomQuadrant) {
            index = 2;
        }
    } else if (ball->position.x > midX) {
        if (topQuadrant) {
            index = 1;
        } else if (bottomQuadrant) {
            index = 3;
        }
    }

    return index;
}