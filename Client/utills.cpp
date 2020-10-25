#include "utills.h"
#include "client.h"

Client *g_client = nullptr;

Client *Utills::client(){
    if(g_client == nullptr){
        g_client = new Client( "127.0.0.1", 1234);
    }
    return g_client;
}
