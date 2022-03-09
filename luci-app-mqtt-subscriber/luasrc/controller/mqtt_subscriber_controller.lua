module("luci.controller.mqtt_subscriber_controller", package.seeall)

function index()
	entry({"admin", "services", "mqtt_config"}, cbi("mqtt_subscriber_model"), _("Mqtt subscriber"),104)
end
