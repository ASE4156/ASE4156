#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include "hello.grpc.pb.h"
#include "hello.pb.h"
#include "hello.grpc.pb.h"


int main(int argc, char** argv) {
    std::string target_str = "0.0.0.0:50051";
    std::shared_ptr<grpc::Channel> channel = grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials());
    // std::unique_ptr<HelloWorld::Stub> stub = HelloWorld::NewStub(channel);
    std::unique_ptr<SumService::Stub> stub = SumService::NewStub(channel);

    // HelloRequest request;
    // request.set_name("Hello");

    SumRequest request;
    if(argc != 3) {
        std::cerr << "Usage: ./client <number1> <number2>" << std::endl;
        return 1;
    }

    int32_t a = std::stoi(argv[1]);
    int32_t b = std::stoi(argv[2]);

    request.set_a(a);
    request.set_b(b);

    SumReply reply;
    grpc::ClientContext context;

    grpc::Status status = stub->Summation(&context, request, &reply);

    if (status.ok()) {
        std::cout << "Reply received: " << reply.result() << std::endl;
    } else {
        std::cout << "RPC failed with code " << status.error_code() << ": " << status.error_message() << std::endl;
    }

    return 0;
}
