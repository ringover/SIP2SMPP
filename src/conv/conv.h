
#ifndef CONV_H
#define CONV_H

#include "../log/log.h"

#include <iconv.h>
#include <error.h>
#include <errno.h>

#define STR_DEFAULT_ALPHABET   "SMSC Default Alphabet (SMPP 3.4) / MC Specific (SMPP 5.0)"
#define STR_ASCII_ANSI_X34     "IA5 (CCITT T.50)/ASCII (ANSI X3.4)"
#define STR_ISO_8859_1         "Latin 1 (ISO-8859-1)"
//#define STR_JIS_X_0208_1990    "JIS (X 0208-1990)"
#define STR_ISO_8859_5         "Cyrillic (ISO-8859-5)"
#define STR_ISO_8859_8         "Latin/Hebrew (ISO-8859-8)"
#define STR_UCS_2              "UCS-2 (ISO/IEC-10646)"
//#define STR_PICTOGRAM_ENCODING "Pictogram Encoding"
#define STR_ISO_2022_JP        "ISO-2022-JP (Music Codes)"
//#define STR_JIS_X_0212_1990    "Extended Kanji JIS (X 0212-1990)"
//#define STR_KS_C_5601          "KS C 5601"

#define STR_DEFAULT_ALPHABET       "GSM7"        //SMSC Default Alphabet (GSM 7 bits)
#define STR_ASCII_ANSI_X34         "ASCII"       //IA5 (CCITT T.50)/ASCII (ANSI X3.4)
#define STR_OCTET_UNSPECIFIED      "none"        //Octet unspecified (8-bit binary) 
#define STR_ISO_8859_1             "ISO-8859-1"  //Latin 1 (ISO-8859-1)
//#define STR_OCTET_UNSPECIFIED    "none"        //Octet unspecified (8-bit binary) 
#define STR_JIS_X_0208_1990        "none"        //JIS (X 0208-1990)
#define STR_ISO_8859_5             "ISO-8859-5"  //Cyrllic (ISO-8859-5)
#define STR_ISO_8859_8             "ISO-8859-8"  //Latin/Hebrew (ISO-8859-8)
#define STR_UCS_2                  "UCS-2"       //UCS2 (ISO/IEC-10646)
#define STR_PICTOGRAM_ENCODING     "none"        //Pictogram Encoding
#define STR_ISO_2022_JP            "ISO-2022-JP" //ISO-2022-JP (Music Codes)
#define STR_RESERVED               "none"        //reserved
//#define STR_RESERVED             "none"        //reserved
#define STR_JIS_X_0212_1990        "none"        //Extended Kanji JIS (X 0212-1990)
#define STR_KS_C_5601              "noen"        //KS C 5601
//#define STR_RESERVED             "none"        //reserved

//#define STR_ICONV_ISO_8859_1         "ISO-8859-15" // Voxbone
//#define STR_ICONV_UCS_2              "UTF-16BE"    // Legos


#define ENUM_DEFAULT_ALPHABET    0
#define ASCII_ANSI_X34      1
#define ISO_8859_1          3
#define JIS_X_0208_1990     5
#define ISO_8859_5          6
#define ISO_8859_8          7
#define UCS_2               8
#define PICTOGRAM_ENCODING  9
#define ISO_2022_JP         10
#define JIS_X_0212_1990     13
#define KS_C_5601           14

#define MAX_TAB_CHARSET     16
#define DEFAULT_SYSTEM_CHARSET "UTF-8"

extern const char* tab_default_charset[MAX_TAB_CHARSET];

int conv_char_codec_str(char *in, size_t s_in, char *codec_in, char **out, size_t s_out, char *codec_out);

#endif /* CONV_H */

