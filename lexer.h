#ifndef BGP_CONF_L_H_
#define BGP_CONF_L_H_

#include <vector>
#include <string>
#include <tuple>
#include <unistd.h>

typedef enum Type {
	TOKEN,
	KEYWORD,
	VAR
} Type;

typedef enum MinorType {
	VAR_ADDR,
	VAR_PREFIX_LEN,
	VAR_NAME,
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

typedef struct LexicalItem {
	Type type;
	MinorType mtype;
	std::string element;
} LexicalItem;

typedef std::vector<LexicalItem> LexicalItems;
typedef std::tuple<bool, LexicalItem, std::string> LexOut;
typedef LexOut (*Lexer) (std::string in);
typedef std::vector<Lexer> Lexers;

LexOut any(Lexers lexs);
LexOut str_match(std::string in, std::string match);
LexOut lex_addr(std::string in);
LexOut lex_prefix_len(std::string in);
LexOut lex_name(std::string in);
LexOut lex_asn(std::string in);
LexOut lex_keyword(std::string in);
LexOut lex_token(std::string in);

#endif // BGP_CONF_L_H_