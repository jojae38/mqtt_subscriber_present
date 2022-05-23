#include "mqtt_pub.h"

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

mqtt_publisher::mqtt_publisher():topic_send("ACS"+ID+">AMR"+ID),topic_receive("AMR"+ID+">ACS"+ID),onetime(false)
{
    message_form_pointer=new struct message_form;
    // mqtt_connect(host,port,ID);
}
mqtt_publisher::~mqtt_publisher()
{
    delete message_form_pointer;
    printf("Exit mqtt_publisher\n");
    mosquitto_loop_stop(mosq,true);
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
}
void mqtt_publisher::mqtt_connect(const char * host,const int port, const std::string ID)
{
    Print_Error(mqtt_init(host,port,ID));
}

int mqtt_publisher::mqtt_init(const char * host,const int port, const std::string ID)
{   
    mosquitto_lib_init();
    mosq=mosquitto_new("publish-test",true,this);
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
    mqtt_publisher* mos=(mqtt_publisher*) obj;
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
    mqtt_publisher* mos=(mqtt_publisher*) obj;
    // printf("New message with topic %s: %X\n",msg->topic,*(uint32_t*)msg->payload);
    char msg_con[msg->payloadlen];
    strcpy(msg_con,(const char*)msg->payload);
    // printf("%s\n", msg_con);

    Json::Value root;
    Json::Reader reader;
    reader.parse(msg_con,root);
    

    message_form* temp=(message_form*)(mos->message_form_pointer);
    // temp->ARMID=stoi(root.get(enum_SEND[0],"UTF-8").asString());
    // temp->UpperType=stoi(root.get(enum_SEND[1],"UTF-8").asString());
    // temp->X=stoi(root.get(enum_SEND[2],"UTF-8").asString());
    // temp->Y=stoi(root.get(enum_SEND[3],"UTF-8").asString());
    // temp->T=stoi(root.get(enum_SEND[4],"UTF-8").asString());
    // temp->State=stoi(root.get(enum_SEND[5],"UTF-8").asString());
    // temp->ChargeStatus=stoi(root.get(enum_SEND[6],"UTF-8").asString());
    // temp->ActionID=stoi(root.get(enum_SEND[7],"UTF-8").asString());
    // temp->Mode=stoi(root.get(enum_SEND[8],"UTF-8").asString());
    // temp->Loaded_1=stoi(root.get(enum_SEND[9],"UTF-8").asString());
    // temp->Loaded_3=stoi(root.get(enum_SEND[11],"UTF-8").asString());
    // temp->Loaded_2=stoi(root.get(enum_SEND[10],"UTF-8").asString());
    // temp->CurrentNode=stoi(root.get(enum_SEND[12],"UTF-8").asString());
    // temp->NextNode=stoi(root.get(enum_SEND[13],"UTF-8").asString());
    // temp->TargetNode=stoi(root.get(enum_SEND[14],"UTF-8").asString());
    // temp->Obstacle=stoi(root.get(enum_SEND[15],"UTF-8").asString());
    // temp->Speed=stoi(root.get(enum_SEND[16],"UTF-8").asString());
    // temp->MapRate=stoi(root.get(enum_SEND[17],"UTF-8").asString());
    // temp->ErrorCode1=stoi(root.get(enum_SEND[18],"UTF-8").asString());
    // temp->ErrorCode2=stoi(root.get(enum_SEND[19],"UTF-8").asString());
    // temp->ErrorCode3=stoi(root.get(enum_SEND[20],"UTF-8").asString());
    // temp->ErrorCode4=stoi(root.get(enum_SEND[21],"UTF-8").asString());
    // temp->ErrorCode5=stoi(root.get(enum_SEND[22],"UTF-8").asString());
    // temp->BatteryVolt=stoi(root.get(enum_SEND[23],"UTF-8").asString());
    // temp->BatteryTemp=stoi(root.get(enum_SEND[24],"UTF-8").asString());
    // temp->Battery=stoi(root.get(enum_SEND[25],"UTF-8").asString());
    // temp->ManualComplete=stoi(root.get(enum_SEND[26],"UTF-8").asString());
    // strcpy(temp->ProtocalVer,root.get(enum_SEND[27],"UTF-8").asCString());


    // std::cout << "bb"<<std::endl;
    for(int i=0;i<27;i++)
    {
        *((uint16_t*)(mos->message_form_pointer)+i)=stoi(root.get(enum_SEND[i],"UTF-8").asString());
    }
    strcpy(temp->ProtocalVer,root.get(enum_SEND[27],"UTF-8").asCString());
    // mosquitto_publish(mosq,NULL,(mos->topic_send).c_str(),msg->payloadlen,msg,0,0);
}
void mqtt_publisher::Print_Error(int Error)
{
    Error+=4;
    if(Error!=4)
    {
        printf(ANSI_COLOR_RED "ERROR_OCCUR : %s" ANSI_COLOR_RESET "\n", enum_ERROR[Error]);
        this->~mqtt_publisher();
    }
    else
    {
        printf(ANSI_COLOR_GREEN "OK" ANSI_COLOR_RESET "\n");
    }
}
void mqtt_publisher::mqtt_send(const void* data)
{
    std::string msg=convert_array_to_json(data);
    Print_Error(mosquitto_publish(mosq,NULL,topic_send.c_str(),msg.size(),msg.c_str(),0,0));
}
std::string mqtt_publisher::convert_array_to_json(const void* data)
{   
    Json::Value root;
    Json::StyledWriter writer;
    int arr_size;
    message_form* temp_arr_int=(message_form*)data;

    for(int i=0;i<27;i++)
    {
        root[enum_SEND[i]]=(std::to_string(*((uint16_t*)temp_arr_int+i)));//이게 맞나 모르겠네 공부좀 해보자
    }
    root[enum_SEND[27]]=temp_arr_int->ProtocalVer;
    std::string str=writer.write(root);
    return str;
}
void mqtt_publisher::print_current_state()
{
    message_form* temp=(message_form*) message_form_pointer;
    std::cout <<"=============================="<<std::endl;
    for(int i=0;i<27;i++)
    {
        std::cout <<"="<<i+1<<". "<<enum_SEND[i]<<": "<<*(((uint16_t*)message_form_pointer)+i)<<std::endl;
    }
    std::cout <<"="<<28<<". "<<enum_SEND[27]<<": "<<temp->ProtocalVer<<std::endl;
    std::cout <<"=============================="<<std::endl;
}
void mqtt_publisher::check_order()
{
    int order=0;
    std::cout << "====================" <<std::endl;
    std::cout << "=1.send AR message =" <<std::endl;
    std::cout << "=2.send AC message =" <<std::endl;
    std::cout << "=3.send AP message =" <<std::endl;
    std::cout << "=4.send AD message =" <<std::endl;
    std::cout << "=5.send DC message =" <<std::endl;
    std::cout << "=6.print Status ====" <<std::endl;
    std::cout << "=7.exit ============" <<std::endl;

    while(true)
    {
        int key=-1;
        std::cin>> key;
        if(key==1)
        {
            AR_publish();
            break;
        }
        else if(key==2)
        {
            AC_publish();
            break;

        }
        else if(key==3)
        {
            AP_publish();
            break;

        }
        else if(key==4)
        {
            AD_publish();
            break;
        }
        else if(key==5)
        {
            DC_publish();
            break;
        }
        else if(key==6)
        {
            print_current_state();
            break;
        }
        else if(key==7)
        {
            this->~mqtt_publisher();
            exit(-1);
            break;
        }
        else
        {
            std::cout <<"Wrong Order try again"<<std::endl;
        }
    }  
}

void mqtt_publisher::AR_publish()
{
    Json::Value root;
    Json::StyledWriter writer;
    root["Cmd"]="AR";
    std::string msg=writer.write(root);
    mosquitto_publish(mosq,NULL,(topic_send).c_str(),msg.size(),msg.c_str(),0,0);
}
void mqtt_publisher::AC_publish()
{
    Json::Value root;
    Json::StyledWriter writer;
    root["Cmd"]="AC";
    std::string msg=writer.write(root);
    mosquitto_publish(mosq,NULL,(topic_send).c_str(),msg.size(),msg.c_str(),0,0);
}
void mqtt_publisher::AP_publish()
{
    Json::Value root;
    Json::StyledWriter writer;
    root["Cmd"]="AP";
    std::string msg=writer.write(root);
    mosquitto_publish(mosq,NULL,(topic_send).c_str(),msg.size(),msg.c_str(),0,0);
}
void mqtt_publisher::AD_publish()
{
    Json::Value root;
    Json::StyledWriter writer;
    root["Cmd"]="AD";
    std::string msg=writer.write(root);
    mosquitto_publish(mosq,NULL,(topic_send).c_str(),msg.size(),msg.c_str(),0,0);    
}
void mqtt_publisher::DC_publish()
{
    Json::Value root;
    Json::StyledWriter writer;
    root["Cmd"]="DC";
    std::string msg=writer.write(root);
    mosquitto_publish(mosq,NULL,(topic_send).c_str(),msg.size(),msg.c_str(),0,0);
}