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

tls_certfile = section:option(FileUpload, "cert_path", translate("Certificate file"))
tls_certfile:depends({use_tls = "1"})



local topic_section = map:section(TypedSection, "topic", translate("Topics"))
topic_section.novaluetext = translate("No topics found. Press add button")
topic_section.addremove = true
topic_section.anonymous = true

local topic = topic_section:option(Value, "topic", translate("Topic"))
topic.datatype = "string"
topic.maxlength = 512
topic.rmempty = false
topic.parse = function(self, section, novld, ...)
	local value = self:formvalue(section)
	Value.parse(self, section, novld, ...)
end

local qos = topic_section:option(ListValue, "qos", translate("Quality of service"))
qos:value("0", "At most once (0)")
qos:value("1", "At least once (1)")
qos:value("2", "Exactly once (2)")
qos.rmempty=false


local event_section = map:section(TypedSection, "event", translate("Events"))
event_section.novaluetext = translate("No events found. Press add button")
event_section.addremove = true
event_section.anonymous = true

local topic = event_section:option(Value, "topic", translate("Topic"))
topic.datatype = "string"
topic.maxlength = 512
topic.rmempty = false
topic.parse = function(self, section, novld, ...)
	local value = self:formvalue(section)
	Value.parse(self, section, novld, ...)
end

local name = event_section:option(Value, "name", translate("Name"))
name.datatype = "string"
name.maxlength = 512
name.rmempty = false
name.parse = function(self, section, novld, ...)
	local value = self:formvalue(section)
	Value.parse(self, section, novld, ...)
end

data_type = event_section:option(Flag, "data_type", "Value is number", "Value is number")

local compare = event_section:option(ListValue, "operator", translate("Compare"))
compare:value("0", "Equal (==)")
compare:value("1", "Less (<)")
compare:value("2", "More (>)")
compare:value("3", "Not equal (!=)")
compare:value("4", "More or equal (>=)")
compare:value("5", "Less or equal (<=)")
compare:depends({data_type = "1"})


topic.parse = function(self, section, novld, ...)
	local value = self:formvalue(section)
	Value.parse(self, section, novld, ...)
end


local variable = event_section:option(Value, "value", translate("Value"))
variable.datatype = "string"
variable.maxlength = 512
variable.rmempty = false
variable.parse = function(self, section, novld, ...)
	local value = self:formvalue(section)
	Value.parse(self, section, novld, ...)
end


sender = event_section:option( Value, "sender_conf", "Sender email")
sender_pass = event_section:option( Value, "sender_pass", "Password")
smtp = event_section:option( Value, "smtp", " SMTP address")
smtp_port = event_section:option( Value, "smtp_port", "SMTP port")

sender.datatype = "string";
smtp.datatype = "string";
sender_pass.password = true;
smtp_port.datatype = "port";


-- local is_group = false
-- mailGroup = event_section:option(ListValue, "sender_conf", translate("Email account"), translate("Recipient's email configuration <br/>(<a href=\"/cgi-bin/luci/admin/system/admin/group/email\" class=\"link\">configure it here</a>)"))
-- map.uci:foreach("user_groups", "email", function(event_section)
-- 	if event_section.username then
-- 		mailGroup:value(event_section.username, event_section.username)
-- 		is_group = true
-- 	end
-- end)
-- if not is_group then
-- 	mailGroup:value(0, translate("No email accounts created"))
-- end

-- function mailGroup.parse(self, section, novld, ...)
-- 	local val = self:formvalue(section)
-- 	if val and val == "0" then
-- 		self:add_error(section, "invalid", translate("No email accounts selected"))
-- 	end
-- 	Value.parse(self, section, novld, ...)
-- end



local receiver = event_section:option(Value, "receiver_mail", translate("Receiver mail"))
receiver.datatype = "string"
receiver.maxlength = 512
receiver.rmempty = false
receiver.parse = function(self, section, novld, ...)
	local value = self:formvalue(section)
	Value.parse(self, section, novld, ...)
end


return map