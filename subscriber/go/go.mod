module main

go 1.15

replace github.com/open-edge-insights/eii-configmgr-go => /app/eii-configmgr-go

replace github.com/open-edge-insights/eii-messagebus-go => /app/eii-messagebus-go

replace (
  github.com/coreos/go-systemd => github.com/coreos/go-systemd/v22 latest
)
