/*
Copyright (c) 2020 Intel Corporation.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

package main

import (
	eismsgbus "EISMessageBus/eismsgbus"
	eiscfgmgr "ConfigMgr/eisconfigmgr"
	"fmt"
	"time"
)

func start_subscriber() {

	configmgr, _ := eiscfgmgr.ConfigManager()

	// subctx, _ := configMgr.GetSubscriberByName("sample_sub")
	subctx, _ := configmgr.GetSubscriberByIndex(0)
	config, err := subctx.GetMsgbusConfig()
	if(err != nil) {
		fmt.Printf("-- Error get message bus config: %v\n", err)
		return
	}
	
	topics := subctx.GetTopics()

	endpoint := subctx.GetEndPoints()
	fmt.Printf("Subscriber endpoint:%s", endpoint)

	fmt.Printf("-- Initializing message bus context for sub %v\n", config)

	client, err := eismsgbus.NewMsgbusClient(config)
	if err != nil {
		fmt.Printf("-- Error initializing message bus context: %v\n", err)
		return
	}
	defer client.Close()

	subscriber, err := client.NewSubscriber(topics[0])
	if err != nil {
		fmt.Printf("-- Error subscribing to topic: %v\n", err)
		return
	}
	defer subscriber.Close()

	fmt.Println("-- Running Subscriber...")

	for {
		select {
		case msg := <-subscriber.MessageChannel:
			fmt.Printf("-- Received Message from publisher: %v \n", msg)
		case err := <-subscriber.ErrorChannel:
			fmt.Printf("-- Error receiving message: %v\n", err)
		}

		time.Sleep(1 * time.Second)
	}
}
