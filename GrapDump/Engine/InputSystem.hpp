
#pragma once

namespace Engine {
    class InputSystem {
        public:
            bool
                key_1,
                key_2,
                key_W,
                key_A,
                key_S,
                key_D,
                key_Space;

        public:
            static InputSystem* INSTANCE;
            static InputSystem* Instance();

        private:
            InputSystem();
            InputSystem(const InputSystem&);
            InputSystem& operator = (const InputSystem&);
    };
}