
#ifndef SIP_REQUEST_H
#define SIP_REQUEST_H

typedef enum sip_request_methods_t {
    MESSAGE = 0, //Sends an instant message using SIP.
    INVITE,      //Requests a session.
    ACK,         //Final response to the INVITE
    OPTIONS,     //Ask for server capabilities
    BYE,         //Terminates a session
    PRACK,       //Similar to ACK, but a provisional confirmation.
    CANCEL,      //Cancel any pending requests.
    REGISTER,    //Registers the client with the server according to the address in the To header field.
    INFO,        //Sends information in the middle of a session that doesn't modify the session's state.
    SUBSCRIBE,   //Subscribes the device for an event notification.
    NOTIFY,			 //Notifies all subscribers of an event.
    PUBLISH,	   //Publishes an event to a server.
    REFER,       //Asks the client to issue a SIP request, typically a call transfer.
    UPDATE       //Modifies a session's state without altering the dialog state. 
} sip_request_methods_t;

#define START_REQUEST  MESSAGE
#define END_REQUEST    UPDATE

#define MESSAGE_STR   "MESSAGE"
#define INVITE_STR    "INVITE"
#define ACK_STR       "ACK"
#define OPTIONS_STR   "OPTIONS"
#define BYE_STR       "BYE"
#define PRACK_STR     "PRACK"
#define CANCEL_STR    "CANCEL"
#define REGISTER_STR  "REGISTER"
#define INFO_STR      "INFO"
#define SUBSCRIBE_STR "SUBSCRIBE"
#define NOTIFY_STR    "NOTIFY"
#define PUBLISH_STR   "PUBLISH"
#define REFER_STR     "REFER"
#define UPDATE_STR    "UPDATE"

extern char *tab_sip_request_methods[];

#endif /* SIP_REQUEST_H */

