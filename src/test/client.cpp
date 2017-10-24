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
        Json::Value txBranch(Json::arrayValue);
        txBranch.append("050ac4a1a1e1bce0c48e555b1a9f935281968c72d6379b24729ca0425a3fc3cb");
        txBranch.append("0000000000000000000000000000000000000000000000000000000000000000");
        int chainIndex = 0;
        std::string hash = "b19e23748f3273316489b096bd379fa010df608ef0f0ccc0bb80d849eaea9a3b";
        std::string header = "0100000008be13295c03e67cb70d00dae81ea06e78b9014e5ceb7d9ba504000000000000e0fd42db8ef6d783f079d126bea12e2d10c104c0927cd68f954d856f9e8111e59a23904e5dee091a1c655086";
        std::string tx = "0101000000010000000000000000000000000000000000000000000000000000000000000000ffffffff080469b2001b010152ffffffff0100f2052a0100000043410489fe91e62847575c98deeab020f65fdff17a3a870ebb05820b414f3d8097218ec9a65f1e0ae0ac35af7247bd79ed1f2a24675fffb5aa6f9620e1920ad4bf5aa6ac00000000";       
        Json::Value chainBranch(Json::arrayValue);
        chainBranch.append("2222222222222222222222222222222222222222222222222222222222222222");
        chainBranch.append("1111111111111111111111111111111111111111111111111111111111111111");
        int txIndex = 0;

        cout << c.rpc_minedAuxpow(hash,header,tx,txBranch,txIndex,chainBranch,chainIndex) << "\n";
    }
    catch (JsonRpcException e)
    {
        cerr << e.what() << endl;
    }
}