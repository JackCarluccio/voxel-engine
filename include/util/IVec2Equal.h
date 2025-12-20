#pragma once

#include <glm/glm.hpp>

namespace Util {

    struct IVec2Equal {
        bool operator()(const glm::ivec2& a, const glm::ivec2& b) const {
            return a.x == b.x && a.y == b.y;
        }
    };

}
