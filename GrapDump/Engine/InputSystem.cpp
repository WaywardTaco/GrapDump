
#include "InputSystem.hpp"

using namespace Engine;

InputSystem::InputSystem() :
    key_1(false),
    key_2(false),
    key_W(false),
    key_A(false),
    key_S(false),
    key_D(false),
    key_Space(false)
{}

InputSystem* InputSystem::INSTANCE = nullptr;
InputSystem::InputSystem(const InputSystem&){}
InputSystem* InputSystem::Instance(){
    if(INSTANCE == nullptr)
        INSTANCE = new InputSystem();

    return INSTANCE;
}