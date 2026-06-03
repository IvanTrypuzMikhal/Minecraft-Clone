#include "HUDSprites.h"



namespace HUDSprites {
	// Atlas
	constexpr float WIDTH{ 1024.0f }, HEIGHT{ 512.0f };

	// Crosshair
	constexpr float CROSSHAIR_U0{ 152.0f }, CROSSHAIR_V0{ 321.0f };
	constexpr float CROSSHAIR_U1{ 159.0f }, CROSSHAIR_V1{ 330.0f };

	constexpr float CROSSHAIR_SIZE_5{ 45.0f };
	constexpr float CROSSHAIR_SIZE_4{ 36.0f };

	
	// Hotbar
	constexpr float HOTBAR_U0{ 49.0f }, HOTBAR_V0{ 95.0f };
	constexpr float HOTBAR_U1{ 229.0f }, HOTBAR_V1{ 115.0f };

	constexpr float HOTBAR_HEIGHT{HOTBAR_V1 - HOTBAR_V0};
	constexpr float HOTBAR_WIDTH{ HOTBAR_U1 - HOTBAR_U0 };

	// XP Bar
	constexpr float XPBAR_U0{ 640.0f }, XPBAR_V0{ 397.0f };
	constexpr float XPBAR_U1{ 822.0f }, XPBAR_V1{ 402.0f };

	constexpr float XPBAR_HEIGHT{ XPBAR_V1 - XPBAR_V0 };
	constexpr float XPBAR_WIDTH{ XPBAR_U1 - XPBAR_U0 };


	// Hearts
	constexpr float HEART_U0{ 144.0f }, HEART_V0{ 3.0f };
	constexpr float HEART_U1{ 153.0f }, HEART_V1{ 12.0f };

	extern  const float HEART_HOLDER_U0{36.0f}, HEART_HOLDER_V0{3.0f};
	extern  const float HEART_HOLDER_U1{45.0f}, HEART_HOLDER_V1{12.0f};


	constexpr float HEART_HEIGHT{ HEART_V1 - HEART_V0 };
	constexpr float HEART_WIDTH{ HEART_U1 - HEART_U0 };


	// Food
	constexpr float FOOD_U0{ 224.0f }, FOOD_V0{ 322.0f };
	constexpr float FOOD_U1{ 230.0f }, FOOD_V1{ 329.0f };

	constexpr float FOOD_HOLDER_U0{ 205.0f }, FOOD_HOLDER_V0{ 321.0f };
	constexpr float FOOD_HOLDER_U1{ 214.0f }, FOOD_HOLDER_V1{ 330.0f };
	
	constexpr float FOOD_HEIGHT{ FOOD_V1 - FOOD_V0 };
	constexpr float FOOD_WIDTH{ FOOD_U1 - FOOD_U0 };

}

