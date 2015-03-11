
#ifndef SIP_HEADER_H
#define SIP_HEADER_H

typedef enum sip_header_t {
    //Main headers
    FROM = 0,
    TO,
    CALL_ID,
    CSEQ,
    VIA,
    MAX_FORWARDS,
    CONTENT_TYPE,
    CONTENT_LENGTH,
    //Others headers
    ACCEPT,
    ACCEPT_ENCODING,
    ACCEPT_LANGUAGE,
    ALERT_INFO,
    ALLOW,
    AUTHENTICATION_INFO,
    AUTHORIZATION,
    CALL_INFO,
    CONTACT,
    CONTENT_DISPOSITION,
    CONTENT_ENCODING,
    CONTENT_LANGUAGE,
    DATE,
    ERROR_INFO,
    EXPIRES,
    IN_REPLY_TO,
    MIN_EXPIRES,
    MIME_VERSION,
    ORGANIZATION,
    PRIORITY,
    PROXY_AUTHENTICATE,
    PROXY_AUTHORIZATION,
    PROXY_REQUIRE,
    RECORD_ROUTE,
    REPLY_TO,
    REQUIRE,
    RETRY_AFTER,
    ROUTE,
    SERVER,
    SUBJECT,
    SUPPORTED,
    TIMESTAMP,
    UNSUPPORTED,
    USER_AGENT,
    WARNING,
    WWW_AUTHENTICATE
} sip_header_t;

#define START_HEADER  FROM
#define END_HEADER    WWW_AUTHENTICATE

//Main headers
#define FROM_STR            "From"
#define TO_STR              "To"
#define CALL_ID_STR         "Call-ID"
#define CSEQ_STR            "CSeq"
#define VIA_STR             "Via"
#define MAX_FORWARDS_STR    "Max-Forwards"
#define CONTENT_TYPE_STR    "Content-Type"
#define CONTENT_LENGTH_STR  "Content-Length"
//Others headers
#define ACCEPT_STR                "Accept"
#define ACCEPT_ENCODING_STR       "Accept-Encoding"
#define ACCEPT_LANGUAGE_STR       "Accept-Language"
#define ALERT_INFO_STR            "Alert-Info"
#define ALLOW_STR                 "Allow"
#define AUTHENTICATION_INFO_STR   "Authentication-Info"
#define AUTHORIZATION_STR         "Authorization"
#define CALL_INFO_STR             "Call-Info"
#define CONTACT_STR               "Contact"
#define CONTENT_DISPOSITION_STR   "Content-Disposition"
#define CONTENT_ENCODING_STR      "Content-Encoding"
#define CONTENT_LANGUAGE_STR      "Content-Language"
#define DATE_STR                  "Date"
#define ERROR_INFO_STR            "Error-Info"
#define EXPIRES_STR               "Expires"
#define IN_REPLY_TO_STR           "In-Reply-To"
#define MIN_EXPIRES_STR           "Min-Expires" 
#define MIME_VERSION_STR          "MIME-Version"
#define ORGANIZATION_STR          "Organization"
#define PRIORITY_STR              "Priority"
#define PROXY_AUTHENTICATE_STR    "Proxy-Authenticate"
#define PROXY_AUTHORIZATION_STR   "Proxy-Authorization"
#define PROXY_REQUIRE_STR         "Proxy-Require"
#define RECORD_ROUTE_STR          "Record_Route"
#define REPLY_TO_STR              "Reply-To"
#define REQUIRE_STR               "Require"
#define RETRY_AFTER_STR           "Retry-After"
#define ROUTE_STR                 "Route"
#define SERVER_STR                "Server"
#define SUBJECT_STR               "Subject"
#define SUPPORTED_STR             "Supported"
#define TIMESTAMP_STR             "Timestamp"
#define UNSUPPORTED_STR           "Unsupported"
#define USER_AGENT_STR            "User-Agent"
#define WARNING_STR               "Warning"
#define WWW_AUTHENTICATE_STR      "WWW-Authenticate"

extern char *tab_sip_header[];

#endif /* SIP_HEADER_H */

