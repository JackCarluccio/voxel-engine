#pragma once

#include <glm/glm.hpp>

namespace Util {

    struct IVec2Hash {
        std::size_t operator()(const glm::ivec2& vec) const {
            return std::hash<int>()(vec.x) ^ std::hash<int>()(vec.y);
        }
    };
    

}
