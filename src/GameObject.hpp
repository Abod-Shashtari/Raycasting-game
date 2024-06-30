#include "Vector2D.hpp"
class GameObject{
    private:
        Vector2D* pos;
    public:
        GameObject(float x, float y);
        ~GameObject();
        void update();
        void render();
};
