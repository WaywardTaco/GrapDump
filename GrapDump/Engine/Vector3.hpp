
#pragma once

#include <string>
#include <cmath>
#include <glm/glm.hpp>

namespace Engine {
    class Vector3 {
        public:
            /* COMPONENTS */
            double x, y, z;

        public:
            /* CONSTRUCTOR */
            Vector3(double x, double y, double z);
            
            /* CONVERSION */
            explicit operator std::string() const;
            explicit operator glm::vec3() const;

            /* COMPARISON */
            bool operator == (const Vector3 vec);

            /* VECTOR ADDITION & SUBTRACTION */
            void operator += (const Vector3 vec);
            void operator -= (const Vector3 vec);
            Vector3 operator + (const Vector3 vec);
            Vector3 operator - (const Vector3 vec);

            /* COMPONENT PRODUCT */
            Vector3 operator *= (const Vector3 vec);
            Vector3 operator * (const Vector3 vec);

            /* SCALAR/DOT PRODUCT */
            double dot(Vector3 vec);
            double scalarProduct(Vector3 vec);

            /* VECTOR/CROSS PRODUCT */ 
            Vector3 cross(Vector3 vec);
            Vector3 vectorProduct(Vector3 vec);

            /* SCALAR MULTIPLICATION & DIVISION */ 
            void operator *= (const double scalar);
            void operator /= (const double scalar);
            Vector3 operator * (const double scalar);
            Vector3 operator / (const double scalar);

            /* MAGNITUDE */ 
            double magnitudeSqrd();
            double magnitude();

            /* DIRECTION/NORMALIZE */
            Vector3 normalize();
            Vector3 direction();

            /* DISTANCE OPERATION */ 
            double distanceSqrdFrom(Vector3 vec);
            double distanceFrom(Vector3 vec);
    };
}