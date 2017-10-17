#include "abstractstubserver.h"
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <iostream>

using namespace jsonrpc;
using namespace std;

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
    cout << "Server got notified" << endl;
}
string MyStubServer::sayHello(const string &name)
{
    return "Hello " + name;
}
void MyStubServer::mergeMiningNote(const std::string& hash, const std::string& bits)
{
    cout << "mergeMiningNote received." << endl;
    cout << "hash: " << hash << endl << "bits: " << bits << endl;
}
bool MyStubServer::minedAuxpow(const Json::Value& auxpow)
{
    cout << "Warning: minedAuxpow which should not be sent to here received." << endl;
    return false;
}
int main()
{
    HttpServer httpserver(8382);
    MyStubServer s(httpserver);
    s.StartListening();
    getchar();
    s.StopListening();
    return 0;
}