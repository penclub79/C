const char __aszNonce[20] = { 0x9E, 0xBD, 0xBB, 0x53, 0x7C, 0x96, 0xB4, 0xC1, 0xCE, 0xEB,
0xFB, 0x06, 0x17, 0x31, 0x41, 0x4E, 0x5B, 0x68, 0x86, 0x93 };



// probe message
const char __aszProbeXml[] =
{
	"<?xml version=\"1.0\" encoding=\"utf-8\"?>\
		<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:a=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\">\
			<s:Header>\
				<a:Action s:mustUnderstand=\"1\">http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe</a:Action>\
				<a:MessageID>uuid:%s</a:MessageID>\
				<a:ReplyTo>\
					<a:Address>http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous</a:Address>\
				</a:ReplyTo>\
				<a:To s:mustUnderstand=\"1\">urn:schemas-xmlsoap-org:ws:2005:04:discovery</a:To>\
			</s:Header>\
			<s:Body>\
				<Probe xmlns=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\">\
					<d:Types xmlns:d=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\"xmlns:dp0=\"%s\">%s</Types>\
				</Probe>\
			</s:Body>\
		</Envelope>"
};

// Resolve message
const char __aszResolveXml[] =
{
	"<?xml version=\"1.0\" encoding=\"utf-8\"?>\
		<s:Envelope xmlns:s=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:a=\"http://schemas.xmlsoap.org/ws/2004/08/addressing\">\
			<s:Header>\
				<a:Action s:mustUnderstand=\"1\">http://schemas.xmlsoap.org/ws/2005/04/discovery/Resolve</a:Action>\
				<a:MessageID>uuid:%s</a:MessageID>\
				<a:ReplyTo>\
					<a:Address>http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous</a:Address>\
				</a:ReplyTo>\
				<a:To s:mustUnderstand=\"1\">urn:schemas-xmlsoap-org:ws:2005:04:discovery</a:To>\
			</s:Header>\
			<s:Body>\
				<d:Resolve xmlns=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\">\
					<a:ReferenceProperties>%s</a:ReferenceProperties>\
				</d:Resolve>\
			</s:Body>\
		</Envelope>"
};

const char __aszXmlForm[] = 
{
	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
			<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\" xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\">\
				<SOAP-ENV:Body>\
					<tds:GetDeviceInformation/>\
				</SOAP-ENV:Body>\
			</SOAP-ENV:Envelope>"
};