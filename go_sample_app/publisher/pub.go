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
	eiscfgmgr "ConfigMgr/eisconfigmgr"
	eismsgbus "EISMessageBus/eismsgbus"
	"flag"
	"fmt"
	"time"

	"github.com/golang/glog"
)

func start_publisher() {
	flag.Parse()

	configmgr, err := eiscfgmgr.ConfigManager()
	if err != nil {
		glog.Fatal("Config Manager initialization failed...")
	}
	defer configmgr.Destroy()

	// pubctx,_ := config_mgr.GetPublisherByName("sample_pub")
	pubctx, err := configmgr.GetPublisherByIndex(0)
	if err != nil {
		glog.Errorf("Error: %v to GetPublisherByIndex", err)
		return
	}
	defer pubctx.Destroy()

	endpoint, err := pubctx.GetEndPoints()
	if err != nil {
		glog.Errorf("Error: %v to GetEndPoints", err)
		return
	}

	fmt.Println("Publisher's endpoint:", endpoint)

	topics, err := pubctx.GetTopics()
	if err != nil {
		glog.Errorf("Error: %v to GetTopics", err)
		return
	}

	fmt.Println("Publisher Topics")

	config, err := pubctx.GetMsgbusConfig()
	if err != nil {
		fmt.Printf("Error occured with error:%v", err)
		return
	}

	client, err := eismsgbus.NewMsgbusClient(config)
	if err != nil {
		fmt.Printf("-- Error initializing message bus context: %v\n", err)
		return
	}
	defer client.Close()

	fmt.Printf("-- Creating publisher for topic %s\n", topics[0])
	publisher, err := client.NewPublisher(topics[0])
	if err != nil {
		fmt.Printf("-- Error creating publisher: %v\n", err)
		return
	}
	defer publisher.Close()

	fmt.Println("-- Running publisher...")

	sMsg := map[string]interface{}{
		"str":   "hello",
		"count": 0,
	}
	count := 0

	data, err := configmgr.GetAppConfig()
	if err != nil {
		fmt.Println("Error found to get app config:", err)
	}

	strdata := fmt.Sprintf("%v", data["loop_interval"])
	loop_interval, _ := time.ParseDuration(strdata + "s")

	for {
		sMsg["count"] = count

		err = publisher.Publish(sMsg)

		if err != nil {
			fmt.Printf("-- Failed to publish message: %v \n", err)
		} else {
			fmt.Printf("-- Published message : %v \n", sMsg)
		}

		count++
		time.Sleep(loop_interval)
	}
}
