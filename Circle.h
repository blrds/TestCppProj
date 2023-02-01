#define _USE_MATH_DEFINES
#include <random>
#include <cmath>

class Circle{
    private:
        double r;
        /// @brief not allowed 
        Circle(const Circle&)=delete;
        /// @brief not allowed 
        void operator=(const Circle&)=delete;
    public:
        Circle();
        Circle(double R);
        double P();
        double S();
        double R();
};