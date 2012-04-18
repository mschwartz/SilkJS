/** @ignore */
#include "SilkJS.h"

static const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

static inline bool is_base64 (unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

string Base64Encode (unsigned char const* bytes_to_encode, unsigned long in_len) {
    string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while ((i++ < 3))
            ret += '=';

    }
    return ret;
}

string Base64Decode (const char *encodedString) {
    string encoded_string(encodedString);
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    string ret;

    printf("indize %ld\n", in_len);

    while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_];
        in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;

        for (j = 0; j < 4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }
    return ret;
}

/**
 * Decode the base64 encoded string 'src' into the memory pointed to by
 * 'dest'. The dest buffer is <b>not</b> NUL terminated.
 * @param dest Pointer to memory for holding the decoded string.
 * Must be large enough to recieve the decoded string.
 * @param src A base64 encoded string.
 * @return TRUE (the length of the decoded string) if decode
 * succeeded otherwise FALSE.
 */

/**
 * Decode a base64 character
 */
static unsigned char decode (char c) {

    if (c >= 'A' && c <= 'Z') return (c - 'A');
    if (c >= 'a' && c <= 'z') return (c - 'a' + 26);
    if (c >= '0' && c <= '9') return (c - '0' + 52);
    if (c == '+') return 62;

    return 63;

}

/**
 * Return TRUE if 'c' is a valid base64 character, otherwise FALSE
 */
static int is_base64 (char c) {

    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
        (c >= '0' && c <= '9') || (c == '+') ||
        (c == '/') || (c == '=')) {

        return true;

    }

    return false;

}

int decode_base64 (unsigned char *dest, const char *src) {

    if (src && *src) {

        unsigned char *p = dest;
        int k, l = strlen(src) + 1;
        unsigned char *buf = (unsigned char *) calloc(sizeof (unsigned char), l);


        /* Ignore non base64 chars as per the POSIX standard */
        for (k = 0, l = 0; src[k]; k++) {

            if (is_base64(src[k])) {

                buf[l++] = src[k];

            }

        }

        for (k = 0; k < l; k += 4) {

            char c1 = 'A', c2 = 'A', c3 = 'A', c4 = 'A';
            unsigned char b1 = 0, b2 = 0, b3 = 0, b4 = 0;

            c1 = buf[k];

            if (k + 1 < l) {

                c2 = buf[k + 1];

            }

            if (k + 2 < l) {

                c3 = buf[k + 2];

            }

            if (k + 3 < l) {

                c4 = buf[k + 3];

            }

            b1 = decode(c1);
            b2 = decode(c2);
            b3 = decode(c3);
            b4 = decode(c4);

            *p++ = ((b1 << 2) | (b2 >> 4));

            if (c3 != '=') {

                *p++ = (((b2 & 0xf) << 4) | (b3 >> 2));

            }

            if (c4 != '=') {

                *p++ = (((b3 & 0x3) << 6) | b4);

            }

        }

        free(buf);

        return (p - dest);

    }

    return false;

}
