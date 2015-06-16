
#include "conv.h"

const char* tab_default_charset[MAX_TAB_CHARSET] = {
     STR_DEFAULT_ALPHABET,
     STR_ASCII_ANSI_X34,
     STR_OCTET_UNSPECIFIED,
     STR_ISO_8859_1,
     STR_OCTET_UNSPECIFIED,
     STR_JIS_X_0208_1990,
     STR_ISO_8859_5,
     STR_ISO_8859_8,
     STR_UCS_2,
     STR_PICTOGRAM_ENCODING,
     STR_ISO_2022_JP,
     STR_RESERVED,
     STR_RESERVED,
     STR_JIS_X_0212_1990,
     STR_KS_C_5601,
     STR_RESERVED
};

int conv_char_codec_str(char *in, size_t s_in, char *codec_in, char **out, size_t s_out, char *codec_out){
    INFO(LOG_SCREEN, "Iconv : %s to %s", codec_in ? codec_in : "", codec_out ? codec_out : "")
    if(in && s_in > 0 && out && codec_in && codec_out && strcmp(codec_in, codec_out) != 0){
    INFO(LOG_SCREEN, "run : %s to %s", codec_in, codec_out)
        if(*out == NULL){
            s_out = 4096;
            *out = (char*)calloc(s_out+1, sizeof(char));
        }
        iconv_t icv = iconv_open(codec_out, codec_in);
        if((int)icv == -1){
            switch(errno){
                case EINVAL:
                    ERROR(LOG_SCREEN, "Conversion is not supported[%d]-> %s", errno, strerror(errno));
                    break;
                default:
                    ERROR(LOG_SCREEN, "Initialization failure[%d]-> %s", errno, strerror(errno));
            }
            free(*out);
            *out = NULL;
            return (int) -1;
        }
        char *converted = *out;
        if((iconv_t)iconv(icv, &in, &s_in, &converted, &s_out) == (iconv_t)-1){
            switch(errno){
                case E2BIG :
                    ERROR(LOG_SCREEN, "The buffer is too short[%d]-> %s", errno, strerror(errno));
                    break;
                default:
                    ERROR(LOG_SCREEN, "iconv failed[%d]-> %s", errno, strerror(errno));
                    free(*out);
                    *out = NULL;
            }
            if(iconv_close(icv) != 0){
                ERROR(LOG_SCREEN, "iconv_clode failed[%d]-> %s", errno, strerror(errno));
            }
            return (int) -1;
        }
        if(iconv_close(icv) != 0){
            ERROR(LOG_SCREEN, "iconv_clode failed[%d]-> %s", errno, strerror(errno));
            free(*out);
            *out = NULL;
            return (int) -1;
        }
        return (int) 0;
    }
    return (int) -1;
}

