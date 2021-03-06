#include "mqtt_sub.h"

const char *host="192.168.50.220";
const int port = 1883;
const std::string ID = "001";        

const char* enum_ERROR[]={
    "MOSQ_ERR_AUTH_CONTINUE",
	"MOSQ_ERR_NO_SUBSCRIBERS",
	"MOSQ_ERR_SUB_EXISTS",
	"MOSQ_ERR_CONN_PENDING",
	"MOSQ_ERR_SUCCESS",
	"MOSQ_ERR_NOMEM",
	"MOSQ_ERR_PROTOCOL",
	"MOSQ_ERR_INVAL",
	"MOSQ_ERR_NO_CONN",
	"MOSQ_ERR_CONN_REFUSED",
	"MOSQ_ERR_NOT_FOUND",
	"MOSQ_ERR_CONN_LOST",
	"MOSQ_ERR_TLS",
	"MOSQ_ERR_PAYLOAD_SIZE",
	"MOSQ_ERR_NOT_SUPPORTED",
	"MOSQ_ERR_AUTH",
	"MOSQ_ERR_ACL_DENIED",
	"MOSQ_ERR_UNKNOWN",
	"MOSQ_ERR_ERROR",
	"MOSQ_ERR_EAI",
	"MOSQ_ERR_PROXY",
	"MOSQ_ERR_PLUGIN_DEFER",
	"MOSQ_ERR_MALFORMED_UTF8",
	"MOSQ_ERR_KEEPALIVE",
	"MOSQ_ERR_LOOKUP",
	"MOSQ_ERR_MALFORMED_PACKET",
	"MOSQ_ERR_DUPLICATE_PROPERTY",
	"MOSQ_ERR_TLS_HANDSHAKE",
	"MOSQ_ERR_QOS_NOT_SUPPORTED",
	"MOSQ_ERR_OVERSIZE_PACKET",
	"MOSQ_ERR_OCSP"
};

const char* enum_ORDER[]={
    "AR",
    "AC",
    "AP",
    "AD",
    "DC"
};

const char* enum_SEND[]={
    "AMRID",
    "UpperType",
    "X",
    "Y",
    "T",
    "State",
    "ChargeStatus",
    "ActionID",
    "Mode",
    "Loaded_1",
    "Loaded_2",
    "Loaded_3",
    "CurrentNode",
    "NextNode",
    "TargetNode",
    "Obstacle",
    "Speed",
    "MapRate",
    "ErrorCode1",
    "ErrorCode2",
    "ErrorCode3",
    "ErrorCode4",
    "ErrorCode5",
    "BatteryVolt",
    "BatteryTemp",
    "Battery",
    "ManualComplete",
    "Protoco.Ver"
};
//data format
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

mqtt_subscriber::mqtt_subscriber():topic_receive("ACS"+ID+">AMR"+ID),topic_send("AMR"+ID+">ACS"+ID),onetime(false)
{
    // mqtt_connect(host,port,ID);
}
mqtt_subscriber::~mqtt_subscriber()
{
    printf("Exit mqtt_subscriber\n");
    mosquitto_loop_stop(mosq,true);
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}
void mqtt_subscriber::mqtt_connect(const char * host,const int port, const std::string ID)
{
    Print_Error(mqtt_init(host,port,ID));
}

int mqtt_subscriber::mqtt_init(const char * host,const int port, const std::string ID)
{   
    mosquitto_lib_init();
    mosq=mosquitto_new("subscribe-test",true,this);
    if(mosq==NULL)
    {
        printf("can't initialize mqtt library\n");
        return 1;
    }
    error=mosquitto_connect(mosq,host,port,10);
    if(error!=0)
    {
        printf("Subscriber could not connect to broker! Error Code: %d\n",error);
        mosquitto_destroy(mosq);
        return error;
    }
    printf("We are now connected to the broker\n");
    mosquitto_connect_callback_set(mosq,connect);
    mosquitto_message_callback_set(mosq,message);
    // mosquitto_disconnect_callback_set(mosq,disconnect);
    // mosquitto_subscribe_callback_set(mosq,subscribe);
    
    // int loop = mosquitto_loop_start(mosq);
    int loop = mosquitto_loop_forever(mosq,5,1);
    if(loop!=MOSQ_ERR_SUCCESS)
    {
        printf("mosquitto loop error\n");
        return loop;     
    }

    return MOSQ_ERR_SUCCESS;
}
void connect(struct mosquitto *mosq, void *obj, int rc)
{
    mqtt_subscriber* mos=(mqtt_subscriber*) obj;
    if(rc)
    {
        printf("Error with result code: %d\n",rc);
    }
    else
    {
        mosquitto_subscribe(mosq,NULL,(mos->topic_receive).c_str(),0);
    }   
}
void message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
    mqtt_subscriber* mos=(mqtt_subscriber*) obj;
    // printf("New message with topic %s: %X\n",msg->topic,*(uint32_t*)msg->payload);
    char msg_con[msg->payloadlen];
    strcpy(msg_con,(const char*)msg->payload);
    printf("%s\n", msg_con);

    Json::Value root;
    Json::Reader reader;
    reader.parse(msg_con,root);
    mos->sequence=0;
    while(mos->sequence>5)
    {
        if(strcmp(root.get("Cmd","UTF-8").asCString(),enum_ORDER[mos->sequence])==0)
        {
            break;
        }
        mos->sequence++;
    }
    // std::cout <<"aa"<<std::endl;
    if(mos->sequence==0)
    {
        mos->m.lock();
        mos->get_order_vector().push_back(msg_con);
        mos->m.unlock();
    }
    else if(mos->sequence<5&&mos->sequence>0)
    {
        mos->m.lock();
        mos->get_emr_vector().push_back(msg_con);
        mos->m.unlock();
    }
    // mosquitto_publish(mosq,NULL,(mos->topic_send).c_str(),msg->payloadlen,msg,0,0);
}
void mqtt_subscriber::Print_Error(int Error)
{
    Error+=4;
    if(Error!=4)
    {
        printf(ANSI_COLOR_RED "ERROR_OCCUR : %s" ANSI_COLOR_RESET "\n", enum_ERROR[Error]);
        this->~mqtt_subscriber();
    }
    else
    {
        printf(ANSI_COLOR_GREEN "OK" ANSI_COLOR_RESET "\n");
    }
}
void mqtt_subscriber::mqtt_send(const void* data)
{
    std::string msg=convert_array_to_json(data);
    Print_Error(mosquitto_publish(mosq,NULL,topic_send.c_str(),msg.size(),msg.c_str(),0,0));
}
std::string mqtt_subscriber::convert_array_to_json(const void* data)
{   
    Json::Value root;
    Json::StyledWriter writer;
    int arr_size;
    message_form* temp_arr_int=(message_form*)data;

    for(int i=0;i<27;i++)
    {
        root[enum_SEND[i]]=(std::to_string(*((uint16_t*)temp_arr_int+i)));//?????? ?????? ???????????? ????????? ?????????
    }
    root[enum_SEND[27]]=temp_arr_int->ProtocalVer;
    std::string str=writer.write(root);
    return str;
}

std::vector<char*> mqtt_subscriber::get_order_vector()
{
    return order_vector;
}
std::vector<char*> mqtt_subscriber::get_emr_vector()
{
    return emr_vector;
}

char* mqtt_subscriber::get_order_vector_element()
{
    char* elem;
    if(order_vector.empty()==true)
    {
        strcpy(elem,"empty");
    }
    else
    {
        elem=order_vector[0];
        order_vector.erase(order_vector.begin());
    }
    return elem;
}
char* mqtt_subscriber::get_emr_vector_element()
{
    char* elem;
    if(emr_vector.empty()==true)
    {
        strcpy(elem,"empty");
    }
    else
    {
        elem=emr_vector[0];
        order_vector.erase(order_vector.begin());
    }
    return elem;
}