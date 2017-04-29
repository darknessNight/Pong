#include "stdafx.h"
#include "CppUnitTest.h"
#include "../ServerLibrary/GameEngine/GameEngine.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> inline std::wstring ToString<Pong::GameEngine::GameObject>(const Pong::GameEngine::GameObject& q)
			{
				return std::to_wstring(q.GetType())+L" " + std::to_wstring(q.GetPos().x) + L" " + std::to_wstring(q.GetPos().y);
			}
			template<> inline std::wstring ToString<Pong::GameEngine::GameObject>(const Pong::GameEngine::GameObject* q)
			{
				return std::to_wstring(q->GetType()) + L" " + std::to_wstring(q->GetPos().x) + L" " + std::to_wstring(q->GetPos().y);
			}
			template<> inline std::wstring ToString<Pong::GameEngine::Pointf>(const Pong::GameEngine::Pointf& q)
			{
				return std::to_wstring(q.x) + L" " + std::to_wstring(q.y);
			}
			template<> inline std::wstring ToString<Pong::GameEngine::Pointf>(const Pong::GameEngine::Pointf* q)
			{
				return std::to_wstring(q->x) + L" " + std::to_wstring(q->y);
			}
			template<> inline std::wstring ToString<std::vector<std::shared_ptr<Pong::GameEngine::GameObject>>>(const std::vector<std::shared_ptr<Pong::GameEngine::GameObject>>& q)
			{
				std::wstringstream wss;
				for (auto el : q)
					wss<<"'" << ToString(*el) << "' ";
				return wss.str();
			}
			template<> inline std::wstring ToString<std::vector<std::shared_ptr<Pong::GameEngine::GameObject>>>(const std::vector<std::shared_ptr<Pong::GameEngine::GameObject>>* q)
			{
				return ToString(*q);
			}
		}
	}
}
namespace Pong
{
	namespace GameEngine
	{


		class TestGameObject:public GameObject
		{
			Pointf pos, size;
			Type type;
		public:
			TestGameObject() :GameObject({ 0,0 }, { 0,0 }, (Type)0) {
				pos = { 0,0 };
				size = { 0,0 };
				type = (Type)0;
			}

			void SetPos(Pointf p) override { pos = p; }
			void SetSize(Pointf s)override { size = s; }
			void SetType(Type t)override
			{
				type = t;
			}

			Pointf GetPos() const override { return pos; }
			Pointf GetSize() const override { return size; }
			Type GetType() const override { return type; }
		};

		TEST_CLASS(GameEngineTests)
		{
		public:

			std::shared_ptr<GameEngine> PrepareGameEngine()
			{
				return std::make_shared<GameEngine>();
			}

			TEST_METHOD(CreateObject_HasPoint_CheckReturnConfiguredGameObject)
			{
				auto gameEngine = PrepareGameEngine();

				auto result = gameEngine->CreateObject({ 100,200 }, { 10,10 }, GameObject::Type::Test);
				TestGameObject expected;
				expected.SetPos({ 100,200 });
				expected.SetSize({ 10,10 });
				expected.SetType({ GameObject::Type::Test });
				Assert::AreEqual<GameObject>(*reinterpret_cast<GameObject*>(&expected), *result);
			}

			TEST_METHOD(WillCollide_HasTwoNonCollideObjects_CheckReturnFalse)
			{
				auto gameEngine = PrepareGameEngine();

				auto o1 = gameEngine->CreateObject({ 100,200 }, { 10,10 }, GameObject::Type::Test);
				auto o2 = gameEngine->CreateObject({ 110,200 }, { 10,10 }, GameObject::Type::Test);

				auto result = gameEngine->WillCollide(o2, { 10,0 });
				Assert::IsFalse(result);
			}

			TEST_METHOD(WillCollide_HasTwoNonCollideObjectsWithBoundaryTouch0XAxis_CheckReturnFalse)
				//mo¿e brakowaæ dla dotykania z pozosta³ych kierunków
			{
				auto gameEngine = PrepareGameEngine();

				auto o1 = gameEngine->CreateObject({ 100,200 }, { 10,10 }, GameObject::Type::Test);
				auto o2 = gameEngine->CreateObject({ 110,200 }, { 10,10 }, GameObject::Type::Test);

				auto result = gameEngine->WillCollide(o2, { -10,0 });
				Assert::IsTrue(result);
			}

			TEST_METHOD(WillCollide_HasTwoNonCollideObjectsWithBoundaryTouch0YAxis_CheckReturnFalse)
			{
				auto gameEngine = PrepareGameEngine();

				auto o1 = gameEngine->CreateObject({ 100,200 }, { 10,10 }, GameObject::Type::Test);
				auto o2 = gameEngine->CreateObject({ 100,210 }, { 10,10 }, GameObject::Type::Test);

				auto result = gameEngine->WillCollide(o2, { 0,-10 });
				Assert::IsTrue(result);
			}

			TEST_METHOD(WillCollide_HasTwoCollideObjects0XAxisMinusShift_CheckReturnTrue)
			{
				auto gameEngine = PrepareGameEngine();

				auto o1 = gameEngine->CreateObject({ 100,200 }, { 10,10 }, GameObject::Type::Test);
				auto o2 = gameEngine->CreateObject({ 110,200 }, { 10,10 }, GameObject::Type::Test);

				auto result = gameEngine->WillCollide(o2, { -11,0 });
				Assert::IsTrue(result);
			}

			TEST_METHOD(WillCollide_HasTwoCollideObjects0XAxisPlusShift_CheckReturnTrue)
			{
				auto gameEngine = PrepareGameEngine();

				auto o1 = gameEngine->CreateObject({ 100,200 }, { 10,10 }, GameObject::Type::Test);
				auto o2 = gameEngine->CreateObject({ 110,200 }, { 10,10 }, GameObject::Type::Test);

				auto result = gameEngine->WillCollide(o1, { 11,0 });
				Assert::IsTrue(result);
			}

			TEST_METHOD(WillCollide_HasTwoCollideObjects0YAxisPlusShift_CheckReturnTrue)
			{
				auto gameEngine = PrepareGameEngine();

				auto o1 = gameEngine->CreateObject({ 100,200 }, { 10,10 }, GameObject::Type::Test);
				auto o2 = gameEngine->CreateObject({ 100,210 }, { 10,10 }, GameObject::Type::Test);

				auto result = gameEngine->WillCollide(o1, { 0,11 });
				Assert::IsTrue(result);
			}

			TEST_METHOD(WillCollide_HasTwoCollideObjects0YAxisMinusShift_CheckReturnTrue)
			{
				auto gameEngine = PrepareGameEngine();

				auto o1 = gameEngine->CreateObject({ 100,200 }, { 10,10 }, GameObject::Type::Test);
				auto o2 = gameEngine->CreateObject({ 100,210 }, { 10,10 }, GameObject::Type::Test);

				auto result = gameEngine->WillCollide(o2, { 0,-11 });
				Assert::IsTrue(result);
			}

			TEST_METHOD(WillCollide_HasTwoCollideObjectsWithShiftAfterObject0XAxis_CheckReturnTrue)
			{
				auto gameEngine = PrepareGameEngine();

				auto o1 = gameEngine->CreateObject({ 100,200 }, { 10,10 }, GameObject::Type::Test);
				auto o2 = gameEngine->CreateObject({ 110,200 }, { 10,10 }, GameObject::Type::Test);

				auto result = gameEngine->WillCollide(o2, { -100,0 });
				Assert::IsTrue(result);
			}

			TEST_METHOD(WillCollide_HasTwoCollideObjectsWithShiftAfterObject0YAxis_CheckReturnTrue)
			{
				auto gameEngine = PrepareGameEngine();

				auto o1 = gameEngine->CreateObject({ 100,200 }, { 10,10 }, GameObject::Type::Test);
				auto o2 = gameEngine->CreateObject({ 100,210 }, { 10,10 }, GameObject::Type::Test);

				auto result = gameEngine->WillCollide(o2, { 0,-100 });
				Assert::IsTrue(result);
			}

			TEST_METHOD(GetAllObjects_HasTwoObjects_CheckReturnAll)
			{
				auto gameEngine = PrepareGameEngine();

				auto o1 = gameEngine->CreateObject({ 100,200 }, { 10,10 }, GameObject::Type::Test);
				auto o2 = gameEngine->CreateObject({ 100,210 }, { 10,10 }, GameObject::Type::Test);

				auto result = gameEngine->GetAllObjects();
				std::vector<std::shared_ptr<GameObject>> expected = { o1,o2 };
				Assert::AreEqual(expected, result);
			}

			TEST_METHOD(MoveObject_HasTwoCollideObject_CheckNotChangedPos)
			{
				auto gameEngine = PrepareGameEngine();

				auto o1 = gameEngine->CreateObject({ 100,200 }, { 10,10 }, GameObject::Type::Test);
				auto o2 = gameEngine->CreateObject({ 100,210 }, { 10,10 }, GameObject::Type::Test);

				auto result = gameEngine->MoveObject(o2, { 0,-10 });
				Assert::AreNotEqual({ 100,200 }, o2->GetPos());
			}

			TEST_METHOD(MoveObject_HasTwoNonCollideObject_CheckChangedPos)
			{
				auto gameEngine = PrepareGameEngine();

				auto o1 = gameEngine->CreateObject({ 100,200 }, { 10,10 }, GameObject::Type::Test);
				auto o2 = gameEngine->CreateObject({ 100,210 }, { 10,10 }, GameObject::Type::Test);

				auto result = gameEngine->MoveObject(o2, { 0,10 });
				Assert::AreEqual({ 100,220 }, o2->GetPos());
			}
		};
	}
}