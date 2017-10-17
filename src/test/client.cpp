#include <iostream>

#include "stubclient.h"
#include <jsonrpccpp/client/connectors/httpclient.h>

using namespace jsonrpc;
using namespace std;

int main()
{
    HttpClient httpclient("http://localhost:8383");
    StubClient c(httpclient);
    try
    {
        cout << c.sayHello("Peter") << endl;
        c.notifyServer();
        
        Json::Value auxpow;
        
        Json::Value txBranch(Json::arrayValue);
        txBranch.append("050ac4a1a1e1bce0c48e555b1a9f935281968c72d6379b24729ca0425a3fc3cb");
        txBranch.append("0000000000000000000000000000000000000000000000000000000000000000");
        int chainIndex = 0;
        std::string hash = "939dd7dcd456699e9245ace0967184b2d43cfd7888466f9735013b0a7b7f24fa";
        std::string header = "0100000008be13295c03e67cb70d00dae81ea06e78b9014e5ceb7d9ba504000000000000e0fd42db8ef6d783f079d126bea12e2d10c104c0927cd68f954d856f9e8111e59a23904e5dee091a1c655086";
        std::string tx = "01000000017b1eabe0209b1fe794124575ef807057c77ada2138ae4fa8d6c4de0398a14f3f00000000494830450221008949f0cb400094ad2b5eb399d59d01c14d73d8fe6e96df1a7150deb388ab8935022079656090d7f6bac4c9a94e0aad311a4268e082a725f8aeae0573fb12ff866a5f01ffffffff01f0ca052a010000001976a914cbc20a7664f2f69e5355aa427045bc15e7c6c77288ac00000000";       
        Json::Value chainBranch(Json::arrayValue);
        chainBranch.append("2222222222222222222222222222222222222222222222222222222222222222");
        chainBranch.append("1111111111111111111111111111111111111111111111111111111111111111");
        int txIndex = 0;
        
        auxpow.append(hash);
        auxpow.append(header);
        auxpow.append(tx);
        auxpow.append(txBranch);
        auxpow.append(txIndex);
        auxpow.append(chainBranch);
        auxpow.append(chainIndex);   

        Json::FastWriter fastWriter;        
        cout << fastWriter.write(auxpow) << "\n";

        if (c.minedAuxpow(auxpow)) {
            cout << "admitted\n"; 
        } else {
            cout << "rejected\n";
        };
    }
    catch (JsonRpcException e)
    {
        cerr << e.what() << endl;
    }
}