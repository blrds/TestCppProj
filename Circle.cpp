#include "Circle.h"

/// @brief Creates a circle with a random radius between 0.1 and 100
Circle::Circle(){
    thread_local static std::mt19937 gen(std::random_device{}());\
    using dist_type = typename std::conditional
    <
        std::is_integral<double>::value
        , std::uniform_int_distribution<double>
        , std::uniform_real_distribution<double>
    >::type;
    thread_local static dist_type dist;
    r=dist(gen, typename dist_type::param_type{0.1, 100});
}

/// @brief Creates a circle with the given radius
/// @param R Radius of the created circle
Circle::Circle(double R){
    r=R;
}

/// @return Perimetr of the circle
 double Circle::P(){
    return 2*r*M_PI;
}

/// @return Area of the circle
inline double Circle::S(){
    return M_PI*r*r;
}

/// @return Radius of the circle
double Circle::R(){
    return r;
}

