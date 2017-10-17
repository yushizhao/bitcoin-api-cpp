#include "abstractstubserver.h"
#include <jsonrpccpp/server/connectors/httpserver.h>

#include "stubclient.h"
#include <jsonrpccpp/client/connectors/httpclient.h>

#include <bitcoinapi/bitcoinapi.h>
#include <bitcoinapi/types.h>

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
        virtual void mergeMiningNote(const std::string& hash, const std::string& bits);
        virtual bool minedAuxpow(const Json::Value& auxpow);
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
void MyStubServer::mergeMiningNote(const std::string& hash, const std::string& bits)
{
    std::cout << "Warning: mergeMiningNote which should not be sent to here received." << std::endl;
}
bool MyStubServer::minedAuxpow(const Json::Value& auxpow)
{
    std::cout << "minedAuxpow received.\n";
    Json::FastWriter fastWriter;  
    std::cout << fastWriter.write(auxpow) << "\n";
    
    return btc.submitauxpow(auxpow);
}

int main()
{
    //call getauxblock    
    mminfo_t res;

    try
    {
        /* Example method - getbalance */
        res = btc.getauxblock();
        std::cout << "hash: " << res.hash << std::endl;
        std::cout << "bits: " << res.bits << std::endl;
    }
    catch(BitcoinException e)
    {
        std::cerr << e.getMessage() << std::endl;
    }

    //send mm_info
    HttpClient httpclient("http://localhost:8382");
    StubClient c(httpclient);
    try
    {
        c.mergeMiningNote(res.hash,res.bits);
    }
    catch (JsonRpcException e)
    {
        std::cerr << e.what() << std::endl;
    }

    //receive and submit block 
    HttpServer httpserver(8383);
    MyStubServer s(httpserver);
    s.StartListening();
    getchar();
    s.StopListening();
    
    return 0;
}