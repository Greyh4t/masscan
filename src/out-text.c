#include "output.h"
#include "masscan.h"
#include "masscan-app.h"
#include "masscan-status.h"
#include "unusedparm.h"
#include "main-globals.h"
#include "port-to-service.h"

#include <ctype.h>

/****************************************************************************
 ****************************************************************************/
static void
text_out_open(struct Output *out, FILE *fp)
{
    UNUSEDPARM(out);
    fprintf(fp, "#masscan\n");
}

/****************************************************************************
 ****************************************************************************/
static void
text_out_close(struct Output *out, FILE *fp)
{
    UNUSEDPARM(out);
    fprintf(fp, "# end\n");
}

/****************************************************************************
 ****************************************************************************/
static void
text_out_status(struct Output *out, FILE *fp, time_t timestamp,
    int status, unsigned ip, unsigned ip_proto, unsigned port, unsigned reason, unsigned ttl)
{
    const char *service = port_to_service(port, ip_proto);

    UNUSEDPARM(ttl);
    UNUSEDPARM(reason);
    UNUSEDPARM(out);

    fprintf(fp, "%s %s %u %u.%u.%u.%u %u %s\n",
        status_string(status),
        name_from_ip_proto(ip_proto),
        port,
        (ip>>24)&0xFF,
        (ip>>16)&0xFF,
        (ip>> 8)&0xFF,
        (ip>> 0)&0xFF,
        (unsigned)timestamp,
        service
        );
    fflush(fp);
}


/*************************************** *************************************
 ****************************************************************************/
static void
text_out_banner(struct Output *out, FILE *fp, time_t timestamp,
        unsigned ip, unsigned ip_proto, unsigned port,
        enum ApplicationProtocol proto, unsigned ttl,
        const unsigned char *px, unsigned length)
{
    char banner_buffer[4096];

    const char *service = masscan_app_to_string(proto);
    if (strcmp(service ,"unknown") == 0) {
        service = port_to_service(port, ip_proto);
    }

    UNUSEDPARM(out);
    UNUSEDPARM(ttl);

    fprintf(fp, "%s %s %u %u.%u.%u.%u %u %s %s\n",
        "banner",
        name_from_ip_proto(ip_proto),
        port,
        (ip>>24)&0xFF,
        (ip>>16)&0xFF,
        (ip>> 8)&0xFF,
        (ip>> 0)&0xFF,
        (unsigned)timestamp,
        service,
        normalize_string(px, length, banner_buffer, sizeof(banner_buffer))
        );
    fflush(fp);
}


/****************************************************************************
 ****************************************************************************/
const struct OutputType text_output = {
    "txt",
    0,
    text_out_open,
    text_out_close,
    text_out_status,
    text_out_banner
};



