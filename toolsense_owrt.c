#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mosquitto.h>

char *topic = NULL;
struct mosquitto *mosq = NULL;

int mqtt_send(char *msg){
  return mosquitto_publish(mosq, NULL, topic, strlen(msg), msg, 0, 0);
}
 
int main(int argc, char *argv[])
{
	char *host = "m15.cloudmqtt.com";
	int port = 10333;
	int keepalive = 60;
	bool clean_session = true;
	
	topic = "temp";
	char *buf = malloc(64);
	// char *buf = "Test";

	mosquitto_lib_init();

	mosq = mosquitto_new(NULL, clean_session, NULL);

	if(!mosq){
		printf("Error: Out of memory.\n");
		return 1;
	}

	mosquitto_username_pw_set(mosq, "thmcoslv", "odUivT2WEIsW");

	if(mosquitto_connect(mosq, host, port, keepalive)){
		printf("Unable to connect.\n");
		return 1;
	}

	int loop = mosquitto_loop_start(mosq);
	if(loop != MOSQ_ERR_SUCCESS){
		printf("Unable to start loop: %i\n", loop);
		return 1;
	}

    printf("\nMosquitto test\n\n");

    time_t t;
    time(&t);
    printf("\n current time is : %s",ctime(&t));
    sprintf(buf,"\n current time is : %s",ctime(&t));

    int snd = mqtt_send(buf);
    printf("mqtt_send error=%i\n", snd);
    
    // mosquitto_loop_forever(mosq, -1, 1);

	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
    return 0;
}
