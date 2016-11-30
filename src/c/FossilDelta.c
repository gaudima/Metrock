#include "FossilDelta.h"
static size_t offset = 0;

static uint32_t readB64Int(uint8_t *delta) {
    uint8_t b64RangeUpper[2] = {65, 90};
    uint8_t b64RangeLower[2] = {97, 122};
    uint8_t b64RangeDigit[2] = {48, 57};

    uint32_t ret = 0;
    while(true) {
        uint8_t c = delta[offset];
        if(b64RangeUpper[0] <= c && c <= b64RangeUpper[1]) {
            c -= 55;
        } else if(b64RangeLower[0] <= c && c <= b64RangeLower[1]) {
            c -= 60;
        } else if(b64RangeDigit[0] <= c && c <= b64RangeDigit[1]) {
            c -= 48;
        } else if(c == '_') {
            c = 36;
        } else if(c == '~') {
            c = 63;
        } else {
            break;
        }
        ret = (ret << 6) + c;
        offset++;
    }
    APP_LOG(APP_LOG_LEVEL_DEBUG, "b64int: %d", ret);
    return ret;
}

uint8_t* applyDelta(uint8_t* delta, size_t length, void (*readFn)(uint8_t *, size_t, size_t)) {
    uint32_t len, offs, size = readB64Int(delta);
    size_t retOffset = 0;
    if(delta[offset] != '\n') {
        APP_LOG(APP_LOG_LEVEL_ERROR, "no \\n after size");
        return NULL;
    }
    offset++;
    uint8_t* ret = malloc(sizeof(uint8_t) * size);
    while(offset < length) {
        len = readB64Int(delta);
        switch(delta[offset]) {
            case '@': {
                offset++;
                offs = readB64Int(delta);
                if (delta[offset] != ',') {
                    APP_LOG(APP_LOG_LEVEL_ERROR, "no ',' after offset");
                    free(ret);
                    return NULL;
                }
                offset++;
                readFn(ret + retOffset, offs, len);
                retOffset += len;
                break;
            }
            case ':': {
                offset++;
                memcpy(ret + retOffset, delta + offset, len);
                retOffset += len;
                offset += len;
                break;
            }
            case ';': {
                uint32_t checkSum = 0;
                uint32_t csLen = size + (4 - size % 4);
                for (uint32_t i = 0; i < csLen; i++) {
                    int s = 0;
                    for (uint32_t j = 0; j < 4; j++) {
                        if (i * 4 + j < size) {
                            s = (s << 8) + ret[i * 4 + j];
                        } else {
                            s = (s << 8);
                        }
                    }
                    checkSum += s;
                }
                if(checkSum != len) {
                    APP_LOG(APP_LOG_LEVEL_ERROR, "wrong checksum");
                    free(ret);
                    return NULL;
                }
                offset++;
                break;
            }
        }
    }
    return ret;
}

