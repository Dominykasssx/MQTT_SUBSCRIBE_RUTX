map = Map("mqtt_subscriber")

section = map:section(NamedSection, "config", "mqtt_subscriber", "MQTT subscriber configuration")

flag = section:option(Flag, "enable", "Enable", "Enable program")

use_tls = section:option( Flag, "use_tls", "Enable TLS", "Enable TLS")
brokerip = section:option( Value, "brokerIp", "Broker IP address")
brokerPort = section:option( Value, "brokerPort", "Broker port")
username = section:option( Value, "username", "Username")
password = section:option( Value, "password", "Password")

username.datatype = "string";
password.password = true;
brokerPort.datatype = "port";
brokerip.datatype = "host";


return map
