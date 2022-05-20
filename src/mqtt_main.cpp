#include "mqtt_sub.h"
using std::thread;
using namespace std;
struct message_form{
    uint16_t ARMID;
    uint16_t UpperType;
    uint16_t X;
    uint16_t Y;
    uint16_t T;
    uint16_t State;
    uint16_t ChargeStatus;
    uint16_t ActionID;
    uint16_t Mode;
    uint16_t Loaded_1;
    uint16_t Loaded_2;
    uint16_t Loaded_3;
    uint16_t CurrentNode;
    uint16_t NextNode;
    uint16_t TargetNode;
    uint16_t Obstacle;
    uint16_t Speed;
    uint16_t MapRate;
    uint16_t ErrorCode1;
    uint16_t ErrorCode2;
    uint16_t ErrorCode3;
    uint16_t ErrorCode4;
    uint16_t ErrorCode5;
    uint16_t BatteryVolt;
    uint16_t BatteryTemp;
    uint16_t Battery;
    uint16_t ManualComplete;
    char ProtocalVer [20];
};

void func1(void *mqtt_subscriber_)
{
    mqtt_subscriber* mqtt_temp=(mqtt_subscriber*)mqtt_subscriber_;
    mqtt_temp->mqtt_connect("192.168.200.220",1883,"001");
}
void func2(const mqtt_subscriber *mqtt_subscriber_)
{
    sleep(1);//func1이 먼저 connect를 진행해줘야함
    mqtt_subscriber* mqtt_temp=(mqtt_subscriber*)mqtt_subscriber_;
    message_form msg{1,2,3,4,5,6,1,2,3,4,5,6,1,2,3,4,5,6,1,2,3,4,5,6,1,2,3,"aa"};
    while(true)
    {
        mqtt_temp->mqtt_send(&msg);
        sleep(1);
    }
}
int main(int argc, char** argv)
{
    int result=0;
    mqtt_subscriber mqtt_subscriber_;
    thread t1(func1,&mqtt_subscriber_);
    thread t2(func2,&mqtt_subscriber_);
    t1.join();
    t2.join();
    return 0;
}

