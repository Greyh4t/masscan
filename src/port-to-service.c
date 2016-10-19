#include <stdio.h>
#include <assert.h>
#include <regex.h>
#include <stdlib.h>
#include <string.h>
#include "logger.h"
#include "dict.h"
#include "main-globals.h"
#include "port-to-service.h"
#define NMATCH 4


void port_service_db_init(){
	char *pattern = "^([_.a-zA-Z0-9-]+)\t([0-9]{1,5})/(\\w+)";
	regex_t regex;
	regmatch_t m[NMATCH];
	int reti;
	int regexInit = regcomp(&regex, pattern, REG_EXTENDED);

	tcp_port_service_db = DictCreate();
	udp_port_service_db = DictCreate();
	sctp_port_service_db = DictCreate();
	
	if (regexInit) {
		LOG(2, "Compile regex failed\n");
	}else{
		char filename[] = "nmap-services";
		FILE *fp;
		char strLine[1024];
		if ((fp=fopen(filename,"r")) == NULL) {
			LOG(0, "Load nmap-services failed\n");
			return;
		}
		while (!feof(fp)) {
			if(fgets(strLine, 1024, fp) == NULL){
				continue;
			};
			reti = regexec(&regex, strLine, NMATCH, m, 0);
			if (REG_NOERROR != reti) {
				LOG(5, "Match failed (%s)\n", strLine);
			}else{
				char k[5] = "\0";    //proto
				char key[6] = "\0";    //port
				char value[20] = "\0";    //service
				strncpy(k, strLine + m[3].rm_so, m[3].rm_eo - m[3].rm_so);
				strncpy(key, strLine + m[2].rm_so, m[2].rm_eo - m[2].rm_so);
				strncpy(value, strLine + m[1].rm_so, m[1].rm_eo - m[1].rm_so);
				switch (k[0]) {
					case 't': DictInsert(tcp_port_service_db, key, value);
					case 'u': DictInsert(udp_port_service_db, key, value);
					case 's': DictInsert(sctp_port_service_db, key, value);
				}
			}
		}
		fclose(fp);
	}
	regfree(&regex);
}

const char *port_to_service(unsigned port, unsigned ip_proto){
	char port_str[6];
	const char *service = "\0";
	sprintf(port_str, "%d" , port);
	switch (ip_proto) {
		case 6:
			service = DictSearch(tcp_port_service_db, port_str);
			break;
		case 17:
			service = DictSearch(udp_port_service_db, port_str);
			break;
		case 132:
			service = DictSearch(sctp_port_service_db, port_str);
			break;
		default:
			return "unknown";
	}
	if (service) {
		return service;
	}
	return "unknown";
}
