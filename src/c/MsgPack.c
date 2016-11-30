#include "MsgPack.h"

static int offset = 0;

static int parseIntLen(uint8_t* data, int length) {
    int ret = 0;
    for(int i = 0; i < length; i++) {
        ret += data[offset + i] << ((length - i - 1) * 8);
    }
    offset += length;
    return ret;
}

static char* parseString(uint8_t *data) {
    int length = 0;
    int intLen = 0;
    if(data[offset] == 0xd9) {
        intLen = 1;
    } else if(data[offset] == 0xda) {
        intLen = 2;
    } else if(data[offset] == 0xdb) {
        intLen = 4;
    } else {
        intLen = 0;
    }
    if(intLen == 0) {
        length = data[offset] & 0x1f;
        offset++;
    } else {
        offset++;
        length = parseIntLen(data, intLen);
    }

    char* ret = malloc(sizeof(char) * (length + 1));
    for(int i = 0; i < length; i++) {
        ret[i] = (char)(data[offset + i]);
    }
    ret[length] = '\0';
    offset += length;
    return ret;
}

static int parseInt(uint8_t *data) {
    int intLen = 0;
    if(data[offset] == 0xd0) {
        intLen = 1;
    } else if(data[offset] == 0xd1) {
        intLen = 2;
    } else if(data[offset] == 0xd2) {
        intLen = 4;
    } else if((data[offset] & 0x80) == 0x00) {
        offset++;
        return data[offset - 1];
    } else if((data[offset] & 0xE0) == 0xE0) {
        offset++;
        return -(data[offset - 1] & 0x1F);
    }
    offset++;
    return parseIntLen(data, intLen);
}

static int parseMapSize(uint8_t* data) {
    int ret = 0;
    int intLen = 0;
    if(data[offset] == 0xde) {
        intLen = 2;
    } else if(data[offset] == 0xdf) {
        intLen = 4;
    } else {
        intLen = 0;
    }
    if(intLen == 0) {
        ret = data[offset] & 0x0f;
        offset++;
    } else {
        offset++;
        ret = parseIntLen(data, intLen);
    }
    return ret;
}

static int parseArraySize(uint8_t* data) {
    int ret = 0;
    int intLen = 0;
    if(data[offset] == 0xdc) {
        intLen = 2;
    } else if(data[offset] == 0xdd) {
        intLen = 4;
    } else {
        intLen = 0;
    }
    if(intLen == 0) {
        ret = data[offset] & 0x0f;
        offset++;
    } else {
        offset++;
        ret = parseIntLen(data, intLen);
    }
    return ret;
}

static Line* parseLines(uint8_t* data, int* linesLen) {
    int len = parseArraySize(data);
    Line *ret = malloc(sizeof(Line) * len);
    *linesLen = len;
    for(int i = 0; i < len; i++) {
        int lineLen = parseArraySize(data);
        for(int j = 0; j < lineLen; j++) {
            if(j == 0) {
                ret[i].name = parseInt(data);
            } else if(j == 1) {
                int color = parseInt(data);
                ret[i].color = GColorFromHEX(color);
            } else if(j == 2) {
                ret[i].startFrom = parseInt(data);
            } else if(j == 3) {
                ret[i].stations = parseInt(data);
            }
        }
    }
    return ret;
}

static Link* parseLinks(uint8_t * data, int* linksLen) {
    int len = parseArraySize(data);
    Link* ret = malloc(sizeof(Link) * len);
    *linksLen = len;
    for(int i = 0; i < len; i++) {
        int len2 = parseArraySize(data);
        ret[i].l = malloc(sizeof(Link2) * len2);
        ret[i].len = len2;
        for(int j = 0; j < len2; j++) {
            int len3 = parseArraySize(data);
            for(int k = 0; k < len3; k++) {
                if(k == 0) {
                    ret[i].l[j].to = parseInt(data);
                } else if(k == 1) {
                    ret[i].l[j].weight = parseInt(data);
                }
            }
        }
    }
    return ret;
}

static int* parseExit(uint8_t *data) {
    if(data[offset] == 0xc0) {
        offset++;
        return NULL;
    }
    int len = parseArraySize(data);
    int *ret = malloc(sizeof(int) * len);
    for(int i = 0; i < len; i++) {
        ret[i] = parseInt(data);
    }
    return ret;
}

static Transfer* parseTransfer(uint8_t *data, int *transferLen) {
    if(data[offset] == 0xc0) {
        offset++;
        return NULL;
    }
    int len = parseArraySize(data);
    Transfer* ret = malloc(sizeof(Transfer) * len);
    *transferLen = len;
    for(int i = 0; i < len; i++) {
        int len2 = parseArraySize(data);
        for(int j = 0; j < len2; j++) {
            if (j == 0) {
                ret[i].prevStation = parseInt(data);
            } else if (j == 1) {
                ret[i].toStation = parseInt(data);
            } else if (j == 2) {
                ret[i].nextStation = parseInt(data);
            } else if (j == 3) {
                ret[i].pos = parseExit(data);
            }
        }
    }
    return ret;
}

static BoardInfo* parseBoardInfo(uint8_t* data) {
    if(data[offset] == 0xc0) {
        offset++;
        return NULL;
    }
    int len = parseArraySize(data);
    BoardInfo* ret = malloc(sizeof(BoardInfo));
    for(int i = 0; i < len; i++) {
        if(i == 0) {
            ret->exit = parseExit(data);
        } else if(i == 1) {
            ret->transfer = parseTransfer(data, &(ret->transferLen));
        }
    }
    return ret;
}

static Station* parseStations(uint8_t *data, int* stationsLen) {
    int len = parseArraySize(data);

    Station* ret = malloc(sizeof(Station) * len);
    *stationsLen = len;
    for(int i = 0; i < len; i++) {
        int len2 = parseArraySize(data);
        for(int j = 0; j < len2; j++) {
            if(j == 0) {
                int id = parseInt(data);
                ret[i].id = id;
            } else if(j == 1) {
                ret[i].line = parseInt(data);
            } else if(j == 2) {
                ret[i].name = parseInt(data);
            } else if(j == 3) {
                ret[i].boardInfo = parseBoardInfo(data);
            }
        }
    }
    return ret;
}

static Scheme* parseScheme(uint8_t* data) {
    Scheme *ret = malloc(sizeof(Scheme));
    int len = parseArraySize(data);
    for(int i = 0; i < len; i++) {
        if(i == 0) {
            ret->lines = parseLines(data, &(ret->linesLen));
        } else if(i == 1) {
            ret->stations = parseStations(data, &(ret->stationsLen));
        } else if(i == 2) {
            ret->links = parseLinks(data, &(ret->linksLen));
        }
    }
    return ret;
}

static int* parseId2ind(uint8_t *data) {
    int len = parseArraySize(data);
    int* ret = malloc(sizeof(int) * len);
    for(int i = 0; i < len; i++) {
        ret[i] = parseInt(data);
    }
    return ret;
}

char** parseNames(uint8_t *data, int* namesLen) {
    offset = 0;
    *namesLen = parseArraySize(data);
    char** ret = malloc(sizeof(char*) * (*namesLen));
    for(int i = 0; i < *namesLen; i++) {
        ret[i] = parseString(data);
    }
    return ret;
}

SchemePack* parseSchemePack(uint8_t *data) {
    offset = 0;
    SchemePack* ret = malloc(sizeof(SchemePack));
    int schemeLen = parseArraySize(data);
    for(int i = 0; i < schemeLen; i++) {
        if(i == 0) {
            ret->id = parseInt(data);
        } else if(i == 1) {
            ret->ver = parseInt(data);
        } else if(i == 2) {
            ret->def = parseScheme(data);
        } else if(i == 3) {
            ret->id2ind = parseId2ind(data);
        }
    }
    return ret;
}

char** parseLocale(uint8_t *data) {
    offset = 0;
    int len = parseArraySize(data);
    char** ret = malloc(sizeof(char*) * len);
    for(int i = 0; i < len; i++) {
        ret[i] = parseString(data);
    }
    return ret;
}