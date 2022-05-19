#include <iostream>
#include <ros/ros.h>
#include <stdio.h>
#include <mosquitto.h>
#include <jsoncpp/json/json.h>
#include <mutex>

#define ANSI_COLOR_RED "\x1b[31m" 
#define ANSI_COLOR_GREEN "\x1b[32m" 
#define ANSI_COLOR_YELLOW "\x1b[33m" 
#define ANSI_COLOR_BLUE "\x1b[34m" 
#define ANSI_COLOR_MAGENTA "\x1b[35m" 
#define ANSI_COLOR_CYAN "\x1b[36m" 
#define ANSI_COLOR_RESET "\x1b[0m"


/*SEND ORDER*/
//AMRID 차량번호
/*RECEIVE ORDER*/

void connect(struct mosquitto *mosq, void *obj, int rc);
void message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg);
void disconnect(struct mosquitto *mosq, void *obj, int rc);
void subscribe(struct mosquitto *mosq, void *obj,int mid,int qos_count,const int* granted_qos);
class mqtt_subscriber
{
    public:
        int sequence;
        bool onetime;
        std::string topic_send;
        
        std::string topic_receive;
        std::mutex m;
        std::vector<char*> get_order_vector();
        
        std::vector<char*> get_emr_vector();//return emr_vector
        char* get_order_vector_element();//get first element and delete first element if empty return (char*)"empty"
        char* get_emr_vector_element();//get first element and delete first element if empty return (char*)"empty"
        void mqtt_connect(const char * host,const int port, const std::string ID);//type in host(ex: 192.168.50.220) /port (ex: 1883) /and ID (ex:001)
        void mqtt_send(const void* data);//should put data match to [messege_form]
        mqtt_subscriber();
        ~mqtt_subscriber();
    private:
        std::vector<char*> order_vector;//where store order's receive from AP
        std::vector<char*> emr_vector;//where store emergency order's receive from AC AP AD DC
        std::string convert_array_to_json(const void* data);
        void Print_Error(int Error);
        int mqtt_init(const char * host,const int port, const std::string ID);
        struct mosquitto *mosq;
        int error;
        
};
