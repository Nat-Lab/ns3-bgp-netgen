#ifndef BGP_CONF_L_H_
#define BGP_CONF_L_H_

#include <vector>
#include <string>
#include <tuple>
#include <regex>
#include <unistd.h>

typedef enum Type {
    TOKEN,
    KEYWORD,
    VAR,
    BOOL,
    IGNORE
} Type;

typedef enum MinorType {
    VAR_ADDR,
    VAR_PREFIX_LEN,
    VAR_NAME,
    VAR_ASN,
    TKN_LBRACE,
    TKN_RBRACE,
    TKN_SEMICOL,
    IGN_NEWLINE,
    IGN_COMMENT,
    KW_NETWORK,
    KW_PREFIX,
    KW_TAP,
    KW_TAP_NAME,
    KW_TAP_MODE,
    KW_TAP_ADDRESS,
    KW_ROUTER,
    KW_AS,
    KW_DEV,
    KW_DEVICE,
    KW_DEVICES,
    KW_PEER,
    KW_PEERS,
    KW_PASSIVE,
    KW_ROUTE,
    KW_ROUTES,
    KW_CONNECT,
    KW_ADDRESS,
    KW_VIA,
    KW_LOCAL,
    KW_OPTIONS,
    KW_LOG,
    KW_IN_FILTER,
    KW_OUT_FILTER,
    KW_DEFAULT_ACTION,
    KW_ACCEPT,
    KW_REJECT,
    BOOL_TRUE,
    BOOL_FALSE
} MinorType;

typedef struct LexicalItem {
    Type type;
    MinorType mtype;
    std::string item;
} LexicalItem;

typedef std::vector<LexicalItem> LexicalItems;
typedef std::tuple<bool, LexicalItem*> LexOut;
typedef std::tuple<bool, std::string> LexResult;
typedef LexOut (*Lexer) (std::string &in);
typedef std::vector<Lexer> Lexers;

void eat_space(std::string &in);
std::string str_shift(std::string &in, uint32_t n);

LexOut any(const Lexers lexs, std::string &in);
LexOut lex_addr(std::string &in);
LexOut lex_prefix_len(std::string &in);
LexOut lex_name(std::string &in);
LexOut lex_asn(std::string &in);
LexOut lex_keyword(std::string &in);
LexOut lex_token(std::string &in);
LexOut lex_bool(std::string &in);

LexResult lexer(const std::string &in_c, LexicalItems &out);

#endif // BGP_CONF_L_H_