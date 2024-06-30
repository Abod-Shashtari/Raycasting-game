#include "Vector2D.hpp"

Vector2D::Vector2D(float x,float y){
    vec.x=x;
    vec.y=y;
}
float Vector2D::getPosX(){
    return vec.x;
}
float Vector2D::getPosY(){
    return vec.y;
}
void Vector2D::setPosX(float x){
    vec.x=x;
}
void Vector2D::setPosY(float y){
    vec.y=y;
}
