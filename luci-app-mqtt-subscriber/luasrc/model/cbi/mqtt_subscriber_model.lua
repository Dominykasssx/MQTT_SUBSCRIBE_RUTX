map = Map("mqtt_subscriber")

section = map:section(NamedSection, "general", "mqtt_subscriber", "MQTT subscriber configuration")

flag = section:option(Flag, "enable", "Enable", "Enable program")

brokerip = section:option( Value, "brokerIp", "Broker IP address")
brokerPort = section:option( Value, "brokerPort", "Broker port")
username = section:option( Value, "username", "Username")
password = section:option( Value, "password", "Password")

username.datatype = "string";
password.password = true;
brokerPort.datatype = "port";
brokerip.datatype = "host";



FileUpload.size = "102401"
FileUpload.sizetext = translate("Selected file is too large, max 100 Kb")
FileUpload.sizetextempty = translate("Selected file is empty")
FileUpload.unsafeupload = true

use_tls = section:option(Flag, "use_tls", "Enable TLS", "Enable TLS")

tls_certfile = section:option(FileUpload, "cert_path", translate("Certificate file"), "")
tls_certfile:depends({use_tls = "1"})

return map