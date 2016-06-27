#include "Util.h"

GRect interpolate_rect(GRect from, GRect to, AnimationProgress progress) {
    return GRect(from.origin.x + (to.origin.x - from.origin.x) * progress / ANIMATION_NORMALIZED_MAX,
                 from.origin.y + (to.origin.y - from.origin.y) * progress / ANIMATION_NORMALIZED_MAX,
                 from.size.w + (to.size.w - from.size.w) * progress / ANIMATION_NORMALIZED_MAX,
                 from.size.h + (to.size.h - from.size.h) *progress / ANIMATION_NORMALIZED_MAX);
}

int strlen_utf8(char *s) {
    int i = 0, j = 0;
    while (s[i]) {
        if ((s[i] & 0xc0) != 0x80) j++;
        i++;
    }
    return j;
}

void custom_itoa(int num, char *buf) {
    int tmp_num = num, length = 0;
    if(num == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    while(tmp_num > 0) {
        tmp_num /= 10;
        length++;
    }
    APP_LOG(APP_LOG_LEVEL_INFO, "itoa: length: %d", length);
    tmp_num = num;
    for(int i = length - 1; i >= 0; i--) {
        buf[i] = '0' + (tmp_num % 10);
        tmp_num/=10;
    }
    buf[length]='\0';
}

void minutes_get_right(int num, char *buf) {
    int last_digit = num % 10;
    if(num >= 11 && num <= 19) {
        strcpy(buf, "минут");
    } else {
        if (last_digit >= 2 && last_digit <= 4) {
            strcpy(buf, "минуты");
        } else if((last_digit >= 5 && last_digit <= 9) || last_digit == 0) {
            strcpy(buf, "минут");
        } else if(last_digit == 1) {
            strcpy(buf, "минута");
        }
    }
}