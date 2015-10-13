#include "Util.h"

GRect interpolate_rect(GRect from, GRect to, AnimationProgress progress) {
    return GRect(from.origin.x + (to.origin.x - from.origin.x) * progress / ANIMATION_NORMALIZED_MAX,
                 from.origin.y + (to.origin.y - from.origin.y) * progress / ANIMATION_NORMALIZED_MAX,
                 from.size.w + (to.size.w - from.size.w) * progress / ANIMATION_NORMALIZED_MAX,
                 from.size.h + (to.size.h - from.size.h) *progress / ANIMATION_NORMALIZED_MAX);
}