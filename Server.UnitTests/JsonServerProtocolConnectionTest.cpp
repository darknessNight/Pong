#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ServerLibrary/InternetProtocol/JsonServerProtocolConnection.h"

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
			unsigned GetId() override { return id; }
			void SetDisconnectAction(std::function<void(Connection*)>) override {}
			void SetTransmissionErrorAction(std::function<void(Connection*)>) override {}
		};


		TEST_CLASS(JsonServerProtocolConnetionTests)
		{
		public:

			TEST_METHOD(AddUserAndGetId_HasFakeUser_CheckSavedUser)
			{
				JsonServerProtocolConnection server;

				auto expected = std::make_shared<FakeConnection>(1);
				server.AddUserConnectionAndGetId(expected);

				auto result = server.GetUserForId(1);

				Assert::AreEqual(expected->id, result->GetId());
			}

			TEST_METHOD(AddUserAndGetId_HasFakeUser_CheckSendedIdToUser)
			{
				JsonServerProtocolConnection server;

				auto user = std::make_shared<FakeConnection>(1);
				server.AddUserConnectionAndGetId(user);

				std::string expected = R"({"playerId":0})";

				Assert::AreEqual<unsigned>(1u, user->sendedMessages.size());
				Assert::AreEqual(expected.c_str(), reinterpret_cast<char*>(user->sendedMessages[0].data()));
			}

			TEST_METHOD(AddUserAndGetId_HasTwoFakeUser_CheckSendedCorrectIdsToUsers)
			{
				JsonServerProtocolConnection server;

				auto user1 = std::make_shared<FakeConnection>(1);
				auto user2 = std::make_shared<FakeConnection>(2);
				server.AddUserConnectionAndGetId(user1);
				server.AddUserConnectionAndGetId(user2);

				std::string expected1 = R"({"playerId":0})";
				std::string expected2 = R"({"playerId":1})";

				Assert::AreEqual<unsigned>(1u, user1->sendedMessages.size());
				Assert::AreEqual(expected1.c_str(), reinterpret_cast<char*>(user1->sendedMessages[0].data()));
				Assert::AreEqual<unsigned>(1u, user2->sendedMessages.size());
				Assert::AreEqual(expected2.c_str(), reinterpret_cast<char*>(user2->sendedMessages[0].data()));
			}

			TEST_METHOD(AddUserAndGetId_AddOneUserTwice_CheckSendedCorrectIdToUser)
			{
				JsonServerProtocolConnection server;

				auto user0 = std::make_shared<FakeConnection>(0);
				auto user1 = std::make_shared<FakeConnection>(1);
				auto user2 = std::make_shared<FakeConnection>(1);
				server.AddUserConnectionAndGetId(user0);
				server.AddUserConnectionAndGetId(user1);
				server.AddUserConnectionAndGetId(user2);

				std::string expected1 = R"({"playerId":1})";
				std::string expected2 = R"({"playerId":1})";

				Assert::AreEqual<unsigned>(1u, user1->sendedMessages.size());
				Assert::AreEqual(expected1.c_str(), reinterpret_cast<char*>(user1->sendedMessages[0].data()));
				Assert::AreEqual<unsigned>(1u, user2->sendedMessages.size());
				Assert::AreEqual(expected2.c_str(), reinterpret_cast<char*>(user2->sendedMessages[0].data()));
			}

			TEST_METHOD(SendObjectsToUser_SendOneObject_CheckConvertedToJsonAndSended)
			{
				JsonServerProtocolConnection server;

				auto user = std::make_shared<FakeConnection>(1);
				std::vector<ConnectionObject> data;
				data.push_back({
					0.4f,0.f, ConnectionObject::Type::BallRed,3
				});

				server.SendObjectsToUser(data, user);

				std::string expected = R"([{"lives":3,"type":2,"x":0.400000005960464,"y":0.0}])";

				Assert::AreEqual<unsigned>(1u, user->sendedMessages.size());
				Assert::AreEqual(expected.c_str(), reinterpret_cast<char*>(user->sendedMessages[0].data()));
			}

			TEST_METHOD(SendObjectsToUser_SendTwoObjects_CheckConvertedToJsonAndSended)
			{
				JsonServerProtocolConnection server;

				auto user = std::make_shared<FakeConnection>(1);
				std::vector<ConnectionObject> data{ {
					0.5f,0.f, ConnectionObject::Type::BallRed,3
				},{
					0.25f,0.125f, ConnectionObject::Type::BallCommon,4
				} };


				server.SendObjectsToUser(data, user);

				std::string expected = R"R([{"lives":3,"type":2,"x":0.5,"y":0.0},{"lives":4,"type":1,"x":0.25,"y":0.125}])R";

				Assert::AreEqual<unsigned>(1u, user->sendedMessages.size());
				Assert::AreEqual(expected.c_str(), reinterpret_cast<char*>(user->sendedMessages[0].data()));
			}

			TEST_METHOD(SendObjectsToAll_SendTwoObjectsToTwoUsers_CheckConvertedToJsonAndSended)
			{
				JsonServerProtocolConnection server;

				auto user1 = std::make_shared<FakeConnection>(1);
				auto user2 = std::make_shared<FakeConnection>(2);
				std::vector<ConnectionObject> data{ {
						0.5f,0.f, ConnectionObject::Type::BallRed,3
					},{
						0.25f,0.125f, ConnectionObject::Type::BallCommon,4
					} };
				server.AddUserConnectionAndGetId(user1);
				server.AddUserConnectionAndGetId(user2);
				user1->sendedMessages.clear();
				user2->sendedMessages.clear();

				server.SendObjectsToAll(data);

				std::string expected = R"R([{"lives":3,"type":2,"x":0.5,"y":0.0},{"lives":4,"type":1,"x":0.25,"y":0.125}])R";

				Assert::AreEqual<unsigned>(1u, user1->sendedMessages.size(), L"User1");
				Assert::AreEqual(expected.c_str(), reinterpret_cast<char*>(user1->sendedMessages[0].data()), L"User1");
				Assert::AreEqual<unsigned>(1u, user2->sendedMessages.size(), L"User2");
				Assert::AreEqual(expected.c_str(), reinterpret_cast<char*>(user2->sendedMessages[0].data()), L"User2");
			}

			TEST_METHOD(GetActionsForUser_ReveiceOneAction_CheckReadedCorrectlyAndConvertedFromJson)
			{
				JsonServerProtocolConnection server;
				auto user1 = std::make_shared<FakeConnection>(1);
				user1->receivedMessages.push_back(R"([{"move":5,"time":342}])");

				auto result = server.GetActionsForUser(user1);

				UserMove expected = { Shoot,std::chrono::microseconds(342) };

				Assert::AreEqual<unsigned>(1, result.size());
				Assert::IsTrue(expected == result[0]);
			}

			TEST_METHOD(GetActionsForUser_ReveiceTwoActions_CheckReadedCorrectlyAndConvertedFromJson)
			{
				JsonServerProtocolConnection server;
				auto user1 = std::make_shared<FakeConnection>(1);
				user1->receivedMessages.push_back(R"([{"move":5,"time":342},{"move":4,"time":32}])");

				auto result = server.GetActionsForUser(user1);

				std::vector<UserMove> expected = { { Shoot,std::chrono::microseconds(342) }, { Shield,std::chrono::microseconds(32) }};

				Assert::IsTrue(expected == result);
			}

		};
	}
}