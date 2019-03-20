#ifndef BGP_CONF_L_H_
#define BGP_CONF_L_H_

#include <vector>
#include <string>
#include <tuple>
#include <regex>
#include <unistd.h>
#include <string.h>

const std::regex r_v4 ("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)");
const std::regex r_len ("^\\/[0-9]{1,2}");
const std::regex r_name ("^[a-zA-Z]+[a-zA-Z0-9\\-_]*");
const std::regex r_path ("^\\/?([a-zA-Z0-9\\-_]*\\/?)+");
const std::regex r_asn ("^[0-9]+");
const std::regex r_kw ("^(monitor(_trigger|_output)?|network|prefix|tap(_name|_mode|_address)?|router|as|dev(ices?)?|peers?|routes?|connect|address|via|options|log|passive|local|(in|out)_filter|default_action|accept|reject|exact)( |;|\\{)");
const std::regex r_comment ("^#.*\\r?\\n");
const std::regex r_token ("^((\\r)?\\n|;|\\{|\\})");
const std::regex r_bool ("^(on|off|true|false)( |;)");

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
    VAR_PATH,
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
    KW_EXACT,
    KW_MONITOR,
    KW_MONITOR_TRIGGER,
    KW_MONITOR_OUTPUT,
    BOOL_TRUE,
    BOOL_FALSE
} MinorType;

typedef struct LexicalItem {
    Type type;
    MinorType mtype;
    std::string item;
} LexicalItem;

typedef std::vector<LexicalItem> LexicalItems;
typedef std::tuple<bool, const char*> LexOut;
typedef LexOut (*Lexer) (const char *in, LexicalItems &out);
typedef std::vector<Lexer> Lexers;

const char* eat_space(const char *in);

LexOut any(const Lexers &lexs, const char *in, LexicalItems &out);
LexOut lex_addr(const char *in, LexicalItems &out);
LexOut lex_prefix_len(const char *in, LexicalItems &out);
LexOut lex_name(const char *in, LexicalItems &out);
LexOut lex_path(const char *in, LexicalItems &out);
LexOut lex_asn(const char *in, LexicalItems &out);
LexOut lex_keyword(const char *in, LexicalItems &out);
LexOut lex_token(const char *in, LexicalItems &out);
LexOut lex_bool(const char *in, LexicalItems &out);

LexOut lexer(const char *in, LexicalItems &out);

#endif // BGP_CONF_L_H_