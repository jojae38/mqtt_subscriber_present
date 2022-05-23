#include "mqtt_pub.h"
using std::thread;



void func1(void *mqtt_publisher_)
{
    mqtt_publisher* mqtt_temp=(mqtt_publisher*)mqtt_publisher_;
    mqtt_temp->mqtt_connect("192.168.200.220",1883,"001");

}

void func2(void *mqtt_publisher_)
{
    sleep(1);
    mqtt_publisher* mqtt_temp=(mqtt_publisher*)mqtt_publisher_;
    while(true)
    {
        mqtt_temp->check_order();   
    }
}
int main()
{
    mqtt_publisher mqtt_publisher_;
    thread t1(func1,&mqtt_publisher_);
    thread t2(func2,&mqtt_publisher_);
    t1.join();
    t2.join();
    return 0;
}