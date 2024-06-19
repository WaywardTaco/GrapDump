
#include "Vector3.hpp"

using namespace Engine;

Vector3::Vector3(double x, double y, double z) : 
    x(x), y(y), z(z){}

Vector3::operator std::string() const{
    std::string str;
    str += "(";
    str += std::to_string(this->x);
    str += ", ";
    str += std::to_string(this->y);
    str += ", ";
    str += std::to_string(this->z);
    str += ")";
    return str;
}

Vector3::operator glm::vec3() const{
    return glm::vec3(this->x, this->y, this->z);
}

bool Vector3::operator == (const Vector3 vec){
    return 
        this->x == vec.x &&
        this->y == vec.y &&
        this->z == vec.z;
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

double Vector3::dot(Vector3 vec) {
    return 
        this->x * vec.x +
        this->y * vec.y +
        this->z * vec.z;
}

double Vector3::scalarProduct(Vector3 vec){
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

void Vector3::operator *= (const double scalar){
    this->x *= scalar;
    this->y *= scalar;
    this->z *= scalar;
}

void Vector3::operator /= (const double scalar){
    this->x /= scalar;
    this->y /= scalar;
    this->z /= scalar;
}

Vector3 Vector3::operator * (const double scalar){
    return Vector3(
        this->x * scalar,
        this->y * scalar,
        this->z * scalar
    );
}

Vector3 Vector3::operator / (const double scalar){
    return Vector3(
        this->x / scalar,
        this->y / scalar,
        this->z / scalar
    );
}

double Vector3::magnitudeSqrd() {
    return (
        this->x * this->x +
        this->y * this->y +
        this->z * this->z 
        );
}

double Vector3::magnitude() {
    return std::sqrt(this->magnitudeSqrd());
}

Vector3 Vector3::normalize() {
    return *this / this->magnitude(); 
}

Vector3 Vector3::direction() {
    return this->normalize();
}

double Vector3::distanceSqrdFrom(Vector3 vec) {
    return
        (*this - vec).magnitudeSqrd();
}

double Vector3::distanceFrom(Vector3 vec) {
    return
        (*this - vec).magnitude();
}