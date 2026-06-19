#include "Globals.h"

namespace Globals {

    constexpr unsigned int WIDTH{ 1080 };
    constexpr unsigned int HEIGHT{ 1980 };

    constexpr float TEXTURE_SIZE{ 1.0f / 4.0f };

    constexpr int CHUNK_WIDTH{ 16 };
    constexpr int CHUNK_HEIGHT{ 256 };

    constexpr int RENDER_RADIOUS{ 10 };
	constexpr int GENERATION_RADIOUS{ RENDER_RADIOUS + 3 };

    constexpr int GRASS_LEVEL{ 128 };
    constexpr int DIRT_DEPTH{ 138 };
    constexpr int STONE_DEPTH{ 254 };

    constexpr float INTERACTION_DISTANCE{ 5.0 };

    constexpr int REGION_BLOCK_SIZE{ 4 * 1024 }; 

	constexpr float TICK_RATE{ 1.0f / 20.0f };
}
