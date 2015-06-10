
#include "conv.h"

const char* tab_codec[MAX_TAB_CODEC] = {
                               0,
                               STR_ICONV_ASCII_ANSI_X34,
                               0,
                               STR_ICONV_ISO_8859_1,
                               0,
                               0,
                               STR_ICONV_ISO_8859_5,
                               STR_ICONV_ISO_8859_8,
                               STR_ICONV_UCS_2,
                               0,
                               STR_ICONV_ISO_2022_JP,
                               0};

int conv_char_codec_str(char *in, size_t s_in, char *codec_in, char **out, size_t s_out, char *codec_out){
    if(in && s_in > 0 && out && s_out > 0 && codec_in && codec_out && strcmp(codec_in, codec_out) != 0){
        if(*out == NULL){
            s_out = 4096;
            *out = (char*)calloc(s_out+1, sizeof(char));
        }
        iconv_t icv = iconv_open(codec_out, codec_in);
        if((int)icv == -1){
            if (errno == EINVAL) {
                ERROR(LOG_SCREEN, "Conversion is not supported[%d]-> %s", errno, strerror(errno));
            }else{
                ERROR(LOG_SCREEN, "Initialization failure[%d]-> %s", errno, strerror(errno));
            }
            return (int) -1;
        }
        char *converted = *out;
        if((iconv_t)iconv(icv, &in, &s_in, &converted, &s_out) == (iconv_t)-1){
            ERROR(LOG_SCREEN, "iconv failed[%d]-> %s", errno, strerror(errno));
            if(iconv_close(icv) != 0){
                ERROR(LOG_SCREEN, "iconv_clode failed[%d]-> %s", errno, strerror(errno));
            }
            return (int) -1;
        }
        if(iconv_close(icv) != 0){
            ERROR(LOG_SCREEN, "iconv_clode failed[%d]-> %s", errno, strerror(errno));
            return (int) -1;
        }
        return (int) 0;
    }
    return (int) -1;
}

int conv_char_codec_enum(char *in, size_t s_in, unsigned short codec_in, char **out, size_t s_out, unsigned short codec_out){
    if(codec_in < MAX_TAB_CODEC && codec_out < MAX_TAB_CODEC && tab_codec[codec_in] != 0 && tab_codec[codec_out] != 0){
        return (int) conv_char_codec_str(in,s_in,tab_codec[codec_in],out,s_out,tab_codec[codec_out]);
    }
    return (int) -1;
}

