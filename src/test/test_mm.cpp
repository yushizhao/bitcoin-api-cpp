#include "abstractstubserver.h"
#include <jsonrpccpp/server/connectors/httpserver.h>

#include "stubclient.h"
#include <jsonrpccpp/client/connectors/httpclient.h>

#include <bitcoinapi/bitcoinapi.h>
#include <bitcoinapi/types.h>

#include <boost/thread.hpp>

using namespace jsonrpc;

std::string username = "rpcuser";
std::string password = "bitcoinzero";
std::string address = "127.0.0.1";
int port = 18232;

/* Constructor to connect to the bitcoin daemon */
BitcoinAPI btc(username, password, address, port);

class MyStubServer : public AbstractStubServer
{
    public:
        MyStubServer(AbstractServerConnector &connector);

        virtual void notifyServer();
        virtual std::string sayHello(const std::string& name);
        virtual std::string rpc_mergeMiningNote(const std::string& hash, const std::string& bits);
        virtual std::string rpc_minedAuxpow(const std::string& param1, const std::string& param2, const std::string& param3, const Json::Value& param4, int param5, const Json::Value& param6, int param7);
};

MyStubServer::MyStubServer(AbstractServerConnector &connector) :
    AbstractStubServer(connector)
{
}
void MyStubServer::notifyServer()
{
    std::cout << "Server got notified" << std::endl;
}
std::string MyStubServer::sayHello(const std::string &name)
{
    return "Hello " + name;
}
std::string MyStubServer::rpc_mergeMiningNote(const std::string& hash, const std::string& bits)
{
    std::cout << "Warning: mergeMiningNote which should not be sent to here received." << std::endl;
}

std::string MyStubServer::rpc_minedAuxpow(const std::string& param1, const std::string& param2, const std::string& param3, const Json::Value& param4, int param5, const Json::Value& param6, int param7)
{
    std::cout << "minedAuxpow received.\n";
    
    Json::Value params;
    params.append(param1);
    params.append(param2);
    params.append(param3);
    params.append(param4);
    params.append(param5);
    params.append(param6);
    params.append(param7);
    
    Json::FastWriter fastWriter;  
    std::cout << fastWriter.write(params) << "\n";
    
    if (btc.submitauxpow(params))
        return "auxpow accepted";
    else
        return "auxpow invalid";
}

void open_port(int n) {    
    HttpServer httpserver(n);
    MyStubServer s(httpserver);
    s.StartListening();
    getchar();
    s.StopListening();
}

void call_rpc(std::string address) {
    //call getauxblock    
    mminfo_t res;

    //send mm_info
    HttpClient httpclient(address);
    StubClient c(httpclient);
    
    //rpc_mergeMiningNote feedback
    std::string fb;
    
    while (true) {
        try
        {
            res = btc.getauxblock();
            std::cout << "hash: " << res.hash << std::endl;
            std::cout << "bits: " << res.bits << std::endl;
        }
        catch(BitcoinException e)
        {
            std::cerr << e.getMessage() << std::endl;
        }

        try
        {
            fb = c.rpc_mergeMiningNote(res.bits,res.hash);
            std::cout << fb << std::endl;
        }
        catch (JsonRpcException e)
        {
            std::cerr << e.what() << std::endl;
        }
        boost::this_thread::sleep(boost::posix_time::seconds(3)); 
    }
}

int main()
{
    boost::thread rpc(call_rpc,"http://192.168.2.110:8382");
    boost::thread port(open_port,8383);
    
    rpc.join();    
    port.join();
    
    return 0;
}