#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ServerLibrary/InternetProtocol/JsonClientProtocolConnection.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Pong
{
	namespace Internet {
		class FakeConnection :public Connection
		{
		public:
			unsigned id;
			std::vector<std::vector<byte>> sendedMessages;
			std::vector<std::string> receivedMessages;
			int currentMessage = 0;

			explicit FakeConnection(unsigned id) :id(id) {}

			std::vector<byte> ReadAllBytes() override {
				if (currentMessage >= receivedMessages.size())
					return std::vector<byte>();
				auto message = receivedMessages[currentMessage++];
				std::vector<byte> result(message.begin(), message.end());
				result.push_back(0);
				return result;
			}
			std::vector<byte> ReadBytesToDelimiter(byte delimiter) override {
				return ReadAllBytes();
			}
			std::vector<byte> ReadBytes(unsigned size) override {
				return ReadAllBytes();
			}
			void SendBytes(std::vector<byte> bytes) override {
				sendedMessages.push_back(bytes);
			}
			bool IsSomethingToReceive()
			{
				return currentMessage < receivedMessages.size();
			}
			unsigned GetId() override { return id; }
			void SetDisconnectAction(std::function<void(Connection*)>) override {}
			void SetTransmissionErrorAction(std::function<void(Connection*)>) override {}
			void Abort(){}
		};


		TEST_CLASS(JsonServerProtocolConnetionTests)
		{
		public:
			/*Removed because use threads

			TEST_METHOD(GetMyId_ServerSendedId_CheckReadedUserFromServer)
			{
				auto fakeConnection = std::make_shared<FakeConnection>(1);
				fakeConnection->receivedMessages.push_back(R"({"playerId":0})");
				JsonClientProtocolConnection client(fakeConnection);

				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				
				auto result=client.GetMyId();

				Assert::AreEqual((int)UserIds::Player1,(int)result);
			}
			
			TEST_METHOD(GetLatestObjectsFromServer_ServerSendedOneMessageWithTwoObjects_CheckSavedUser)
			{
				auto fakeConnection = std::make_shared<FakeConnection>(1);
				fakeConnection->receivedMessages
					.push_back(R"R([{"lives":3,"type":2,"x":0.5,"y":0.0,"shielded":true},{"lives":4,"type":1,"x":0.25,"y":0.125,"shielded":false}])R");
				JsonClientProtocolConnection client(fakeConnection);

				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				
				auto result = client.GetLatestObjectsFromServer();

				std::vector<ConnectionObject> expected{ {
						0.5f,0.f, ConnectionObject::Type::BallRed,3,true
					},{
						0.25f,0.125f, ConnectionObject::Type::BallCommon,4,false
					} };

				Assert::IsTrue(expected==result);
			}
			TEST_METHOD(GetLatestObjectsFromServer_ServerSendedTwoMessageWithTwoObjects_CheckSavedUser)
			{
				auto fakeConnection = std::make_shared<FakeConnection>(1);
				fakeConnection->receivedMessages
					.push_back(R"R([{"lives":3,"type":4,"x":0.5,"y":0.0,"shielded":true},{"lives":4,"type":4,"x":0.25,"y":0.125,"shielded":false}])R");
				fakeConnection->receivedMessages
					.push_back(R"R([{"lives":3,"type":2,"x":0.5,"y":0.0,"shielded":true},{"lives":4,"type":1,"x":0.25,"y":0.125,"shielded":false}])R");				
				JsonClientProtocolConnection client(fakeConnection);

				std::this_thread::sleep_for(std::chrono::milliseconds(1000));

				auto result = client.GetLatestObjectsFromServer();

				std::vector<ConnectionObject> expected{ {
						0.5f,0.f, ConnectionObject::Type::BallRed,3,true
					},{
						0.25f,0.125f, ConnectionObject::Type::BallCommon,4,false
					} };

				Assert::IsTrue(expected == result);
			}*/

			class FakeClock: public Clock
			{
				std::chrono::microseconds GetNow() override{
					return std::chrono::microseconds(100);
				}
			};

			TEST_METHOD(SendActionToServer_ServerSendedTwoMessageWithTwoObjects_CheckSavedUser)
			{
				auto fakeConnection = std::make_shared<FakeConnection>(1);
				JsonClientProtocolConnection client(fakeConnection);

				client.SetClock(std::make_shared<FakeClock>());

				client.SendActionToServer(Shield);

				const char* expected = R"({"move":4,"time":100})";

				Assert::AreEqual(expected, reinterpret_cast<char*>(fakeConnection->sendedMessages[0].data()));
			}

		};
	}
}