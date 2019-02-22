#ifndef BGP_CONF_L_H_
#define BGP_CONF_L_H_

#include <vector>
#include <string>

typedef enum Type {
	TOKEN,
	KEYWORD,
	VAR
} Type;

typedef enum MinorType {
	VAR_ADDR,
	VAR_PREFIX,
	VAR_PREFIX_LEN,
	VAR_NETNAME,
	VAR_DEVNAME,
	VAR_ASN,
	KW_DEVICES,
	KW_DEVICE,
	KW_DEV,
	KW_NET,
	KW_ROUTER,
	KW_ROUTES,
	KW_ROUTE,
	KW_ASN,
	KW_VIA,
	KW_ADDR,
	KW_CONNECT,
	KW_PEERS,
	KW_PEER,
	TKN_LBRACE,
	TKN_RBRACE,
	TKN_SEMICOL
} MinorType;

typedef struct Element {
	Type type;

	std::string element;
} Element;

typedef std::vector<Element> Tokens; 

#endif // BGP_CONF_L_H_