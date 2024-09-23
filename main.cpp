#include <iostream>
#include <cpprest/json.h>
#include <cpprest/http_listener.h>
#include <thread>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
using namespace std;

bool is_running = true;

void handle_get(http_request request) {
    ucout << "GET request received" << std::endl;

    json::value response_data;
    response_data[U("message")] = json::value::string(U("Get, Okay!"));

    request.reply(status_codes::OK, response_data);
}

void handle_post(http_request request) {
    ucout << "POST request received" << std::endl;

    json::value response_data;
    response_data[U("message")] = json::value::string(U("Post, Okay!"));

    request.reply(status_codes::OK, response_data);
}

void handle_shutdown(http_request request) {
    ucout << "Shutdown request received" << std::endl;

    is_running = false;

    json::value response_data;
    response_data[U("message")] = json::value::string(U("Server is shutting down"));

    request.reply(status_codes::OK, response_data);
}

void handle_request(http_request request) {
    ucout << "Request received: " << request.request_uri().to_string() << std::endl;

    if (request.request_uri().path() == U("/shutdown")) {
        handle_shutdown(request);
    } else if (request.method() == methods::GET) {
        handle_get(request);
    } else if (request.method() == methods::POST) {
        handle_post(request);
    } else {
        request.reply(status_codes::MethodNotAllowed, U("Method Not Allowed"));
    }
}

int main() {
    uri_builder uri(U("http://localhost:8080"));
    auto addr = uri.to_uri().to_string();
    http_listener listener(addr);

    listener.support(handle_request);

    try {
        listener
            .open()
            .then([&listener]() { ucout << "Starting server at: " << listener.uri().to_string() << std::endl; })
            .wait();

        while (is_running) {
            this_thread::sleep_for(chrono::seconds(1));
        }

        listener.close().wait();
        cout << "Server Stopped" << endl;
    } catch (const exception& e) {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}