


if (Qt6_FOUND)
	qt_add_executable(newoutput newoutput.cpp)
	add_test(NAME newoutput COMMAND newoutput wss://api.testnet.shimmer.network:443/api/mqtt/v1 rms1qrzgmpr0lzvqxzu73qakkvg7v2qd2lngkaf64w256c76vepag4sqs27e25s)
	target_link_libraries(newoutput PRIVATE qclientMQTT)

endif(Qt6_FOUND)
