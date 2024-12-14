#include <iostream>
#include <cstdint> // For uintptr_t type

namespace offsets {
    // Memory addresses for key game entities
    namespace addresses {
        constexpr uintptr_t localPlayer = 0xDE8964;    // Local player base address
        constexpr uintptr_t entityList = 0x4DFDE84;    // Entity list base address  
        constexpr uintptr_t viewMatrix = 0x4DEECB4;    // View matrix base address
    }

    // Entity property offsets
    namespace entity {
        constexpr uintptr_t position = 0x138;          // Vector position offset
        constexpr uintptr_t health = 0x100;            // Health value offset
        constexpr uintptr_t teamNum = 0xF4;            // Team number offset
        constexpr uintptr_t aimPunch = 0x303C;         // Aim punch angle offset
    }
}
