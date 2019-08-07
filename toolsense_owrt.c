#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <mosquitto.h>

#define DELAY_SEND_TIME 5 //sec
// #define PORT 10333
#define PORT_SSL 20333

char *topic = NULL;
struct mosquitto *mosq = NULL;

bool send_mqtt_1m = false;

void timer_handler_1m()
{
	send_mqtt_1m = true;
}

void timer_init()
{
	struct sigaction psa;
	struct itimerval tv;

	memset(&psa, 0, sizeof(psa));
	psa.sa_handler = &timer_handler_1m;
	sigaction(SIGALRM, &psa, NULL);


	tv.it_interval.tv_sec = DELAY_SEND_TIME;
	tv.it_interval.tv_usec = 0;
	tv.it_value.tv_sec = DELAY_SEND_TIME;
	tv.it_value.tv_usec = 0;
	setitimer(ITIMER_REAL, &tv, NULL);
}

int mqtt_init()
{
	char *host = "m15.cloudmqtt.com";
	int port = PORT_SSL;
	int keepalive = 60;
	bool clean_session = true;

	int rc = 0;
	
	topic = "temp";

	mosquitto_lib_init();

	mosq = mosquitto_new(NULL, clean_session, NULL);

	if(!mosq){
		printf("Error: Out of memory.\n");
		return 1;
	}

	mosquitto_username_pw_set(mosq, "thmcoslv", "odUivT2WEIsW");

	rc = mosquitto_tls_set(mosq, "/etc/ssl/certs/ca-certificates.crt", NULL, NULL, NULL, NULL);
	if (rc) {
		printf ("set ssl failed %d\n", rc);
		return 1;
	}

	rc = mosquitto_tls_insecure_set(mosq, true);

	if (rc) {
		printf ("set insecure failed %d\n", rc);
		return 1;
	}	

	if(mosquitto_connect(mosq, host, port, keepalive)){
		printf("Unable to connect.\n");
		return 1;
	}

	int loop = mosquitto_loop_start(mosq);
	if(loop != MOSQ_ERR_SUCCESS){
		printf("Unable to start loop: %i\n", loop);
		return 1;
	}

	return 0;
}

int mqtt_send(char *msg){
  return mosquitto_publish(mosq, NULL, topic, strlen(msg), msg, 0, 0);
}

int send_time()
{
	char *buf = malloc(64);
	time_t t;

	time(&t);
	printf("\nCurrent time is: %s",ctime(&t));
	sprintf(buf,"\nCurrent time is: %s",ctime(&t));

	int rc = mqtt_send(buf);
	printf("mqtt_send error=%i\n", rc);

	return rc;
}

int main(int argc, char *argv[])
{
	printf("\nMosquitto test SSL\n\n");

	mqtt_init();
	timer_init();

	while(1) {

		if (send_mqtt_1m) {
			send_time();
			send_mqtt_1m = false;
		}

		sleep(1);

	}

	// mosquitto_loop_forever(mosq, -1, 1);
	// mosquitto_destroy(mosq);
	// mosquitto_lib_cleanup();
	return 0;
}
