#include <iostream>
#include <ros/ros.h>
#include <stdio.h>
#include <mosquitto.h>
#include <jsoncpp/json/json.h>
#include <mutex>
#include <thread>
#define ANSI_COLOR_RED "\x1b[31m" 
#define ANSI_COLOR_GREEN "\x1b[32m" 
#define ANSI_COLOR_YELLOW "\x1b[33m" 
#define ANSI_COLOR_BLUE "\x1b[34m" 
#define ANSI_COLOR_MAGENTA "\x1b[35m" 
#define ANSI_COLOR_CYAN "\x1b[36m" 
#define ANSI_COLOR_RESET "\x1b[0m"

void connect(struct mosquitto *mosq, void *obj, int rc);
void message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg);

class mqtt_publisher
{
    public:
        int sequence;
        bool onetime;
        std::string topic_send;
        std::string topic_receive;
        std::mutex m;
        void *message_form_pointer;
        void mqtt_connect(const char * host,const int port, const std::string ID);//type in host(ex: 192.168.50.220) /port (ex: 1883) /and ID (ex:001)
        void mqtt_send(const void* data);//should put data match to [messege_form]
        void check_order();
        mqtt_publisher();
        ~mqtt_publisher();
    private:
        std::string convert_array_to_json(const void* data);
        void Print_Error(int Error);
        int mqtt_init(const char * host,const int port, const std::string ID);
        void print_current_state();
        void AR_publish();
        void AC_publish();
        void AP_publish();
        void AD_publish();
        void DC_publish();
        struct mosquitto *mosq;
        int error;
};
