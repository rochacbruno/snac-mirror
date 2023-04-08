/* snac - A simple, minimalistic ActivityPub instance */
/* copyright (c) 2022 - 2023 grunfink / MIT license */

#include "xs.h"
#include "xs_encdec.h"
#include "xs_json.h"
#include "xs_time.h"

#include "snac.h"

int oauth_post_handler(xs_dict *req, char *q_path, char *payload, int p_size,
                      char **body, int *b_size, char **ctype)
{
    if (!xs_startswith(q_path, "/oauth/"))
        return 0;

    int status   = 404;
    xs_dict *msg = xs_dict_get(req, "p_vars");
    xs *cmd      = xs_replace(q_path, "/oauth", "");

    if (strcmp(cmd, "/authorize") == 0) {
        const char *cid   = xs_dict_get(msg, "client_id");
        const char *ruri  = xs_dict_get(msg, "redirect_uri");
        const char *rtype = xs_dict_get(msg, "response_type");
        const char *scope = xs_dict_get(msg, "scope");

        if (cid && ruri && rtype && strcmp(rtype, "code") == 0) {
        }
        else
            status = 400;
    }
    else
    if (strcmp(cmd, "/token") == 0) {
        const char *gtype = xs_dict_get(msg, "grant_type");
        const char *code  = xs_dict_get(msg, "code");
        const char *cid   = xs_dict_get(msg, "client_id");
        const char *csec  = xs_dict_get(msg, "client_secret");
        const char *ruri  = xs_dict_get(msg, "redirect_uri");
        const char *scope = xs_dict_get(msg, "scope");

        if (gtype && code && cid && csec && ruri) {
            xs *rsp = xs_dict_new();
            xs *cat = xs_number_new(time(NULL));

            rsp = xs_dict_append(rsp, "access_token", "abcde");
            rsp = xs_dict_append(rsp, "token_type",   "Bearer");
            rsp = xs_dict_append(rsp, "scope",        scope);
            rsp = xs_dict_append(rsp, "created_at",   cat);

            *body  = xs_json_dumps_pp(rsp, 4);
            *ctype = "application/json";
            status = 200;
        }
        else
            status = 400;
    }
    else
    if (strcmp(cmd, "/revoke") == 0) {
    }

    return status;
}


int mastoapi_post_handler(xs_dict *req, char *q_path, char *payload, int p_size,
                      char **body, int *b_size, char **ctype)
{
    if (!xs_startswith(q_path, "/api/v1/"))
        return 0;

    int status    = 404;
    xs *msg       = NULL;
    char *i_ctype = xs_dict_get(req, "content-type");

    if (xs_startswith(i_ctype, "application/json"))
        msg = xs_json_loads(payload);
    else
        msg = xs_dup(xs_dict_get(req, "p_vars"));

    if (msg == NULL)
        return 400;

    {
        xs *j = xs_json_dumps_pp(req, 4);
        printf("%s\n", j);
    }
    {
        xs *j = xs_json_dumps_pp(msg, 4);
        printf("%s\n", j);
    }

    xs *cmd = xs_replace(q_path, "/api/v1", "");

    if (strcmp(cmd, "/apps") == 0) {
        const char *name = xs_dict_get(msg, "client_name");
        const char *ruri = xs_dict_get(msg, "redirect_uris");

        if (name && ruri) {
            xs *app = xs_dict_new();
            xs *id  = xs_replace_i(tid(0), ".", "");

            app = xs_dict_append(app, "name", name);
            app = xs_dict_append(app, "redirect_uri", ruri);
            app = xs_dict_append(app, "client_id", "abcde");
            app = xs_dict_append(app, "client_secret", "abcde");
            app = xs_dict_append(app, "vapid_key", "abcde");
            app = xs_dict_append(app, "id", id);

            *body  = xs_json_dumps_pp(app, 4);
            *ctype = "application/json";
            status = 200;
        }
    }

    return status;
}
