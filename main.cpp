#include <iostream>
#include <cpprest/json.h>
#include <cpprest/http_listener.h>
#include <cpprest/uri.h>
#include <fstream>
#include <thread>
#include <vector>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
using namespace std;

bool is_running = true;
vector<json::value> jsonData;

void load_json_data() {
    ifstream file("../sample.json");
    if (file) {
        string json_string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        json::value data = json::value::parse(json_string);  // 파일 내용을 파싱
        for (const auto& item : data.as_array()) {
            jsonData.push_back(item);
        }
    } else {
        cerr << "Failed to open sample.json" << endl;
    }
}

json::value find_person_by_name(const utility::string_t& name, const vector<web::json::value>& jsonData) {
    for (const auto& person : jsonData) {
        if (person.at(U("name")).as_string() == name) { // .at() 사용
            return person;
        }
    }
    return json::value(); // 빈 JSON 객체 반환 (없는 경우)
}

void handle_get(http_request request) {
    cout << "GET request received" << endl;

    auto query = uri::split_query(request.request_uri().query());
    if (query.find(U("name")) != query.end()) {
        auto name = query[U("name")];

        json::value person = find_person_by_name(name, jsonData); // jsonData 전달
        if (!person.is_null()) {
            request.reply(status_codes::OK, person);
        } else {
            json::value error_response;
            error_response[U("error")] = json::value::string(U("Person not found"));
            request.reply(status_codes::NotFound, error_response);
        }
    } else {
        json::value error_response;
        error_response[U("error")] = json::value::string(U("Name parameter is required"));
        request.reply(status_codes::BadRequest, error_response);
    }
}

void handle_post(http_request request) {
    cout << "POST request received" << endl;

    json::value response_data;
    response_data[U("message")] = json::value::string(U("Post, Okay!"));

    request.reply(status_codes::OK, response_data);
}

void handle_shutdown(http_request request) {
    cout << "Shutdown request received" << endl;

    is_running = false;

    json::value response_data;
    response_data[U("message")] = json::value::string(U("Server is shutting down"));

    request.reply(status_codes::OK, response_data);
}

void handle_request(http_request request) {
    cout << "Request received: " << request.request_uri().to_string() << endl;

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
    load_json_data(); 
    uri_builder uri(U("http://localhost:8080"));
    auto addr = uri.to_uri().to_string();
    http_listener listener(addr);

    listener.support(handle_request);

    try {
        listener
            .open()
            .then([&listener]() { cout << "Starting server at: " << listener.uri().to_string() << endl; })
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
