
#include "Vector3.hpp"

using namespace Physics;

Vector3::Vector3(float x, float y, float z) : 
    x(x), y(y), z(z){}

Vector3::operator std::string() const{
    std::string str;
    str += "[";
    str += std::to_string(this->x);
    str += ", ";
    str += std::to_string(this->y);
    str += ", ";
    str += std::to_string(this->z);
    str += "]";
    return str;
}

Vector3::operator glm::vec3() const{
    return glm::vec3(this->x, this->y, this->z);
}

void Vector3::operator += (const Vector3 vec){
    this->x += vec.x;
    this->y += vec.y;
    this->z += vec.z;
}

void Vector3::operator -= (const Vector3 vec){
    this->x -= vec.x;
    this->y -= vec.y;
    this->z -= vec.z;
}

Vector3 Vector3::operator + (const Vector3 vec){
    return Vector3(
        this->x + vec.x,
        this->y + vec.y,
        this->z + vec.z
    );
}

Vector3 Vector3::operator - (const Vector3 vec){
    return Vector3(
        this->x - vec.x,
        this->y - vec.y,
        this->z - vec.z
    );
}

Vector3 Vector3::operator *= (const Vector3 vec){
    return Vector3(
        this->x *= vec.x,
        this->y *= vec.y,
        this->z *= vec.z
    );
}

Vector3 Vector3::operator * (const Vector3 vec){
    return Vector3(
        this->x * vec.x,
        this->y * vec.y,
        this->z * vec.z
    );
}

float Vector3::dot(Vector3 vec) {
    return 
        this->x * vec.x +
        this->y * vec.y +
        this->z * vec.z;
}

float Vector3::scalarProduct(Vector3 vec){
    return this->dot(vec);
}

Vector3 Vector3::cross(Vector3 vec){
    return Vector3(
        this->y * vec.z - vec.y * this->z, 
        this->z * vec.x - vec.z * this->x, 
        this->x * vec.y - vec.x * this->y 
    );
}

Vector3 Vector3::vectorProduct(Vector3 vec){
    return this->cross(vec);
}

void Vector3::operator *= (const float scalar){
    this->x *= scalar;
    this->y *= scalar;
    this->z *= scalar;
}

void Vector3::operator /= (const float scalar){
    this->x /= scalar;
    this->y /= scalar;
    this->z /= scalar;
}

Vector3 Vector3::operator * (const float scalar){
    return Vector3(
        this->x * scalar,
        this->y * scalar,
        this->z * scalar
    );
}

Vector3 Vector3::operator / (const float scalar){
    return Vector3(
        this->x / scalar,
        this->y / scalar,
        this->z / scalar
    );
}

float Vector3::magnitudeSqrd() {
    return (
        this->x * this->x +
        this->y * this->y +
        this->z * this->z 
        );
}

float Vector3::magnitude() {
    return std::sqrt(this->magnitudeSqrd());
}

Vector3 Vector3::normalize() {
    return *this / this->magnitude(); 
}

Vector3 Vector3::direction() {
    return this->normalize();
}