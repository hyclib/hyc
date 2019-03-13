#include "../../preheader.h"

#ifdef HYC_COMPILE_MODULE_NET

#include "../../../include/network/smtp/smtp.h"
#include "../../../include/network/socket/tcpTransceiver.h"
#include "../../../include/common/cnObjectRef.h"
#include "../../../include/network/netType.h"
#include "../../../include/network/smtp/smtpType.h"
#include "../../../include/container/byteStream.h"

namespace hyc
{

#define MAX_RECV_BUF    512

	class Smtp::PrivateClass : public MemoryAllocObject
	{
	public:
		// host info
		cString     mHost;
		PORT        mPort;

		cString		mUserName;
		cString		mPassWord;
		cString		mFrom;
		cString		mTo;

		static cString&		GetNumStr(char* pBuf, cString& outStr)
		{
			outStr = cString(pBuf, 3);
			return outStr;
		}

		static cString&		Base64Encode(cString& src, cString& dst)
		{
			static const char *base64 = (char*)("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
			char char_array_3[3] = {0};
			char char_array_4[4] = {0};
			s32 i = 0;
			s32 j = 0;
			s32 srcLen = src.size();
			dst = "";
			s32 index = 0;
			while (srcLen--) {
				char_array_3[i++] = src[index++];
				if (i == 3) {
					char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
					char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
					char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
					char_array_4[3] = char_array_3[2] & 0x3f;

					for(i = 0; (i <4) ; i++)
						dst += base64[char_array_4[i]];
					i = 0;
				}
			}

			
			
			if (i)
			{
				for(j = i; j < 3; j++)
					char_array_3[j] = '0';

				char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
				char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
				char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
				char_array_4[3] = char_array_3[2] & 0x3f;

				for (j = 0; (j < i + 1); j++)
					dst += base64[char_array_4[j]];

				while((i++ < 3))
					dst += '=';
			}
			return dst;
		}
		struct sockaddr_storage		mConnectAddr;
	};

	Smtp::Smtp()
	{
		PRIVATE_CLASS_CREATE;
	}

	Smtp::~Smtp()
	{
		PRIVATE_CLASS_DESTROY;
	}

	void Smtp::setHostInfo( const char* host, u32 port )
	{
		 PRIVATE_CLASS_GET(p);
		 p->mHost = host;
		 p->mPort = port;

		 String strHost;
#if defined(_UNICODE) || defined(UNICODE)
		 StringUtils::Narrow2wide(host, strHost);
#else
		 strHost = host;
#endif

		 p->mConnectAddr = SocketUtils::AddrCreate(strHost, port, ProtocolSupport_IPv4, false);
	}

	void Smtp::setUserInfo( const char* name, const char* passWord )
	{
		PRIVATE_CLASS_GET(p);
		p->mUserName = name;
		p->mPassWord = passWord;
	}

	void Smtp::setFrom( const char* name )
	{
		PRIVATE_CLASS_GET(p);
		p->mFrom = name;
	}

	void Smtp::setTo( const char* name )
	{
		PRIVATE_CLASS_GET(p);
		p->mTo = name;
	}

	s32	_Send(const cString& cmdStr, TcpTransceiver* socket, cString r)
	{
		static cString rNum;
		static char bufSzRecv[MAX_RECV_BUF];
		static ByteStream buf(false);
		buf.clear();
		buf.add((Byte*)cmdStr.c_str(), (u32)cmdStr.size());
		bool rSend = socket->write(&buf);
		if(!rSend)
		{
			return StmpResult_net_error;
		}
		s32 slen = (s32)socket->readPart(bufSzRecv, MAX_RECV_BUF);
		if ( slen <= 0 )
		{
			socket->close();
			return StmpResult_net_error;
		}
		if(Smtp::PrivateClass::GetNumStr(bufSzRecv, rNum) != r)
		{
			socket->close();
			return -1;
		}

		return StmpResult_ok;
	}

	s32 Smtp::send( const String& msg, const String& subject)
	{
		PRIVATE_CLASS_GET(p);

		ObjectRef<TcpTransceiver> netObj = ObjectRef<TcpTransceiver>(	new TcpTransceiver());
		TcpTransceiver* pTcpTransceiver = netObj.get();
		pTcpTransceiver->initialize();

		// ���ӷ�����
		if ( ConnectResult_Ok != pTcpTransceiver->connect(p->mConnectAddr) )
		{
			pTcpTransceiver->close();
			return StmpResult_timeout_connect;
		}
		
		cString rNum;
		char bufSzRecv[MAX_RECV_BUF];
		s32 slen = (s32)pTcpTransceiver->readPart(bufSzRecv, MAX_RECV_BUF);
		if ( slen <= 0 )
		{
			pTcpTransceiver->close();
			return StmpResult_net_error;
		}

		if(Smtp::PrivateClass::GetNumStr(bufSzRecv, rNum) != "220")
		{
			pTcpTransceiver->close();
			return StmpResult_server_refuse;
		}
		
		cString cmdStr;
		s32 r;

		// ����������Ϣ
		cmdStr = "HELO ";
		cmdStr += p->mHost;
		cmdStr += "\r\n";
		r = _Send(cmdStr, pTcpTransceiver, "250");
		if(0 != r)
		{
			if(-1 == r)
			{
				return StmpResult_server_refuse;
			}
			return r;
		}

		// ��¼
		cmdStr = "auth login\r\n";
		r = _Send(cmdStr, pTcpTransceiver, "334");
		if(0 != r)
		{
			if(-1 == r)
			{
				return StmpResult_error_hello;
			}
			return r;
		}

		// �û���
		Smtp::PrivateClass::Base64Encode(p->mUserName, cmdStr);
		cmdStr += "\r\n";
		r = _Send(cmdStr, pTcpTransceiver, "334");
		if(0 != r)
		{
			if(-1 == r)
			{
				return StmpResult_username_error;
			}
			return r;
		}

		// ����
		Smtp::PrivateClass::Base64Encode(p->mPassWord, cmdStr);
		cmdStr += "\r\n";
		r = _Send(cmdStr, pTcpTransceiver, "235");
		if(0 != r)
		{
			if(-1 == r)
			{
				return StmpResult_passwrod_error;
			}
			return r;
		}		

		// set the sender
		cmdStr = "mail from:<";
		cmdStr += p->mUserName;
		cmdStr += ">\r\n";
		r = _Send(cmdStr, pTcpTransceiver, "250");
		if(0 != r)
		{
			if(-1 == r)
			{
				return StmpResult_sender_error;
			}
			return r;
		}

		// set the target
		cmdStr = "rcpt to:<";
		cmdStr += p->mTo;
		cmdStr += ">\r\n";
		r = _Send(cmdStr, pTcpTransceiver, "250");
		if(0 != r)
		{
			if(-1 == r)
			{
				return StmpResult_target_error;
			}
			return r;
		}

		// data
		cmdStr = "data\r\n";
		r = _Send(cmdStr, pTcpTransceiver, "354");
		if(0 != r)
		{
			if(-1 == r)
			{
				return StmpResult_target_error;
			}
			return r;
		}

		cString strSubject;
		cString strMsg;
#if defined(_UNICODE) || defined(UNICODE)
		StringUtils::Wide2narrow(subject, strSubject);
		StringUtils::Wide2narrow(msg, strMsg);
#else
		strSubject = subject;
		strMsg = msg;
#endif

		// msg
		cmdStr = "from:";
		cmdStr += p->mFrom;
		cmdStr += "\r\nto:";
		cmdStr += p->mTo;
		cmdStr += "\r\nSubject:";
		cmdStr += strSubject;
		cmdStr += "\r\n\r\n";
		cmdStr += strMsg;
		cmdStr += "\r\n.\r\n";
		r = _Send(cmdStr, pTcpTransceiver, "250");
		if(0 != r)
		{
			if(-1 == r)
			{
				return StmpResult_content_error;
			}
			return r;
		}

		// quit
		cmdStr = "quit\r\n";
		r = _Send(cmdStr, pTcpTransceiver, "221");
		if(0 == r)
		{
			pTcpTransceiver->close();
		}
		return StmpResult_ok;
	}
}

#endif // HYC_COMPILE_MODULE_NET
