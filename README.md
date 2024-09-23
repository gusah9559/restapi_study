# restapi_study

## Required

```bash
sudo apt update
sudo apt install libcpprest-dev
```

## Run Server

```bash
mkdir build
cd build
cmake ..
make -j16
./restapi
```

## Request

### Can using test.sh

```bash
curl -X GET http://localhost:8080
curl -X POST http://localhost:8080
curl -X GET http://localhost:8080/shutdown
```
