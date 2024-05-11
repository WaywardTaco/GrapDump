
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include <cmath>

namespace Physics {
    class Vector3 {
        public:
            float x, y, z;

        public:
            Vector3(float x, float y, float z);
            
            // CONVERSION
            explicit operator std::string() const;
            explicit operator glm::vec3() const;

            // VECTOR ADD & SUBTRACT
            void operator += (const Vector3 vec);
            void operator -= (const Vector3 vec);
            Vector3 operator + (const Vector3 vec);
            Vector3 operator - (const Vector3 vec);

            // COMPONENT PRODUCT
            Vector3 operator *= (const Vector3 vec);
            Vector3 operator * (const Vector3 vec);

            // SCALAR/DOT PRODUCT
            float dot(Vector3 vec);
            float scalarProduct(Vector3 vec);

            // VECTOR/CROSS PRODUCT
            Vector3 cross(Vector3 vec);
            Vector3 vectorProduct(Vector3 vec);

            // SCALAR MULT & DIV
            void operator *= (const float scalar);
            void operator /= (const float scalar);
            Vector3 operator * (const float scalar);
            Vector3 operator / (const float scalar);

            // MAGNITUDE
            float magnitudeSqrd();
            float magnitude();

            // DIRECTION
            Vector3 normalize();
            Vector3 direction();
    };
}