/*
Copyright (c) 2021 Intel Corporation

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

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
	eiicfgmgr "github.com/open-edge-insights/eii-configmgr-go/eiiconfigmgr"
	eiimsgbus "github.com/open-edge-insights/eii-messagebus-go/eiimsgbus"
	"fmt"
	"strconv"
	"time"

	"github.com/golang/glog"
)

func start_client() {
	configmgr, err := eiicfgmgr.ConfigManager()
	if err != nil {
		glog.Fatal("Config Manager initialization failed...")
	}
	defer configmgr.Destroy()

	numOfClients, _ := configmgr.GetNumClients()
	if numOfClients == -1 {
		glog.Errorf("No client instances found, exiting...")
		return
	}

	// clientctx, err := configmgr.GetClientByName("echo_client")
	clientctx, err := configmgr.GetClientByIndex(0)
	if err != nil {
		glog.Fatalf("-- Error to get client object: %v\n", err)
	}
	defer clientctx.Destroy()

	interfaceVal, err := clientctx.GetInterfaceValue("Name")
	if err != nil {
		fmt.Printf("Error to GetInterfaceValue of 'Name': %v\n", err)
		return
	}

	serviceName, err := interfaceVal.GetString()
	if err != nil {
		fmt.Printf("Error to GetString value of 'Name'%v\n", err)
		return
	}

	config, err := clientctx.GetMsgbusConfig()

	if err != nil {
		glog.Fatalf("-- Error to get message bus config: %v\n", err)
	}

	client1, err := eiimsgbus.NewMsgbusClient(config)
	if err != nil {
		fmt.Printf("-- Error initializing message bus context: %v\n", err)
		return
	}
	defer client1.Close()

	fmt.Printf("-- Initializing service requester %s\n", serviceName)
	serviceRequester, err := client1.GetService(serviceName)
	if err != nil {
		fmt.Printf("-- Error initializing service requester: %v\n", err)
		return
	}
	defer serviceRequester.Close()
	fmt.Println("-- Running Client...")

	req := map[string]interface{}{"command": "C"}
	count := 1

	data, err := configmgr.GetAppConfig()
	if err != nil {
		fmt.Println("Error found to get app config:", err)
	}

	strdata := fmt.Sprintf("%v", data["loop_interval"])
	loop_interval, _ := time.ParseDuration(strdata + "s")

	for {
		var cmd interface{} = req["command"].(string) + strconv.Itoa(count)
		req["command"] = cmd

		if count == 10 {
			count = 0
			req["command"] = "C"
		} else {
			count++
		}

		err := serviceRequester.Request(req)
		if err != nil {
			fmt.Printf("-- Error sending request: %v\n", err)
			return
		}

		res, err := serviceRequester.ReceiveResponse(-1)
		if err != nil {
			fmt.Printf("-- Error receiving response: %v\n", err)
			return
		}

		fmt.Printf("---Client sent Request %v and received response %v\n", req, res)
		time.Sleep(loop_interval)
	}
}
