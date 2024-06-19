
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

#include <cmath>

namespace Engine {
    class Vector3 {
        public:
            double x, y, z;

        public:
            Vector3(double x, double y, double z);
            
            // CONVERSION
            explicit operator std::string() const;
            explicit operator glm::vec3() const;

            // COMPARISON
            bool operator == (const Vector3 vec);

            // VECTOR ADD & SUBTRACT
            void operator += (const Vector3 vec);
            void operator -= (const Vector3 vec);
            Vector3 operator + (const Vector3 vec);
            Vector3 operator - (const Vector3 vec);

            // COMPONENT PRODUCT
            Vector3 operator *= (const Vector3 vec);
            Vector3 operator * (const Vector3 vec);

            // SCALAR/DOT PRODUCT
            double dot(Vector3 vec);
            double scalarProduct(Vector3 vec);

            // VECTOR/CROSS PRODUCT
            Vector3 cross(Vector3 vec);
            Vector3 vectorProduct(Vector3 vec);

            // SCALAR MULT & DIV
            void operator *= (const double scalar);
            void operator /= (const double scalar);
            Vector3 operator * (const double scalar);
            Vector3 operator / (const double scalar);

            // MAGNITUDE
            double magnitudeSqrd();
            double magnitude();

            // DIRECTION
            Vector3 normalize();
            Vector3 direction();

            // DISTANCE
            double distanceSqrdFrom(Vector3 vec);
            double distanceFrom(Vector3 vec);
    };
}