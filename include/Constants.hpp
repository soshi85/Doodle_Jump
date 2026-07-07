#pragma once

namespace Constants {
    const int WINDOW_WIDTH= 400;
    const int WINDOW_HEIGHT= 600;

    const float GRAVITY= 1200.f;
    const float MOVEMENT_SPEED= 300.f;

    const float JUMP_VELOCITY= 600.f;
    const float SPRING_JUMP_VELOCITY= 1000.f;

    const float PLAYER_START_Y= 500.f;

    const float SCROLL_THRESHOLD_RATIO= 0.4f;

    const float PLATFORM_MIN_VERTICAL_GAP= 70.f;
    const float MOVING_PLATFORM_MIN_SPEED= 60.f;
    const float MOVING_PLATFORM_MAX_SPEED= 140.f;

    const float PROB_MOVING_PLATFORM=0.22f;
    const float PROB_BREAKABLE_PLATFORM= 0.18f;
    const float PROB_SPRING_ON_NORMAL= 0.18f;

    const float GENERATION_MARGIN_ABOVE_SCREEN= 400.f;
    const float REMOVAL_MARGIN_BELOW_SCREEN= 80.f;
}
