#!/bin/bash

count=0

while true; do
	    count=$((count + 1))
	    echo "Count: $count"

		if (( count % 3 == 0 )); then
			curl -X GET http://localhost:8080
		elif ((count % 3 == 1 )); then
			curl -X POST http://localhost:8080
		else
			# curl -X GET http://localhost:8080/shutdown		
			curl -X GET "http://localhost:8080?name=Name1"		    
		fi
			sleep 1
		done
