
#ifndef SIP_RESPONSE_H
#define SIP_RESPONSE_H

typedef enum sip_response_code_t {
    //Provisional (1xx)
    TRYING                  = 100,
    RINGING                 = 180,
    CALL_IS_BEING_FORWARDED = 181,
    QUEUED                  = 182,
    SESSION_PROGRESS        = 183,
    //Success (2xx)
    OK                      = 200,
    ACCEPTED                = 202,
    //Redirection (3xx)
    MULTIPLE_CHOICES        = 300,
    MOVED_PERMANENTLY       = 301,
    MOVED_TEMPORARILY       = 302,
    USE_PROXY               = 305,
    ALERNATE_SERVICE        = 380,
    //Client Error (4xx)
    BAD_REQUEST             = 400,
    UNAUTHORIZED            = 401,
    PAYMENT_REQUIRED        = 402,
    FORBIDDEN               = 403,
    NOT_FOUND               = 404,
    METHOD_NOT_ALLOWED      = 405,
    NOT_ACCEPTABLE          = 406,
    PROXY_AUTHENTICATION_REQUIRED = 407,
    REQUEST_TIMEOUT         = 408,
    GONE                    = 410,
    REQUEST_ENTITY_TOO_LARGE= 413,
    REQUEST_URI_TOO_LONG    = 414,
    UNSUPPORTED_MEDIA_TYPE  = 415,
    UNSUPPORTED_URI_SCHEME  = 416,
    BAD_EXTENSION           = 420,
    EXTENSION_REQUIRED      = 421,
    INTERVAL_TOO_BRIEF      = 423,
    TEMPORARILY_UNAVAILABLE = 480,
    CALL_TRANSACTION_DOES_NOT_EXIST = 481,
    LOOP_DETECTED           = 482,
    TOO_MANY_HOPS           = 483,
    ADDRESS_INCOMPLETE      = 484,
    AMBIGUOUS               = 485,
    BUSY_HERE               = 486,
    REQUEST_TERMINATED      = 487,
    NOT_ACCEPTABLE_HERE     = 488,
    REQUEST_PENDING         = 491,
    UNDECIPHERABLE          = 493,
    //Server Error (5xx)
    INTERNAL_SERVER_ERROR   = 500,
    NOT_IMPLEMENTED         = 501,
    BAD_GATEWAY             = 502,
    SERVICE_UNAVAILABLE     = 503,
    SERVER_TIMEOUT          = 504,
    VERSION_NOT_SUPPORTED   = 505,
    MESSAGE_TOO_LARGE       = 513,
    //Global Failure (6xx)
    BUSY_EVERYWHERE         = 600,
    DECLINE                 = 603,
    DOES_NOT_EXIST_ANYWHERE = 604
} sip_response_code_t;


//Provisional (1xx)
#define TRYING_STR                  "Trying"
#define RINGING_STR                 "Ringing"
#define CALL_IS_BEING_FORWARDED_STR "Call Is Being Forwarded"
#define QUEUED_STR                  "Queued"
#define SESSION_PROGRESS_STR        "Session Progress"
//Success (2xx)
#define OK_STR                      "OK"
#define ACCEPTED_STR                "Accepted"
//Redirection (3xx)
#define MULTIPLE_CHOICES_STR        "Multiple Choices"
#define MOVED_PERMANENTLY_STR       "Moved Permanently"
#define MOVED_TEMPORARILY_STR       "Moved Temporarily"
#define USE_PROXY_STR               "Use Proxy"
#define ALERNATE_SERVICE_STR        "Alernate Service"
//Client Error (4xx)
#define BAD_REQUEST_STR             "Bad Request"
#define UNAUTHORIZED_STR            "Unauthorized"
#define PAYMENT_REQUIRED_STR        "Payment Required"
#define FORBIDDEN_STR               "Forbidden"
#define NOT_FOUND_STR               "Not Found"
#define METHOD_NOT_ALLOWED_STR      "Method Not Allowed"
#define NOT_ACCEPTABLE_STR          "Not Acceptable"
#define PROXY_AUTHENTICATION_REQUIRED_STR "Proxy Authentication Required"
#define REQUEST_TIMEOUT_STR         "Request Timeout"
#define GONE_STR                    "Gone"
#define REQUEST_ENTITY_TOO_LARGE_STR "Request Entity Too Large"
#define REQUEST_URI_TOO_LONG_STR    "Request-URI Too Large"
#define UNSUPPORTED_MEDIA_TYPE_STR  "Unsupported Media Type"
#define UNSUPPORTED_URI_SCHEME_STR  "Unsupported URI Scheme"
#define BAD_EXTENSION_STR           "Bad Extension"
#define EXTENSION_REQUIRED_STR      "Extension Required"
#define INTERVAL_TOO_BRIEF_STR      "Interval Too Brief"
#define TEMPORARILY_UNAVAILABLE_STR "Temporarily not available"
#define CALL_TRANSACTION_DOES_NOT_EXIST_STR "Call Leg/Transaction Does Not Exist"
#define LOOP_DETECTED_STR           "Loop Detected"
#define TOO_MANY_HOPS_STR           "Too Many Hops"
#define ADDRESS_INCOMPLETE_STR      "Address Incomplete"
#define AMBIGUOUS_STR               "Ambiguous"
#define BUSY_HERE_STR               "Busy Here"
#define REQUEST_TERMINATED_STR      "Request Terminated"
#define NOT_ACCEPTABLE_HERE_STR     "Not Acceptable Here"
#define REQUEST_PENDING_STR         "Request Pending"
#define UNDECIPHERABLE_STR          "Undecipherable"
//Server Error (5xx)
#define INTERNAL_SERVER_ERROR_STR   "Internal Server Error"
#define NOT_IMPLEMENTED_STR         "Not Implemented"
#define BAD_GATEWAY_STR             "Bad Gateway"
#define SERVICE_UNAVAILABLE_STR     "Service Unavailable"
#define SERVER_TIMEOUT_STR          "Server Time-out"
#define VERSION_NOT_SUPPORTED_STR   "SIP Version not supported"
#define MESSAGE_TOO_LARGE_STR       "Message Too Large"
//Global Failure (6xx)
#define BUSY_EVERYWHERE_STR         "Busy Everywhere"
#define DECLINE_STR                 "Decline"
#define DOES_NOT_EXIST_ANYWHERE_STR "Does not exist anywhere"

#endif /* SIP_RESPONSE_H */

