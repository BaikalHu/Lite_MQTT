#ifndef __ATINY_CONFIG_H__
#define __ATINY_CONFIG_H__

#include <stddef.h>

#define ATINY_SEND_BUF_SIZE      (1024)
#define ATINY_RECV_BUF_SIZE      (1024)




#ifdef WITH_DTLS
#ifndef SERVER_PORT
#define SERVER_PORT (8883)
#endif
#ifndef SERVER_NAME
#define SERVER_NAME "MacBook-Air.local"
#endif
extern const char server_name[];

#ifndef MQTT_TEST_CA_CRT
#define MQTT_TEST_CA_CRT    \
"-----BEGIN CERTIFICATE-----\r\n"	 \
"MIICxjCCAa6gAwIBAgIJAJk1DbZBu8FDMA0GCSqGSIb3DQEBCwUAMBMxETAPBgNV\r\n"	  \
"BAMMCE15VGVzdENBMB4XDTE3MTEwMjEzNDI0N1oXDTE5MTEwMjEzNDI0N1owEzER\r\n"	  \
"MA8GA1UEAwwITXlUZXN0Q0EwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIB\r\n"	  \
"AQDshDho6ef1JClDJ24peSsXdFnFO3xIB7+BSp1YPcOvmRECKUG0mLORw3hNm15m\r\n"	  \
"8eGOn1iLGE/xKlaZ74/xjyq8f7qIGZCmvZj59m+eiJCAmy8SiUJZtSVoOlOzepJd\r\n"	  \
"PoDgcBvDKA4ogZ3iJHMUNI3EdlD6nrKEJF2qe2JUrL0gv65uo2/N7XVNvE87Dk3J\r\n"	  \
"83KyCAmeu+x+moS1ILnjs2DuPEGSxZqzf7IQMbXuNWJYAOZg9t4Fg0YjTiAaWw3G\r\n"	  \
"JKAoMY4tI3JCqlvwGR4lH7kfk3WsD4ofGlFhxU4nEG0xgnJl8BcoJWD1A2RjGe1f\r\n"	  \
"qCijqPSe93l2wt8OpbyHzwc7AgMBAAGjHTAbMAwGA1UdEwQFMAMBAf8wCwYDVR0P\r\n"	  \
"BAQDAgEGMA0GCSqGSIb3DQEBCwUAA4IBAQAi+t5jBrMxFzoF76kyRd3riNDlWp0w\r\n"	  \
"NCewkohBkwBHsQfHzSnc6c504jdyzkEiD42UcI8asPsJcsYrQ+Uo6OBn049u49Wn\r\n"	  \
"zcSERVSVec1/TAPS/egFTU9QMWtPSAm8AEaQ6YYAuiwOLCcC+Cm/a3e3dWSRWt8o\r\n"	  \
"LqKX6CWTlmKWe182MhFPpZYxZQLGapti4R4mb5QusUbc6tXbkcX82GjDPTOuAw7b\r\n"	  \
"mWpzVd5xnlp7Vz+50u+YaAYUmCobg0hR/AuTrA4GDMlgzTnuZQhF6o8iVkypXOtS\r\n"	  \
"Ufz6X3tVVErVVc7UUfzSnupHj1M2h4rzlQ3oqHoAEnXcJmV4f/Pf/6FW\r\n"	  \
"-----END CERTIFICATE-----\r\n"
#endif

#ifndef MQTT_TEST_CLI_CRT
#define MQTT_TEST_CLI_CRT \
"-----BEGIN CERTIFICATE-----\r\n"	 \
"MIIC6jCCAdKgAwIBAgIBAjANBgkqhkiG9w0BAQsFADATMREwDwYDVQQDDAhNeVRl\r\n"	 \
"c3RDQTAeFw0xNzExMDIxMzQyNDhaFw0xOTExMDIxMzQyNDhaMC0xGjAYBgNVBAMT\r\n"	 \
"EU1hY0Jvb2stQWlyLmxvY2FsMQ8wDQYDVQQKEwZjbGllbnQwggEiMA0GCSqGSIb3\r\n"	 \
"DQEBAQUAA4IBDwAwggEKAoIBAQC8GptpL25hv1Qa3jCn4VLvDRH/SrHg9wXvqRkz\r\n"	 \
"HuiKMxYT30m4+kcaXv350CJrkV+8lR24wdN7DBVewpCUnyUBbzkLccy1LUzunZ3z\r\n"	 \
"nm37j6cautD3rlC9gsC9d0uJ745FLx5t/6f1jMk9rWxn+4iSGAnkWC3mVaQxP1zQ\r\n"	 \
"q8GI97uob9HNb0OH6ygHJAcKOWB+85a29LIMa1uo/lT3hMr8sBg2vX+1F/gTusmW\r\n"	 \
"xVoQc9XJxBCs995qsH0UkZIuOY0XZp9/qFfcZv2QmslG8DojIIHKcujzu8bItE2M\r\n"	 \
"OyL5NlWLvN6qg59hHzF4+D+T+8GkhhKWSC+xdY14eQ5fB4S5AgMBAAGjLzAtMAkG\r\n"	 \
"A1UdEwQCMAAwCwYDVR0PBAQDAgeAMBMGA1UdJQQMMAoGCCsGAQUFBwMCMA0GCSqG\r\n"	 \
"SIb3DQEBCwUAA4IBAQBLV4ZfhiKiFVnL/xO0MRGSKr3xd0LK64SW8Iw5DYkc0jNX\r\n"	 \
"sDrRbj2I/KJ/Rc4AeKT751L+C+KBzYpFgiLrxDmt/5pmgiFH51hPQtL7kRC0z2NY\r\n"	 \
"EY/P+u4IFVSo+b1hHYU7y+OMj6/Vvd4x0ETS4rHWI4mPDfGfvClEVLOktgRKrMU5\r\n"	 \
"9aTltF4U0FBUlYZTQBNBUFwBzj1+0lxK4EdhRmmWJ+uW9rgkQxpnUdbCPGvUKFRp\r\n"	 \
"3AbdHBAU9H2zVd2VZoJu6r7LMp6agxu0rYLgmamRAt+8rnDXvy7H1ZNdjT6fTbUO\r\n"	 \
"omVBMyJAc1+10gjpHw/EUD58t5/I5tZrnrANPgIs\r\n"	 \
"-----END CERTIFICATE-----\r\n"
#endif


#ifndef MQTT_TEST_CLI_KEY
#define MQTT_TEST_CLI_KEY \
"-----BEGIN RSA PRIVATE KEY-----\r\n"	 \
"MIIEowIBAAKCAQEAvBqbaS9uYb9UGt4wp+FS7w0R/0qx4PcF76kZMx7oijMWE99J\r\n"	 \
"uPpHGl79+dAia5FfvJUduMHTewwVXsKQlJ8lAW85C3HMtS1M7p2d855t+4+nGrrQ\r\n"	 \
"965QvYLAvXdLie+ORS8ebf+n9YzJPa1sZ/uIkhgJ5Fgt5lWkMT9c0KvBiPe7qG/R\r\n"	 \
"zW9Dh+soByQHCjlgfvOWtvSyDGtbqP5U94TK/LAYNr1/tRf4E7rJlsVaEHPVycQQ\r\n"	 \
"rPfearB9FJGSLjmNF2aff6hX3Gb9kJrJRvA6IyCBynLo87vGyLRNjDsi+TZVi7ze\r\n"	 \
"qoOfYR8xePg/k/vBpIYSlkgvsXWNeHkOXweEuQIDAQABAoIBAHnFV7peRDzvGUlT\r\n"	 \
"cXgcvA2ZDn+QIVsbTzJ466FWbv+YVsCCmj0veHwv5oakIMQ2Fh4FAnqqr3dGuUbg\r\n"	 \
"+avc4p3tHKa2Aul+7ADE9I3TkCt8MZdyPPk6VXZ5gMCmy7X96MIM4Mwg5uBlRZmx\r\n"	 \
"/S3Lffvlp/G0y/ICmwpulG1Z4y4A5Vc0Qf7fBO03Ekl31oReARnB6ex7RnDHH1mW\r\n"	 \
"RyLWNqyu9BhUbFpIyFPWDSkBcajNIbQ6qVJfGLm5Y2xVhwdqbyvY8M06uuMKz/IR\r\n"	 \
"SYfdIpiC4PpQZQzzXMn/6LTKWcCe0T+dBcWTZHC3C2abrC7+5fwFobs2xoUaCwz0\r\n"	 \
"1CclogECgYEA6Jdv+2VSYIBLbS0VIe07JiZaQNd1QNg63MK/y7oqAKEzYvpWzJel\r\n"	 \
"owPdBU3GxZH6vUUF7sCABcjumEDazoqTtzHQBo0xYpJrjmAL0ANNGVvF09pJK2eq\r\n"	 \
"yotxJJAS5/lQNSgWOxGVc6qu6ZpgeIXVLIx816yq04h10yVgZ2Lm3+ECgYEAzwj5\r\n"	 \
"/UVpN/ak6PwZ+Tq/8qOYjY2ABylRmP+T0Rkqmwh2B5Sp9oXjkDQwWseY0Wybhd8F\r\n"	 \
"kO6BUCMUApnB3uU0baawVbDUSrt43SkkKV9m3pA35wA3pYw1a56QIEFr56npFYBS\r\n"	 \
"sn9yl/ZtNvnuwmrHWOq8HdwPJsWREyO61yknn9kCgYB1PdixpSo4AJOErePoHRfi\r\n"	 \
"rBR0eObez+Aj5Xsea3G+rYMkkkHskUhp+omPodvfPS1h+If8CEbAI7+5OX/R+uJo\r\n"	 \
"xpAwrT1Gjb3vn5R0vyU+8havKmoVmgTqYg2fO4x8KBz5HoLONZfbHR9cG3gjaHrD\r\n"	 \
"IPHRGXVmeXPDAiUtGBp+oQKBgQDDRIAkNPdMZUCczknhG1w3Cb20pKUAHCRt3YAZ\r\n"	 \
"U1cv6gcIl1rGvPko5VBGDsM/ouP8m6CwVYN5hdw1p7eG9z8/vFvMNn/EDJWuYkNN\r\n"	 \
"EkH/4J4ZLcdOSLOJ0X+2LH4Nfd/s+58D49i9IxtXItviWruyTZMnxooz01tFZgmv\r\n"	 \
"LY3F4QKBgDirafhlJqFK6sa8WesHpD5+lm3Opzi4Ua8fAGHy2oHN3WCEL74q691C\r\n"	 \
"fA0P2UrzYiF7dXf4fgK9eMMQsdWS4nKyCSqM6xE4EAhAHUTYzY3ApNjI3XFDIrKC\r\n"	 \
"oQefIOLum2UyWFuEoUtrEfc5fxktiQohCwuAvwC59EwhmsNlECA8\r\n"	 \
"-----END RSA PRIVATE KEY-----\r\n"
#endif

extern const char mqtt_test_cas_pem[];
extern const size_t mqtt_test_cas_pem_len;
extern const char mqtt_test_cli_pem[];
extern const size_t mqtt_test_cli_pem_len;
extern const char mqtt_test_cli_key[];
extern const size_t mqtt_test_cli_key_len;



#else

#ifndef SERVER_PORT
#define SERVER_PORT (1883)
#endif

#endif










#endif
